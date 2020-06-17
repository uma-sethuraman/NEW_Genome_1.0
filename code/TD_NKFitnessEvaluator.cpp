#include "TD_NKFitnessEvaluator.h"

#include "TetianaGenome.h"
#include <iostream>
#include <random>




double rand_double() {
    // returns random double value in range [0, 1)
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);
    
    return dist(e2);
}

int rand_int(int min, int max) {
    // returns random int value in range [min, max)
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(min, max);
    
    return dist(e2);
}

std::vector<std::pair<AbstractGenome*, double>> reproduce(AbstractGenome* genome, int n, int k, int group_size) {
    
    // Gets genome and returns a vector of mutated copies
    std::vector<std::pair<AbstractGenome*, double>> genomes_group; // double score
    
    for (int i = 0; i < group_size; ++i) {
        
        // Create new genome and do deep copy
        AbstractGenome* temp_genome = new TetianaGenome(8);
        temp_genome->resize(n);
        auto readHead = temp_genome->data();
        for (size_t i(0); i < temp_genome->size(); ++i)
            readHead[i] = GN::genomeRead<std::byte>(genome, i);
        
        if (i % 2 == 0) //  mutate every second one
            temp_genome->mutate(); // change one random site to random value
        
        std::pair<AbstractGenome*, double> genome_score_pair = std::make_pair(temp_genome, 0.0);
        genomes_group.push_back(genome_score_pair);
    }
    
    return genomes_group;
}


AbstractGenome* nk_fitness_evaluate(const std::vector<std::vector<double>>& nk_table, std::vector<std::pair<AbstractGenome*, double>>& genomes_group) {
    
    // Returns a genome with highest score
    
    // Calculate score for each genome and keep track of the best one
    int best_ind = 0;
    double best_score = 0;
    for (int i = 0; i < genomes_group.size(); ++i) {
        double score = 0;
        AbstractGenome* temp_genome = genomes_group[i].first;
        for (size_t n_ind(0); n_ind < temp_genome->size(); ++n_ind) { // genome->size() == n
            int k_ind = (int)GN::genomeRead<std::byte>(temp_genome, n_ind);
            score += nk_table[n_ind][k_ind];
        }
        if (score > best_score) {
            best_score = score;
            best_ind = i;
        }
        genomes_group[i].second = score;
    }
    
    /*
    std::cout << "Genomes vals and their scores:\n";
    for (int i = 0; i < genomes_group.size(); ++i) {
        AbstractGenome* temp_genome = genomes_group[i].first;
        std::cout << "Genome " << i << ": ";
        for (size_t i(0); i < temp_genome->size(); ++i) {
            int num = (int)GN::genomeRead<std::byte>(temp_genome, i);
            std::cout << num << ' ';
        }
        std::cout << "; Score: " << genomes_group[i].second;
        std::cout << std::endl;
    }*/
    
    AbstractGenome* best_genome = genomes_group[best_ind].first;
    std::cout << "Best score: " << genomes_group[best_ind].second << std::endl;
    
    /*
    std::cout << "Best genome: ";
    for (size_t i(0); i < best_genome->size(); ++i) {
        int num = (int)GN::genomeRead<std::byte>(best_genome, i);
        std::cout << num << ' ';
    }
    std::cout << "; Best score: " << genomes_group[best_ind].second;
    std::cout << std::endl;
     */
    
    return best_genome;
}

void evolve_NK() {
    
    // Create NK table of size nxk
    int n = 10;
    int k = 3;
    std::vector<std::vector<double>> nk_table = std::vector<std::vector<double>>(n, std::vector<double>(std::pow(2, k), 0.0));
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < pow(2, k); ++col) {
            nk_table[row][col] = rand_double();
        }
    }
    
    // Show the table
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < pow(2, k); ++col) {
            std::cout << nk_table[row][col] << ' ';
        }
        std::cout << std::endl;
    }
    
    // Initialize a vector of genomes
    std::vector<std::pair<AbstractGenome*, double>> genomes_group; // double score
    int group_size = 10;
    for (int i = 0; i < group_size; ++i) {
        
        // Init each genome with n values in the range [0, 2^k)
        AbstractGenome* genome = new TetianaGenome(8); // 8 bits?
        genome->resize(n);
        auto readHead = genome->data();
        for (size_t i(0); i < genome->size(); ++i)
            readHead[i] = (std::byte)(rand_int(0, std::pow(2, k)));
        
        // Add it to the vectors of genomes together with it's score (initial score = 0)
        std::pair<AbstractGenome*, double> curr_genome = std::make_pair(genome, 0.0);
        genomes_group.push_back(curr_genome);
        
        /*
         std::cout << "Current genome: ";
        for (size_t i(0); i < genome->size(); ++i) { // genome->size() == n
            int num = (int)GN::genomeRead<std::byte>(genome, i);
            std::cout << num << ' ';
        }
        std::cout << std::endl;
        */
    }
    
    // Iterate over multiple evolution cycles
    int evolution_iterations = 20;
    for (int e_it = 0; e_it < evolution_iterations; ++e_it) {
        
        // Evaluate, choose best
        AbstractGenome* best_genome = nk_fitness_evaluate(nk_table, genomes_group); // returns one best genome
        
        // Update genome (+ point mutation)
        genomes_group = reproduce(best_genome, n, k, group_size); //
    }
}
