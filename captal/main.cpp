#include <iostream>

#include "src/engine.hpp"
#include "src/texture.hpp"
#include "src/sprite.hpp"
#include "src/sound.hpp"
#include "src/view.hpp"
#include "src/physics.hpp"
#include "src/text.hpp"

#include "src/components/node.hpp"
#include "src/components/camera.hpp"
#include "src/components/drawable.hpp"
#include "src/components/audio_emiter.hpp"
#include "src/components/listener.hpp"
#include "src/components/physical_body.hpp"
#include "src/components/controller.hpp"

#include "src/systems/frame.hpp"
#include "src/systems/audio.hpp"
#include "src/systems/render.hpp"
#include "src/systems/physics.hpp"
#include "src/systems/sorting.hpp"

#include <apyre/messagebox.hpp>

using namespace cpt::enum_operations;

class sawtooth_generator : public swl::sound_reader
{
public:
    sawtooth_generator(std::uint32_t frequency, std::uint32_t channels, std::uint32_t wave_lenght)
    :m_frequency{frequency}
    ,m_channels{channels}
    ,m_wave_lenght{wave_lenght}
    {

    }

    ~sawtooth_generator() = default;
    sawtooth_generator(const sawtooth_generator&) = delete;
    sawtooth_generator& operator=(const sawtooth_generator&) = delete;
    sawtooth_generator(sawtooth_generator&& other) noexcept = default;
    sawtooth_generator& operator=(sawtooth_generator&& other) noexcept = default;

    bool read(float* output, std::size_t frame_count) override
    {
        for(std::size_t i{}; i < frame_count; ++i)
        {
            const float value{next_value()};

            for(std::size_t j{}; j < m_channels; ++j)
            {
                output[i * m_channels + j] = value;
            }
        }

        return true;
    }

    std::uint64_t frame_count() override
    {
        return std::numeric_limits<std::uint64_t>::max();
    }

    std::uint32_t frequency() override
    {
        return m_frequency;
    }

    std::uint32_t channel_count() override
    {
        return m_channels;
    }

private:
    float next_value() noexcept
    {
        //Pretty easy implementation
        m_value += 2.0f / static_cast<float>(m_wave_lenght);

        if(m_value >= 1.0f)
        {
            m_value -= 2.0f;
        }

        return m_value;
    }

private:
    float m_value{-1.0f};
    std::uint32_t m_frequency{};
    std::uint32_t m_channels{};
    std::uint32_t m_wave_lenght{};
};

static constexpr const char sansation_regular_font_data[]
{
    #include "Sansation_Regular.ttf.txt"
};

static constexpr cpt::collision_type_t player_type{1};
static constexpr cpt::collision_type_t wall_type{2};

