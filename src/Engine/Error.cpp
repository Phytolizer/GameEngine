#include "Error.hpp"
Error::Error(std::string_view message) : std::runtime_error(message.data())
{
}
const char *Error::what() const noexcept
{
    return std::runtime_error::what();
}
