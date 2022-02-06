#pragma once

#include "Data.hpp"

#include <vector>

void generatePopulation();
void chooseParents();
double getRandomDouble();
int calculateProfit(std::vector<int> v);
void cross();
void mutate(std::vector<int>& child);
void generateNewPopulation();
void genetic(Data& data);
