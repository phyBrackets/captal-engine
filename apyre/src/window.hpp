#ifndef APYRE_WINDOW_HPP_INCLUDED
#define APYRE_WINDOW_HPP_INCLUDED

#include "config.hpp"

#include <string>

class SDL_Window;

#ifndef VULKAN_H_
    #define APYRE_VK_DEFINE_HANDLE(object) typedef struct object##_T* object;

    #if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
        #define APYRE_VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef struct object##_T *object;
    #else
        #define APYRE_VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef uint64_t object;
    #endif

    APYRE_VK_DEFINE_HANDLE(VkInstance)
    APYRE_VK_DEFINE_NON_DISPATCHABLE_HANDLE(VkSurfaceKHR)
#endif

namespace apr
{

class application;
class monitor;

enum class window_options : std::uint32_t
{
    none = 0,
    fullscreen = 0x01,
    hidden = 0x02,
    borderless = 0x04,
    resizable = 0x08,
    minimized = 0x10,
    maximized = 0x20,
};

template<> struct enable_enum_operations<window_options> {static constexpr bool value{true};};

class APYRE_API window
{
public:
    using id_type = std::uint32_t;

public:
    constexpr window() = default;
    window(application& application, const std::string& title, std::uint32_t width, std::uint32_t height, window_options options = window_options::none);
    window(application& application, const monitor& monitor, const std::string& title, std::uint32_t width, std::uint32_t height, window_options options = window_options::none);

    ~window();
    window(const window&) = delete;
    window& operator=(const window&) = delete;
    window(window&& other) noexcept;
    window& operator=(window&& other) noexcept;

    void resize(std::uint32_t width, std::uint32_t height);
    void change_limits(std::uint32_t min_width, std::uint32_t min_height, std::uint32_t max_width, std::uint32_t max_height);
    void move(std::int32_t relative_x, std::int32_t relative_y);
    void move_to(std::int32_t x, std::int32_t y);
    void move_to(const monitor& monitor, std::int32_t x, std::int32_t y);
    void hide();
    void show();
    void hide_cursor();
    void show_cursor();
    void grab_cursor();
    void release_cursor();
    void minimize();
    void maximize();
    void enable_resizing();
    void disable_resizing();
    void restore();
    void raise();
    void change_title(const std::string& title);
    void change_icon(const std::uint8_t* rgba, std::uint32_t width, std::uint32_t height);
    void change_opacity(float opacity);
    void switch_to_fullscreen();
    void switch_to_fullscreen(const monitor& monitor);
    void switch_to_windowed_fullscreen();
    void switch_to_windowed_fullscreen(const monitor& monitor);
    void switch_to_windowed();
    void switch_to_windowed(const monitor& monitor);

    id_type id() const noexcept;
    std::uint32_t width() const noexcept;
    std::uint32_t height() const noexcept;
    std::int32_t x() const noexcept;
    std::int32_t x(const monitor& monitor) const noexcept;
    std::int32_t y() const noexcept;
    std::int32_t y(const monitor& monitor) const noexcept;
    bool has_focus() const noexcept;
    bool is_minimized() const noexcept;
    bool is_maximized() const noexcept;
    const monitor& current_monitor() const noexcept;

    VkSurfaceKHR make_surface(VkInstance instance);

private:
    SDL_Window* m_window{};
    const monitor* m_monitors{};
};

}

#endif
