#include <random>
#include "Uma_NKGenome.h"
#include "Uma_NKEvaluator.h"
#include <cstddef>
#include <algorithm>

// Returns random integer in the range [min, max]
int getRandomInt(int min, int max) {
    std::random_device dev;     
	std::mt19937 engine(dev());
	std::uniform_int_distribution<int> dist(min,max); 
	int random_num = dist(engine);
    return random_num;
} 

// Returns random double in the range [min, max]
double getRandomDouble(double min, double max) {
	std::random_device dev;     
	std::mt19937 engine(dev());
	std::uniform_real_distribution<double> dist(min, max); 
	return dist(engine);
} 

// Used to sort list of organisms (containing a genome and its score) by score
bool compareOrganismByScore (const NK::Organism& a, const NK::Organism& b) {
	return a.score > b.score;
}

// Performs NK fitness score calculation given a genome and the random score matrix
double getScore(AbstractGenome* genome, std::vector<std::vector<double>> scoreMatrix, int N, int K) {
	double score = 0;
	
	// loop through all values in genome
	for (int n_index = 0; n_index < N; n_index++) {

        // read the int version of each byte of data, which is 
        // equivalent to the int version of group of k sites in genome
		unsigned int group_int = (unsigned int)GN::genomeRead<std::byte>(genome, n_index);	
		score += scoreMatrix[group_int][n_index]; // update score using score matrix
	}

	// divide score by number of values in genome
	score = score/N;
	return score;
}

// Prints out a genome of bytes (converted to ints)
void printNKGenome(AbstractGenome* genome) {
	std::cout << "\nGenome: ";
	for (int ind = 0; ind < genome->size(); ind++) {
		std::byte& num = GN::genomeRead<std::byte>(genome, ind);
		std::cout << " " << (int)num;
	}
	std::cout << "\n";
}

// Copies genome a into genome b,
// a and b must both be byte genomes and must be same size
void copyGenome(AbstractGenome* a, AbstractGenome* b) {
    for (int ind = 0; ind < a->size(); ind++) {
		int val = (int)GN::genomeRead<std::byte>(a, ind);	
		GN::genomeWrite<uint8_t>(b, ind, val);
	}
}

// Chooses top 10% of genome population based on highest scores,
// Clears and refills genome population vector with mutated versions of this top 10%,
// Performs evolution on genome population
void evolvePopulation(std::vector<NK::Organism>& population, int N, int K, bool debug) {

	// choose top 10 percent of population to move to next generation
    // this evolution method relies on population size being a multiple of 10
	int pop_size = population.size();
	int top10percent = pop_size/10;

    // temporarily stores next generation population of genomes (and scores)
	std::vector<NK::Organism> new_population(pop_size);

    // index into new population vector, iterates from 0 to pop_size
	int newpop_ind = 0; 

    // loop through top 10 percent of original population
	for (int pop_ind = 0; pop_ind < top10percent; pop_ind++) {

        // for each genome in top 10 percent, 
        // add 1 direct copy and 9 mutated copies to the new population
		for (int count = 0; count < 10 && newpop_ind < pop_size; count++) {
			new_population[newpop_ind].genome = new Uma_NKGenome(N);
			new_population[newpop_ind].score = 0.0;

			if (count == 0){
				// add 1 direct copy of current genome to new population
                copyGenome(population[pop_ind].genome, new_population[newpop_ind].genome);
			}
			else {
                // mutate case
				// make a copy of original genome and mutate it
				AbstractGenome* copy = new Uma_NKGenome(N);
                copyGenome(population[pop_ind].genome, copy);
				copy->mutate();

				// add mutated copy into new population
                copyGenome(copy, new_population[newpop_ind].genome);
    
				delete copy; // delete temporary variable
			}
			newpop_ind++;  // increment index in new population vector
		}
	}

	// we want to put the changes back into population parameter, 
    // so copy from new_population back into population
	for (int p = 0; p < pop_size; p++) {
        copyGenome(new_population[p].genome, population[p].genome);
        population[p].score = 0.0;
	}

    // delete genomes from new_population temp vector, as we are done using them
	for (int ind = 0; ind < new_population.size(); ind++)
		delete new_population[ind].genome;
}

// Creates random score matrix with values between 0 of 1 for NK fitness landscape
// matrix is 2^K rows x N columns
std::vector<std::vector<double>> createScoreMatrix(int N, int K, bool debug) {
	int rows = std::pow(2, K);
	std::vector<std::vector<double>> scoreMat(rows);
	if (debug)
		std::cout << "\nScore Matrix:\n";
	for (int i = 0; i < rows; i++) { 
		scoreMat[i].resize(N);
		for (int j = 0; j < N; j++){ 
			scoreMat[i][j] = getRandomDouble(0,1);
			if(debug)
				std::cout << scoreMat[i][j] << " ";
		} 
		if(debug)
			std::cout << "\n";
	}
	if(debug)
		std::cout << "\n";
	return scoreMat;
}

// Runs an NK fitness model on a population of pop_size genomes,
// where updates is the number of generations to evolve the population
void runNKFitness(int N, int K, int updates, int pop_size, bool debug) {
	
	std::cout << "\nNK FITNESS TEST: \n\n";

	int time = 0; // which update we are currently on

    // population of genomes (and their scores)
	std::vector<NK::Organism> population(pop_size);
	
	// initialize population of pop_size random genomes of size N
	for (int org = 0; org < pop_size; org++) {
		population[org].genome = new Uma_NKGenome(N);
		population[org].score = 0.0;

		// initialize each value in genome to random num
        // between 0 and 2^K-1
		for (int gen_ind = 0; gen_ind < N; gen_ind++) {
			int maximum = static_cast<int>(std::pow(2,K)-1);
			int random_num = getRandomInt(0, maximum);
			GN::genomeWrite<uint8_t>(population[org].genome, gen_ind, random_num);
		}

		if (debug) printNKGenome(population[org].genome);
	}

	// generate and print random score matrix
	std::vector<std::vector<double>> scoreMatrix = createScoreMatrix(N, K, debug);
		
	// evolution loop: run for updates # of generations
	while (time < updates) {
		double scores_ave = 0.0; // average score for this generation/update

		if (debug) std::cout << "Unsorted scores: ";

		// loop through current population, calculate score for each organism
		for (int org = 0; org < pop_size; org++) {
			population[org].score = getScore(population[org].genome, scoreMatrix, N, K);
			scores_ave += population[org].score;
			if (debug) std::cout << population[org].score << " ";
		}

		// sort scores in descending order
		std::sort(population.begin(), population.end(), compareOrganismByScore);

		if(debug){
			std::cout << "\nSorted scores: ";
			for (int pop_i = 0; pop_i < population.size(); pop_i++) {
				std::cout << " " << population[pop_i].score;
			}
			std::cout << "\n";
		}

		// print average score for this update
        scores_ave = scores_ave/pop_size;
		std::cout << "Scores AVE at update " << time << ": " << scores_ave << std::endl;

		if (debug) std::cout << "\nUPDATE: " << time+1; 

		// evolve/change population vector by filling it with copies and
        // mutated versions of the top 10% of genomes in the current population
		evolvePopulation(population, N, K, debug);

        if(debug){
            // print new/evolved population
            for (int p = 0; p < population.size(); p++)
			    printNKGenome(population[p].genome);
        }

		time++; // increment update count
	  }

	  // delete all genomes in population at end
	  for (int pop_ind = 0; pop_ind < population.size(); pop_ind++)
		delete population[pop_ind].genome;
}