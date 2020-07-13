#include "UmaGenome.h"

struct Organism {
    AbstractGenome* genome;
    double score;
};

int getRandomInt(int min, int max);
double getRandomDouble(double min, double max);
bool compareOrganismByScore (const Organism& a, const Organism& b);
double getScore(AbstractGenome* genome, std::vector<std::vector<double>> scoreMatrix, int N, int K);
void printNKGenome(AbstractGenome* genome);
void evolvePopulation(std::vector<Organism>& population, int N, int K, bool debug);
std::vector<std::vector<double>> createScoreMatrix(int N, int K, bool debug);
void runNKFitness(int N, int K, int updates, int pop_size, bool debug);