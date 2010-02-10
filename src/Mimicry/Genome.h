/**
 * \file Genome.h
 */

#ifndef GENOME_H
#define GENOME_H

#include "System.h"
#include "randutil.h"

#include <iostream>

/**
 * An instance of \a Genome containing binary genes for each agent.
 */
template<int SIZE>
class Genome
{
private:
	/**
	 * int array containing binary gene. 
	 */
	int gene[SIZE];

public:
	Genome();
	~Genome();
	Genome(int rule, bool palatability);
	int get(int index);
	void set(int index, int value);
	Genome<SIZE> crossOver(Genome<SIZE>* _genome);
	Genome<SIZE> crossOverExceptPatternGene(Genome<SIZE>* _genome);
	void mutate();
	void mutateExceptPatternGene();
	void mutatePatternGene();
};

/**
 * Construct a \a Genome. Initialize all gene to zero.
 */
template<int SIZE>
Genome<SIZE>::Genome()
{
	for(int i = 0; i < SIZE; i++)
		gene[i] = 0;
}

template<int SIZE>
Genome<SIZE>::~Genome()
{
	std::cout << "Deleting Genome..." << std::endl;
}

/**
 * Construct a \a Genome. Initialize all gene to zero.
 */
template<int SIZE>
Genome<SIZE>::Genome(int rule, bool palatability)
{
	Genome();
	if(palatability)
	{
		gene[8] = 0;
		gene[9] = 0;
	} else {
		gene[8] = 1;
		gene[9] = 1;
	}
	std::bitset<PATTERN_GENE_SIZE> bs(rule);

	for(int g = 0; g < PATTERN_GENE_SIZE; g++)
		gene[g] = bs[PATTERN_GENE_SIZE - 1 - g];

	std::bitset<7> bs1(randomInteger((int)pow(2.0, 7)));
	for(int g = 0; g < 7; g++)
		gene[10 + g] = bs1[7 - 1 - g];
}

/**
 * Get an individual gene from \a Genome
 * \return value of a single gene.
 * \param index of the gene to get.
 */
template<int SIZE>
inline int Genome<SIZE>::get(int index)
{ 
	return gene[index];
}

/**
 * Set value of an individual gene.
 * \param index index of the gene to set.
 * \param value of the gene to set.
 */
template<int SIZE>
inline void Genome<SIZE>::set(int index, int value)
{
	gene[index] = value;
}

/**
 * Function to perform Crossover operation with another \a Genome.
 * Finds a random index inside the genome and attaches two parts from each to create a new one. 
 * \param _genome pointer to another \a Agent 's genome to perform Crossover operation.
 */
template<int SIZE>
Genome<SIZE> Genome<SIZE>::crossOver(Genome<SIZE> *_genome)
{
	Genome<SIZE> newGenome;
	int rand = randomInteger(SIZE);
	for(int i = 0; i < SIZE; i++)
		if(i <= rand)
			newGenome.set( i, gene[i]);
		else
			newGenome.set( i, _genome->get(i));
	return newGenome;
}

/**
 * Function to perform Crossover operation with another \a Genome.
 * Subtract the pattern genome and finds a random index inside the rest,
 * and attaches two parts from each to create a new one. 
 * \param _genome pointer to another \a Agent 's genome to perform Crossover operation.
 */
template<int SIZE>
Genome<SIZE> Genome<SIZE>::crossOverExceptPatternGene(Genome<SIZE> *_genome)
{
	Genome<SIZE> newGenome;
	int rand = randomInteger(SIZE - PATTERN_GENE_SIZE) + PATTERN_GENE_SIZE;
	for(int i = 0; i < SIZE; i++)
		if(i <= rand)
			newGenome.set( i, gene[i]);
		else
			newGenome.set( i, _genome->get(i));
	return newGenome;
}

/**
 * Function to perform Mutation operation on this \a Genome.
 * Finds a random index inside the genome and flips it. 
 */
template<int SIZE>
void Genome<SIZE>::mutate()
{
	int rand = randomInteger(SIZE);
	if(gene[rand])
		gene[rand] = 0;
	else
		gene[rand] = 1;
}

/**
 * Function to perform Mutation operation on this \a Genome 
 * except the pattern gene.
 * Finds a random index inside the genome and flips it. 
 */
template<int SIZE>
void Genome<SIZE>::mutateExceptPatternGene()
{
	int rand = randomInteger(SIZE - PATTERN_GENE_SIZE) + PATTERN_GENE_SIZE;
	if(gene[rand])
		gene[rand] = 0;
	else
		gene[rand] = 1;
}

/**
 * Function to perform Mutation operation on the pattern \a Genome.
 * Finds a random index inside the pattern genome and flips it. 
 */
template<int SIZE>
void Genome<SIZE>::mutatePatternGene()
{
	int rand = randomInteger(PATTERN_GENE_SIZE);
	if(gene[rand])
		gene[rand] = 0;
	else
		gene[rand] = 1;
}

/**
 * Print the \a Genome in output stream.
 */
template<int SIZE>
std::ostream & operator<<(std::ostream & os, Genome<SIZE> g)
{
	for(int i = 0; i < SIZE; i++)
		os << g.get(i);
	return os;
}

#endif