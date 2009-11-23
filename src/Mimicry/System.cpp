/**
 * \file System.cpp
 */

#include "System.h"

/**
 * Half the length of one side of the cube bounding the universe.
 */
const double System::HALF_WORLD_SIZE = 10;

/**
 * The length of one side of the cube bounding the universe.
 */
const double System::WORLD_SIZE = 2 * System::HALF_WORLD_SIZE;

/**
 * The number of cells in the universe.
 */
const int System::TOTAL_NUMBER_OF_CELLS = ISIZE * ISIZE * ISIZE;

/**
 * Cell size in distance units.
 */
const double System::CELL_SIZE = WORLD_SIZE / ISIZE;

/**
 * A unit vector is multiplied by this amount before being added to the force vector.
 */
const double System::FORCE_FACTOR = 40;

/**
 * The time step used for first-order integration of the motion equations.
 */
const double System::DT = 0.01;

/**
 * The friction constant used for motion calculations.
 */
const double System::FRICTION = 5;

/**
 * Work done = \a WF * \a force * \a distance, where \a WF is this constant.
 */
const double System::WORK_FACTOR = 1;

/**
 * Size of \a Prey species in the model.
 */
int System::PREY_SIZE = 2;

/**
 * Mimminum age for \a Prey species to start reproduction.
 */
const int System::PREY_REPRODUCTION_AGE_LIMIT = 100;

/**
 * Time interval to wait before a \a Prey starts reproducing again.
 */
const int System::PREY_REPRODUCTION_INTERVAL = 800;

/**
 * Maximum age upto which prey species will survie in the environment.
 */
const int System::PREY_DEMISE_AGE = 2000;

/**
 * Probability of the number of new born \a Prey species mutating 
 * to a random new pattern
 */
const float System::PATTERN_MUTATION_RATE = (float)0.00;

/**
 * Probability of the number of new born \a Prey species with a 
 * mutating genome excluding the pattern genome
 */
const float	System::PREY_GENOME_MUTATION_RATE = (float)0.3;

/**
 * Hamming distance between two patterns for which they are considred in
 * the same Mimicry ring.
 */
const int System::MIMICRY_RING_HAMMING_DIST = (int)(PATTERN_SIZE * PATTERN_SIZE * 0.1);

/**
 * Minimum number of patterns that need to be stored in Predator memory before starting recognition.
 */
const int System::MIN_MEMORY_SIZE = 2;

/**
 * Maximum number of patterns stored in Hopfield Memory
 */
const int System::MAX_MEMORY_SIZE = 5;

/**
 * Maximum number of iterations in Hopfield Memory recognition process.
 */
const int System::HOPFIELD_MAX_ITERATION = 20;

/**
 * Mimminum age for \a Predators to attack \a Prey species.
 */
const int System::PREDATOR_ATTACK_AGE = 500;

/**
 * Time interval to wait before a \a Predator starts attacking again.
 */
const int System::PREDATOR_ATTACK_INTERVAL = 100;

/**
 * 
 */
const float System::PREDATOR_GENOME_MUTATION_RATE = (float)0.3;

/**
 * Mimminum age for \a Prey species to start reproduction.
 */
const int System::PREDATOR_REPRODUCTION_AGE_LIMIT = 500;

/**
 * Time interval to wait before a \a Prey starts reproducing again.
 */
const int System::PREDATOR_REPRODUCTION_INTERVAL = 2000;

/**
 * 
 */
const int System::PREDATOR_DEMISE_AGE = 5000;

/**
 * Display outline in the model window when non-zero.
 */
int System::showOutline = 0;

/**
 * Display all individual cells of the model
 */
int System::showCells = 0;
