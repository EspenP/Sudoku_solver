/*
 * Sudoku.cpp
 *
 *  Created on: Nov 3, 2019
 *      Author: espen
 */
#include "Sudoku.h"
#include <algorithm>
#include <iostream>
#include <exception>

Sudoku::Sudoku () :
  m_solved_puzzle()
{
  //No op
}

bool Sudoku::Solve_puzzle(matrix puzzle)
{
  if(m_beginning)
  {
    m_beginning = false;
    std::cout << "Checking puzzle safety...";
  for(size_t i = 0; i < puzzle.size(); i++)
  {
    for(size_t j = 0; j < puzzle[i].size(); j++)
    {
      if(!is_safe(coord(i, j), puzzle))
      {
        std::cerr << "\nERROR: GIVEN PUZZLE IS NOT VALID!\n" <<
                     "Coordinate " << i << "," << j << "is invalid\n";
        Print_puzzle(puzzle);
        return false;
      }
    }
  }
  }
  coord tile(0, 0);
  if(!is_full(puzzle))
  {
    tile = next_option(puzzle);
  }
  else if(is_solved(puzzle))
  {
    m_solved_puzzle = puzzle;
    return true;
  }
  else
  {
    m_solved_puzzle = puzzle;
  }

  for(int val = 1; val <= 9; val++)
    {
      puzzle[tile.first][tile.second] = val;
      if(!is_safe(tile, puzzle))
        {
          continue;
        }

      if(Solve_puzzle(puzzle))
        {
          return true;
        }
    }
  //Triggers backtracking
  return false;
}

bool Sudoku::is_safe(const coord tile, matrix puzzle)
{
  int val = puzzle[tile.first][tile.second];
  if(val == BLANK)
  {
    // Don't worry about blank squares.
    return true;
  }
  puzzle[tile.first][tile.second] = BLANK;
  //Get row
  int rownum = tile.first;
  std::vector<int> row(puzzle[rownum]);

  for(size_t i = 0; i < row.size(); i++)
  {
    if(row[i] == val)
    {
      return false;
    }
  }

  //Get col
  int colnum = tile.second;
  std::vector<int> col;
  for(int i = 0; i < 9; i++)
  {
    col.push_back(puzzle[i][colnum]);
  }
  for(size_t i = 0; i < col.size(); i++)
  {
    if(col[i] == val)
    {

      return false;
    }
  }

  //Get section
  int rowstart = 0;
  int colstart = 0;
  int rowend = 2;
  int colend = 2;

  if(rownum > 2 && rownum <= 5)
  {
    rowstart = 3;
    rowend = 5;
  }
  else if(rownum > 5)
  {
    rowstart = 6;
    rowend = 8;
  }

  if(colnum > 2 && colnum <= 5)
  {
    colstart = 3;
    colend = 5;
  }
  if(colnum > 5)
  {
    colstart = 6;
    colend = 8;
  }
  for(int i = rowstart; i <= rowend; i++)
  {
    for(int j = colstart; j <= colend; j++)
    {
      if(val == puzzle[i][j])
      {
        return false;
      }
    }
  }
  //If we haven't found it at this point, it is a safe move.
  return true;
}

bool Sudoku::is_solved(const matrix puzzle)
{
  for(int i = 0; i < 9; i++)
  {
    for(int j = 0; j < 9; j++)
    {
      if(!is_safe(coord(i, j), puzzle))
        {
          return false;
        }
    }
  }
  return true;
}

coord Sudoku::next_option(const matrix puzzle)
{
  for(int i = 0; i < 9; i++)
  {
    for(int j = 0; j < 9; j++)
    {
      if(puzzle[i][j] == BLANK)
      {
        return coord(i, j);
      }
    }
  }
  std::cerr << "ERROR: ATTEMPTED TO FIND A BLANK IN A COMPLETE PUZZLE\n";
}


bool Sudoku::is_full(const matrix puzzle)
{
  for(int i = 0; i < 9; i++)
  {
    for(int j = 0; j < 9; j++)
    {
      if(puzzle[i][j] == BLANK)
      {
        return false;
      }
    }
  }
  //puzzle is already full somehow, return false.
  return true;
}

void Sudoku::Print_puzzle(const matrix& puzzle)
{
  std::string tab("\t");
  std::string space = " ";
  for(size_t i = 0; i < puzzle.size(); i++)
  {
    std::cout << "\n" << tab;
    for(size_t j = 0; j < puzzle[i].size(); j++)
    {
      std::cout << puzzle[i][j] << space;
    }
  }
  std::cout << "\n\n";
}
