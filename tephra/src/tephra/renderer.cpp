#include "renderer.hpp"

#include <iostream>
#include <set>

#include "vulkan/vulkan_functions.hpp"

#include "application.hpp"

using namespace tph::vulkan::functions;

namespace tph
{

static std::vector<VkLayerProperties> available_device_layers(VkPhysicalDevice physical_device)
{
    std::vector<VkLayerProperties> extensions{};

    std::uint32_t count{};
    vkEnumerateDeviceLayerProperties(physical_device, &count, nullptr);
    extensions.resize(count);
    vkEnumerateDeviceLayerProperties(physical_device, &count, std::data(extensions));

    return extensions;
}

static renderer_layer layer_from_name(std::string_view name) noexcept
{
    if(name == "VK_LAYER_LUNARG_standard_validation")
    {
        return renderer_layer::validation;
    }

    return renderer_layer::none;
}

static std::vector<const char*> filter_device_layers(VkPhysicalDevice physical_device, std::vector<const char*> layers, renderer_layer& layer_bits)
{
    const std::vector<VkLayerProperties> available{available_device_layers(physical_device)};

    for(auto it{std::cbegin(layers)}; it != std::cend(layers); ++it)
    {
        const auto pred = [it](const VkLayerProperties& layer)
        {
            return std::string_view{layer.layerName} == *it;
        };

        if(std::find_if(std::begin(available), std::end(available), pred) == std::end(available))
        {
            #ifndef NDEBUG
            std::cerr << "Layer \"" << *it << "\" is not available." << std::endl;
            #endif

            layer_bits &= ~layer_from_name(*it);
            it = layers.erase(it);
        }
    }

    return layers;
}

static std::vector<const char*> required_device_layers(VkPhysicalDevice physical_device, renderer_layer& layers)
{
    std::vector<const char*> output{};

    if(static_cast<bool>(layers & renderer_layer::validation))
    {
        output.emplace_back("VK_LAYER_LUNARG_standard_validation");
    }

    return filter_device_layers(physical_device, std::move(output), layers);
}

static std::vector<VkExtensionProperties> available_device_extensions(VkPhysicalDevice physical_device, const std::vector<const char*>& layers)
{
    std::vector<VkExtensionProperties> extensions{};

    std::uint32_t count{};
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, nullptr);
    extensions.resize(count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, std::data(extensions));

    for(auto layer : layers)
    {
        std::vector<VkExtensionProperties> layer_extensions{};

        vkEnumerateDeviceExtensionProperties(physical_device, layer, &count, nullptr);
        layer_extensions.resize(count);
        vkEnumerateDeviceExtensionProperties(physical_device, layer, &count, std::data(layer_extensions));

        extensions.insert(std::cend(extensions), std::cbegin(layer_extensions), std::cend(layer_extensions));
    }

    return extensions;
}

static renderer_extension extension_from_name(std::string_view name) noexcept
{
    if(name == "VK_KHR_swapchain")
    {
        return renderer_extension::swapchain;
    }

    return renderer_extension::none;
}

static std::vector<const char*> filter_device_extensions(VkPhysicalDevice physical_device, const std::vector<const char*>& layers, std::vector<const char*> extensions, renderer_extension& extension_bits)
{
    const std::vector<VkExtensionProperties> available{available_device_extensions(physical_device, layers)};

    for(auto it{std::cbegin(extensions)}; it != std::cend(extensions); ++it)
    {
        const auto pred = [it](const VkExtensionProperties& ext)
        {
            return std::string_view{ext.extensionName} == *it;
        };

        if(std::find_if(std::begin(available), std::end(available), pred) == std::end(available))
        {
            #ifndef NDEBUG
            std::cerr << "Extension \"" << *it << "\" is not available." << std::endl;
            #endif

            extension_bits &= ~extension_from_name(*it);
            it = extensions.erase(it);
        }
    }

    return extensions;
}

static std::vector<const char*> required_device_extensions(VkPhysicalDevice physical_device, const std::vector<const char*>& layers, renderer_extension& extensions)
{
    std::vector<const char*> output{"VK_KHR_swapchain"};

    return filter_device_extensions(physical_device, layers, std::move(output), extensions);
}

