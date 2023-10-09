#include "read_from_file.hpp"

ReadFromFile::ReadFromFile(void) { }
ReadFromFile::~ReadFromFile(void) { }

void ReadFromFile::set_name_file(char *arg_file_name) {
    file_name = arg_file_name;
}

void ReadFromFile::read_data(void) {
    ifstream fd(file_name);

    if (!fd.is_open()) {
        std::cerr << "Error: file didn't opened" << std::endl;
    }


    fd.close();
}

ComplexMatrix *ReadFromFile::get_data(void) {

}

