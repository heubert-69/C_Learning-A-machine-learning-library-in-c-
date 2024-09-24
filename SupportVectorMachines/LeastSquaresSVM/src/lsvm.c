#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"
#include "../include/lsvm.h"

LeastSquaresSVM* CreateLSVM(int features, double regularization)
{
	LeastSquaresSVM *model = (*LeastSquaresSVM)malloc(sizeof(LeastSquaresSVM));
	model->weights = CreateMatrix(1, features);
	model->bias = 0.0;
	model->regularization = regularization;
	return model;
}

void TrainLSVM(LeastSquaresSVM *model, Matrix *X, Matrix *Y)
{
	int samples = model->rows;
	int features = model->cols;

	Matrix *A = CreateMatrix(features + 1, features + 1);
	Matrix *B = CreateMatrix(features + 1, 1);

	//doing the populating rizz on matrix A with regularization
	for(int i=0; i < features; i++){
		for(int j=0; j < features; j++){
			double sum = 0.0;
			for(int x=0; x < samples; x++){
				sum += X->data[x][i] * X->data[x][j];
			}
			A->data[i][j] = sum + (i == j ? model->regularization : 0);
		}
	}
	//adding the bias term on the last column
	for(int i=0; i < features; i++){
		double sum = 0.0;
		for(int k=0; k < samples; k++){
			sum += X->data[k][i] * Y->data[k][0];
		}
		B->data[i][0] = sum;
	}

	//putting the bias term in matrix b
	double bias_sum = 0.0;
	for(int x=0; x < samples; x++){
		bias_sum += Y->data[x][0];
	}
	B->data[features][0] = bias_sum;

	//solve linear system equation
	Matrix *solution = solve_linear_system(A, B);

	for(int i=0; i < features; i++){
		set_value(model->weights, 0, i, solution->data[i][0]);
	}
	model->bias = solution[features][0];

	//destroy allocated memory
	FreeMatrix(A);
	FreeMatrix(B);
	FreeMatrix(solution);
}

double PredictLSVM(LeastSquaresSVM *model, Matrix *X)
{
	double score = model->bias;
	for(int i=0; i < X->cols; i++){
		score += model->weights->data[0][i] * X->data[0][i];
	}
	return score;
}


void FreeLSVM(LeastSquaresSVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}
