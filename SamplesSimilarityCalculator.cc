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
#include "SamplesSimilarityCalculator.hh"

Matrix<double> SamplesSimilarityCalculator::compute_samples_similarity(const Matrix<unsigned long>& matrix, const int num_threads)
{

    const size_t num_genes = matrix.nrow();
    const size_t num_samples = matrix.ncol();

    //Matrix<unsigned long> total_count_union(num_samples, num_samples);
    Matrix<unsigned long> total_count_intersection(num_samples, num_samples);
    Matrix<unsigned long> total_count_union(num_samples, num_samples);

    #pragma omp parallel num_threads(num_threads)
    {
        Matrix<unsigned long> private_count_intersection(num_samples, num_samples);
        Matrix<unsigned long> private_count_union(num_samples, num_samples);

        #pragma omp for nowait
        for (size_t curr_gene = 0; curr_gene < num_genes; ++curr_gene)
        {
            for (size_t curr_sample1 = 0; curr_sample1 < num_samples; ++curr_sample1)
            {
                for (size_t curr_sample2 = curr_sample1 + 1; curr_sample2 < num_samples; ++curr_sample2)
                {
                    private_count_union(curr_sample1, curr_sample2) +=
                        matrix(curr_gene,curr_sample1) + matrix(curr_gene,curr_sample2);

                    if ((matrix(curr_gene,curr_sample1) != 0) && (matrix(curr_gene,curr_sample2) !=0))
                    {
                        private_count_intersection(curr_sample1, curr_sample2) +=
                            matrix(curr_gene,curr_sample1) + matrix(curr_gene,curr_sample2);
                    }
                }
            }
        }

        // Save results in global matrix
        for (size_t curr_sample1 = 0; curr_sample1 < num_samples; ++curr_sample1)
        {
            for (size_t curr_sample2 = curr_sample1 + 1; curr_sample2 < num_samples; ++curr_sample2)
            {
                #pragma omp atomic
                total_count_intersection(curr_sample1, curr_sample2) +=
                    private_count_intersection(curr_sample1, curr_sample2);

                #pragma omp atomic
                total_count_union(curr_sample1, curr_sample2) +=
                    private_count_union(curr_sample1, curr_sample2);
            }
        }
    }


    // Compute jaccard index
    Matrix<double> samples_similarity(num_samples, num_samples);
    for (size_t curr_sample1 = 0; curr_sample1 < num_samples; ++curr_sample1)
    {
        for (size_t curr_sample2 = curr_sample1 + 1; curr_sample2 < num_samples; ++curr_sample2)
        {
            samples_similarity(curr_sample1, curr_sample2) =
                static_cast<double>(total_count_intersection(curr_sample1, curr_sample2)) /
                total_count_union(curr_sample1, curr_sample2);
        }
    }

    // Make the matrix symmetric
    for (size_t curr_sample1 = 0; curr_sample1 < num_samples; ++curr_sample1)
    {
        for (size_t curr_sample2 = curr_sample1 + 1; curr_sample2 < num_samples; ++curr_sample2)
        {
            samples_similarity(curr_sample2, curr_sample1) = samples_similarity(curr_sample1, curr_sample2);
        }
        samples_similarity(curr_sample1, curr_sample1) = 1.0;
    }

    samples_similarity.rownames() = samples_similarity.colnames() = 
        matrix.colnames();

    return samples_similarity;
}
