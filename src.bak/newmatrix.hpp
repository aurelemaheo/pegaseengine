 /*
 *             TMatrix c++ class
 *        
 *
 *        This class implement an object with 2 dimensional array
 *        like a matrix. Elements are stored into a simple array. Another
 *        array contain a pointer on each row.
 * When you build a matrix, like "TMatrix<float> a(6,4);", you must
 * de-allocate memory using the function "a.destroy();". The destructor
 * TMatrix::~Tmatrix(); cause malfunctions... sorry :(
 * If you give only one dimension, like "TMatrix<float> a(3);", you
 * create a square matrix 3x3. Default is 2x2.
 *        To put data on a matrix, create before an array of float with
 *        values and use the function "M.in(DATAARRAY);" if M is the matrix.
 * If DATAARRAY size not correspond at the size of matrix, the result
 * is not guaranteed.
 *        Next, use functions and operators classically:         
 *            M.reverse();
 *            M.determinant();
 *            M0=M1*M2;
 *            ...etc.
 *        For more, contact me at s.c@oreka.com
 */
  
 #ifndef __NewMATRIX_H__
 #define __NewMATRIX_H__
  
 #include <iostream>
 #include <math.h>

using namespace std;
  
 // Uses this declaTn for C ONLY
 // version of memory allocation
 // include <alloc.h>
  
/* Exception class for TFloatMatix */
/* class TMatrixException 

 {
     public:
       enum Exception
       {
            DIM_TOO_SMALL ,
            NOT_SAME_DIM ,
            NOT_SQUARE ,
            NOT_MULTIPLICABLE
       };
  
       TMatrixException(Exception);
       Exception Err;
 };
  */
 template <class T> class TMatrix
 {
     public:
       /* ---------------------- Attributes --------------------------- */
       T *M;                             /* Linear matrix data storage */
       T **P;                         /* Pointers on each rows */
       int dimR;                         /* Number of rows ( column size ) */
       int dimC;                         /* Number of columns ( row size ) */
         
      /* ----------------------- Builders --------------------------- */
       TMatrix(int dim=2); /* Default, for square matrix */
       TMatrix(int,int); /* For NxM matrix */
       TMatrix(const TMatrix<T>&); /* Copy */
       ~TMatrix();                                 /* de-allocate memory */
  
       /* -------------------- Utils procedures ----------------------- */
       void clear();                            /* Reset to 0 all elements */
       void destroy(); /* de-allocate memory */
       void resize(int,int); /* Set a new dimension */
       void resize(int dim=2); /* The same, for square */
       void in(T*);                         /* Input data in a matrix */
       ostream& out(ostream&) const;     /* Display matrix on std::out */
  
      /* ------------- Functions and procedure on this object ------- */
       TMatrix cofactor();                      /* Return cofactor matrix */
       T cofactor(int, int);                          /* 1 element cofactor */
       T minor(int, int);                             /* 1 element minor */
       T determinant();                        /* Determinant of a matrix */
       void transpose();                             /* Transpose a matrix */
       void reverse();                             /* Reverse a matrix */
       void TC();                     /* Transpose and cofactor matrix */
  
      /* ---------------------- Operators ---------------------------- */
       TMatrix<T>& operator=(const TMatrix<T>&);
     
      TMatrix<T> operator-()                  ;
       TMatrix<T> operator+(const TMatrix<T>&) ;
       TMatrix<T> operator-(const TMatrix<T>&) ;
       TMatrix<T> operator*(const TMatrix<T>&) ;
       TMatrix<T> operator*(const T);
       TMatrix<T> operator*=(const T);
  
     private:
       T det3x3();            /* return the determinant of a 3x3 matrix */
       T det2x2();            /* return the determinant of a 2x2 matrix */
       TMatrix<T> matrix4minor(int,int);     /* Return matrix of minor */
       TMatrix<T> mul(const TMatrix<T>&);      /* Multiply two matrices */
 };
  
 template <class T>
 ostream& operator<< (ostream& o, const TMatrix<T>& m)
 {return m.out(o);}
  
 template <class T>
 istream& operator>> (istream&, TMatrix<T>&);

 //===========================================================================

