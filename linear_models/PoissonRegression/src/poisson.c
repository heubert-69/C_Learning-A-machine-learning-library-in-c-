#include<stdlib.h>
#include<stdio.h>
#include "../include/poisson.h"
#include "../include/matrix.h"



PoissonRegression* CreatePoissonRegression(int features)
{
	PoissonRegression *model = (*PoissonRegression)malloc(sizeof(PoissonRegression));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	return model;
}

Matrix* PredictPoissonRegression(PoissonRegression *model, Matrix *X)
{

	Matrix *linear_conbinations = MultiplyMatrix(X, model->weights);
	Matrix *predictions = CreateMatrix(linear_combinations->rows, 1);

	for(int i=0; i < linear_combinations->rows; i++){
		double z = linear-combinations->data[i][0] + model->bias;
		predictions->data[i][0] = exp(z);
	}
	FreeMatrix(linear_combination);
	return predictions;
}

void TrainPoissonRegression(PoissonRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs)
{

	int m = X->rows; //traversing through the rows of X

	for(int epoch=0; epoch < epochs; epoch++){
		Matrix *predictions = PredictPoissonRegression(model, X);
       		Matrix *errors = MatrixSubtract(predictions, Y);

		Matrix *grad_w = MatrixMultiply(TransposeMatrix(X), errors);
       		grad_w = ScalarMultiply(grad_w, 1.0/m);

		model->weights  = MatrixSubtract(model->weights, ScalarMultiply(grad_w, learning_rate));
		model->bias -= learning_rate * 1.0/m * MatrixAdd(errors);

		//acts a sort or destructor
		FreeMatrix(predictions);
		FreeMatrix(errors);
		FreeMatrix(grad_w);
	}
}

void FreePoissonRegression(PoissonRegression *model)
{
	FreeMatrix(model->weights);
	free(model);
}
