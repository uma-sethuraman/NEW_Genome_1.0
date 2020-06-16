#include "TD_NKFitnessEvaluator.h"

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

int nk_fitness_evaluate(AbstractGenome* genome) {
    
    int n = 7;
    int k = 3;
    
    // Create NK table of size nxk
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
    
    // In it genome with n values in the range [0, 2^k)
    genome->resize(n);
    auto readHead = genome->data();
    for (size_t i(0); i < genome->size(); ++i)
        readHead[i] = (std::byte)(rand_int(0, std::pow(2, k)));
    
    for (size_t i(0); i < genome->size(); ++i) {
        int num = (int)GN::genomeRead<std::byte>(genome, i);
        std::cout << num << std::endl;
    }
    
    // Calculate score
    double score = 0;
    for (size_t n_ind(0); n_ind < genome->size(); ++n_ind) { // genome->size() == n
        int k_ind = (int)GN::genomeRead<std::byte>(genome, n_ind);
        score += nk_table[n_ind][k_ind];
        
        std::cout << "n_ind = " << n_ind << '\n';
        std::cout << "k_ind = " << k_ind << '\n';
        std::cout << "nk_table[n_ind][k_ind] = " << nk_table[n_ind][k_ind] << '\n';
    }
    
    std::cout << "score / n: " << score / n << std::endl;
    
    return score / n;
}


