#ifndef READ_FROM_FILE_HPP
# define READ_FROM_FILE_HPP

# include "complex_matrix.hpp"
# include <fstream>

class ReadFromFile {

    file_name;

    ReadFromFile(const ReadFromFile &obj);
    ReadFromFile &operator=(const ReadFromFile &obj);

public:

    ReadFromFile(void);
    ~ReadFromFile(void);

    void set_name_file(char *arg_file_name);
    void read_data(void);
    ComplexMatrix *get_data(void);

};

#endif
