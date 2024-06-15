#pragma once

#include <cstdint>
#include <cassert>
#include <string>

struct Position
{
    bool is_valid() const;
    int get_index() const;

    std::string to_string() const
    {
        assert(is_valid());
        return std::string(1, 'a' + x) + std::to_string(y);
    }

    Position operator +(Position other) const
    {
        other.x += x;
        other.y += y;
        return other;
    }

    Position operator *(int value) const
    {
        Position result = *this;
        result.x *= value;
        result.y *= value;
        return result;
    }

    bool operator ==(Position other) const
    {
        assert(is_valid());
        assert(other.is_valid());
        return x == other.x && y == other.y;
    }

    static Position parse(char x, char y)
    {
        x = tolower(x);

        Position position;
        position.x = x - 'a';
        position.y = y - '0';
        return position;
    }

    int8_t x = -1;
    int8_t y = -1;
};
