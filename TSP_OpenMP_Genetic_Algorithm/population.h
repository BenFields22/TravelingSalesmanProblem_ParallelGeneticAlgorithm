#ifndef POPULATION_H
#define POPULATION_H
#include "chromosome.h"

const int NUMCHROMOSOMES = 40;

class Population{
private:
	int m_numChromosomes;
	std::vector<Chromosome> m_chromosomes;
public:
	Population();
	Population(std::vector<city> cities);
	void AddChromosome(Chromosome newChromosome);
	int getPopulationSize();
	void printPopulation();
	void calculateFitness();
	void sortPopulation();
	void breed(int crossoverPercent, int mutationPercent, int numFromParent, int* seeds);
	Chromosome getBest();
	void printFitness();
};
#endif
