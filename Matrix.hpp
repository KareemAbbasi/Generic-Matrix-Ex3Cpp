#include <vector>
#include <iostream>

template <class T>
class Matrix
{
public:
    Matrix();
    Matrix(unsigned int rows, unsigned int cols);
    Matrix(const Matrix& mat);
    Matrix(unsigned int rows, unsigned int cols, const std::vector<T> & cells);
    //TODO Destructor.

    Matrix& operator= (const Matrix mat);
    template <class U> friend Matrix<U> operator+(const Matrix<U> m1, const Matrix<U> m2);
    template <class U> friend Matrix<U> operator-(const Matrix<U> m1, const Matrix<U> m2);
    template <class U> friend Matrix<U> operator*(const Matrix<U> m1, const Matrix<U> m2);
    static T multiplyVector(const std::vector<T> v1, const std::vector<T> v2);

    template <class U> friend bool operator==(const Matrix<U> m1, const Matrix<U> m2);
    template <class U> friend bool operator!=(const Matrix<U> m1, const Matrix<U> m2);

    bool isSquareMatrix();
    //TODO create a const and a non-const version of this method.
    T& operator()(unsigned int row, unsigned int col);


    template <class U> friend std::ostream& operator<<(std::ostream &out, Matrix<U> m1);


    void printMat();
    template <class U> friend Matrix<U> findTranspose(Matrix<U> mat);

//TODO make private
//private:
    unsigned int _numColumns, _numRows;
    std::vector<std::vector <T>> _matrix;

};

template <class T>
Matrix<T>::Matrix()
{
    _numRows = _numColumns = 1;
    std::vector<T> row;
    row.resize(1);
    _matrix.push_back(row);
}

template <class T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols)
{
    _numRows = rows;
    _numColumns = cols;
    _matrix.resize(_numRows);

    for (unsigned i = 0; i < _matrix.size(); ++i)
    {
        _matrix[i].resize(_numColumns);
    }
}

template <class T>
Matrix<T>::Matrix(const Matrix &mat)
{
    _numColumns = mat._numColumns;
    _numRows = mat._numRows;
    _matrix.resize(_numRows);

    for (unsigned i = 0; i < _matrix.size(); ++i)
    {
        _matrix[i].resize(_numColumns);
        for (unsigned j = 0 ; j < _numColumns; ++j)
        {
            _matrix[i][j] = mat._matrix[i][j];
        }
    }
}

template <class T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols, const std::vector<T> &cells)
{
    _numColumns = cols;
    _numRows = rows;
    _matrix.resize(_numRows);

    for (unsigned i = 0; i < _matrix.size(); ++i)
    {
        _matrix[i].resize(_numColumns);
        for (unsigned j = 0; j < _numColumns; ++j)
        {
            //TODO check if cells doesn't have values in the current index, maybe using exceptions.
            _matrix[i][j] = cells[j + i * _numColumns];
        }
    }
}

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix mat)
{
    this->_numColumns = mat._numColumns;
    this->_numRows = mat._numRows;
    this->_matrix.resize(_numRows);

    for (unsigned i = 0; i < this->_matrix.size(); ++i)
    {
        this->_matrix[i].resize(_numColumns);
        for (unsigned j = 0 ; j < this->_numColumns; ++j)
        {
            this->_matrix[i][j] = mat._matrix[i][j];
        }
    }
    return *this;
}

template <class U>
Matrix<U> operator+(const Matrix<U> m1, const Matrix<U> m2)
{
    //TODO if going to do the bonus, add the bool value
    if (m1._numColumns != m2._numColumns || m1._numRows != m2._numRows)
    {
        throw "The two matrices don't have the same size!";
    }

    Matrix<U> newMatrix(m1._numRows, m1._numColumns);

    for (unsigned i = 0 ; i < m1._numRows; ++i)
    {
        for (unsigned j = 0; j < m1._numColumns; ++j)
        {
            //TODO change to use operator()
            newMatrix._matrix[i][j] = m1._matrix[i][j] + m2._matrix[i][j];
        }
    }
    return newMatrix;
}

