#ifndef CAPTAL_ZLIB_HPP_INCLUDED
#define CAPTAL_ZLIB_HPP_INCLUDED

#include "config.hpp"

#include <memory>
#include <array>
#include <iterator>

struct z_stream_s;

namespace cpt
{

namespace impl
{

class deflate
{
public:
    static constexpr std::uint32_t default_compression{6};
    static constexpr bool flush{true};
    static constexpr bool known_compress_bound{true};

public:
    constexpr deflate() noexcept = default;
    deflate(std::uint32_t compression_level, std::int32_t window_bits);
    ~deflate();
    deflate(const deflate&) = delete;
    deflate& operator=(const deflate&) = delete;
    deflate(deflate&&) noexcept = default;
    deflate& operator=(deflate&&) noexcept = default;

    template<typename InContiguousIt, typename OutContiguousIt>
    bool compress(InContiguousIt& input_begin, InContiguousIt input_end, OutContiguousIt& output_begin, OutContiguousIt output_end, bool flush = false)
    {
        static_assert(sizeof(typename std::iterator_traits<InContiguousIt>::value_type) == 1, "cpt::deflate::compress only works on bytes.");
        static_assert(sizeof(typename std::iterator_traits<OutContiguousIt>::value_type) == 1, "cpt::deflate::compress only works on bytes.");

        const std::uint8_t* const input_address{reinterpret_cast<const std::uint8_t*>(std::addressof(*input_begin))};
        const std::uint8_t* input_ptr{input_address};
        const std::size_t input_size{static_cast<std::size_t>(std::distance(input_begin, input_end))};

        std::uint8_t* const output_address{reinterpret_cast<std::uint8_t*>(std::addressof(*output_begin))};
        std::uint8_t* output_ptr{output_address};
        const std::size_t output_size{static_cast<std::size_t>(std::distance(output_begin, output_end))};

        if(!compress_impl(input_ptr, input_size, output_ptr, output_size, flush))
            return false;

        std::advance(input_begin, input_ptr - input_address);
        std::advance(output_begin, output_ptr - output_address);

        return true;
    }

    std::size_t compress_bound(std::size_t input_size) const noexcept;

    template<typename InputIt>
    std::size_t compress_bound(InputIt begin, InputIt end) const
    {
        static_assert(sizeof(typename std::iterator_traits<InputIt>::value_type) == 1, "cpt::deflate::compress only works on bytes.");

        return compress_bound(static_cast<std::size_t>(std::distance(begin, end)));
    }

private:
    bool compress_impl(const std::uint8_t*& input, std::size_t input_size, std::uint8_t*& output, std::size_t output_size, bool finish);

private:
    std::unique_ptr<z_stream_s> m_stream{};
};

}

class deflate : public impl::deflate
{
public:
    constexpr deflate() noexcept = default;

    deflate(std::uint32_t compression_level)
    :impl::deflate{compression_level, -15}
    {

    }

    ~deflate() = default;
    deflate(const deflate&) = delete;
    deflate& operator=(const deflate&) = delete;
    deflate(deflate&&) noexcept = default;
    deflate& operator=(deflate&&) noexcept = default;
};

class zlib_deflate : public impl::deflate
{
public:
    constexpr zlib_deflate() noexcept = default;

    zlib_deflate(std::uint32_t compression_level)
    :impl::deflate{compression_level, 15}
    {

    }

    ~zlib_deflate() = default;
    zlib_deflate(const zlib_deflate&) = delete;
    zlib_deflate& operator=(const zlib_deflate&) = delete;
    zlib_deflate(zlib_deflate&&) noexcept = default;
    zlib_deflate& operator=(zlib_deflate&&) noexcept = default;
};

class gzip_deflate : public impl::deflate
{
public:
    constexpr gzip_deflate() noexcept = default;

    gzip_deflate(std::uint32_t compression_level)
    :impl::deflate{compression_level, 15 + 16}
    {

    }

    ~gzip_deflate() = default;
    gzip_deflate(const gzip_deflate&) = delete;
    gzip_deflate& operator=(const gzip_deflate&) = delete;
    gzip_deflate(gzip_deflate&&) noexcept = default;
    gzip_deflate& operator=(gzip_deflate&&) noexcept = default;
};

}

#endif