static entt::entity add_physics(entt::registry& world, const cpt::physical_world_ptr& physical_world)
{
    //A background (to slighlty increase scene's complexity)
    const auto background_entity{world.create()};
    world.assign<cpt::components::node>(background_entity, glm::vec3{0.0f, 0.0f, 0.0f});
    world.assign<cpt::components::drawable>(background_entity, cpt::make_sprite(640, 480, cpt::colors::yellowgreen));

    //Add some squares to the scene
    constexpr std::array<glm::vec2, 4> positions{glm::vec2{200.0f, 140.0f}, glm::vec2{540.0f, 140.0f}, glm::vec2{200.0f, 340.0f}, glm::vec2{540.0f, 340.0f}};
    for(auto&& position : positions)
    {
        cpt::physical_body_ptr sprite_body{cpt::make_physical_body(physical_world, cpt::physical_body_type::dynamic)};
        sprite_body->set_position(position);

        const auto item{world.create()};
        world.assign<cpt::components::node>(item, glm::vec3{position, 0.5f}, glm::vec3{12.0f, 12.0f, 0.0f});
        world.assign<cpt::components::drawable>(item, cpt::make_sprite(24, 24, cpt::colors::blue));
        world.assign<cpt::components::physical_body>(item, std::move(sprite_body)).add_shape(24.0f, 24.0f);
    }

    //Walls are placed at window's limits
    auto walls{world.create()};
    auto& walls_body{world.assign<cpt::components::physical_body>(walls)};
    walls_body.attach(cpt::make_physical_body(physical_world, cpt::physical_body_type::steady));
    walls_body.add_shape(glm::vec2{0.0f, 0.0f},   glm::vec2{0.0f, 480.0f}  )->set_collision_type(wall_type);
    walls_body.add_shape(glm::vec2{0.0f, 0.0f},   glm::vec2{640.0f, 0.0f}  )->set_collision_type(wall_type);
    walls_body.add_shape(glm::vec2{640.0f, 0.0f}, glm::vec2{640.0f, 480.0f})->set_collision_type(wall_type);
    walls_body.add_shape(glm::vec2{0.0f, 480.0f}, glm::vec2{640.0f, 480.0f})->set_collision_type(wall_type);

    //The player
    cpt::physical_body_ptr player_physical_body{cpt::make_physical_body(physical_world, cpt::physical_body_type::dynamic)};
    player_physical_body->set_position(glm::vec2{320.0f, 240.0f});

    const auto player{world.create()};
    world.assign<cpt::components::node>(player, glm::vec3{320.0f, 240.0f, 0.5f}, glm::vec3{16.0f, 16.0f, 0.0f});
    world.assign<cpt::components::drawable>(player, cpt::make_sprite(32, 32, cpt::colors::red));
    world.assign<cpt::components::physical_body>(player, player_physical_body).add_shape(32.0f, 32.0f)->set_collision_type(player_type);
    world.assign<cpt::components::audio_emiter>(player, cpt::make_sound(std::make_unique<sawtooth_generator>(44100, 2, 240)))->set_volume(0.5f);
    auto& controller{world.assign<cpt::components::controller>(player, cpt::physical_body_weak_ptr{player_physical_body})};

    auto& pivot{controller.add_constraint(cpt::pivot_joint, glm::vec2{}, glm::vec2{})};
    pivot->set_max_bias(0.0f);
    pivot->set_max_force(10000.0f);

    auto& gear{controller.add_constraint(cpt::gear_joint, 0.0f, 1.0f)};
    gear->set_error_bias(0.0f);
    gear->set_max_bias(1.0f);
    gear->set_max_force(10000.0f);

    //This set of variables is required to control the player entity
    return player;
}

