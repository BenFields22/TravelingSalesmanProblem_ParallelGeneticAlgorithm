#include "population.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <stdio.h>

Population::Population() {
	m_numChromosomes = 0;
};

bool compare(Chromosome i, Chromosome j) {
	return (i.getFitness() < j.getFitness());
}

Population::Population(std::vector<city> Cities) {
	m_numChromosomes = 0;
	for (int i = 0; i < NUMCHROMOSOMES; i++)
	{
		std::vector<city> shuffleVec = Cities;
		std::random_shuffle(shuffleVec.begin(), shuffleVec.end());
		AddChromosome(Chromosome(shuffleVec,i));
	}
};

void Population::AddChromosome(Chromosome newChromosome)
{
	m_chromosomes.push_back(newChromosome);
	m_numChromosomes++;
};

int Population::getPopulationSize()
{
	return m_numChromosomes;
};

void Population::printPopulation() {
	for (int i = 0; i < m_numChromosomes; i++)
	{
		printf_s("Chromosome %d: ", m_chromosomes[i].getId());
		for (int j = 0; j < m_chromosomes[0].getNumCities(); j++)
		{
			printf_s("%d ",m_chromosomes[i].getCityId(j));
		}
		printf_s("\n Fitness: %d\n\n",m_chromosomes[i].getFitness());
	}
}

void Population::calculateFitness() {
	for (int i = 0; i < m_numChromosomes; i++)
	{
		m_chromosomes[i].CalculateFitness();
	}
}

void Population::sortPopulation() {
	std::sort(m_chromosomes.begin(), m_chromosomes.end(), compare);
}

void Population::breed(int crossoverPercent, int mutationPercent, int numFromParent, int* seeds) {
	for (int i = 0; i < m_numChromosomes; i+=2)
	{
		m_chromosomes[i].crossOver(m_chromosomes[i + 1],crossoverPercent,numFromParent, seeds);
		m_chromosomes[i].mutate(mutationPercent);
		m_chromosomes[i+1].mutate(mutationPercent);
	}
}

Chromosome Population::getBest() {
	return m_chromosomes[0];
}

void Population::printFitness()
{
	for (int i = 0; i < m_numChromosomes; i++)
	{
		printf_s("Chromosome %d: %d miles\n", m_chromosomes[i].getId(), m_chromosomes[i].getFitness());
	}
}