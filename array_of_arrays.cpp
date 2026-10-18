#include <cstddef>
#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>

bool overflow(size_t a, size_t b) {
    return b != 0 && a > std::numeric_limits<size_t>::max() / b;
}

void free_matrix(int** m, size_t rows) {
    if (m == nullptr) {
        return;
    }
    for (size_t i = 0; i < rows; ++i) {
        delete[] m[i];
    }
    delete[] m;
}

int** create_matrix(size_t rows, size_t cols) {
    if (overflow(rows, sizeof(int*)) || overflow(cols, sizeof(int))) {
        throw std::bad_alloc();
    }
    int** m = new int*[rows]();
    try {
        for (size_t i = 0; i < rows; ++i) {
            m[i] = new int[cols];
        }
    } catch (...) {
        free_matrix(m, rows);
        throw;
    }
    return m;
}

bool input_matrix(int** m, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (!(std::cin >> m[i][j])) {
                return false;
            }
        }
    }
    return true;
}

size_t row_len(size_t i, size_t n1, size_t m1, size_t n2, size_t m2) {
    size_t len = 0;
    if (i < n1) len += m1;
    if (i < n2) len += m2;
    return len;
}

int** concat_rows(const int* const* lhs, size_t n1, size_t m1,
                  const int* const* rhs, size_t n2, size_t m2) {
    if (m1 > std::numeric_limits<size_t>::max() - m2) {
        throw std::bad_alloc();
    }

    size_t rows = n1 > n2 ? n1 : n2;
    int** res = new int*[rows]();
    try {
        for (size_t i = 0; i < rows; ++i) {
            size_t len = row_len(i, n1, m1, n2, m2);
            res[i] = new int[len];

            size_t k = 0;
            if (i < n1) {
                for (size_t j = 0; j < m1; ++j) {
                    res[i][k++] = lhs[i][j];
                }
            }
            if (i < n2) {
                for (size_t j = 0; j < m2; ++j) {
                    res[i][k++] = rhs[i][j];
                }
            }
        }
    } catch (...) {
        free_matrix(res, rows);
        throw;
    }
    return res;
}

int** read_matrix(size_t& rows, size_t& cols) {
    long long r = 0, c = 0;
    if (!(std::cin >> r >> c) || r <= 0 || c <= 0) {
        throw std::runtime_error("input");
    }
    rows = static_cast<size_t>(r);
    cols = static_cast<size_t>(c);

    int** m = create_matrix(rows, cols);
    if (!input_matrix(m, rows, cols)) {
        free_matrix(m, rows);
        throw std::runtime_error("input");
    }
    return m;
}

int main() {
    size_t n1 = 0, m1 = 0, n2 = 0, m2 = 0;
    int** lhs = nullptr;
    int** rhs = nullptr;
    int** res = nullptr;

    try {
        lhs = read_matrix(n1, m1);
        rhs = read_matrix(n2, m2);
        res = concat_rows(lhs, n1, m1, rhs, n2, m2);
    } catch (const std::bad_alloc&) {
        free_matrix(lhs, n1);
        free_matrix(rhs, n2);
        std::cerr << "Not enough memory\n";
        return 2;
    } catch (const std::exception&) {
        free_matrix(lhs, n1);
        free_matrix(rhs, n2);
        std::cerr << "Input error\n";
        return 1;
    }

    size_t rows = n1 > n2 ? n1 : n2;
    for (size_t i = 0; i < rows; ++i) {
        size_t len = row_len(i, n1, m1, n2, m2);
        for (size_t j = 0; j < len; ++j) {
            std::cout << res[i][j];
            if (j + 1 < len) {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }

    free_matrix(res, rows);
    free_matrix(lhs, n1);
    free_matrix(rhs, n2);
    return 0;
}