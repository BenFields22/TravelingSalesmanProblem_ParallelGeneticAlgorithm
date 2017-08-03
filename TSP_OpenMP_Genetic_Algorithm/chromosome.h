#ifndef CHROMOSOME_H
#define CHROMOSOME_H
#include <vector>

typedef struct {
	int id;
	int x;
	int y;
} city;

class Chromosome {
private:
	int m_Chromid;
	int m_numCities;
	int m_fitness;
	std::vector<city> m_cities;
public:
	Chromosome();
	Chromosome(std::vector<city> &cities, int id);
	int getNumCities();
	int getCityId(int);
	int getId();
	void CalculateFitness();
	int getFitness();
	void crossOver(Chromosome other, int percentCrossover, int numFromParent, int* seeds);
	void mutate(int percentMutation);
	void assignNewCities(std::vector<city> newCities);
	void setFitness(int num);
	int getXAt(int pos);
	int getYAt(int pos);
};
#endif