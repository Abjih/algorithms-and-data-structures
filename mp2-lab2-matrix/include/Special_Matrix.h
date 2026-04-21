#pragma once
#include "tmatrix.h"
#include <algorithm>
#include "ctime"

template <typename T>
class CRD
{
private:
    T* values;      
    int* rows;      
    int* cols;      
    int non_zero;   
    int size;       

public:
    CRD() : values(nullptr), rows(nullptr), cols(nullptr), non_zero(0), size(0) {}

    CRD(const TDynamicMatrix<T>& m) : size(m.size())
    {
        non_zero = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (m[i][j] != 0) non_zero++;
            }
        }
        values = new T[non_zero];
        rows = new int[non_zero];
        cols = new int[non_zero];
        int index = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (m[i][j] != 0) {
                    values[index] = m[i][j];
                    rows[index] = i;
                    cols[index] = j;
                    index++;
                }
            }
        }
    }

    ~CRD() {
        delete[] values;
        delete[] rows;
        delete[] cols;
    }

    TDynamicMatrix<T> Matrix() const {
        TDynamicMatrix<T> result(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result[i][j] = 0;
            }
        }
        for (int i = 0; i < non_zero; i++) {
            result[rows[i]][cols[i]] = values[i];
        }
        return result;
    }

    CRD operator*(const CRD& other) const {
        TDynamicMatrix<T> A = this->Matrix();
        TDynamicMatrix<T> B = other.Matrix();
        TDynamicMatrix<T> C = A * B;
        return CRD(C);
    }
};

template <typename T>
class CSR
{
private:
    T* values;          
    int* cols;   
    int* row_ptrs;      
    int non_zero;       
    int size;           

public:
    CSR() : values(nullptr), cols(nullptr), row_ptrs(nullptr), non_zero(0), size(0) {}

    CSR(const TDynamicMatrix<T>& m) : size(m.size())
    {
        non_zero = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (m[i][j] != 0) non_zero++;
            }
        }
        values = new T[non_zero];
        cols = new int[non_zero];
        row_ptrs = new int[size + 1];
        int index = 0;
        row_ptrs[0] = 0;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (m[i][j] != 0) {
                    values[index] = m[i][j];
                    cols[index] = j;
                    index++;
                }
            }
            row_ptrs[i + 1] = index;
        }
    }

    ~CSR() {
        delete[] values;
        delete[] cols;
        delete[] row_ptrs;
    }

    TDynamicMatrix<T> Matrix() const {
        TDynamicMatrix<T> result(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result[i][j] = 0;
            }
        }
        for (int i = 0; i < size; i++) {
            for (int j = row_ptrs[i]; j < row_ptrs[i + 1]; j++) {
                result[i][cols[j]] = values[j];
            }
        }
        return result;
    }

    CSR operator*(const CSR& other) const {
        TDynamicMatrix<T> A = this->Matrix();
        TDynamicMatrix<T> B = other.Matrix();
        TDynamicMatrix<T> C = A * B;
        return CSR(C);
    }
};

template <typename T>
class BandMatrix
{
private:
    T** diagonals;      
    int size;           
    int band_width;     
    int total_diags;    

public:
    BandMatrix() : diagonals(nullptr), size(0), band_width(0), total_diags(0) {}
    BandMatrix(const TDynamicMatrix<T>& m) : size(m.size())
    {
        band_width = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (m[i][j] != 0) {
                    int distance = std::abs(i - j);
                    if (distance > band_width) {
                        band_width = distance;
                    }
                }
            }
        }
        total_diags = 2 * band_width + 1;
        diagonals = new T * [total_diags];
        for (int d = 0; d < total_diags; d++) {
            diagonals[d] = new T[size]();
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (m[i][j] != 0 && std::abs(i - j) <= band_width) {
                    int diag_index = j - i + band_width; 
                    diagonals[diag_index][i] = m[i][j];
                }
            }
        }
    }

    ~BandMatrix() {
        if (diagonals != nullptr) {
            for (int d = 0; d < total_diags; d++) {
                delete[] diagonals[d];
            }
            delete[] diagonals;
        }
    }

    T get(int i, int j) const {
        if (i < 0 || i >= size || j < 0 || j >= size) {
            throw std::out_of_range("Matrix indices out of range");
        }

        int diag_index = j - i + band_width;
        if (diag_index < 0 || diag_index >= total_diags) {
            return 0; 
        }
        return diagonals[diag_index][i];
    }

    void set(int i, int j, const T& value) {
        if (i < 0 || i >= size || j < 0 || j >= size) {
            throw std::out_of_range("Matrix indices out of range");
        }

        int diag_index = j - i + band_width;
        if (diag_index < 0 || diag_index >= total_diags) {
            throw std::out_of_range("Element outside band width");
        }
        diagonals[diag_index][i] = value;
    }

    TDynamicMatrix<T> Matrix() const {
        TDynamicMatrix<T> result(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result[i][j] = get(i, j);
            }
        }
        return result;
    }

    BandMatrix operator*(const BandMatrix& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Matrix sizes must be equal for multiplication");
        }
        TDynamicMatrix<T> result(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                T sum = T();
                for (int k = 0; k < size; k++) {
                    sum += get(i, k) * other.get(k, j);
                }
                result[i][j] = sum;
            }
        }

        return BandMatrix(result);
    }

    // §£§ã§á§à§Þ§à§Ô§Ñ§ä§Ö§Ý§î§ß§í§Ö §Þ§Ö§ä§à§Õ§í
    int getSize() const { return size; }
    int getBandWidth() const { return band_width; }
    int getTotalDiagonals() const { return total_diags; }

    // §£§í§Ó§à§Õ §Ý§Ö§ß§ä§à§é§ß§à§Ô§à §á§â§Ö§Õ§ã§ä§Ñ§Ó§Ý§Ö§ß§Ú§ñ (§Õ§Ý§ñ §à§ä§Ý§Ñ§Õ§Ü§Ú)
    void printBandStorage() const {
        std::cout << "Band storage (" << total_diags << " diagonals):" << std::endl;
        for (int d = 0; d < total_diags; d++) {
            std::cout << "Diagonal " << (d - band_width) << ": ";
            for (int i = 0; i < size; i++) {
                std::cout << diagonals[d][i] << " ";
            }
            std::cout << std::endl;
        }
    }
};

