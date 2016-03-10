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

#include "Settings.hh"
#include <iostream>
#include <fstream>
#include <exception>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <omp.h>

Settings Settings::parse(int argc, char* argv[])
{
    Settings settings;

    // Create options decription
    po::options_description all_settings("");
    po::options_description input_settings("[input]");
    po::options_description performance_settings("[performance]");
    po::options_description output_settings("[output]");
    po::options_description misc_settings("[miscellaneous]");

    input_settings.add_options()
        ("count-matrix-file,i", po::value<std::string>(&settings.input.count_matrix_file)->required(), "")
        ;

    performance_settings.add_options()
        ("num-threads,t", po::value<int>(&settings.performance.num_threads)->default_value(omp_get_max_threads()), "")
        ;

    output_settings.add_options()
        ("similary-matrix-file,o", po::value<std::string>(&settings.output.similarity_matrix_file)->required(), "")
        ;

    misc_settings.add_options()
        ("help,h", "Display this help and exit.")
        ;

    all_settings.add(input_settings).add(performance_settings).add(output_settings).add(misc_settings);

    // Retrieve and settingse command line settings
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, all_settings), vm);

    // Print help
    if (argc == 1 || vm.count("help"))
    {
        std::cout << all_settings << std::endl;
        std::exit(0);
    }

    po::notify(vm);

    check_file_is_readable(settings.input.count_matrix_file);
    check_val_within_bounds("--num-threads", settings.performance.num_threads, 1, omp_get_max_threads());
    check_file_is_writable(settings.output.similarity_matrix_file);

    return settings;
}

void Settings::check_file_is_readable(const std::string& filepath)
{
    std::ifstream ifs;
    ifs.open(filepath.c_str());

    if (ifs.good())
    {
        ifs.close();
    }
    else
    {
        throw (std::invalid_argument("error: " + filepath +
                    " cannot be opened. Check that the path is valid and that you have read permissions."));
    }
}



void Settings::check_file_is_writable(const std::string& filepath)
{
    std::ofstream ofs;
    ofs.open(filepath.c_str());

    if (ofs.good())
    {
        ofs.close();
    }
    else
    {
        throw (std::invalid_argument("error: " + filepath +
                    " cannot be created. Check that the path is valid and that you have write permissions."));
    }
}

template <typename T>
void Settings::check_val_within_bounds(const std::string& val_name, T value, T lower, T higher)
{
    if(value < lower || value > higher)
    {
        throw (std::invalid_argument("error: " + val_name + " must be a value within range: " +
                    '[' + boost::lexical_cast<std::string>(lower) + ';' + boost::lexical_cast<std::string>(higher) + ']'));
    }
}

std::ostream& operator<<(std::ostream& os, const Settings& settings)
{
    const char sep[] = "---------------------\n";

    os << sep;
    os << "Settings summary:\n";

    os << "\n[input]\n";
    os << "--count-matrix-file = " << settings.input.count_matrix_file << '\n';

    os << "\n[performance]\n";
    os << "--num-threads = " << settings.performance.num_threads << '\n';

    os << "\n[output]\n";
    os << "--similarity-matrix-file = " << settings.output.similarity_matrix_file << '\n';

    os << sep;

    return os;
}

