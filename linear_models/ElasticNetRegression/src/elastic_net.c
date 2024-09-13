#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"
#include "../include/elastic_net.h"


ElasticNetRegression CreateNetRegression(int features, double alpha)
{
	ElasticNetRegression *model = (*ElasticNetRegression)malloc(sizeof(ElasticNetRegression));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	model->alpha = alpha;
	model->l1 = l1;

	return model;
}


ElasticNetRegression* PredictNetRegression(ElasticNetRegression *model, Matrix *X)
{

	Matrix *predictions = MultiplyMatrix(X, model->weights);
	for(int i=0; i < predictions->rows; i++){
		predictions->data[i][0] += model->bias;
	}
	return predictions;
}



void TrainNetRegression(ElasicNetRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs)
{

	int m = X->rows;

	for(int epoch=0; epoch < epochs; epoch++){
		Matrix *predictions = PredictNetRegression(model, X);
		Matrix *errors = MatrixSubtract(predictions, y);

		Matrix *grad_w = MultiplyMatrix(TransposeMatrix(X), errors);
		grad_w = ScalarMultiply(grad_w, 1.0/m);


		//performing regularization
		for(int i=0; i < model->weights->rows; i++){
			if(model->weights->data[0][0] > 0){
				model->weights->data[i][0] -= model->alpha * model->l1;
			} else {model->weights->data[i][0] += model->alpha * model->l1;}
		}

		grad_w = MatrixAdd(grad_w, model->l1);

		model->weights = MatrixSubtract(model->weights, ScalarMultiply(grad_w, learning_rate));
		model->bias -= learning_rate * 1.0/m * errors->data[0][0];

		//destroying memory
		FreeMatrix(predictions);
		FreeMatrix(grad_w);
		FreeMatrix(errors);
		FreeMatrix(l1);
	}
}

void FreeNetRegression(ElasticNetRegression *model)
{
	FreeMatrix(model->weights);
	free(model
}
