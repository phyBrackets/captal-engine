#include "swapchain.hpp"

#include "vulkan/vulkan_functions.hpp"

#include "renderer.hpp"
#include "surface.hpp"

namespace tph
{

using namespace vulkan::functions;

swapchain::swapchain(renderer& renderer, surface& surface, const swapchain_info& info, optional_ref<swapchain> old_swapchain)
:m_queue{underlying_cast<VkQueue>(renderer, queue::present)}
,m_info{info}
{
    VkSwapchainKHR old{};
    if(old_swapchain)
    {
        old = old_swapchain->m_swapchain;
    }

    m_swapchain = vulkan::swapchain
    {
        underlying_cast<VkDevice>(renderer),
        underlying_cast<VkSurfaceKHR>(surface),
        VkExtent2D{info.width, info.height},
        info.image_count,
        VkSurfaceFormatKHR{static_cast<VkFormat>(info.format), VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
        static_cast<VkImageUsageFlags>(info.usage),
        std::vector<std::uint32_t>{},
        static_cast<VkSurfaceTransformFlagBitsKHR>(info.transform),
        static_cast<VkCompositeAlphaFlagBitsKHR>(info.composite),
        static_cast<VkPresentModeKHR>(info.present_mode),
        static_cast<VkBool32>(info.clipping),
        old
    };

    if(auto result{vkGetSwapchainImagesKHR(underlying_cast<VkDevice>(renderer), m_swapchain, &m_info.image_count, nullptr)}; result != VK_SUCCESS)
        throw vulkan::error{result};

    std::vector<VkImage> images{};
    images.resize(static_cast<std::size_t>(m_info.image_count));
    if(auto result{vkGetSwapchainImagesKHR(underlying_cast<VkDevice>(renderer), m_swapchain, &m_info.image_count, std::data(images))}; result != VK_SUCCESS)
        throw vulkan::error{result};

    m_textures.reserve(m_info.image_count);
    for(auto image : images)
    {
        auto& texture{m_textures.emplace_back()};

        texture.m_image = vulkan::image{image};
        texture.m_image_view = vulkan::image_view
        {
                underlying_cast<VkDevice>(renderer),
                image,
                VK_IMAGE_VIEW_TYPE_2D,
                static_cast<VkFormat>(info.format),
                VkComponentMapping{},
                static_cast<VkImageAspectFlags>(aspect_from_format(info.format))
        };
        texture.m_format = info.format;
        texture.m_aspect = aspect_from_format(info.format);
        texture.m_width = info.width;
        texture.m_height = info.height;
        texture.m_depth = 1;
    }
}

swapchain_status swapchain::acquire(optional_ref<semaphore> semaphore, optional_ref<fence> fence)
{
    VkSemaphore native_semaphore{semaphore.has_value() ? underlying_cast<VkSemaphore>(*semaphore) : VkSemaphore{}};
    VkFence native_fence{fence.has_value() ? underlying_cast<VkFence>(*fence) : VkFence{}};

    const auto result{vkAcquireNextImageKHR(m_swapchain.device(), m_swapchain, std::numeric_limits<std::uint64_t>::max(), native_semaphore, native_fence, &m_image_index)};

    if(result == VK_SUBOPTIMAL_KHR)
    {
        return swapchain_status::suboptimal;
    }
    else if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return swapchain_status::out_of_date;
    }
    else if(result == VK_ERROR_SURFACE_LOST_KHR)
    {
        return swapchain_status::surface_lost;
    }
    else if(result != VK_SUCCESS)
    {
        throw vulkan::error{result};
    }

    return swapchain_status::valid;
}

swapchain_status swapchain::present(const std::vector<std::reference_wrapper<semaphore>>& wait_semaphores)
{
    VkSwapchainKHR native_swapchain{m_swapchain};

    std::vector<VkSemaphore> native_semaphores{};
    native_semaphores.reserve(std::size(wait_semaphores));

    for(semaphore& semaphore : wait_semaphores)
    {
        native_semaphores.emplace_back(underlying_cast<VkSemaphore>(semaphore));
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &native_swapchain;
    present_info.pImageIndices = &m_image_index;
    present_info.waitSemaphoreCount = static_cast<std::uint32_t>(std::size(native_semaphores));
    present_info.pWaitSemaphores = std::data(native_semaphores);

    const auto result{vkQueuePresentKHR(m_queue, &present_info)};

    if(result == VK_SUBOPTIMAL_KHR)
    {
        return swapchain_status::suboptimal;
    }
    else if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return swapchain_status::out_of_date;
    }
    else if(result == VK_ERROR_SURFACE_LOST_KHR)
    {
        return swapchain_status::surface_lost;
    }
    else if(result != VK_SUCCESS)
    {
        throw vulkan::error{result};
    }

    return swapchain_status::valid;
}

swapchain_status swapchain::present(semaphore& wait_semaphore)
{
    VkSwapchainKHR native_swapchain{m_swapchain};
    VkSemaphore native_semaphore{underlying_cast<VkSemaphore>(wait_semaphore)};

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &native_swapchain;
    present_info.pImageIndices = &m_image_index;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &native_semaphore;

    const auto result{vkQueuePresentKHR(m_queue, &present_info)};

    if(result == VK_SUBOPTIMAL_KHR)
    {
        return swapchain_status::suboptimal;
    }
    else if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return swapchain_status::out_of_date;
    }
    else if(result == VK_ERROR_SURFACE_LOST_KHR)
    {
        return swapchain_status::surface_lost;
    }
    else if(result != VK_SUCCESS)
    {
        throw vulkan::error{result};
    }

    return swapchain_status::valid;
}

}
