#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <omp.h>
#include <vector>
#include <time.h>
#include "population.h"

#define NUMTHREADS 8


int readInputFile(std::vector<city> &Cities, std::string &fileName) 
{
	std::ifstream inputFile;
	inputFile.open(fileName, std::ios::in);
	if (!inputFile.is_open())
	{
		std::cout << "Failed to open InputFile with name '" << fileName << "'\n";
		return 1;
	}
	std::cout << "File: '" << fileName << "' Opened Succesfully\n";
	std::string V;
	while (std::getline(inputFile, V))
	{
		city myCity;
		char *str = _strdup(V.c_str());
		char *substr;
		substr = strtok(str, " ");
		while (substr != NULL)
		{
			myCity.id = atoi(substr);
			substr = strtok(NULL, " ");
			myCity.x = atoi(substr);
			substr = strtok(NULL, " ");
			myCity.y = atoi(substr);
			substr = strtok(NULL, " ");
			Cities.push_back(myCity);
		}
	}
	inputFile.close();
	return 0;
}

void TSP(std::vector<city> Cities, int crossoverPercentage, int mutationPercentage, Chromosome* answer)
{
	omp_set_num_threads(NUMTHREADS);
	int iterations = 0;
	int seeds[] = { 12,99999,1435,6543,789,2,1222222222,5656556 };
#pragma omp parallel for default(none), shared(Cities), shared(crossoverPercentage), shared(mutationPercentage),shared(answer),shared(iterations) private(seeds)
	for (int i = 0; i < NUMTHREADS; i++)
	{
		Population myPopulation(Cities);
		myPopulation.calculateFitness();
		myPopulation.sortPopulation();
		Chromosome best = myPopulation.getBest();
		best.setFitness(1000000000);
		printf_s("Size of population in thread %d is %d\n", omp_get_thread_num(), myPopulation.getPopulationSize());
		int numWithoutImprovement = 0;
		while (numWithoutImprovement < 100)
		{
			myPopulation.breed(crossoverPercentage, mutationPercentage, (best.getNumCities()/2)-1,seeds);
			myPopulation.calculateFitness();
			myPopulation.sortPopulation();
			Chromosome newAnswer = myPopulation.getBest();
			if (newAnswer.getFitness()<best.getFitness())
			{
				best = newAnswer;
				numWithoutImprovement = 0;
			}
			else
			{
				numWithoutImprovement++;
			}
		}
		answer[omp_get_thread_num()] = best;
	}
}


int main(int argc, char* argv[])
{
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif
	std::cout << "openMP can support " << omp_get_max_threads() << " threads\n";
	std::cout << "FileName is: " << argv[1] << "\n";
	std::string fileName = argv[1];
	std::vector<city> Cities;
	int status = readInputFile(Cities, fileName);
	if (status)
	{
		return 2;
	}
	Chromosome *answer = new Chromosome[NUMTHREADS];
	double time0 = omp_get_wtime();
	TSP(Cities,90,2,answer);
	double time1 = omp_get_wtime();
	double seconds = time1 - time0;
	int bestFitness = answer[0].getFitness();
	int index = 0;
	for (int i = 0; i < NUMTHREADS; i++)
	{
		if (answer[i].getFitness() < bestFitness )
		{
			index = i;
		}
		std::cout << "Thread: " << i + 1 << "\n";
		std::cout << "There are " << answer[i].getNumCities() << " cities\n";
		std::cout << "Best Tour: " << answer[i].getFitness() << "\n";
	}
	std::cout << "\nPerformance: " << seconds << " seconds\n";
	std::cout << "Best Tour: " << answer[index].getFitness() << "\n";
	std::ofstream outFile;
	fileName = fileName + ".tour";
	outFile.open(fileName, std::ios::out);
	if (!outFile.is_open())
	{
		std::cout << "Failed to open Output File\n";
		return 3;
	}
	
	outFile << answer[index].getFitness() << "\n";
	for (int i = 0; i < answer[index].getNumCities(); i++)
	{
		outFile << answer[index].getCityId(i) << " " << answer[index].getXAt(i) << " " << answer[index].getYAt(i) << "\n";
	}
	outFile.close();
	std::cout << "******************Program complete. Press ENTER to close application*****************";
	std::cin.get();
	return 0;
}