void testCRD() {
    cout << "=== Testing CRD (Coordinate) ===" << endl;

    // §³§à§Ù§Õ§Ñ§Ö§Þ §á§Ý§à§ä§ß§å§ð §Þ§Ñ§ä§â§Ú§è§å 4x4
    TDynamicMatrix<int> dense(4);
    dense[0][0] = 1; dense[0][1] = 2; dense[0][2] = 0;  dense[0][3] = 0;
    dense[1][0] = 3; dense[1][1] = 4; dense[1][2] = 5;  dense[1][3] = 0;
    dense[2][0] = 0; dense[2][1] = 6; dense[2][2] = 7;  dense[2][3] = 8;
    dense[3][0] = 0; dense[3][1] = 0; dense[3][2] = 9;  dense[3][3] = 10;

    cout << "Original dense matrix:" << endl;
    cout << dense << endl;

    // §±§â§Ö§à§Ò§â§Ñ§Ù§å§Ö§Þ §Ó CRD
    CRD<int> crd(dense);
    cout << "Converted to CRD and back:" << endl;
    TDynamicMatrix<int> fromCRD = crd.Matrix();
    cout << fromCRD << endl;

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ §å§Þ§ß§à§Ø§Ö§ß§Ú§Ö
    int start=clock();

    CRD<int> crd2 = crd * crd;
    int finish = clock();
    cout << "CRD * CRD result:" << endl;
    cout << crd2.Matrix() << endl<<finish-start<<endl;

    cout << "CRD test completed!" << endl << endl;
}

void testCSR() {
    cout << "=== Testing CSR (Compressed Sparse Row) ===" << endl;

    // §³§à§Ù§Õ§Ñ§Ö§Þ §á§Ý§à§ä§ß§å§ð §Þ§Ñ§ä§â§Ú§è§å 4x4
    TDynamicMatrix<int> dense(4);
    dense[0][0] = 1; dense[0][1] = 2; dense[0][2] = 0;  dense[0][3] = 0;
    dense[1][0] = 3; dense[1][1] = 4; dense[1][2] = 5;  dense[1][3] = 0;
    dense[2][0] = 0; dense[2][1] = 6; dense[2][2] = 7;  dense[2][3] = 8;
    dense[3][0] = 0; dense[3][1] = 0; dense[3][2] = 9;  dense[3][3] = 10;

    cout << "Original dense matrix:" << endl;
    cout << dense << endl;

    // §±§â§Ö§à§Ò§â§Ñ§Ù§å§Ö§Þ §Ó CSR
    CSR<int> csr(dense);
    cout << "Converted to CSR and back:" << endl;
    TDynamicMatrix<int> fromCSR = csr.Matrix();
    cout << fromCSR << endl;

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ §å§Þ§ß§à§Ø§Ö§ß§Ú§Ö
    int start = clock();
    CSR<int> csr2 = csr * csr;
    int finish = clock();
    cout << "CSR * CSR result:" << endl;
    cout << csr2.Matrix() << endl<<finish-start<<endl;

    cout << "CSR test completed!" << endl << endl;
}

