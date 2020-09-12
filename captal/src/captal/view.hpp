#ifndef CAPTAL_VIEW_HPP_INCLUDED
#define CAPTAL_VIEW_HPP_INCLUDED

#include "config.hpp"

#include <vector>
#include <numbers>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "asynchronous_resource.hpp"
#include "uniform_buffer.hpp"
#include "binding.hpp"
#include "render_technique.hpp"
#include "render_window.hpp"
#include "render_texture.hpp"

namespace cpt
{

enum class view_type
{
    orthographic = 0,
};

class CAPTAL_API view
{
    struct view_impl final : public asynchronous_resource
    {
        view_impl() = default;

        view_impl(uniform_buffer&& ubo)
        :buffer{std::move(ubo)}
        {

        }

        ~view_impl() = default;
        view_impl(const view_impl&) = delete;
        view_impl& operator=(const view_impl&) = delete;
        view_impl(view_impl&& other) noexcept = default;
        view_impl& operator=(view_impl&& other) noexcept = default;

        uniform_buffer buffer{};
        std::unordered_map<std::uint32_t, cpt::binding> bindings{};
        render_technique_ptr render_technique{};
    };

public:
    struct uniform_data
    {
        glm::vec4 position{};
        glm::mat4 view{1.0f};
        glm::mat4 projection{1.0f};
    };

public:
    view();
    view(const render_target_ptr& target, const render_technique_info& info = render_technique_info{});
    view(const render_target_ptr& target, render_technique_ptr technique);

    ~view() = default;
    view(const view&) = delete;
    view& operator=(const view&) = delete;
    view(view&&) noexcept = default;
    view& operator=(view&&) noexcept = default;

    void set_viewport(const tph::viewport& viewport) noexcept
    {
        m_viewport = viewport;
    }

    void set_scissor(const tph::scissor& scissor) noexcept
    {
        m_scissor = scissor;
    }

    void move_to(const glm::vec3& position) noexcept
    {
        m_position = position;
        update();
    }

    void move(const glm::vec3& relative) noexcept
    {
        m_position += relative;
        update();
    }

    void set_origin(const glm::vec3& origin) noexcept
    {
        m_origin = origin;
        update();
    }

    void move_origin(const glm::vec3& relative) noexcept
    {
        m_origin += relative;
        update();
    }

    void resize(float witdh, float height) noexcept
    {
        m_size = glm::vec2{witdh, height};
        update();
    }

    void set_z_near(float z_near) noexcept
    {
        m_z_near = z_near;
        update();
    }

    void set_z_far(float z_far) noexcept
    {
        m_z_far = z_far;
        update();
    }

    void set_rotation(float angle) noexcept
    {
        m_rotation = angle;
        update();
    }

    void rotate(float angle) noexcept
    {
        m_rotation = std::fmod(m_rotation + angle, std::numbers::pi_v<float> * 2.0f);
        update();
    }

    void set_scale(const glm::vec3& scale) noexcept
    {
        m_scale = scale;
        update();
    }

    void scale(const glm::vec3& scale) noexcept
    {
        m_scale *= scale;
        update();
    }

    void set_render_technique(render_technique_ptr technique) noexcept
    {
        m_impl->render_technique = std::move(technique);
        update_uniforms();
    }

    void set_target(const render_target_ptr& target, const render_technique_info& info = render_technique_info{});
    void set_target(const render_target_ptr& target, render_technique_ptr technique);
    void fit_to(const render_texture_ptr& window);
    void fit_to(const render_window_ptr& texture);

    template<typename T>
    void set_push_constant(std::size_t index, T&& value) noexcept
    {
        get_push_constant<T>(index) = std::forward<T>(value);
    }

    cpt::binding& add_binding(std::uint32_t index, cpt::binding binding);
    void set_binding(std::uint32_t index, cpt::binding new_binding);

    void update() noexcept
    {
        m_need_upload = true;
    }

    void update_uniforms() noexcept
    {
        m_need_descriptor_update = true;
    }

    void upload();

    const tph::viewport& viewport() const noexcept
    {
        return m_viewport;
    }

    const tph::scissor& scissor() const noexcept
    {
        return m_scissor;
    }

    const glm::vec3& position() const noexcept
    {
        return m_position;
    }

    const glm::vec3& origin() const noexcept
    {
        return m_origin;
    }

    float width() const noexcept
    {
        return m_size.x;
    }

    float height() const noexcept
    {
        return m_size.y;
    }

    float z_near() const noexcept
    {
        return m_z_near;
    }

    float z_far() const noexcept
    {
        return m_z_far;
    }

    float rotation() const noexcept
    {
        return m_rotation;
    }

    const glm::vec3& scale() const noexcept
    {
        return m_scale;
    }

    view_type type() const noexcept
    {
        return m_type;
    }

    const render_technique_ptr& render_technique() const noexcept
    {
        return m_impl->render_technique;
    }

    render_target& target() const noexcept
    {
        return *m_target;
    }

    tph::buffer& get_buffer() noexcept
    {
        return m_impl->buffer.get_buffer();
    }

    const tph::buffer& get_buffer() const noexcept
    {
        return m_impl->buffer.get_buffer();
    }

    template<typename T>
    T& get_push_constant(std::size_t index) noexcept
    {
        static_assert(alignof(T) <= 4, "Alignment of T is too big.");

        const auto range{m_impl->render_technique->ranges()[index]};
        assert(range.size == sizeof(T) && "Size of T does not match range size.");

        return *std::launder(reinterpret_cast<T*>(std::data(m_push_constant_buffer) + range.offset / 4u));
    }

    template<typename T>
    const T& get_push_constant(std::size_t index) const noexcept
    {
        static_assert(alignof(T) <= 4, "Alignment of T is too big.");

        const auto range{m_impl->render_technique->ranges()[index]};
        assert(range.size == sizeof(T) && "Size of T does not match range size.");

        return *std::launder(reinterpret_cast<const T*>(std::data(m_push_constant_buffer) + range.offset / 4u));
    }

    cpt::binding& binding(std::uint32_t index)
    {
        return m_impl->bindings.at(index);
    }

    const cpt::binding& binding(std::uint32_t index) const
    {
        return m_impl->bindings.at(index);
    }

    bool has_binding(std::uint32_t index) const
    {
        return m_impl->bindings.find(index) != std::end(m_impl->bindings);
    }

    std::unordered_map<std::uint32_t, cpt::binding>& bindings() noexcept
    {
        return m_impl->bindings;
    }

    const std::unordered_map<std::uint32_t, cpt::binding>& bindings() const noexcept
    {
        return m_impl->bindings;
    }

    bool need_descriptor_update(bool new_value = false) noexcept
    {
        return std::exchange(m_need_descriptor_update, new_value);
    }

    const std::vector<std::uint32_t>& push_constant_buffer() const noexcept
    {
        return m_push_constant_buffer;
    }

    asynchronous_resource_ptr resource() const noexcept
    {
        return m_impl;
    }

private:
    render_target* m_target{};

    tph::viewport m_viewport{};
    tph::scissor m_scissor{};
    glm::vec3 m_position{};
    glm::vec3 m_origin{};
    glm::vec2 m_size{};
    float m_z_near{1.0f};
    float m_z_far{0.0f};
    glm::vec3 m_scale{1.0f};
    float m_rotation{};
    view_type m_type{};

    bool m_need_upload{true};
    bool m_need_descriptor_update{};

    std::vector<std::uint32_t> m_push_constant_buffer{};
    std::shared_ptr<view_impl> m_impl{};
};

}

#endif
