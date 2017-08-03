#include "chromosome.h"
#include <cmath>
#include <cstdlib>
#include <omp.h>
#include <time.h>


float SQUARE(float num) {
	return num*num;
}

Chromosome::Chromosome() {
	m_Chromid = -1;
	m_numCities = -1;
	m_fitness = -1;
}

Chromosome::Chromosome(std::vector<city> &cities, int id)
{
	m_numCities = cities.size();
	m_cities = cities;
	m_Chromid = id;
	m_fitness = -1;
}

int Chromosome::getNumCities() {
	return m_numCities;
}

void Chromosome::setFitness(int num) {
	m_fitness = num;
}

int Chromosome::getCityId(int num) {
	return m_cities[num].id;
};

int Chromosome::getId(){
	return m_Chromid;
};

int Chromosome::getFitness() {
	return m_fitness;
};
void Chromosome::CalculateFitness() {
	int sum = 0;
	for (int i = 0; i < m_numCities-1; i++)
	{
		float distance = SQUARE(m_cities[i].x - m_cities[i + 1].x) + SQUARE(m_cities[i].y - m_cities[i + 1].y);
		int answer = round(sqrt(distance));
		sum += answer;
	}
	float returnDist = SQUARE(m_cities[0].x - m_cities[m_numCities - 1].x) + SQUARE(m_cities[0].y - m_cities[m_numCities - 1].y);
	int lastDist = round(sqrt(returnDist));
	sum += lastDist;
	m_fitness = sum;
}

void Chromosome::assignNewCities(std::vector<city> newCities)
{
	m_cities = newCities;
}

bool found(std::vector<city> cities, city myCity) {
	bool found = false;
	for (int i = 0; i < cities.size(); i++)
	{
		if (cities[i].id == myCity.id)
		{
			found = true;
			break;
		}
	}
	return found;
}

void Chromosome::crossOver(Chromosome other, int percentCrossover, int numFromParent, int* seeds) {
	srand(seeds[omp_get_thread_num()]+other.getId());
	int crossDecision = rand() % 101;
	if (crossDecision > percentCrossover)
	{
		return;
	}
	city fillCity;
	fillCity.id = -1;
	fillCity.x = -1;
	fillCity.y = -1;
	std::vector<city> parent1 = m_cities;
	std::vector<city> parent2 = other.m_cities;
	std::vector<city> offspring1;
	std::vector<city> offspring2;
	for (int i = 0; i < m_numCities; i++)
	{
		offspring1.push_back(fillCity);
		offspring2.push_back(fillCity);
	}
	int startCrossPoint1 = rand() % 40;
	for (int i = startCrossPoint1; i < startCrossPoint1+numFromParent; i++)
	{
		offspring1[i] = parent1[i];
	}

	int startCrossPoint2 = rand() % 40;
	for (int i = startCrossPoint2; i < startCrossPoint2 + numFromParent; i++)
	{
		offspring2[i] = parent2[i];
	}

	for (int i = 0; i < m_numCities; i++)
	{
		if (offspring1[i].id == -1)
		{
			for (int j = 0; j < m_numCities; j++)
			{
				if (found(offspring1,parent2[j]))
				{
					continue;
				}
				else {
					offspring1[j] = parent2[j];
				}
			}
		}
	}

	for (int i = 0; i < m_numCities; i++)
	{
		if (offspring2[i].id == -1)
		{
			for (int j = 0; j < m_numCities; j++)
			{
				if (found(offspring2, parent1[j]))
				{
					continue;
				}
				else {
					offspring2[j] = parent1[j];
				}
			}
		}
	}

	m_cities = offspring1;
	other.assignNewCities(offspring2);
}

void Chromosome::mutate(int percentMutation) {
	int mutateDecision = rand() % percentMutation + 1;
	if (mutateDecision > percentMutation)
	{
		return;
	}
	int position1, position2;
	do {
		position1 = rand() % m_numCities;
		position2 = rand() % m_numCities;
	} while (position1 == position2);
	std::swap(m_cities[position1], m_cities[position2]);
}

int Chromosome::getXAt(int num) {
	return m_cities[num].x;
}
int Chromosome::getYAt(int num) {
	return m_cities[num].y;
}