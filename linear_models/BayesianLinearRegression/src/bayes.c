#include<stdio.h>
#include<stdlib.h>
#include "../include/bayes.h"
#include "../include/matrix.h"



BayesianLinearRegression* CreateBayesianModel(int features)
{
	BayesianLinearRegression *model = (*BayesianLinearRegression)malloc(sizeof(BayesianLinearRegression));
	model->weights = CreateMatrix(features, 1);
	model->covariance = CreateMatrix(features, features);
	return model;
}


void TrainBayesianModel(BayesianLinearRegression *model, Matrix *X, Matrix *Y, double alpha, double beta)
{
	//alpha: define precision parameters
	//beta: calculates the errors

	Matrix *Xt = TransposeMatrix(X); //X^t
	Matrix *XtX = MatrixMultiply(Xt, X); //X^t * X

	//regularization
	Matrix *I = MatrixIdentity(X->cols);
	Matrix *regularization_term = ScalarMultiply(I, alpha);

	//posterior covariance
	Matrix *inverse_covariance = MatrixAdd(XtX, regularization_term);
	model->covariance = InverseMatrix(inverse_covariance);

	//posterior weights
	Matrix *Xt_y = MatrixMultiply(Xt, Y);
	model->weights = MatrixMultiply(model->covariance, Xt_y);

	//sort of destructor
	FreeMatrix(Xt);
	FreeMatrix(XtX);
	FreeMatrix(I);
	FreeMatrix(regularization_term);
	FreeMatrix(inverse_covariance);
	FreeMatrix(Xt_y);
}

Matrix* PredictBayesianModel(BayesianLinearRegression *model, Matrix *X)
{
	return MatrixMultiply(X, model->weights);
}

void FreeBayesianModel(BayesianLinearRegression *model)
{
	FreeMatrix(model->weights)
	FreeMatrix(model->covariance);
	free(model);
}
