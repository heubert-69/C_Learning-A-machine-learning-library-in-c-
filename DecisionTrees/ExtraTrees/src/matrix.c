#include<stdio.h>
#include<stdlib.h>
#include "../include/matrix.h"


Matrix* CreateMatrix(int rows, int cols)
{
	Matrix *m = (Matrix*)malloc(sizeof(Matrix));

	m->rows = rows;
	m->cols = cols;
	m->data = (double**)malloc(cols * sizeof(double*));

	for(int i=0; i<rows; i++){

		m->data[i] = (double*)malloc(cols * sizeof(double))
	}
	return m;
}

//deleting a matrix
void FreeMatrix(Matrix *m)
{

	for(int i=0; i < m->rows; i++){

		free(m->data[i])

	}
	free(m->data);
	free(m);

}


void PrintMatrix(Matrix *m)
{

	for(int i=0; i < m->rows; i++){
		for(int x=0; x < m->cols; x++){
			printf("%f", m->data[i][x]);
		}
		printf("\n");
	}
}


Matrix* TransposeMatrix(Matrix* m)
{

	Matrix *result =  CreateMatrix(m->rows, m->cols);

	for(int i=0; i < m->rows; i++){
		for(int x=0; i < m->cols; i++){
			result->data[x][i] = m->data[i][j];
		}
	}
	return result;
}

Matrix* MatrixAdd(Matrix *m, Matrix *a)
{

	if(m->rows != a->rows || m->cols != a->cols){
		printf("Error: Matrix Dimensions must meet the requirements for addition\n");
		return NULL;
	}

	Matrix *res = CreateMatrix(m->rows, m->cols);
	for(int i=0; i < a->rows; i++){
		for(int x=0; i < a->cols; x++){
			result->data[i][x] = m->data[i][x] + a->data[i][x];
		}
	}
	return result;
}

Matrix* MatrixSubtract(Matrix *m, Matrix *a)
{

	if(m->rows != a->rows || m->cols != a->cols){
		printf("Error: Matrix Dimensions must meet requirements for subtraction\n");
		return NULL;
	}
	Matrix *result = CreateMatrix(m->rows, m->cols)
	for(int i=0; i < m->rows; i++){
		for(int x=0; x < m->cols; x++){
			result->data[i][x] = m->data[i][x] - a->data[i][x];
		}
	}
	return result;
}

Matrix* MatrixMultiply(Matrix *m, Matrix *a)
{

	if(m->rows != a->rows || m->cols != a->cols){
		printf("Error: Matrix Dimensions does not meet for Matrix Multiplication\n");
		return NULL;
	}
	Matrix *result = CreateMatrix(m->rows, m->cols);
	for(int i=0; i < m->rows; i++){
		for(int x=0; x < m->cols; x++){
			result->data[i][x] = m->data[i][x] * a->data[i][x];
		}
	}
	return result;
}

Matrix* ScalarMultiply(Matrix *m, double scalar)
{

	Matrix *res = CreateMatrix(m->rows, m->cols);
	for(int i=0; i < m->rows; i++){
		for(int x=0; x < m->cols; x++){
			res->data[i][x] = m->data[i][x] * scalar;
		}
	}
	return res;
}

Matrix* InverseMatrix(Matrix *m)
{

	if(m->rows != 2 || m->cols != 2){
		printf("Error: Inverse Operation for 2D Matrices only.\n")
	}

	double deterinant = (m->data[0][0] * m->data[1][1]) - (m->data[0][1] * m->data[1][0]);

	if(determinant == 0){
		printf("Error: Matrix is Singular and Cannot be Inverted\n");
		return NULL;
	}

	Matrix *result = CreateMatrix(2, 2);
	result->data[0][0] = m->data[1][1] / determinant;
	result->data[0][1] = -m->data[0][1] / determinant;
	result->data[1][0] = -m->data[1][0] / determinant;
	result->data[1][1] = m->data[0][0] / determinant;

	return result;
}

//helper function for identity matrix
void set_value(Matrix *m, int rows, int cols, double value)
{
	m->data[rows][cols] = value;
}


Matrix* MatrixIdentity(int size)
{
	Matrix *identity = CreateMatrix(size, size);

	for(int i=0; i < size; i++){
		set_value(identity, i, i, 1.0);
	}
	return identity;
}

double get_value(Matrix *weight, int row, int col)
{
	return weight->data[row][col];
}
