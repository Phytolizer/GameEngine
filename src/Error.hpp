#pragma once

#include <stdexcept>
#include <string_view>

class Error : public std::runtime_error
{
  public:
    Error(std::string_view message);
    const char *what() const noexcept;
};