void testBandMatrix() {
    cout << "=== Testing BandMatrix ===" << endl;

    // §³§à§Ù§Õ§Ñ§Ö§Þ §Ý§Ö§ß§ä§à§é§ß§å§ð §Þ§Ñ§ä§â§Ú§è§å 4x4
    TDynamicMatrix<int> dense(4);
    dense[0][0] = 1; dense[0][1] = 2; dense[0][2] = 0;  dense[0][3] = 0;
    dense[1][0] = 3; dense[1][1] = 4; dense[1][2] = 5;  dense[1][3] = 0;
    dense[2][0] = 0; dense[2][1] = 6; dense[2][2] = 7;  dense[2][3] = 8;
    dense[3][0] = 0; dense[3][1] = 0; dense[3][2] = 9;  dense[3][3] = 10;

    cout << "Original dense matrix:" << endl;
    cout << dense << endl;

    // §±§â§Ö§à§Ò§â§Ñ§Ù§å§Ö§Þ §Ó BandMatrix
    BandMatrix<int> band(dense);
    cout << "Converted to BandMatrix and back:" << endl;
    TDynamicMatrix<int> fromBand = band.Matrix();
    cout << fromBand << endl;

    // §£§í§Ó§à§Õ§Ú§Þ §Ú§ß§æ§à§â§Þ§Ñ§è§Ú§ð §à §Ý§Ö§ß§ä§à§é§ß§à§Û §Þ§Ñ§ä§â§Ú§è§Ö
    cout << "Band matrix info:" << endl;
    cout << "Size: " << band.getSize() << endl;
    cout << "Band width: " << band.getBandWidth() << endl;
    cout << "Total diagonals: " << band.getTotalDiagonals() << endl;

    // §±§à§Ü§Ñ§Ù§í§Ó§Ñ§Ö§Þ §Ý§Ö§ß§ä§à§é§ß§à§Ö §ç§â§Ñ§ß§Ö§ß§Ú§Ö
    cout << "Band storage format:" << endl;
    band.printBandStorage();

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ §Õ§à§ã§ä§å§á §Ü §ï§Ý§Ö§Þ§Ö§ß§ä§Ñ§Þ
    cout << "Element (1,1): " << band.get(1, 1) << " (expected: 4)" << endl;
    cout << "Element (2,3): " << band.get(2, 3) << " (expected: 8)" << endl;
    cout << "Element (0,3): " << band.get(0, 3) << " (expected: 0)" << endl;

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ §å§ã§ä§Ñ§ß§à§Ó§Ü§å §ï§Ý§Ö§Þ§Ö§ß§ä§Ñ
    band.set(1, 2, 99);
    cout << "After setting element (1,2) to 99:" << endl;
    cout << band.Matrix() << endl;

    
    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ §å§Þ§ß§à§Ø§Ö§ß§Ú§Ö
    int start = clock();
    BandMatrix<int> band2 = band * band;
    int finish = clock();
    cout << "BandMatrix * BandMatrix result:" << endl;
    cout << band2.Matrix() << endl<<finish-start<<endl;

    cout << "BandMatrix test completed!" << endl << endl;
}

void testAllTogether() {
    cout << "=== Testing All Formats Together ===" << endl;

    // §³§à§Ù§Õ§Ñ§Ö§Þ §ä§Ö§ã§ä§à§Ó§å§ð §Þ§Ñ§ä§â§Ú§è§å
    TDynamicMatrix<int> original(3);
    original[0][0] = 1; original[0][1] = 2; original[0][2] = 0;
    original[1][0] = 3; original[1][1] = 4; original[1][2] = 5;
    original[2][0] = 0; original[2][1] = 6; original[2][2] = 7;

    cout << "Original matrix:" << endl;
    cout << original << endl;

    // §¬§à§ß§Ó§Ö§â§ä§Ú§â§å§Ö§Þ §Ó§à §Ó§ã§Ö §æ§à§â§Þ§Ñ§ä§í
    CRD<int> crd(original);
    CSR<int> csr(original);
    BandMatrix<int> band(original);

    // §¬§à§ß§Ó§Ö§â§ä§Ú§â§å§Ö§Þ §à§Ò§â§Ñ§ä§ß§à §Ú §ã§â§Ñ§Ó§ß§Ú§Ó§Ñ§Ö§Þ
    TDynamicMatrix<int> fromCRD = crd.Matrix();
    TDynamicMatrix<int> fromCSR = csr.Matrix();
    TDynamicMatrix<int> fromBand = band.Matrix();

    cout << "All formats should produce the same matrix:" << endl;
    cout << "From CRD: " << endl << fromCRD << endl;
    cout << "From CSR: " << endl << fromCSR << endl;
    cout << "From Band: " << endl << fromBand << endl;

    // §±§â§à§Ó§Ö§â§ñ§Ö§Þ, §é§ä§à §Ó§ã§Ö §â§Ö§Ù§å§Ý§î§ä§Ñ§ä§í §à§Õ§Ú§ß§Ñ§Ü§à§Ó§í
    if (fromCRD == fromCSR && fromCSR == fromBand) {
        cout << "SUCCESS: All formats produce identical results!" << endl;
    }
    else {
        cout << "ERROR: Formats produce different results!" << endl;
    }
}