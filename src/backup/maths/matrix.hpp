#ifndef _MATRIX
#define _MATRIX

#include <vector>

class Matrix
{
  public: 
   Matrix();
   size_t nrow();
   size_t ncol();
   template <typename T> std::vector<T> matrix<T> operator*(const std::vector<T>&);

  private:
   size_t ncols;
   size_t nrows;
};

#endif // _MATRIX
