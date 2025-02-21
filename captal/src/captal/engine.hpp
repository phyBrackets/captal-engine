//MIT License
//
//Copyright (c) 2021 Alexy Pellegrini
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#ifndef CAPTAL_ENGINE_HPP_INCLUDED
#define CAPTAL_ENGINE_HPP_INCLUDED

#include "config.hpp"

#include <optional>
#include <memory>

#include <swell/stream.hpp>
#include <swell/audio_pulser.hpp>

#include <tephra/renderer.hpp>
#include <tephra/commands.hpp>
#include <tephra/shader.hpp>

#include "signal.hpp"
#include "application.hpp"
#include "render_window.hpp"
#include "memory_transfer.hpp"
#include "buffer_pool.hpp"
#include "render_technique.hpp"
#include "translation.hpp"
#include "font.hpp"

namespace cpt
{

struct system_parameters
{
    apr::application_extension extensions{};
};

struct audio_parameters
{
    std::uint32_t channel_count{};
    std::uint32_t frequency{};
    swl::seconds minimum_latency{0.010};
    swl::seconds resync_threshold{0.050};
    optional_ref<const swl::physical_device> physical_device{};
};

struct graphics_parameters
{
    tph::renderer_options options{};
    tph::renderer_layer layers{};
    tph::renderer_extension extensions{};
    tph::physical_device_features features{};
    optional_ref<const tph::physical_device> physical_device{};
};

using update_signal = cpt::signal<float>;
using frame_per_second_signal = cpt::signal<std::uint32_t>;

class CAPTAL_API engine
{
    static engine* m_instance;

public:
    static constexpr std::uint32_t no_frame_rate_limit{std::numeric_limits<std::uint32_t>::max()};

public:
    explicit engine(const std::string& application_name, cpt::version version);
    explicit engine(const std::string& application_name, cpt::version version, const system_parameters& system, const audio_parameters& audio, const graphics_parameters& graphics);
    explicit engine(cpt::application application, const system_parameters& system, const audio_parameters& audio, const graphics_parameters& graphics);

    ~engine();
    engine(const engine&) = delete;
    engine& operator=(const engine&) = delete;
    engine(engine&& other) noexcept = delete;
    engine& operator=(engine&& other) noexcept = delete;

    void set_framerate_limit(std::uint32_t frame_per_second) noexcept;
    void set_translator(cpt::translator new_translator);
    void set_default_render_layout(render_layout_ptr new_default_render_layout) noexcept;
    void set_default_vertex_shader(tph::shader new_default_vertex_shader) noexcept;
    void set_default_fragment_shader(tph::shader new_default_fragment_shader) noexcept;

    memory_transfer_info begin_transfer();
    void submit_transfers();

    bool run();

    static engine& instance() noexcept;
    static const engine& cinstance() noexcept;

    cpt::application& application() noexcept
    {
        return m_application;
    }

    const cpt::application& application() const noexcept
    {
        return m_application;
    }

    const swl::physical_device& audio_device() const noexcept
    {
        return m_audio_device;
    }

    swl::audio_world& audio_world() noexcept
    {
        return m_audio_world;
    }

    const swl::audio_world& audio_world() const noexcept
    {
        return m_audio_world;
    }

    swl::audio_pulser& audio_pulser() noexcept
    {
        return m_audio_pulser;
    }

    const swl::audio_pulser& audio_pulser() const noexcept
    {
        return m_audio_pulser;
    }

    swl::listener& listener() noexcept
    {
        return *m_listener;
    }

    const swl::listener& listener() const noexcept
    {
        return *m_listener;
    }

    swl::stream& audio_stream() noexcept
    {
        return m_audio_stream;
    }

    const swl::stream& audio_stream() const noexcept
    {
        return m_audio_stream;
    }

    const tph::physical_device& graphics_device() const noexcept
    {
        return m_graphics_device;
    }

    tph::renderer& renderer() noexcept
    {
        return m_renderer;
    }

    const tph::renderer& renderer() const noexcept
    {
        return m_renderer;
    }

    memory_transfer_scheduler& transfer_scheduler() noexcept
    {
        return m_transfer_scheduler;
    }

    const memory_transfer_scheduler& transfer_scheduler() const noexcept
    {
        return m_transfer_scheduler;
    }

    buffer_pool& uniform_pool() noexcept
    {
        return m_uniform_pool;
    }

    const buffer_pool& uniform_pool() const noexcept
    {
        return m_uniform_pool;
    }

    update_signal& on_update() noexcept
    {
        return m_update_signal;
    }

    std::mutex& submit_mutex() noexcept
    {
        return m_queue_mutex;
    }

    tph::shader& default_vertex_shader() noexcept
    {
        return m_default_vertex_shader;
    }

    tph::shader& default_fragment_shader() noexcept
    {
        return m_default_fragment_shader;
    }

    const render_layout_ptr& default_render_layout() noexcept
    {
        return m_default_layout;
    }

    const cpt::translator& translator() const noexcept
    {
        return m_translator;
    }

    cpt::font_engine& font_engine() noexcept
    {
        return m_font_engine;
    }

    const cpt::font_engine& font_engine() const noexcept
    {
        return m_font_engine;
    }

    float frame_time() const noexcept
    {
        return m_frame_time;
    }

    std::uint32_t frame_per_second() const noexcept
    {
        return m_frame_per_second;
    }

    std::uint64_t frame() const noexcept
    {
        return m_frame_id;
    }

    frame_per_second_signal& frame_per_second_update_signal() noexcept
    {
        return m_frame_per_second_signal;
    }

private:
    void init();
    void update_frame();

private:
    cpt::application m_application;

    const swl::physical_device& m_audio_device;
    swl::audio_world m_audio_world;
    swl::audio_pulser m_audio_pulser;
    swl::listener_bind m_listener;
    swl::stream m_audio_stream;

    const tph::physical_device& m_graphics_device;
    tph::renderer m_renderer;

    buffer_pool m_uniform_pool;
    memory_transfer_scheduler m_transfer_scheduler;

    std::mutex m_queue_mutex{};
    tph::shader m_default_vertex_shader{};
    tph::shader m_default_fragment_shader{};
    render_layout_ptr m_default_layout{};

    cpt::translator m_translator{};
    cpt::font_engine m_font_engine{};

    std::chrono::steady_clock::time_point m_last_update{std::chrono::steady_clock::now()};
    float m_frame_time{};
    std::uint32_t m_frame_rate_limit{no_frame_rate_limit};
    float m_frame_per_second_timer{};
    std::uint32_t m_frame_per_second_counter{};
    std::uint32_t m_frame_per_second{};
    std::uint64_t m_frame_id{};
    frame_per_second_signal m_frame_per_second_signal{};
    update_signal m_update_signal{};
};

}

#endif
