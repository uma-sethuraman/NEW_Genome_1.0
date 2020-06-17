#pragma once

#include <iostream>
#include <random>
#include "AbstractGenome.h"

double rand_double();
int rand_int(int min, int max);
std::vector<std::pair<AbstractGenome*, double>> reproduce(AbstractGenome* genome, int n, int k, int group_size);
AbstractGenome* nk_fitness_evaluate(const std::vector<std::vector<double>>& nk_table, std::vector<std::pair<AbstractGenome*, double>>& genomes_group);
void evolve_NK();

