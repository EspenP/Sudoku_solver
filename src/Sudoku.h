/*
 * Sudoku.h
 *
 *  Created on: Nov 3, 2019
 *      Author: Espen
 */

#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <map>
#include <vector>

typedef std::pair<int, int> coord;
typedef std::vector<std::vector<int>> matrix;
class Sudoku
{
public:
  Sudoku();

  matrix Get_solved_puzzle()
  {
    return m_solved_puzzle;
  }

  bool Solve_puzzle(matrix puzzle);

  void Print_puzzle(const matrix& puzzle);

  enum SUDOKU_STATE
  {
    BLANK = 0
  };
private:

  bool is_solved(const matrix puzzle);

  bool is_safe(const coord square, matrix puzzle);

  coord next_option(const matrix obj);

  bool is_full(const matrix puzzle);

  /**
   * Puzzle to be returned once solved
   */
  matrix m_solved_puzzle;

  bool m_beginning = true;
};

#endif /* SUDOKU_H_ */
