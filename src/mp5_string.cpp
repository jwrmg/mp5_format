#include "mp5_string.h"
#include <regex>

namespace mp5
{
    std::string mp5_string::l_trim()
    {
        return std::regex_replace(content, std::regex("^\\s+"), std::string(""));
    }

    std::string mp5_string::r_trim()
    {
        return std::regex_replace(content, std::regex("\\s+$"), std::string(""));
    }

    std::string mp5_string::trim()
    {
        content = r_trim();
        content = l_trim();
        return content;
    }

    bool mp5_string::is_float() const
    {
        try
        {
            std::stof(content.substr(0, content.size() - 1));
        }
        catch (...)
        {
            // silent error. (I don't want ill syntax to crash the application)
            return false;
        }
        return content[content.size() - 1] == 'f';
    }

    bool mp5_string::is_double() const
    {
        try
        {
            std::stod(content.substr(0, content.size() - 1));
        }
        catch (...)
        {
            // silent error. (I don't want ill syntax to crash the application)
            return false;
        }
        return content[content.size() - 1] == 'd';
    }

    bool mp5_string::is_integer() const
    {
        try
        {
            // if it passes, no exception will be thrown.
            std::stoi(content);
        }
        catch (...)
        {
            // silent error. (I don't want ill syntax to crash the application)
            return false;
        }
        return true;
    }

    bool mp5_string::is_boolean() const
    {
        return content == "true" || content == "false";
    }

    bool mp5_string::is_string() const
    {
        return content[0] == '"' && content[content.size() - 1] == '"';
    }

    mp5_value_type mp5_string::get_type() const
    {
        if (is_boolean())
        {
            return mp5_value_type::t_boolean;
        }
        if (is_string())
        {
            return mp5_value_type::t_string;
        }
        if (is_double())
        {
            return mp5_value_type::t_double;
        }
        if (is_float())
        {
            return mp5_value_type::t_float;
        }
        if (is_integer())
        {
            return mp5_value_type::t_int;
        }
        return mp5_value_type::t_invalid;
    }

    mp5_string::mp5_string()
    {
        content = "";
    }

    mp5_string::mp5_string(const std::string& rhs)
    {
        content = rhs;
    }

    mp5_string::mp5_string(const char* rhs)
    {
        content = rhs;
    }

    mp5_string mp5_string::operator+(const mp5_string& rhs) const
    {
        return content + rhs.content;
    }
    mp5_string mp5_string::operator+=(const mp5_string& rhs)
    {
        return content += rhs.content;
    }

    mp5_string mp5_string::operator<<(const mp5_string& rhs)
    {
        return content += rhs.content;
    }

    bool mp5_string::operator==(const mp5_string& rhs) const
    {
        return content == rhs.content;
    }

    char mp5_string::operator[](unsigned index) const
    {
        return content[index];
    }

    
}