/*TMatrixException::TMatrixException(Exception e) : Err(e)
 {
     if (e==DIM_TOO_SMALL)
     {
         cout <<"ERR: DIM TOO SMALL"<<endl<<endl;
         system("pause");
         return;//exit(0);
     }
     if (e==NOT_SAME_DIM)
     {
         cout <<"ERR: NOT SAME DIM"<<endl;
         cout <<"US: Add or substract only same size matrix."<<endl;
         cout <<"FR: Additionner ou soustraire uniquement des ";
         cout <<"matrices de meme taille"<<endl;
         system("pause");
         return;//exit(0);
     }
     if (e==NOT_SQUARE)
     {
         cout <<"ERR: NOT SQUARE"<<endl<<endl;
         cout <<"US: Attempt to use a function reserved to square matrix ";
         cout <<"on a not-square matrix."<<endl;
         cout <<"FR: Tentative d'utilisation d'une fonction reservee aux ";
         cout <<"matrices carrees sur une matrice non-carree."<<endl;
         system("pause");
         return;//exit(0);
     }
     if (e==NOT_MULTIPLICABLE)
     {
         cout <<"ERR: NOT MULTIPLICABLE"<<endl<<endl;
         cout <<"US: To multiply, the number of columns in first matrix ";
         cout <<"must be equal to number of rows in the second."<<endl;
         cout <<"FR: Pour multiplier, le nombre de colonnes de la ";
         cout <<"premiere matrice doit etre egal au nombre de lignes ";
         cout <<"de la deuxieme."<<endl;
         system("pause");
         return;//exit(0);
     }
     else
     {
         cout <<"UNKNOW EXCEPTION"<<endl;
         system("pause");
         return ;//exit(0);
     }
 }*/
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T>::TMatrix(int dimRow, int dimCol)
 {
     if (dimRow<1 || dimCol<1)
         throw TMatrixException(TMatrixException::DIM_TOO_SMALL);
  
     dimR=dimRow;
     dimC=dimCol;
     
     // C ONLY VERSION
     // M=(float*) malloc (dimR*dimR*sizeof(float));
     // P=(float**) malloc (dimR*sizeof(float*));
  
     // C++ VERSION
     M=new T[dimR*dimC*sizeof(T)];
     P=new T*[dimR*sizeof(T*)];
  
     T *ref;
     ref=M;
     for (int i=0; i<dimR; i++)
     {
         P[i]=ref;
         ref+=dimC;
     }
     clear();
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T>::TMatrix(int dim)
 {
     if (dim<1)
         throw TMatrixException(TMatrixException::DIM_TOO_SMALL);
  
     dimR=dim;
     dimC=dim;
     
     // C ONLY VERSION
     // M=(float*) malloc (dimR*dimR*sizeof(float));
     // P=(float**) malloc (dimR*sizeof(float*));
  
     // C++ VERSION
     M=new T[dimR*dimC*sizeof(T)];
     P=new T*[dimR*sizeof(T*)];
  
     T *ref;
     ref=M;
     for (int i=0; i<dimR; i++)
     {
         P[i]=ref;
         ref+=dimC;
     }
     clear();
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T>::TMatrix(const TMatrix<T>& m)
 {
     dimR=m.dimR;
     dimC=m.dimC;
     M=m.M;
     P=m.P;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T>::~TMatrix()
 {
     /* use the function destroy() after use. */
     /* This destructor cause exceptions ... */
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::destroy()
 {
     // C ONLY VERSION
     // free(M);
     // free(P);
  
     // C++ VERSION
     for (int i=0; i<dimR; i++)
         delete [] P[i];
     delete[] P;
     delete[] M;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::clear()
 {
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimC; j++)
             P[i][j]=0;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::resize(int rw, int cl)
 {
     TMatrix<T> m(rw,cl);
     if (dimR*dimC <= rw*cl)
     {
         for (int i=0; i<dimR;i++)
             for (int j=0; j<dimC; j++)
                 m.P[i][j]=P[i][j];
         for (int i2=dimR; i2<rw; i2++)
             for (int j2=dimC; j2<cl; j2++)
                 m.P[i2][j2]=0;
     }
     else
     {
         for (int i=0; i<rw;i++)
             for (int j=0; j<cl; j++)
                 m.P[i][j]=P[i][j];
     }
     *this=m;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::resize(int dim)
 {
     TMatrix<T> m(dim);
     if (dimR*dimC <= dim*dim)
     {
         for (int i=0; i<dimR;i++)
             for (int j=0; j<dimC; j++)
                 m.P[i][j]=P[i][j];
         for (int i2=dimR; i2<dim; i2++)
             for (int j2=dimC; j2<dim; j2++)
                 m.P[i2][j2]=0;
     }
     else
     {
         for (int i=0; i<dim;i++)
             for (int j=0; j<dim; j++)
                 m.P[i][j]=P[i][j];
     }
     *this=m;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::in(T *data)
 {
     for (int i=0; i<dimR;i++)
         for (int j=0; j<dimC; j++)
             P[i][j]=data[dimC*i+j];
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 ostream& TMatrix<T>::out(ostream& o) const
 {
     for (int i=0; i<dimR; i++)
     {
         for (int j=0; j<dimC; j++)
             o <<P[i][j]<<" ";
         o <<endl;
     }
     return o;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T>& TMatrix<T>::operator=(const TMatrix<T>& m)
 {
     dimR=m.dimR;
     dimC=m.dimC;
     M=m.M;
     P=m.P;
     return *this;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::operator-()
 {
     TMatrix<T> r(dimR,dimC);
     r=*this * T(-1);
     return r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::operator+(const TMatrix<T>& m)
 {
     if (dimR!=m.dimR || dimC!=m.dimC)
         throw TMatrixException(TMatrixException::NOT_SAME_DIM);
  
     TMatrix<T> r(dimR,dimC);
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimC; j++)
             r.P[i][j]=P[i][j]+m.P[i][j];
     return r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::operator-(const TMatrix<T>& m)
 {
     if (dimR!=m.dimR || dimC!=m.dimC)
         throw TMatrixException(TMatrixException::NOT_SAME_DIM);
  
     TMatrix<T> r(dimR,dimC);
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimC; j++)
             r.P[i][j]=P[i][j]-m.P[i][j];
     return r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::operator*(const TMatrix<T>& m)
 {
     if (dimC!=m.dimR)
         throw TMatrixException(TMatrixException::NOT_MULTIPLICABLE);
  
     TMatrix<T> r(dimR,m.dimC);
     r=this->mul(m);
     return r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::operator*(const T f)
 {
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimC; j++)
             P[i][j]*=f;
     return *this;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::operator*=(const T f)
 {
     *this=*this*f;
     return *this;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::mul(const TMatrix<T>& m)
 {
     TMatrix<T> r(dimR,m.dimC);
     T e=0;
     for (int i=0; i<dimR; i++)
         for (int j=0; j<m.dimC; j++)
         {
             for (int k=0; k<dimC; k++)
                 e+=P[i][k]*m.P[k][j];
             r.P[i][j]=e;
             e=0;
         }
     return r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::reverse()
 {
     if (dimR!=dimC)
         throw TMatrixException(TMatrixException::NOT_SQUARE);
  
     TMatrix<T> r(dimR,dimC);
     r=cofactor();
     r.transpose();
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimR; j++)
             r.P[i][j]=r.P[i][j]*pow(-1,i+j+2);
     r*=T(1)/determinant();
     *this=r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::TC()
 {
     if (dimR!=dimC)
         throw TMatrixException(TMatrixException::NOT_SQUARE);
  
     TMatrix<T> r(dimR,dimC);
     r=cofactor();
     r.transpose();
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimR; j++)
             r.P[i][j]=r.P[i][j]*pow(-1,i+j+2);
     *this=r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 void TMatrix<T>::transpose()
 {
     TMatrix<T> m(dimC,dimR);
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimC; j++)
         { m.P[i][j]=P[j][i];}
     *this=m;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 T TMatrix<T>::det3x3()
 {
     return P[0][0] * ( (P[1][1]*P[2][2])-(P[1][2]*P[2][1]) )
            - P[1][0] * ( (P[0][1]*P[2][2])-(P[0][2]*P[2][1]) )
            + P[2][0] * ( (P[0][1]*P[1][2])-(P[0][2]*P[1][1]) );
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 T TMatrix<T>::det2x2()
 {
     return P[0][0]*P[1][1] - P[0][1]*P[1][0];
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 T TMatrix<T>::determinant()
 {
     if (dimR!=dimC)
         throw TMatrixException(TMatrixException::NOT_SQUARE);
  
     if (dimR==2) return det2x2();
     if (dimR==3) return det3x3();
     T d=0;
     for (int i=0; i<dimR; i++)
         d+=P[i][0] * cofactor(i,0);
     return d;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 T TMatrix<T>::cofactor(int ir, int ic)
 {
     if (dimR!=dimC)
         throw TMatrixException(TMatrixException::NOT_SQUARE);
  
     return minor(ir,ic) * pow(-1, ir+ic+2);
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 T TMatrix<T>::minor(int ir, int ic)
 {
     if (dimR!=dimC)
         throw TMatrixException(TMatrixException::NOT_SQUARE);
  
     TMatrix<T> m4m(dimR-1);
     m4m=matrix4minor(ir,ic);
     return m4m.determinant();
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::cofactor()
 {
     if (dimR!=dimC)
         throw TMatrixException(TMatrixException::NOT_SQUARE);
  
     TMatrix<T> r(dimR);
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimR; j++)
             r.P[i][j]=cofactor(i,j);
     return r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 TMatrix<T> TMatrix<T>::matrix4minor(int ir, int ic)
 {
     if (dimR!=dimC)
         throw TMatrixException(TMatrixException::NOT_SQUARE);
     
     int indexR=0;
     int indexC=0;
     TMatrix<T> r(dimR-1);
     for (int i=0; i<dimR; i++)
         for (int j=0; j<dimR; j++)
         {
             if (i!=ir && j!=ic)
             {
                 r.P[indexR][indexC]=P[i][j];
                 indexC++;
             }
             if (indexC==r.dimR)
             {
                 indexC=0;
                 indexR++;
             }
             if (indexR==r.dimR)
                 return r;
         }
     return r;
 }
 //---------------------------------------------------------------------------
  
 template <class T>
 istream& operator>> (istream& in, TMatrix<T> &m)
 {
     int rw,cl;
     cout << "DIM ROW: ";
     in >> rw;
     cout << "DIM COL: ";
     in >> cl;
     
     if (rw<1 && cl<1)
         throw TMatrixException(TMatrixException::DIM_TOO_SMALL);
  
     m.resize(rw,cl);
     T *data=new T[rw*cl];
     T element;
     for (int i=0; i<rw; i++)
         for (int j=0; j<cl; j++)
         {
             cout << i <<','<<j<<':';
             in >> element;
             data[cl*i+j]=element;
         }
      for (int x=0; x<rw; x++)
         for (int y=0; y<cl; y++)
             m.P[x][y]=data[cl*x+y];
     delete [] data;
     return in;
 }
 //---------------------------------------------------------------------------  
  #endif
