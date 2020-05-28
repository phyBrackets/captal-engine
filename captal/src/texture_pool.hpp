#ifndef CAPTAL_TEXTURE_POOL_HPP_INCLUDED
#define CAPTAL_TEXTURE_POOL_HPP_INCLUDED

#include "config.hpp"

#include <functional>
#include <unordered_map>
#include <filesystem>

#include "texture.hpp"

namespace cpt
{

class CAPTAL_API texture_pool
{
    struct path_hash
    {
        std::size_t operator()(const std::filesystem::path& path) const noexcept
        {
            return std::filesystem::hash_value(path);
        }
    };

public:
    static cpt::texture_ptr default_load_callback(const std::filesystem::path& path, const tph::sampling_options& sampling, color_space space);

public:
    using load_callback_t = std::function<cpt::texture_ptr(const std::filesystem::path& path, const tph::sampling_options& sampling, color_space space)>;

public:
    texture_pool() noexcept;
    texture_pool(load_callback_t load_callback) noexcept;
    ~texture_pool() = default;
    texture_pool(const texture_pool&) = delete;
    texture_pool& operator=(const texture_pool&) = default;
    texture_pool(texture_pool&&) noexcept = default;
    texture_pool& operator=(texture_pool&&) noexcept = default;

    cpt::texture_ptr load(const std::filesystem::path& path, const tph::sampling_options& sampling = tph::sampling_options{}, color_space space = color_space::srgb);
    cpt::texture_ptr load(const std::filesystem::path& path, const load_callback_t& load_callback, const tph::sampling_options& sampling = tph::sampling_options{}, color_space space = color_space::srgb);
    cpt::texture_weak_ptr weak_load(const std::filesystem::path& path) const;
    std::pair<cpt::texture_ptr, bool> emplace(std::filesystem::path path, texture_ptr texture);

    void clear(std::size_t threshold = 1);

    template<typename Predicate>
    void clear_if(Predicate predicate)
    {
        auto it = std::begin(m_pool);
        while(it != std::end(m_pool))
        {
            if(predicate(it->first, it->second))
            {
                it = m_pool.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void remove(const std::filesystem::path& path);
    void remove(const texture_ptr& texture);

    void set_load_callback(load_callback_t new_callback)
    {
        m_load_callback = std::move(new_callback);
    }

    const load_callback_t& load_callback() const noexcept
    {
        return m_load_callback;
    }

private:
    std::unordered_map<std::filesystem::path, texture_ptr, path_hash> m_pool{};
    load_callback_t m_load_callback{};
};

}

#endif
