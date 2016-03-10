#ifndef MATRIX_READER_HH
#define MATRIX_READER_HH

/*
 * Copyright 2016 Enterome
 *
 * This file is part of samples-similarity
 *
 * samples-similarity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * samples-similarity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with samples-similarity. If not, see <http://www.gnu.org/licenses/>.
 */ 

#include "Matrix.hh"
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

template <typename T>
class MatrixReader
{
    public:
        static Matrix<T> read(const std::string& matrix_file);

    private:
        static const size_t MAX_LINE_LENGTH;
        struct ConversionFunction;

        const std::string matrix_file_;
};

template <typename T>
Matrix<T> MatrixReader<T>::read(const std::string& matrix_file)
{
    char line[MAX_LINE_LENGTH];

    std::ifstream ifs;
    ifs.open(matrix_file.c_str());

    // FIRST PASS: get the dimensions of the matrix
    // Get the number of columns
    std::string header;
    std::getline(ifs, header);
    size_t ncol = std::count_if(header.begin(), header.end(), static_cast<int(*)(int)>(std::isspace));

    // Get the number of rows
    size_t nrow = 0;
    while (ifs.getline(line, MAX_LINE_LENGTH))
        ++nrow;

    // Allocate memory to store the count matrix
    Matrix<T> matrix(nrow, ncol, false);

    //SECOND PASS: fill the matrix
    ifs.clear();
    ifs.seekg(0);

    // Skip potential first column string
    while (!std::isspace(ifs.get()))
        ;

    // Read colnames
    for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
    {
        ifs >> matrix.colnames()[curr_col];
    }

    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Read rownames and matrix content
    for (size_t curr_row = 0; curr_row < matrix.nrow(); ++curr_row)
    {
        ifs.getline(line, MAX_LINE_LENGTH);
        char* pos = line;

        while (*pos != '\0' && !std::isspace(*pos))
            ++pos;

        const std::string rowname(line, pos-line);
        matrix.rownames()[curr_row] = rowname;

        ConversionFunction conversion_function;
        for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
        {
            matrix(curr_row, curr_col) = conversion_function(pos, &pos);
        }
    }

    ifs.close();

    return matrix;
}

template <>
struct MatrixReader<unsigned long>::ConversionFunction
{
    unsigned long operator()(const char *nptr, char **endptr)
    {
        return strtoul(nptr, endptr, 10);
    }
};

template <>
struct MatrixReader<float>::ConversionFunction
{
    float operator()(const char *nptr, char **endptr)
    {
        return strtof(nptr, endptr);
    }
};

template <>
struct MatrixReader<double>::ConversionFunction
{
    double operator()(const char *nptr, char **endptr)
    {
        return strtod(nptr, endptr);
    }
};

template <typename T>
const size_t MatrixReader<T>::MAX_LINE_LENGTH = 100000;

#endif // MATRIX_READER_HH
