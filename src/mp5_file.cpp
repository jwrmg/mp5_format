#include "mp5_file.h"

#include <fstream>
#include <iostream>
#include <regex>

#include "mp5_string.h"

namespace mp5
{
    std::function<void(int, const char*)> mp5_file::log_pointer = nullptr;

    bool mp5_file::load_file(const mp5::mp5_string& file_path)
    {
        return _load_file(file_path);
    }

    bool mp5_file::_load_file(const mp5::mp5_string& file_path)
    {
        std::ifstream file(file_path.content);
        if (file.is_open())
        {
            mp5::mp5_string line = "";
            while (std::getline(file, line.content))
            {
                _parse(line);
            }
            file.close();
        }
        else
        {
            return false;
        }
        return true;
    }

    void mp5_file::_parse(const mp5::mp5_string& line)
    {
        if (line[0] == line[1] && line[1] == '/')
        {
            // return as this is a comment.
            log_pointer(0, "comment found... skipping line.");
            return;
        }

        switch (_state)
        {
        case file_load_states::initial_state:

            if (line.content.front() == '#')
            {
                mp5::mp5_string object_name = line.content.substr(1);

                object_definitions.emplace(object_name, object_fields());

                current_object_scope = object_name;

                _state = file_load_states::object_found;

                std::cout << "Object Definition " << object_name.content << std::endl;
            }
            break;
        case file_load_states::object_found:
            // search for scope entrance
            if (line[0] == '{')
            {
                std::cout << "entering scope" << std::endl;
                _state = file_load_states::enter_scope;
            }
            break;
        case file_load_states::enter_scope:
            // load variables

            // check if line is exit scope
            if (line[0] == '}')
            {
                std::cout << "exiting scope" << std::endl;
                _state = file_load_states::initial_state;
                // reset current object scope;
                current_object_scope = "";
                return;
            }

            size_t field_name_begin = line.content.find_first_of('-');

            for (int i = 0; i < field_name_begin; i++)
            {
                // check for whitespace
                if (line[i] != ' ' && line[i] != '\t')
                {
                    // is not an variable declaration.
                    return;
                }
            }

            size_t field_name_end = line.content.find_first_of(':');

            mp5::mp5_string field_name = line.content.substr(field_name_begin, field_name_end);

            std::cout << field_name.content << std::endl;

            mp5::mp5_string value = line.content.substr(field_name_end);

            std::cout << value.content << std::endl;
            _handle_value(value, field_name);
            break;
        }
    }

    void mp5_file::_handle_value(const mp5::mp5_string& string_value, const mp5::mp5_string& field_name)
    {
        if (string_value[0] != ':')
        {
            // invalid mp5_value
            return;
        }

        // remove colon get raw mp5_value.
        const mp5::mp5_string value = string_value.content.substr(1);

        switch (value.get_type())
        {
        case mp5_value_type::t_invalid: /*void*/ break;
        case mp5_value_type::t_float: _allocate_float(value, field_name); break;
        case mp5_value_type::t_int: _allocate_int(value, field_name); break;
        case mp5_value_type::t_double: _allocate_double(value, field_name); break;
        case mp5_value_type::t_string: _allocate_string(value, field_name); break;
        case mp5_value_type::t_boolean: _allocate_bool(value, field_name); break;
        case mp5_value_type::t_count: /*void*/break;
        }
    }

    void mp5_file::_allocate_bool(const mp5::mp5_string& value, const mp5::mp5_string& field_name)
    {
        try
        {
            bool* data = value == "true" ? new bool(true) : value == "false" ? new bool(false) : nullptr;
            if (data != nullptr)
            {
                _emplace(field_name, mp5_value_type::t_boolean, data, sizeof(bool));
            }
            else throw;
        }
        catch (...)
        {
            std::cout << "failed to allocate field as boolean: " << field_name.content << "as the mp5_value was inappropriate: mp5_value: " << value.content;
        }
    }

    void mp5_file::_allocate_string(const mp5::mp5_string& value, const mp5::mp5_string& field_name)
    {
        try
        {
            mp5::mp5_string* data = nullptr;
            // check for quotations
            if (value.is_float())
            {
                // remove quotations
                const mp5::mp5_string temp = value.content.substr(1, value.content.size() - 1);
                data = new mp5::mp5_string(temp);
            }
            if (data)
            {
                _emplace(value, mp5_value_type::t_string, data, static_cast<int>(data->content.size()) * sizeof(char));
            }
            else throw;
        }
        catch (...)
        {
            // std::cout << "failed to allocate field as double: " << field_name << "as the mp5_value was inappropriate: mp5_value: " << mp5_value;
        }
    }

    void mp5_file::_allocate_double(const mp5::mp5_string& value, const mp5::mp5_string& field_name)
    {
        try
        {
            double* data = new double(std::stod(value.content));
            if (data != nullptr)
            {
                _emplace(field_name, mp5_value_type::t_double, data, sizeof(double));
            }
            else throw;
        }
        catch (...)
        {
            // std::cout << "failed to allocate field as double: " << field_name << "as the mp5_value was inappropriate: mp5_value: " << mp5_value;
        }
    }

    void mp5_file::_allocate_float(const mp5::mp5_string& value, const mp5::mp5_string& field_name)
    {
        try
        {
            // allocate float
            float* data = new float(std::stof(value.content));
            if (data != nullptr)
            {
                _emplace(field_name, mp5_value_type::t_float, data, sizeof(float));
            }
            else throw;
        }
        catch (...)
        {
            // std::cout << "failed to allocate field as float: " << field_name << "as the mp5_value was inappropriate: mp5_value: " << mp5_value;
        }
    }

    void mp5_file::_allocate_int(const mp5::mp5_string& value, const mp5::mp5_string& field_name)
    {
        try
        {
            int* data = new int(std::stoi(value.content));
            if (data)
            {
                _emplace(field_name, mp5_value_type::t_int, data, sizeof(int));
            }
            else throw;
        }
        catch (...)
        {
            // std::cout << "failed to allocate field as int: " << field_name << "as the mp5_value was inappropriate: mp5_value: " << mp5_value;
        }
    }

    void mp5_file::_emplace(const mp5::mp5_string& name, mp5::mp5_value_type type, void* data, int size)
    {
        if (data)
        {
            mp5::mp5_value new_value
            { name,type,data,size };
            object_definitions[current_object_scope].emplace(name, new_value);
        }
    }
}