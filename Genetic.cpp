#include "Genetic.hpp"
#include <algorithm>
#include <iostream>

std::vector<std::vector<int> > parents;
std::vector<std::vector<int> > children;
std::vector<std::vector<int> > population;

std::vector<int> bestResultVec;
int bestResult = 0;

double randomFactor = 0.99;
Data dataInternal;

void generatePopulation()
{
    // geenrate random population
    for (int i = 0; i < dataInternal.populationSize; i++)
    {
      std::vector<int> v;
        for (size_t k = 0; k < dataInternal.weights.size(); k++)
        {
            v.push_back(getRandomDouble() > randomFactor ? 1 : 0);
        }
        population.push_back(v);
    }
}

void chooseParents()
{
    // roulette method
    // choose paretns for max completion time function
    parents.clear();

    // values inits
    unsigned long long permutationSum = 0;
    std::vector <int> permutationValues(dataInternal.populationSize);
    unsigned long long currentValue = 0;
    unsigned long long lastValue = 0;

    // calculate sum of all completion time functions (pol. funkcji przystosowania)
    for (int i = 0; i < dataInternal.populationSize; i++)
    {
        permutationValues[i] = calculateProfit(population[i]);
        permutationSum += permutationValues[i];
    }

    // find max completion time function equals random value
    for (int i = 0; i < dataInternal.weights.size(); i++)
    {
        //generate random value
        unsigned long randomValue = getRandomDouble() * permutationSum;
        // zeroes params
        currentValue = 0;
        lastValue = 0;

        // for i in 0..populationSize
        for (int i = 0; i < dataInternal.populationSize; i++)
        {
            // accumulate permutation values in currentValue
            currentValue += permutationValues[i];

            // if randomValue in <lastValue;currentValue>
            if (randomValue >= lastValue and randomValue <= currentValue)
            {
                // push back population[i] to parents
                parents.push_back(population.at(i));
                break;
            }
            // accumulate permutation values in lastValue
            lastValue += permutationValues[i];
        }
    }
}

