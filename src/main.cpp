#include "Sudoku.h"
#include <iostream>

int main()
{
  matrix puzzle;

  puzzle.push_back(std::vector<int>{2, 0, 0, 7, 0, 0, 0, 0, 0});
  puzzle.push_back(std::vector<int>{5, 0, 0, 0, 0, 1, 0, 0, 0});
  puzzle.push_back(std::vector<int>{0, 0, 8, 0, 0, 9, 0, 6, 3});
  puzzle.push_back(std::vector<int>{9, 0, 0, 0, 0, 4, 0, 0, 0});
  puzzle.push_back(std::vector<int>{0, 0, 0, 0, 0, 0, 8, 3, 0});
  puzzle.push_back(std::vector<int>{0, 4, 1, 0, 6, 0, 0, 0, 9});
  puzzle.push_back(std::vector<int>{0, 9, 7, 0, 0, 8, 6, 0, 0});
  puzzle.push_back(std::vector<int>{0, 0, 4, 0, 0, 0, 0, 1, 5});
  puzzle.push_back(std::vector<int>{0, 0, 0, 0, 0, 0, 3, 0, 0});

  Sudoku solver;
  try
  {
    if(solver.Solve_puzzle(puzzle))
    {
      std::cout << "SOLUTION FOUND! \n";
    }
    else
    {
      std::cout << "NO SOLUTION FOUND\n";
      solver.Print_puzzle(solver.Get_solved_puzzle());
    }
  }
  catch(const std::string& e)
  {
    std::cerr << e;
  }
  solver.Print_puzzle(solver.Get_solved_puzzle());
  return 0;
}
