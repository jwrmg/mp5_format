#include <iostream>

#include "mp5_file.h"

int main()
{
    // mp5::mp5_file file;
    // file.load_file("./test.txt");

    auto t1 = mp5::mp5_string("true").is_boolean();
    auto t2 = mp5::mp5_string("25.03235f").is_float();
    auto t3 = mp5::mp5_string("25d").is_double();
    auto t4 = mp5::mp5_string("25").is_integer();
    auto t5 = mp5::mp5_string("\"test\"").is_string();
    system("pause");
    return 0;
}