#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
//#include "mp5_allocator.h"
#include "mp5_string.h"
#include "mp5_value.h"

namespace mp5
{
    struct mp5_allocator;
    // for clarity.
    typedef mp5_string object_name;
    typedef mp5_string field_name;
    typedef std::unordered_map<field_name, mp5_value, mp5::mp5_string::hash> object_fields;

    class mp5_file
    {
    public:
        // (safe to use api)
        template<typename t_type>
        t_type* yoink_info_unsafe(const mp5::mp5_string& variable_name);
        template<typename t_type>
        bool yoink_info_non_alloc(const mp5::mp5_string& variable_name, t_type& ref_type);
        bool load_file(const mp5::mp5_string& file_path);
        void unload_file();

        static std::function<void(int, const char*)> log_pointer;

        enum struct file_load_states
        {
            initial_state,
            object_found,
            enter_scope,
            load_variables,
            exit_scope,
            state_count
        };

        file_load_states _state = file_load_states::initial_state;

        mp5::mp5_string current_object_scope;

        // raw api (typically unsafe to use on its own).
        template<typename t_type>
        t_type* _yoink_info_unsafe(const mp5::mp5_string& variable_name);
        template<typename t_type>
        bool _yoink_info_non_alloc(const mp5::mp5_string& variable_name, t_type& ref_type);
        bool _load_file(const mp5::mp5_string& file_path);
        void _unload_file();

        void _parse(const mp5::mp5_string& line);
        void _handle_value(const mp5::mp5_string& string_value, const mp5::mp5_string& field_name);
        void _allocate_bool(const mp5::mp5_string& value, const mp5::mp5_string& field_name);
        void _allocate_string(const mp5::mp5_string& value, const mp5::mp5_string& field_name);
        void _allocate_double(const mp5::mp5_string& value, const mp5::mp5_string& field_name);
        void _allocate_float(const mp5::mp5_string& value, const mp5::mp5_string& field_name);
        void _allocate_int(const mp5::mp5_string& value, const mp5::mp5_string& field_name);
        void _emplace(const mp5::mp5_string& name, mp5::mp5_value_type type, void* data, int size);

        std::unordered_map<object_name, object_fields, mp5_string::hash> object_definitions = {};

    private:
    };
}
