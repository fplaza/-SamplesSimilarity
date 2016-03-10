#ifndef MATRIX_PRINTER_HH
#define MATRIX_PRINTER_HH

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

template <typename T>
class MatrixPrinter
{
    public:
        static void print(const Matrix<T>& matrix, const std::string& output_file);
};

template <typename T>
void MatrixPrinter<T>::print(const Matrix<T>& matrix, const std::string& output_file)
{
    std::ofstream ofs;
    ofs.open(output_file.c_str());

    ofs << "rownames";
    for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
    {
        ofs << '\t' << matrix.colnames()[curr_col];
    }

    ofs << '\n';

    for (size_t curr_row = 0; curr_row < matrix.nrow(); ++curr_row)
    {
        ofs << matrix.rownames()[curr_row];

        for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
        {
            const T val = matrix(curr_row, curr_col);

            if (val == T(0))
            {
                ofs << "\t0";
            }
            else
            {
                ofs << '\t' << val;
            }
        }

        ofs << '\n';
    }

    ofs.close();
}


#endif // MATRIX_PRINTER_HH