static VkPhysicalDeviceFeatures parse_enabled_features(const physical_device_features& features)
{
    VkPhysicalDeviceFeatures output{};

    output.wideLines = static_cast<VkBool32>(features.wide_lines);
    output.largePoints = static_cast<VkBool32>(features.large_points);
    output.sampleRateShading = static_cast<VkBool32>(features.sample_shading);
    output.fragmentStoresAndAtomics = static_cast<VkBool32>(features.fragment_stores_and_atomics);

    return output;
}

static std::uint32_t choose_generic_family(const std::vector<VkQueueFamilyProperties>& queue_families) //It's the one used for graphics
{
    for(std::size_t i{}; i < std::size(queue_families); ++i)
    {
        if((queue_families[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) == (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))
        {
            return static_cast<std::uint32_t>(i);
        }
    }

    std::terminate();
}

static std::uint32_t choose_present_family(VkPhysicalDevice physical_device [[maybe_unused]], const std::vector<VkQueueFamilyProperties>& queue_families)
{
#if defined(TPH_PLATFORM_WIN32)

    for(std::size_t i{}; i < std::size(queue_families); ++i)
    {
        if(vkGetPhysicalDeviceWin32PresentationSupportKHR(physical_device, static_cast<std::uint32_t>(i)) == VK_TRUE)
        {
            return static_cast<std::uint32_t>(i);
        }
    }

#elif defined(TPH_PLATFORM_XLIB)

    Display* display{XOpenDislay(nullptr)};
    Visual* visual{XDefaultVisual(display, XDefaultScreen(display))};
    VisualID id{XVisualIDFromVisual(visual)};

    for(std::size_t i{}; i < std::size(queue_families); ++i)
    {
        if(vkGetPhysicalDeviceXlibPresentationSupportKHR(physical_device, static_cast<std::uint32_t>(i), display, id) == VK_TRUE)
        {
            XCloseDisplay(display);
            return static_cast<std::uint32_t>(i);
        }
    }

    XCloseDisplay(display);

#elif defined(TPH_PLATFORM_XCB)

    const auto screen_of_display = [](xcb_connection_t* connection, int screen) -> xcb_screen_t*
    {
        for(xcb_screen_iterator_t iter{xcb_setup_roots_iterator(xcb_get_setup(connection))}; iter.rem; --screen, xcb_screen_next(&iter))
        {
            if(!screen)
            {
                return iter.data;
            }
        }

        return nullptr;
    };

    int screen{};
    xcb_connection_t* connection{xcb_connect(nullptr, &screen)};
    xcb_screen_t* screen{screen_of_display(connection, screen)};
    xcb_visualid_t id{screen->root_visual};

    for(std::size_t i{}; i < std::size(queue_families); ++i)
    {
        if(vkGetPhysicalDeviceXcbPresentationSupportKHR(physical_device, static_cast<std::uint32_t>(i), connection, id) == VK_TRUE)
        {
            xcb_disconnect(connection);
            return static_cast<std::uint32_t>(i);
        }
    }

    xcb_disconnect(connection);

#elif defined(TPH_PLATFORM_WAYLAND)

    wl_display* display{wl_display_connect(nullptr)};

    for(std::size_t i{}; i < std::size(queue_families); ++i)
    {
        if(vkGetPhysicalDeviceWaylandPresentationSupportKHR(physical_device, static_cast<std::uint32_t>(i), display) == VK_TRUE)
        {
            wl_display_disconnect(connection);
            return static_cast<std::uint32_t>(i);
        }
    }

    wl_display_disconnect(connection);

#endif

    return choose_generic_family(queue_families);
}

static std::uint32_t choose_transfer_family(const std::vector<VkQueueFamilyProperties>& queue_families)
{
    for(std::size_t i{}; i < std::size(queue_families); ++i)
    {
        const bool support_transfer{(queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0};
        const bool support_other{(queue_families[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) != 0};

        if(support_transfer && !support_other)
        {
            return static_cast<std::uint32_t>(i);
        }
    }

    return choose_generic_family(queue_families);
}

static std::uint32_t choose_compute_family(const std::vector<VkQueueFamilyProperties>& queue_families)
{
    for(std::size_t i{}; i < std::size(queue_families); ++i)
    {
        const bool support_compute{(queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0};
        const bool support_other{(queue_families[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT)) != 0};

        if(support_compute && !support_other)
        {
            return static_cast<std::uint32_t>(i);
        }
    }

    return choose_generic_family(queue_families);
}

static renderer::queue_families_t choose_queue_families(VkPhysicalDevice physical_device, renderer_options options, renderer::transfer_granularity& granularity)
{
    std::uint32_t count{};
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_family_properties{};
    queue_family_properties.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, std::data(queue_family_properties));

    renderer::queue_families_t output{};
    output[static_cast<std::size_t>(queue::graphics)] = choose_generic_family(queue_family_properties);
    output[static_cast<std::size_t>(queue::present)] = choose_present_family(physical_device, queue_family_properties);

    if(static_cast<bool>(options & renderer_options::standalone_transfer_queue))
    {
        output[static_cast<std::size_t>(queue::transfer)] = choose_transfer_family(queue_family_properties);

        const VkExtent3D native_granularity{queue_family_properties[output[static_cast<std::size_t>(queue::transfer)]].minImageTransferGranularity};
        granularity.width = native_granularity.width;
        granularity.height = native_granularity.height;
        granularity.depth = native_granularity.depth;
    }
    else
    {
        output[static_cast<std::size_t>(queue::transfer)] = output[static_cast<std::size_t>(queue::graphics)];
    }

    if(static_cast<bool>(options & renderer_options::standalone_compute_queue))
    {
        output[static_cast<std::size_t>(queue::compute)] = choose_compute_family(queue_family_properties);
    }
    else
    {
        output[static_cast<std::size_t>(queue::compute)] = output[static_cast<std::size_t>(queue::graphics)];
    }

    return output;
}

static std::vector<VkDeviceQueueCreateInfo> make_queue_create_info(const renderer::queue_families_t& families, renderer_options options)
{
    std::vector<std::uint32_t> unique_families{};
    unique_families.emplace_back(families[static_cast<std::size_t>(queue::graphics)]);
    unique_families.emplace_back(families[static_cast<std::size_t>(queue::present)]);

    if(static_cast<bool>(options & renderer_options::standalone_transfer_queue))
    {
        unique_families.emplace_back(families[static_cast<std::size_t>(queue::transfer)]);
    }

    if(static_cast<bool>(options & renderer_options::standalone_compute_queue))
    {
        unique_families.emplace_back(families[static_cast<std::size_t>(queue::compute)]);
    }

    std::sort(std::begin(unique_families), std::end(unique_families));
    unique_families.erase(std::unique(std::begin(unique_families), std::end(unique_families)), std::end(unique_families));

    std::vector<VkDeviceQueueCreateInfo> queues{};
    queues.reserve(std::size(unique_families));

    for(auto family : unique_families)
    {
        VkDeviceQueueCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        create_info.queueFamilyIndex = family;
        create_info.queueCount = 1;

        queues.emplace_back(create_info);
    }

    return queues;
}

static std::uint64_t upper_power_of_two(std::uint64_t value) noexcept
{
    --value;

    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;

    ++value;

    return value;
}

static vulkan::memory_allocator::heap_sizes compute_heap_sizes(const physical_device& physical_device, renderer_options options)
{
    vulkan::memory_allocator::heap_sizes output{};

    if(physical_device.memory_properties().device_shared > physical_device.memory_properties().device_local)
    {
        //If we have more device shared memory than "pure" device local memory, it means that the device is probably the host.
        //In that case device shared memory will be a part of the system memory, so we reduce the heap size to prevent overallocation.
        output.device_shared = upper_power_of_two(physical_device.memory_properties().device_shared / 128);
    }
    else
    {
        //Otherwise, it is probably a small part of the device memory that is accessible from the host, so we use a bigger heap size.
        output.device_shared = upper_power_of_two(physical_device.memory_properties().device_shared / 16);
    }

    output.device_local = upper_power_of_two(physical_device.memory_properties().device_local / 64);
    output.host_shared = upper_power_of_two(physical_device.memory_properties().host_shared / 256);

    if(static_cast<bool>(options & renderer_options::tiny_memory_heaps))
    {
        output.device_shared /= 4;
        output.device_local /= 4;
        output.host_shared /= 4;
    }
    else if(static_cast<bool>(options & renderer_options::small_memory_heaps))
    {
        output.device_shared /= 2;
        output.device_local /= 2;
        output.host_shared /= 2;
    }
    else if(static_cast<bool>(options & renderer_options::large_memory_heaps))
    {
        output.device_shared *= 2;
        output.device_local *= 2;
        output.host_shared *= 2;
    }
    else if(static_cast<bool>(options & renderer_options::giant_memory_heaps))
    {
        output.device_shared *= 4;
        output.device_local *= 4;
        output.host_shared *= 4;
    }

    return output;
}

renderer::renderer(const physical_device& physical_device, renderer_layer layers, renderer_extension extensions, const physical_device_features& enabled_features, renderer_options options)
{
    m_physical_device = underlying_cast<VkPhysicalDevice>(physical_device);
    m_queue_families = choose_queue_families(m_physical_device, options, m_transfer_queue_granularity);

    const std::vector<const char*> layer_names{required_device_layers(m_physical_device, layers)};
    const std::vector<const char*> extension_names{required_device_extensions(m_physical_device, layer_names, extensions)};
    const VkPhysicalDeviceFeatures features{parse_enabled_features(enabled_features)};

    std::vector<VkDeviceQueueCreateInfo> queues{make_queue_create_info(m_queue_families, options)};
    const float priority{1.0f};
    for(auto& queue : queues)
    {
        queue.pQueuePriorities = &priority;
    }

    m_device = vulkan::device{m_physical_device, layer_names, extension_names, queues, features};
    m_layers = layers;
    m_extensions = extensions;

    tph::vulkan::functions::load_device_level_functions(m_device);

    for(std::uint32_t i{}; i < std::size(m_queue_families); ++i)
    {
        vkGetDeviceQueue(m_device, m_queue_families[i], 0, &m_queues[i]);
    }

    m_allocator = std::make_unique<vulkan::memory_allocator>(m_physical_device, m_device, compute_heap_sizes(physical_device, options));
}

static renderer::transfer_granularity compute_transfer_granularity(VkPhysicalDevice physical_device, std::uint32_t family)
{
    std::uint32_t count{};
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_family_properties{};
    queue_family_properties.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, std::data(queue_family_properties));

    const VkExtent3D native_granularity{queue_family_properties[family].minImageTransferGranularity};

    renderer::transfer_granularity output{};
    output.width = native_granularity.width;
    output.height = native_granularity.height;
    output.depth = native_granularity.depth;

    return output;
}

renderer::renderer(const physical_device& physical_device, vulkan::device device, renderer_layer layers, renderer_extension extensions,
                   const queue_families_t& queue_families, const queues_t& queues, const vulkan::memory_allocator::heap_sizes& sizes)
:m_physical_device{underlying_cast<VkPhysicalDevice>(physical_device)}
,m_device{std::move(device)}
,m_layers{layers}
,m_extensions{extensions}
,m_queue_families{queue_families}
,m_queues{queues}
,m_transfer_queue_granularity{compute_transfer_granularity(m_physical_device, queue_family_index(queue::transfer))}
{
    tph::vulkan::functions::load_device_level_functions(m_device);

    m_allocator = std::make_unique<vulkan::memory_allocator>(m_physical_device, m_device, sizes);
}

void renderer::wait()
{
    if(auto result{vkDeviceWaitIdle(m_device)}; result != VK_SUCCESS)
        throw vulkan::error{result};
}

void set_object_name(renderer& renderer, const std::string& name)
{
    VkDebugUtilsObjectNameInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    info.objectType = VK_OBJECT_TYPE_DEVICE;
    info.objectHandle = reinterpret_cast<std::uint64_t>(underlying_cast<VkDevice>(renderer));
    info.pObjectName = std::data(name);

    if(auto result{vkSetDebugUtilsObjectNameEXT(underlying_cast<VkDevice>(renderer), &info)}; result != VK_SUCCESS)
        throw vulkan::error{result};
}

}