double getRandomDouble()
{
    // generate random double value
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

int calculateProfit(std::vector<int> v)
{
    // calculating max completion time functions - to change, here it is not time
    unsigned int sum = 0;
    int currentWeight = 0;
    for (int i = 0; i < dataInternal.weights.size(); i++)
    {
      if (v[i]==1)
      {
        sum+= dataInternal.profits[i];
          currentWeight += dataInternal.weights[i];
      }
    }

    if (currentWeight > dataInternal.maxCapacity)
    {
        return 0;
    }

    return sum;
}

void cross()
{
    // cross function with two variants
    std::vector<int> parent1 = parents.at(rand() % dataInternal.weights.size());
    std::vector<int> parent2 = parents.at(rand() % dataInternal.weights.size());

    std::vector<int> child1(dataInternal.weights.size());
    std::vector<int> child2(dataInternal.weights.size());

    if(dataInternal.crossMethodVariant==1)
    {
        //first variant PMX
        for (int i = 0; i < dataInternal.weights.size(); i++)
        {
            // assign parentsX[i] to proper child
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        }

        // get random cross points
        int crossPoint1 = rand() % dataInternal.weights.size();
        int crossPoint2 = rand() % dataInternal.weights.size();

        // cross if first if greater than second make swap
        if (crossPoint1 > crossPoint2)
        {
            std::swap(crossPoint2, crossPoint1);
        }

        // for crossPoint1..crossPoint2 make swaps
        for (int i = crossPoint1; i <= crossPoint2; ++i)
        {
            std::swap(child1[i], child2[i]);
        }

        // for crossPoint1..crossPoint2 do child assigns
        for (int i = crossPoint1; i <= crossPoint2; ++i)
        {
            // for crossPoints1 number do
            for (int j = 0; j < crossPoint1; j++)
            {
                // if childs equals then assign
                if (child1[j] == child1[i])
                    child1[j] = child2[i];
                if (child2[j] == child2[i])
                    child2[j] = child1[i];
            }
        }
    }
    else if (dataInternal.crossMethodVariant == 2)
    {
        //second method
        //experimantal
        // get random cross points
        int crossPoint1 = (int) (rand() * parent1.size());
        int crossPoint2 = (int) (rand() * parent1.size());

        // for 0..child1.size make
        for (int i = 0; i < child1.size(); i++)
        {
            // if crossPoints1 is less than crossPoint2, and i is in (crossPoint1;crossPoint2)
            if (crossPoint1 < crossPoint2 and i > crossPoint1 and i < crossPoint2)
            {
                child1[i] = parent1[i];
            }
            // if crossPoints1 is greater than crossPoint2
            else if (crossPoint1 > crossPoint2)
            {
                // if i is not in (crossPoint2;crossPoint1)
                if (not (i < crossPoint1 and i > crossPoint2))
                    child2[i] = parent1[i];
            }
        }

        // for 0..parent2.size make
        for (int i = 0; i < parent2.size(); i++)
        {
            // if child doesn't have key -> add
            if (std::count(child1.begin(), child1.end(), parent2[i]))
            {
                // for 0..child1.size find free position
                for (int ii = 0; ii < child1.size(); ii++)
                {
                    // free position found -> add
                    if (not child1[ii])
                    {
                        child1[ii] = parent2[i];
                        break;
                    }
                }
            }
        }
    }

    //     make mutations
    mutate(child1);
    mutate(child2);

    // push back children to children vector
    children.push_back(child1);
    children.push_back(child2);

    parent1.clear();
    parent2.clear();

    child1.clear();
    child2.clear();
}

void mutate(std::vector<int>& child)
{
    // mutation finction
    // if random double if less than mutationFactor

    if (dataInternal.mutationMethodVariant == 1)
    {
        //first method
        if (getRandomDouble() < dataInternal.mutationFactor)
        {
            // for 0..swapsInMutation make swaps
            for (int i = 0; i < dataInternal.swapsInMutation; i++)
            {
                std::swap(child[rand() % dataInternal.weights.size()],child[rand() % dataInternal.weights.size()]);
            }
        }
    }
    else if (dataInternal.mutationMethodVariant == 2)
    {
        // second method
        // adjacent swap
        if (getRandomDouble() < dataInternal.mutationFactor)
        {
            for (int i = 0; i < dataInternal.swapsInMutation; i++)
            {
                // get random index from <0;dimension-2> to avoid getting last index or number that not belongs to index set
                int point_ = ((rand() % dataInternal.weights.size()) - 2);
                int point = point_ < 0 ? 0 : point_;

                // make swap of drawn vert with his adjacent
                std::swap(child[point],child[point+1]);
                // example:
                // 1 2 3 4 5    Drawn: 2
                //after: 1 2 4 3 5
            }
        }
    }
}

void generateNewPopulation()
{
    // generating new population
    //vecs inits
    std::vector<std::vector<int>> newPopulation;
    newPopulation.reserve(dataInternal.populationSize);

    // make asc sort population
    sort(population.begin(), population.end(),
         [](auto i, auto j)
         {
            return calculateProfit(i) < calculateProfit(j);
         });

    // make asc sort on children
    sort(children.begin(), children.end(),
         [](auto i, auto j)->bool
         {
            return calculateProfit(i) < calculateProfit(j);
         });

    // for 0..populationSize
    for (int i = 0; i < dataInternal.populationSize; i++)
    {
        // if children empty and population not empty
        if (children.empty() and not population.empty())
        {
            newPopulation.push_back(population.back());
            population.pop_back();
            continue;
        }

        // if children not empty and population empty
        if (not children.empty() and population.empty())
        {
            newPopulation.push_back(children.back());
            children.pop_back();
            continue;
        }

        // choose better
        if (population.back() > children.back())
        {
            newPopulation.push_back(population.back());
            population.pop_back();
        }
        else
        {
            newPopulation.push_back(children.back());
            children.pop_back();
        }
    }

    // clear vecs
    population.clear();
    children.clear();

    // assign new population
    population = newPopulation;
}

void genetic(Data& data)
{
    dataInternal = data;
    parents.clear();
    children.clear();
    population.clear();
    bestResultVec.clear();
    bestResult = 0;

    // random seed init
    srand((unsigned int)time(NULL));

    // time stop criteria init
    auto internalBeginTime = std::chrono::system_clock::now();
    auto currentTimeInSeconds = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    // generate initial population
    generatePopulation();

    for (;;)
    {
        // stop criteria implementation
        currentTimeInSeconds = std::chrono::system_clock::now();
        elapsed_seconds = currentTimeInSeconds - internalBeginTime;
        if ( elapsed_seconds.count() > dataInternal.stopCriteria)
        {
            break;
        }

        // choose paretns for max completion time function
        chooseParents();

        // make cross function with choosen cross method variant
        if (getRandomDouble() < dataInternal.crossFactor)
        {
            cross();
        }
        // generate new population after crossing
        generateNewPopulation();

        //calculate current length using max completion time function
        int currentLength = calculateProfit(population.front());

        //update if better
        if (currentLength > bestResult)
        {
            bestResult = currentLength;
            bestResultVec = population.front();
        }
    }

    data.resultVec.clear();
    for (int k=0; k<dataInternal.weights.size(); k++)
    {
      if (bestResultVec[k]==1)
      {
        data.resultVec.push_back(dataInternal.weights[k]);
      }
    }

    data.result = bestResult;
}