static void add_logic(const cpt::render_window_ptr& window, entt::registry& world, const cpt::physical_world_ptr& physical_world, entt::entity camera)
{
    cpt::text_drawer drawer{cpt::font{std::string_view{std::data(sansation_regular_font_data), std::size(sansation_regular_font_data)}, 24}};

    const auto text{world.create()};
    world.assign<cpt::components::node>(text, glm::vec3{4.0f, 4.0f, 1.0f});
    world.assign<cpt::components::drawable>(text);

    //Display current FPS in window title, and GPU memory usage (only memory allocated using Tephra's renderer's allocator)
    cpt::engine::instance().frame_per_second_update_signal().connect([&world, text, drawer = std::move(drawer)](std::uint32_t frame_per_second) mutable
    {
        const auto format_data = [](std::size_t amount)
        {
            std::stringstream ss{};
            ss << std::setprecision(2);

            if(amount < 1024)
            {
                ss << amount << " o";
            }
            else if(amount < 1024 * 1024)
            {
                ss << std::fixed << static_cast<double>(amount) / 1024.0 << " kio";
            }
            else
            {
                ss << std::fixed << static_cast<double>(amount) / (1024.0 * 1024.0) << " Mio";
            }

            return ss.str();
        };

        cpt::engine::instance().renderer().free_memory();

        //Just some info displayed at the end of the demo.
        const auto memory_used{cpt::engine::instance().renderer().allocator().used_memory()};
        const auto memory_alloc{cpt::engine::instance().renderer().allocator().allocated_memory()};

        std::string info{};
        info += "Device local : " + format_data(memory_used.device_local) + " / " + format_data(memory_alloc.device_local) + "\n";
        info += "Host shared : " + format_data(memory_used.host_shared) + " / " + format_data(memory_alloc.host_shared) + "\n";
        info += std::to_string(frame_per_second) + " FPS";

        world.get<cpt::components::drawable>(text).attach(drawer.draw(info, cpt::colors::black));
        world.get<cpt::components::node>(text).update();
    });

    //Add a zoom support
    window->on_mouse_wheel_scroll().connect([&world, camera](const apr::mouse_event& event)
    {
        if(event.wheel > 0)
        {
            world.get<cpt::components::node>(camera).scale(1.0f / 2.0f);
        }
        else
        {
            world.get<cpt::components::node>(camera).scale(2.0f / 1.0f);
        }
    });

    //These booleans will holds WASD keys states for player's movements
    //We store this information as a booleans array because we want smooth movements.
    std::shared_ptr<bool[]> pressed_keys{new bool[4]{}};

    //Check what keys have been pressed.
    window->on_key_pressed().connect([pressed_keys](const apr::keyboard_event& event)
    {
        if(event.scan == apr::scancode::d) pressed_keys[0] = true;
        if(event.scan == apr::scancode::s) pressed_keys[1] = true;
        if(event.scan == apr::scancode::a) pressed_keys[2] = true;
        if(event.scan == apr::scancode::w) pressed_keys[3] = true;
    });

    //Check what keys have been released.
    window->on_key_released().connect([pressed_keys](const apr::keyboard_event& event)
    {
        if(event.scan == apr::scancode::d) pressed_keys[0] = false;
        if(event.scan == apr::scancode::s) pressed_keys[1] = false;
        if(event.scan == apr::scancode::a) pressed_keys[2] = false;
        if(event.scan == apr::scancode::w) pressed_keys[3] = false;
    });

    //Add all physics to the world.
    auto player{add_physics(world, physical_world)};

    //Add some physics based behaviour.
    //The player can collide with multiple walls at the same time, so we don't use a boolean but an integer.
    auto current_collisions{std::make_shared<std::uint32_t>()};

    //Contains all callbacks for collision handling.
    cpt::physical_world::collision_handler collision_handler{};

    //We don't use the parameters.
    collision_handler.collision_begin = [&world, player, current_collisions](auto&, auto&, auto&, auto, auto*)
    {
        *current_collisions += 1;

        //Start the sawtooth when we first collide.
        if(*current_collisions == 1)
        {
            world.get<cpt::components::audio_emiter>(player)->start();
        }

        return true;
    };

    collision_handler.collision_end = [&world, player, current_collisions](auto&, auto&, auto&, auto, auto*)
    {
        *current_collisions -= 1;

        //Stop the sawtooth when we no longer collide with any wall.
        if(*current_collisions == 0)
        {
            world.get<cpt::components::audio_emiter>(player)->stop();
        }

        return true;
    };

    //When the player and a wall collide, our callbacks will be called.
    physical_world->add_collision(player_type, wall_type, std::move(collision_handler));

    //This signal will be called withing cpt::engine::instanc()::run()
    //We could have write this code inside the main loop instead.
    cpt::engine::instance().on_update().connect([pressed_keys, physical_world, &world, player](float time)
    {
        glm::vec2 new_velocity{};

        if(pressed_keys[0]) new_velocity += glm::vec2{256.0f, 0.0f};
        if(pressed_keys[1]) new_velocity += glm::vec2{0.0f, 256.0f};
        if(pressed_keys[2]) new_velocity += glm::vec2{-256.0f, 0.0f};
        if(pressed_keys[3]) new_velocity += glm::vec2{0.0f, -256.0f};

        //Update player controller based on user inputs.
        world.get<cpt::components::controller>(player)->set_velocity(new_velocity);
        //Update the physical world with the elapsed time.
        physical_world->update(time);
    });
}

