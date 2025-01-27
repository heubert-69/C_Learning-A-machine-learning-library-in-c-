#include <stdio.h>
#include <stdlib.h>
#include "../include/matrix.h"

// Function to perform forward elimination
void forward_elimination(Matrix *A, Matrix *B) {
    int n = A->rows;

    for (int i = 0; i < n; i++) {
        // Partial pivoting
        for (int k = i + 1; k < n; k++) {
            if (fabs(A->data[k][i]) > fabs(A->data[i][i])) {
                // Swap rows in A
                for (int j = 0; j < n; j++) {
                    double temp = A->data[i][j];
                    A->data[i][j] = A->data[k][j];
                    A->data[k][j] = temp;
                }
                // Swap corresponding entries in B
                double temp = B->data[i][0];
                B->data[i][0] = B->data[k][0];
                B->data[k][0] = temp;
            }
        }

        // Elimination process
        for (int k = i + 1; k < n; k++) {
            double factor = A->data[k][i] / A->data[i][i];
            for (int j = i; j < n; j++) {
                A->data[k][j] -= factor * A->data[i][j];
            }
            B->data[k][0] -= factor * B->data[i][0];
        }
    }
}

// Function to perform back substitution
Matrix* back_substitution(Matrix *A, Matrix *B) {
    int n = A->rows;
    Matrix *X = create_matrix(n, 1);

    for (int i = n - 1; i >= 0; i--) {
        X->data[i][0] = B->data[i][0];
        for (int j = i + 1; j < n; j++) {
            X->data[i][0] -= A->data[i][j] * X->data[j][0];
        }
        X->data[i][0] /= A->data[i][i];
    }

    return X;
}

// Function to solve the linear system A * x = B
Matrix* solve_linear_system(Matrix *A, Matrix *B) {
    forward_elimination(A, B);  // Perform forward elimination
    return back_substitution(A, B);  // Perform back substitution to get solution
}
