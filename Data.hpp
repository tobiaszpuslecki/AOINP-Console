#pragma once

#include <ncurses.h>
#include <string>
#include <vector>


struct Data
{
    int knapsackSize; // instance size - to change
    int maxCapacity;
    std::vector<int> profits;
    std::vector<int> weights;

    int result;
    std::vector<int> resultVec;

    bool isValid = false;

    int stopCriteria = 4;
    int populationSize = 100;
    double mutationFactor = 0.01;
    double crossFactor = 0.8;
    int crossMethodVariant = 1;
    int mutationMethodVariant = 1;
    int swapsInMutation=10;
};

extern Data data;

inline std::string mygetstring()
{
    std::string input;

    nocbreak();
    echo();

    int ch = getch();

    while ( ch != '\n' )
    {
        input.push_back( ch );
        ch = getch();
    }

    cbreak();

    return input;
}

inline void setStopCriteria()
{
  clear();

  printw( "Podaj kryterium stopu w sekundach (>0)\n" );
  std::string ss= mygetstring();
  if (ss=="")
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
    return;
  }
  int stopCriteria_ = std::stoi(ss);

  if (stopCriteria_ > 0)
  {
      data.stopCriteria = stopCriteria_;
  }
  else
  {
      mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
      getch();
  }

}

inline void setPopulationSize()
{
  clear();

  printw( "Podaj wielkość populacji początkowej (>0)\n" );
  std::string ss= mygetstring();
  if (ss=="")
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
    return;
  }

  int populationSize_ = std::stoi(ss);
  if (populationSize_ > 0)
  {
      data.populationSize = populationSize_;
  }
  else
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
  }
}

inline void setMutationFactor()
{
  clear();

  printw( "Podaj współczynnik mutacji (0:1)\n" );
  std::string ss= mygetstring();
  if (ss=="")
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
    return;
  }

  double mutationFactor_ = std::stof(ss);
  if (mutationFactor_ > 0 and mutationFactor_ < 1)
  {
      data.mutationFactor = mutationFactor_;
  }
  else
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
  }
}

inline void setCrossFactor()
{
  clear();

  printw( "Podaj współczynnik krzyżowania (0:1)\n" );
  std::string ss= mygetstring();
  if (ss=="")
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
    return;
  }

  double crossFactor_ = std::stof(ss);
  if (crossFactor_ > 0 and crossFactor_ < 1)
  {
      data.crossFactor = crossFactor_;
  }
  else
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
  }
}

inline void setCrossMethodVariant()
{
  clear();

  printw( "Wybierz metodę krzyżowania (1 lub 2)\n" );
  std::string ss= mygetstring();
  if (ss=="")
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
    return;
  }

  int crossMethodVariant_ = std::stoi(ss);
  if (crossMethodVariant_ > 0 and crossMethodVariant_ < 3)
  {
      data.crossMethodVariant = crossMethodVariant_;
  }
  else
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
  }
}

inline void setMutationMethodVariant()
{
  clear();

  printw( "Wybierz metodę mutacji (1 lub 2)\n" );
  std::string ss= mygetstring();
  if (ss=="")
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
    return;
  }

  int mutationMethodVariant_ = std::stoi(ss);
  if (mutationMethodVariant_ > 0 and mutationMethodVariant_ < 3)
  {
      data.mutationMethodVariant = mutationMethodVariant_;
  }
  else
  {
    mvprintw(2,0,"Niepoprawne dane ! Wciśnij Enter\n");
    getch();
  }
}
