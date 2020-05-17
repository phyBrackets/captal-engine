#include <iostream>
#include <fstream>

#include <captal/engine.hpp>
#include <captal/state.hpp>
#include <captal/tiled_map.hpp>

#include "states/splash_screen.hpp"

static void run()
{
    cpt::render_window_ptr window{cpt::engine::instance().make_window("My project, the real one", cpt::video_mode{860, 480}, apr::window_options::resizable)};
    window->change_limits(640, 360, 1024, std::numeric_limits<std::uint32_t>::max());

    cpt::engine::instance().frame_per_second_update_signal().connect([&window](std::uint32_t frame_per_second)
    {
        window->change_title("My project, the real one - " + std::to_string(frame_per_second) + " FPS");
    });

    cpt::state_stack states{cpt::make_state<mpr::states::splash_screen>(window)};

    while(cpt::engine::instance().run())
    {
        states.update(cpt::engine::instance().frame_time());

        if(window->is_rendering_enable())
            window->present();
    }
}

int main()
{
    try
    {
        const cpt::audio_parameters audio{2, 44100};
        const cpt::graphics_parameters graphics{tph::renderer_options::tiny_memory_heaps};
        cpt::engine engine{"my_project", mpr::game_version, audio, graphics};

        run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
