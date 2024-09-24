#ifndef MATRIX_H
#define MATRIX_H


typedef struct 
{
	int rows;
	int cols;
	double **data;

} Matrix;


//creates the matrix constructor and other functions

Matrix* CreateMatrix(int rows, int cols);
void FreeMatrix(Matrix *m);
void PrintMatrix(Matrix *m);
Matrix* TransposeMatrix(Matrix *m);
Matrix* MatrixMultiply(Matrix *m, Matrix *a);
Matrix* InverseMatrix(Matrix *m);
Matrix* MatrixAdd(Matrix *m, Matrix *a);
Matrix* MatrixSubtract(Matrix *m, Matrix *a);
Matrix* ScalarMultiply(Matrix *m, double scalar);
Matrix* MatrixIdentity(int size);
Matrix* solve_linear_system(Matrix *A, Matrix *B);
double get_value(Matrix *weight, int row, int col);


#endif //MATRIX_H
