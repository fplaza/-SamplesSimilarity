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

#include <iostream>
#include <new>
#include <cstdlib>
#include <stdexcept>
#include "Settings.hh"
#include "MatrixReader.hh"
#include "SamplesSimilarityCalculator.hh"
#include "MatrixPrinter.hh"
#include "TimeProfiler.hh"

void no_more_memory()
{
    std::cerr << "error: memory allocation failed." << std::endl;
    std::exit (1);
}

int main(int argc, char *argv[])
{
    std::set_new_handler(no_more_memory);

    try
    {
        TimeProfiler time_profiler;
        time_profiler.start_new_timer("Total");

        const Settings& settings = Settings::parse(argc, argv);
        std::cout << settings << std::endl;

        std::cout << "Reading count matrix..." << std::endl;
        time_profiler.start_new_timer("Reading count matrix");
        const Matrix<unsigned long>& count_matrix =
            MatrixReader<unsigned long>::read(settings.input.count_matrix_file);
        time_profiler.stop_last_timer();
        std::cout << "Done. Count matrix has " << count_matrix.ncol() << " samples and " << count_matrix.nrow() << " genes.\n" << std::endl;

        std::cout << "Computing samples similarity..." << std::endl;
        time_profiler.start_new_timer("Computing samples similarity");
        const Matrix<double>& samples_similarity = 
            SamplesSimilarityCalculator::compute_samples_similarity(count_matrix, settings.performance.num_threads);
        std::cout << "Done\n" << std::endl;
        time_profiler.stop_last_timer();

        std::cout << "Computing samples similarity..." << std::endl;
        time_profiler.start_new_timer("Writing samples similarity matrix");
        MatrixPrinter<double>::print(samples_similarity, settings.output.similarity_matrix_file);
        time_profiler.stop_last_timer();
        std::cout << "Done\n" << std::endl;

        time_profiler.stop_last_timer();
        std::cout << time_profiler << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(-1);
    }

    std::exit(0);
}
