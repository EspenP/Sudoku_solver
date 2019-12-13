/*
 * Matrix.h
 *
 *  Created on: Nov 2, 2019
 *      Author: espen
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include <algorithm>
#include <iostream>

typedef std::pair<int, int> coord;
template<typename T>
class Matrix
{
public:
  /**
   * Default constructor
   */
  Matrix() :
  m_rows(3),
  m_cols(3),
  m_row_coord(0),
  m_col_coord(0),
  m_matrix(m_rows, std::vector<T>(m_cols, 0)),
  m_current_row(Get_row(0)),
  m_current_col(Get_col(0))
  {

  }

  /**
   * Constructor to construct uniquely sized arrays
   */
  Matrix(int rows, int cols, T fill = 0) :
  m_rows(rows),
  m_cols(cols),
  m_row_coord(0),
  m_col_coord(0),
  m_matrix(m_rows, std::vector<T>(m_cols, fill)),
  m_current_row(Get_row(0)),
  m_current_col(Get_col(0))
  {

  }

  /**
   * Copy constructor
   */
  Matrix(Matrix& obj)  :
    Matrix(obj.Get_num_rows(), obj.Get_num_cols())
  {
    *this = obj;
  }

  /**
   * Copy constructor
   */
  Matrix(std::vector<std::vector<T>> vec) :
    Matrix(9, 9)
  {
    for(int i = 0; i < m_rows; i++)
      {
        for(int j = 0; j < m_cols; j++)
          {
            this->At(i, j) = vec[i][j];
          }
      }
  }

  /**
   * Copy constructor for a portion of a larger matrix
   */
  Matrix(Matrix& obj, int rowstart, int rowend, int colstart, int colend) :
    Matrix(rowend - rowstart, colend - colstart)
  {
    int main_row = rowstart;
    int main_col =  colstart;
    for(int row = 0; row < m_rows; row++)
      {
        for(int col = 0; col < m_cols; col++)
          {
            this->At(row, col) = obj.At(row, col);
          }
        main_row++;
        main_col = colstart;
      }
  }

  /**
   * Getter for m_rows
   */
  int Get_num_rows(){return m_rows;}

  /**
   * Getter for m_cols
   */
  int Get_num_cols(){return m_cols;}

  int Get_col_coord(){return m_col_coord;}

  /**
   * Get a specific row
   */
  std::vector<T> Get_row(int rownum)
  {
    std::vector<T> ret_vector;
    for(int i = 0; i < m_cols; i++)
      {
        ret_vector.push_back(m_matrix[rownum][i]);
      }

    m_current_row.clear();
    m_current_row = ret_vector;
    return m_current_row;
  }

  /**
   * Get the iterator to a specific row
   */
  typename std::vector<T>::iterator Get_row_itr(int rownum)
  {
    Get_row(rownum);
    return m_current_row.begin();
  }

  /**
   * Get a specific column
   */
  std::vector<T> Get_col(int colnum)
  {
    std::vector<T> ret_vector;
    for(int i = 0; i < m_rows; i++)
      {
        ret_vector.push_back(m_matrix[i][colnum]);
      }

    m_current_col.clear();
    m_current_col = ret_vector;
    return m_current_col;
  }

  /**
   * Get the iterator to a specific column
   */
  typename std::vector<T>::iterator Get_col_itr(int rownum)
  {
    Get_row(rownum);
    return m_current_col.begin();
  }

  /**
   * At operator for the matrix
   */
  T& At(int row, int col)
  {
    return m_matrix[row][col];
  }

  T& At(std::pair<T, T> coords)
  {
    return this->At(coords.first, coords.second);
  }

  Matrix& operator=(const Matrix* copy)
  {
    if((this->Get_num_rows() != copy->Get_num_rows()) ||
       (this->Get_num_cols() != copy->Get_num_cols()))
      {
        //TODO Make your own exceptions class. Just inherit from std::exception
      }
    int row_count = 0;
    for(auto& i : m_matrix)
      {
        i = copy.Get_row(row_count++);
      }
  }

  bool Contains(T n)
  {
    for(auto it : m_matrix)
      {
        if(Contains(n, it))
          {
            return true;
          }
      }
    return false;
  }

  bool Contains(T n, std::vector<T> vec)
  {
    if(std::find(vec.begin(), vec.end(), n) != vec.end())
      {
        return true;
      }
    else return false;
  }

  bool Row_contains(T n, int rownum)
  {
    if(Contains(n, this->Get_row(rownum)))
      {
        return true;
      }
    else return false;
  }

  bool Col_contains(T n, int colnum)
  {
    if(Contains(n, this->Get_col(colnum)))
      {
        return true;
      }
    else return false;
  }

  coord Find_next_of(T thing)
  {
    for(int i = 0; i < this->Get_num_rows(); i++)
          {
            for(int j = 0; j < this->Get_num_cols(); j++)
              {
                if(m_matrix[i][j] == thing)
                  {
                    return coord(i, j);
                  }
              }
          }
    std::cout << "No instances of this thing\n";
    return coord(0, 0);
  }

  void Print_matrix()
  {
    std::cout << std::endl << std::endl;
    for(auto r_it : m_matrix)
      {
        for(auto it : r_it)
          {
            std::cout << it << "  ";
          }
        std::cout << std::endl;
      }
  }
private:

  int m_rows;

  int m_cols;

  int m_row_coord;

  int m_col_coord;

  std::vector<std::vector<T>> m_matrix;

  std::vector<T> m_current_row;

  std::vector<T> m_current_col;
};

#endif /* MATRIX_H_ */
