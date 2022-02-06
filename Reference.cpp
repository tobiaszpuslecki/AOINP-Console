/*
 * Knapsack 0/1 - Dynamic Programming
 * References:
 * http://cse.unl.edu/~goddard/Courses/CSCE310J/Lectures/Lecture8-DynamicProgramming.pdf
 * Time Complexity: O(N*W) (no. of weight element * capacity)
 * DP[i][j] == maximum value of j-weight considering all values from 1 to i
*/

#include "Reference.hpp"
#include <algorithm>
#include <iostream>

// sort by profit/weight factor -> then take best elements up to backpack is full
void reference(Data& data)
{
  std::vector<std::pair<int, int>> pairs;
  for (size_t i = 0; i < data.profits.size(); i++)
  {
    pairs.push_back(std::pair<int, int>(data.profits[i], data.weights[i]));
  }

  sort(pairs.begin(), pairs.end(),
       [](const auto a, const auto b)->bool
       {
          return static_cast<float>(a.first)/a.second > static_cast<float>(b.first)/b.second;
       });

  int currentCapacity = data.maxCapacity;
  data.resultVec.clear();
  data.result = 0;
  for (size_t i = 0; i < pairs.size(); i++)
  {
      if(currentCapacity - pairs[i].second >= 0)
      {
        data.resultVec.push_back(pairs[i].second);
        data.result += pairs[i].first;
        currentCapacity -= pairs[i].second;
      }
  }

}

// Dynamic Programming
void reference2(Data& data)
{
    int capacity, K[data.knapsackSize + 1][data.maxCapacity + 1];

    // Build table knapsackSize+1 * maxCapacity+1 in bottom up (from down to up) way
    for (int i = 0; i <= data.knapsackSize; i++)
    {
        for (capacity = 0; capacity <= data.maxCapacity; capacity++)
        {
            if (i == 0 || capacity == 0)
                K[i][capacity] = 0;
            else if (data.weights[i - 1] <= capacity)
                K[i][capacity] = std::max(data.profits[i - 1] + K[i - 1][capacity - data.weights[i - 1]], K[i - 1][capacity]);
            else
                K[i][capacity] = K[i - 1][capacity];
        }
    }

    int res = K[data.knapsackSize][data.maxCapacity];
    data.result = K[data.knapsackSize][data.maxCapacity];
    capacity = data.maxCapacity;
    data.resultVec.clear();
    for (int i = data.knapsackSize; i > 0 && res > 0; i--)
    {
        if (res != K[i - 1][capacity])
        {
            data.resultVec.push_back(data.weights[i - 1]);
            std::cout << " " << data.weights[i - 1];

            // if weight included -> need to reduce value
            res = res - data.profits[i - 1];
            capacity = capacity - data.weights[i - 1];
        }
    }
}
