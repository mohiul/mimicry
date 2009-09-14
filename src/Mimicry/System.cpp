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
const int System::PREY_SIZE = 3;

/**
 * Minimum number of patterns that need to be stored in Predator memory before starting recognition.
 */
const int System::MIN_MEMORY_SIZE = 5;

/**
 * Maximum number of patterns stored in Hopfield Memory
 */
const int System::MAX_MEMORY_SIZE = 10;

/**
 * Maximum number of iterations in Hopfield Memory recognition process.
 */
const int System::HOPFIELD_MAX_ITERATION = 20;

/**
 * Mimminum age for \a Predators to attack \a Prey species.
 */
const int System::PREDATOR_ATTACK_AGE = 500;

/**
 * Mimminum age for \a Prey species to start reproduction.
 */
const int System::REPRODUCTION_AGE_LIMIT = 100;

/**
 * Time interval to wait before a \a Prey starts reproducing again.
 */
const int System::REPRODUCTION_INTERVAL = 200;