static void run()
{
    //-The first value is the window width (640 pixels). (no default value)
    //-The second value is the window height (480 pixels). (no default value)
    //-The third value is the number of image in the swapchain of the window's surface. (default: 2)
    //    2 means double buffering, 3 triple buffering and so on.
    //    This value is limited in a specific interval by the implementation, but 2 is one of the commonest value and should work everywhere.
    //-The present mode defines window behaviour on presentation. (default: tph::present_mode::fifo)
    //    FIFO is the only one available on any hardware (cf. Vulkan Specification), and correspond to V-Sync.
    //-The sample count enable MSAA (MultiSample Anti-Aliasing). (default: tph::sample_count::msaa_x1)
    //    MSAA will smoother the edges of polygons rendered in the window.
    //    MSAAx4 and no MSAA (MSAAx1), are always available (cf. Vulkan Specification)
    //-The texture format is given to enable depth buffering.
    //    Depth format "d32_sfloat" is widely available, so it is hardcoded. But in real world appliction you should check for it's availability.
    constexpr cpt::video_mode video_mode{640, 480, 2, tph::present_mode::fifo, tph::sample_count::msaa_x4, tph::texture_format::d32_sfloat};

    //Create the window
    cpt::render_window_ptr window{cpt::engine::instance().make_window("Captal test", video_mode, apr::window_options::resizable)};
    //Clear color is a part of tph::render_target, returned by cpt::render_target::get_target()
    window->set_clear_color(cpt::colors::white);

    //Our world. Captal does not provide ECS (Entity Components Systems), but is designed to work with Entt.
    //Check out how Entt works on its Github repo: https://github.com/skypjack/entt
    entt::registry world{};

    //Since we use multisampling, we must use a compatible pipeline.
    //A render technique describes how a view will render the scene it seens.
    //Here we need to turn on multisampling within the technique's pipeline...
    cpt::render_technique_info technique_info{};
    technique_info.multisample.sample_count = tph::sample_count::msaa_x4;
    //And also depth buffering.
    technique_info.depth_stencil.depth_test = true;
    technique_info.depth_stencil.depth_write = true;
    technique_info.depth_stencil.depth_compare_op = tph::compare_op::greater_or_equal;

    //Our camera, it will hold the cpt::view for our scene.
    const auto camera{world.create()};
    world.assign<cpt::components::node>(camera, glm::vec3{320.0f, 240.0f, 1.0f}, glm::vec3{320.0f, 240.0f, 0.0f});
    world.assign<cpt::components::camera>(camera, cpt::make_view(window, technique_info))->fit_to(window);

    //Our physical world. See add_logic() function for more informations.
    cpt::physical_world_ptr physical_world{cpt::make_physical_world()};
    //Physical world's objects' velocity will be multiplied by the damping each second.
    //So more the damping is low, less physical world's objects will preserve their velocity.
    physical_world->set_damping(0.1f);
    //Sleep threshold will put idle objects asleep if they didn't move for more than the specified time. This will increase perfomances.
    physical_world->set_sleep_threshold(0.5f);

    //See above.
    add_logic(window, world, physical_world, camera);

    //The game engine will return true if there is at least one window opened.
    //Run function will update all windows created within the engine and process their events.
    //It also trigger on_update().
    //It also keeps track of the elapsed time within to call of run().
    //This function is usually be used as the main loop of your game.
    while(cpt::engine::instance().run())
    {
        //Physics system will update objects' nodes based on value given by the physical world.
        //Will call this system first so other systems will have the newest positions data.
        cpt::systems::physics(world);

        //Audio systems will update objects' and listener' position withing the audio world.
        cpt::systems::audio(world);

        //We must not present a window that has rendering disabled.
        //Window rendering may be disabled when it is closed or minimized.
        if(window->is_rendering_enable())
        {
            //Render system will update all views within the world,
            //and draw all drawable items to all render targets associated to the views.
            cpt::systems::render(world);

            //This will update window's swapchain.
            //Doing so will put the newly drawn image in the rendering queue of the system's presentation engine.
            //It will then display it on screen. It's accual behaviour will depends on window presention mode.
            window->present();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }

        //End frame system marks the end of the current frame.
        //It will reset some states within the world to prepare it for a new frame.
        cpt::systems::end_frame(world);
    }
}

int main()
{
    try
    {
        //The first value is the number of channels, 2 is stereo.
        //The second value is the frequency of the output stream.
        const cpt::audio_parameters audio{2, 44100};
        //The first value is the renderer options (c.f. tph::renderer_options)
        const cpt::graphics_parameters graphics{tph::renderer_options::tiny_memory_heaps};

        //The engine instance. It must be created before most call to captal functions.
        //The first value is your application name. It will be passed to Tephra's instance then to Vulkan's instance.
        //The second value is your application version. It will be passed to Tephra's instance then to Vulkan's instance.
        cpt::engine engine{"captal_test", cpt::version{0, 1, 0}, audio, graphics};

        //The engine is reachable by its static address. We don't need to keep a reference on it.
        run();
    }
    catch(const std::exception& e)
    {
        apr::message_box(apr::message_box_type::error, "Error", "An exception as been throw:\n" + std::string{e.what()});
    }
    catch(...)
    {
        apr::message_box(apr::message_box_type::error, "Unknown error", "Exception's type does not inherit from std::exception");
    }
}
