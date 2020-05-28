#ifndef APYRE_MESSAGEBOX_HPP_INCLUDED
#define APYRE_MESSAGEBOX_HPP_INCLUDED

#include "config.hpp"

#include <string>
#include <vector>

namespace apr
{

class window;

enum class message_box_type : std::uint32_t
{
    error = 1,
    warning = 2,
    information = 4
};

enum class message_box_button_bind : std::uint32_t
{
    no_key = 0,
    return_key = 1,
    escape_key = 2
};

struct message_box_button
{
    std::uint32_t id{};
    std::string text{};
    message_box_button_bind bind{};
};

APYRE_API std::uint32_t message_box(message_box_type type, const std::string& title, const std::string& message, const std::vector<message_box_button>& buttons);
APYRE_API std::uint32_t message_box(window& window, message_box_type type, const std::string& title, const std::string& message, const std::vector<message_box_button>& buttons);
APYRE_API void message_box(message_box_type type, const std::string& title, const std::string& message);
APYRE_API void message_box(window& window, message_box_type type, const std::string& title, const std::string& message);

}

#endif