template <class U>
Matrix<U> operator-(const Matrix<U> m1, const Matrix<U> m2)
{
    if (m1._numColumns != m2._numColumns || m1._numRows != m2._numRows)
    {
        throw "The two matrices don't have the same size!";
    }

    Matrix<U> newMatrix(m1._numRows, m1._numColumns);
    for (unsigned i = 0; i < m1._numRows; ++i)
    {
        for (unsigned j = 0; j < m1._numColumns; ++j)
        {
            //TODO change to use operator()
            newMatrix._matrix[i][j] = m1._matrix[i][j] - m2._matrix[i][j];
        }
    }
    return newMatrix;
}

template <class U>
Matrix<U> operator*(const Matrix<U> m1, const Matrix<U> m2)
{
    //TODO exception m1 cols = m2 rows
    Matrix<U> newMatrix(m1._numRows, m2._numColumns);
    Matrix<U> transposedMat = findTranspose(m2);

    for (unsigned i = 0; i < m1._numRows; ++i)
    {
        for (unsigned j = 0; j < m2._numColumns; ++j)
        {
            newMatrix._matrix[i][j] = Matrix<U>::multiplyVector(m1._matrix[i], transposedMat._matrix[j]);
        }
    }
    return newMatrix;
}

template <class T>
T Matrix<T>::multiplyVector(const std::vector<T> v1, const std::vector<T> v2)
{
    if (v1.size() != v2.size())
    {
        throw "Not same size";
    }

    T result;

    for (int i = 0; i < v1.size(); ++i)
    {
        result += v1[i] * v2[i];
    }
    return result;
}

template <class U>
bool operator==(const Matrix<U> m1, const Matrix<U> m2)
{
    if (m1._numColumns != m2._numColumns || m1._numRows != m2._numRows)
    {
        return false;
    }
    else
    {
        for (unsigned i = 0; i < m1._numColumns; ++i)
        {
            for (unsigned j = 0; j < m1._numRows; ++j)
            {
                if (m1._matrix[i][j] != m2._matrix[i][j])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

template <class U>
bool operator!=(const Matrix<U> m1, const Matrix<U> m2)
{
    return !(m1 == m2);
}

template <class T>
bool Matrix<T>::isSquareMatrix()
{
    return this->_numRows == this->_numColumns;
}

template <class U>
std::ostream& operator<<(std::ostream &out, Matrix<U> m1)
{
    for (unsigned i = 0; i < m1._matrix.size(); ++i)
    {
        for (unsigned j = 0 ; j < m1._numColumns; ++j)
        {
            out << m1._matrix[i][j] << "\t";
        }
        if (i < m1._numRows - 1)
        {
            out << "" << std::endl;
        }
    }
    return out;
}

template <class T>
T& Matrix<T>::operator()(unsigned int row, unsigned int col)
{
    if (row > this->_numRows || row <= 0 || col > this->_numColumns || col <= 0)
    {
        throw "Indexes out of range";
    }
    return this->_matrix[row - 1][col - 1];
}

template <class T>
void Matrix<T>::printMat()
{
    for (unsigned i = 0; i < _matrix.size(); ++i)
    {
        for (unsigned j = 0 ; j < _numColumns; ++j)
        {
            std::cout << _matrix[i][j] << " ";
        }
        std::cout << "" << std::endl;
    }
}

template <class U>
Matrix<U> findTranspose(Matrix<U> mat)
{
    Matrix<U> transposedMat(mat._numColumns, mat._numRows);
    for (unsigned i = 0; i < mat._numRows; ++i)
    {
        for (unsigned j = 0; j < mat._numColumns; ++j)
        {
            transposedMat._matrix[j][i] = mat._matrix[i][j];
        }
    }
    return transposedMat;
}