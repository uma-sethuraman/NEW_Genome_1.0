#include "NK_FitnessEvaluator.h"

#include "JamellGenome.h"
#include <iostream>
#include <random>




double rand_double() {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);
    
    return dist(e2);
}

int rand_int(int min, int max) {
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(min, max);
    
    return dist(e2);
}

std::vector<std::pair<AbstractGenome*, double>> reproduce(AbstractGenome* genome, int n, int k, int group_size) {
    
    // Returns a vector of mutated copies of genomes
    std::vector<std::pair<AbstractGenome*, double>> genomes_group; // double score
    
    for (int i = 0; i < group_size; ++i) {
        
        AbstractGenome* temp_genome = new JamellGenome(8);
        temp_genome->resize(n);
        auto readHead = temp_genome->data();
        for (size_t i(0); i < temp_genome->size(); ++i)
            readHead[i] = GN::genomeRead<std::byte>(genome, i);
        
        if (i % 2 == 0) //  mutatation
            temp_genome->mutate(); // 
        
        std::pair<AbstractGenome*, double> genome_score_pair = std::make_pair(temp_genome, 0.0);
        genomes_group.push_back(genome_score_pair);
    }
    
    return genomes_group;
}


AbstractGenome* nk_fitness_evaluate(const std::vector<std::vector<double>>& nk_table, std::vector<std::pair<AbstractGenome*, double>>& genomes_group) {
    
    // Returns the genome with highest score
    
    // Calculate the score of each genome 
    int best_ind = 0;
    double best_score = 0;
    for (int i = 0; i < genomes_group.size(); ++i) {
        double score = 0;
        AbstractGenome* temp_genome = genomes_group[i].first;
        for (size_t n_ind(0); n_ind < temp_genome->size(); ++n_ind) { // genome->size() == n
            int k_ind = (int)GN::genomeRead<std::byte>(temp_genome, n_ind);
            score += nk_table[n_ind][k_ind];
        }
        //Tracks the best score
        if (score > best_score) {
            best_score = score;
            best_ind = i;
        }
        genomes_group[i].second = score;
    }
    
    
    AbstractGenome* best_genome = genomes_group[best_ind].first;
    std::cout << "Best score: " << genomes_group[best_ind].second << std::endl;
    
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
        
        AbstractGenome* genome = new JamellGenome(8); 
        genome->resize(n);
        auto readHead = genome->data();
        for (size_t i(0); i < genome->size(); ++i)
            readHead[i] = (std::byte)(rand_int(0, std::pow(2, k)));
        
        std::pair<AbstractGenome*, double> curr_genome = std::make_pair(genome, 0.0);
        genomes_group.push_back(curr_genome);
        
    }
    
    // Iterate 
    int evolution_iterations = 20;
    for (int e_it = 0; e_it < evolution_iterations; ++e_it) {
        
        AbstractGenome* best_genome = nk_fitness_evaluate(nk_table, genomes_group); // returns one best genome
        
        genomes_group = reproduce(best_genome, n, k, group_size); //
    }
}
