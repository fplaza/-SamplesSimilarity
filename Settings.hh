#ifndef SETTINGS_HH
#define SETTINGS_HH

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

#include <string>
#include <ostream>

struct Settings
{

    struct
    {
        std::string count_matrix_file;
    } input;

    struct
    {
        int num_threads;
    } performance;

    struct
    {
        std::string similarity_matrix_file;
    } output; 

    static Settings parse(int argc, char* argv[]);
    static void check_file_is_readable(const std::string& filepath);
    static void check_file_is_writable(const std::string& filepath);
    template <typename T>
    static void check_val_within_bounds(const std::string& val_name, T value, T lower, T higher);
};

std::ostream& operator<<(std::ostream& os, const Settings& settings);

#endif // SETTINGS_HH
