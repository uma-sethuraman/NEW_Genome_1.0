#pragma once

#include <iostream>
#include <random>
#include "AbstractGenome.h"

double rand_double();
int rand_int(int min, int max);
int nk_fitness_evaluate(AbstractGenome* genome);
