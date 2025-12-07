#include "Matrices.h"


namespace Matrices {
    Matrix::Matrix(int _rows, int _cols) {
        rows = _rows;
        cols = _cols;

        a.resize(_rows);
        for (vector<double>& t : a) {
            t.resize(cols);
        }

    }

    Matrix operator+(const Matrix& a, const Matrix& b) {
        if (a.getCols() != b.getCols() || a.getRows() != b.getRows()) {
            throw runtime_error("Error: dimensions must agree");
        }
        Matrix randObj(a.getRows(), a.getCols());

        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < a.getCols(); ++j) {
                randObj(i, j) = a(i, j) + b(i, j);
            }
        }

        return randObj;


    }

    bool operator ==(const Matrix& a, const Matrix& b) {
        if (a.getCols() != b.getCols() || a.getRows() != b.getRows())
        {
            return false;
        }
        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < a.getCols(); ++j) {
                if (abs(a(i, j) - b(i, j)) < 0.001) { return true; }



            }
        }

        return false;

    }

    bool operator!=(const Matrix& a, const Matrix& b) {
        return !(a == b);
    }

    ostream& operator<<(ostream& os, const Matrix& a) {

        for (int i = 0; i < a.getRows(); ++i) {
            for (int j = 0; j < a.getCols(); ++j) {
                os << setw(10) << a(i, j) /*wow*/;
            }

            os << "\n";

        }


        return os;
    }

    Matrix operator*(const Matrix& a, const Matrix& b) {

        if (a.getCols() != b.getRows()) {
            throw runtime_error("Error: dimensions must agree");
        }
        double sum{ 0 };
        Matrix c(a.getRows(), b.getCols());

        for (int k = 0; k < b.getCols(); k++) {
            for (int i = 0; i < a.getRows(); ++i) {
                for (int j = 0; j < a.getCols() && j < b.getRows(); ++j) {
                    sum += a(i, j) * b(j, k);
                }

                c(i, k) = sum;
                sum = 0;
            }
        }

        return c;
    }


    RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) {
        (*this)(0, 0) = cos(theta);
        a.at(0).at(1) = -sin(theta);
        (*this)(1, 0) = sin(theta);
        (*this)(1, 1) = cos(theta);
    }

    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols) {
        for (int j = 0; j < nCols; ++j) {
            (*this)(0, j) = xShift;
            (*this)(1, j) = yShift;
        }

    }


    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {
        (*this)(0, 0) = scale;
        operator()(1, 1) = scale;
    }
}
