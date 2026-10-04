#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>

void *alloc_malloc(const size_t n, const size_t elem_size) {
  if (n > (std::numeric_limits<size_t>::max() / elem_size)) {
    return nullptr;
  }
  return malloc(n * elem_size);
}

void free_matrix(int **m, const size_t rows) {
  if (m == nullptr) {
    return;
  }
  for (size_t i = 0; i < rows; ++i) {
    free(m[i]);
  }
  free(m);
}

int **create_matrix(const size_t rows, const size_t cols) {
  int **m = static_cast<int **>(alloc_malloc(rows, sizeof(int *)));
  if (m == nullptr) {
    return nullptr;
  }

  for (size_t i = 0; i < rows; ++i) {
    m[i] = static_cast<int *>(alloc_malloc(cols, sizeof(int)));
    if (m[i] == nullptr) {
      free_matrix(m, i);  
      return nullptr;
    }
  }
  return m;
}

bool input_matrix(int **m, const size_t rows, const size_t cols) {
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (!(std::cin >> m[i][j])) {
        return false;
      }
    }
  }
  return true;
}

void print_matrix(int **m, const size_t rows, const size_t cols) {
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      std::cout << m[i][j];
      if (j + 1 < cols) {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}

int main() {
  long long r = 0, c = 0;
  if (!(std::cin >> r >> c) || r <= 0 || c <= 0) {
    return 1;  
  }
  const size_t rows = static_cast<size_t>(r);
  const size_t cols = static_cast<size_t>(c);
  int **matrix = create_matrix(rows, cols);
  if (matrix == nullptr) {
    return 2;  
  }

  if (!input_matrix(matrix, rows, cols)) {
    free_matrix(matrix, rows);
    return 1;  
  }

  int **transposed = create_matrix(cols, rows);
  if (transposed == nullptr) {
    free_matrix(matrix, rows);
    return 2;
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      transposed[j][i] = matrix[i][j];
    }
  }

  print_matrix(transposed, cols, rows);
  free_matrix(matrix, rows);
  free_matrix(transposed, cols);
  return 0;
}