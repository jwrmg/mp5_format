#pragma once
#include <string>

#include "mp5_value_type.h"

namespace mp5
{
    class mp5_string
    {
    public:
        std::string content;

        /**
         * \brief removes the leading whitespace.
         * \return whitespace removed from leading side.
         */
        std::string l_trim();
        /**
         * \brief removes the the trailing whitespace.
         * \return whitespace removed from trailing side.
         */
        std::string r_trim();
        /**
         * \brief removes the trailing and leading whitespace.
         * \return whitespace removed from both leading & trailing sides.
         */
        std::string trim();

        bool is_float() const;
        bool is_double() const;
        bool is_integer() const;
        bool is_boolean() const;
        bool is_string() const;

        mp5_value_type get_type() const;

        mp5_string();
        mp5_string(const std::string& rhs);
        mp5_string(const char* rhs);

        mp5_string operator+(const mp5_string& rhs) const;
        mp5_string operator+=(const mp5_string& rhs);
        mp5_string operator<<(const mp5_string& rhs);
        bool operator==(const mp5_string& rhs) const;
        char operator[](unsigned int index) const;

        struct hash
        {
            size_t operator()(const mp5_string& rhs) const
            {
                return std::hash<std::string>{}(rhs.content);
            }
        };
    };
}
