#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include "Data.hpp"
#include "Reference.hpp"
#include "Genetic.hpp"
#include <chrono>
#include <queue>

const char alts[][100] = {{"1. Wczytanie danych z pliku i wyświetlenie"},
                     {"2. Kryterium stopu"},
                     {"3. Ustawienie wielkości populacji początkowej"},
                    {"4. Ustawienie wspołczynnika mutacji"},
                    {"5. Ustawienie wspołczynnika krzyżowania"},
                    {"6. Wybór metody krzyżowania"},
                    {"7. Wybór metody mutacji"},
                    {"8. Uruchomienie algorytmu referencyjnego"},
                    {"9. Uruchomienie algorytmu genetycznego"},
                     {"0. Wyjście"},};
const int offset=12;
const short int menuSize = sizeof(alts)/sizeof(alts[0]);
Data data;
std::string filename = "";

void printMenuWithHilighted(unsigned int selected)
{
  for (size_t i = 0; i < menuSize; i++)
  {
    if (i==selected)
    {
      attron( A_REVERSE );
      mvprintw( offset+i, 0, alts[i] );
      attroff( A_REVERSE );
      continue;
    }
    mvprintw( offset+i, 0, alts[i] );
  }
}

void foo(){}

void ncursesExit()
{
  clear();
  reset_prog_mode(); // return to previous tty mode stored by def_prog_mode
  refresh();         // restore screen contents
  // ... we are back in ncures mode
  endwin();          // end ncurses for real this time
}

void appendToResultFile(int method)
{
  std::ofstream outfile;

  outfile.open("accumulatedResults.csv", std::ios_base::app);
  outfile << filename << "," << method << "," << data.knapsackSize << "," << data.maxCapacity << "," << data.result << ","
  << data.stopCriteria << "," << data.populationSize << "," << data.mutationFactor << "," <<  data.crossFactor << ","
  << data.crossMethodVariant << "," << data.mutationMethodVariant
   << "\n";

   outfile.close();
}

void printResults(std::string algorithmName, void (*func)(Data&))
{
  clear();

  if (data.isValid)
  {
    func(data);
    mvprintw(1,0,"%s Result        : %d", algorithmName.c_str(), data.result );
    appendToResultFile(1);

    int i=0;
    for(const auto& v : data.resultVec)
    {
      mvprintw(3+i++,0,"%d", v );
    }
  }
  else
  {
    printw( "data is not valid " );
  }

  getch();
}

void geneticAlgorithm()
{
  printResults("Genetic", genetic);
}

void brutalAlgorithm()
{
  printResults("Reference", reference);
}

void fileProcessing()
{
  clear();
  const auto entries = std::filesystem::directory_iterator(".");

  std::vector<std::string> ssv;

  for (const auto & entry : entries)
  {
    std::string t = entry.path();
    std::string s = "./";

    std::string::size_type i = t.find(s);

    if (i != std::string::npos)
       t.erase(i, s.length());

      ssv.push_back(t);
  }

  int entriesSize = ssv.size();

  int selected = 1;
  int pressed=0xFF;


  while (( selected != entriesSize || pressed != 10 ))
  {
      clear();
      printw( "Wybierz plik z danymi | Nawigacja a/z | arrow | enter  ...\n" );

      if( (pressed == 'a' || pressed == 65) && selected != 1 )
      {
          selected--;
      }
      else if( (pressed == 'z' || pressed == 66) && selected != entriesSize )
      {
          selected++;
      }

      for (size_t i = 0; i < entriesSize; i++)
      {
        if (i==selected-1)
        {
          attron( A_REVERSE );
          mvprintw( 2+i, 0, ssv[i].c_str() );
          attroff( A_REVERSE );
          continue;
        }
        mvprintw( 2+i, 0, ssv[i].c_str() );
      }

      pressed = getch();

      if( pressed == 10 )
      {
          filename = ssv[selected-1];
          break;
      }
  }


  std::ifstream ifs (filename, std::ifstream::in);

  data.profits.clear();
  data.weights.clear();

  ifs >> data.knapsackSize;
  ifs >> data.maxCapacity;
  data.isValid = (data.knapsackSize!=0 and data.maxCapacity !=0);
  int buff;

  for (size_t i = 0; i < data.knapsackSize; i++)
  {
    ifs >> buff;
    data.profits.push_back(buff);
    ifs >> buff;
    data.weights.push_back(buff);
  }

  ifs.close();
}

int main()
{
    def_prog_mode();
    setlocale (LC_CTYPE, "");

    initscr();
    noecho();

    void (*func_ptr[menuSize])() = {fileProcessing, setStopCriteria, setPopulationSize,
      setMutationFactor, setCrossFactor, setCrossMethodVariant,
      setMutationMethodVariant, brutalAlgorithm, geneticAlgorithm, foo};

    int selected = 1;
    int pressed=0xFF;

    while (( selected != menuSize || pressed != 10 ))
    {
        clear();
        printw( "AOINP Projekt - Tobiasz Puślecki & Mikołaj Kamiński | Nawigacja a/z | arrow | enter  ...\n" );

        mvprintw(2,0,"Parametry: ");
        mvprintw(3,0,"stop Criteria         : %d", data.stopCriteria );
        mvprintw(4,0,"populationSize        : %d", data.populationSize );
        mvprintw(5,0,"mutationFactor        : %f", data.mutationFactor );
        mvprintw(6,0,"crossFactor           : %f", data.crossFactor );
        mvprintw(7,0,"crossMethodVariant    : %d", data.crossMethodVariant );
        mvprintw(8,0,"mutationMethodVariant : %d", data.mutationMethodVariant );
        mvprintw(9,0,"filename              : %s", filename.c_str() );

        if( (pressed == 'a' || pressed == 65) && selected != 1 )
        {
            selected--;
        }
        else if( (pressed == 'z' || pressed == 66) && selected != menuSize )
        {
            selected++;
        }

        printMenuWithHilighted(selected-1);

        pressed = getch();

        if( pressed == 10 )
        {
          (*func_ptr[selected-1])();

        }
    }

    ncursesExit();
}
