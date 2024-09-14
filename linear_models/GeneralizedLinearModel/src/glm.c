#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/glm.h"
#include "../include/matrix.h"

//Helper functions to apply link and the inverse of it
double ApplyLinkFunction(double z, LinkFunctionType link)
{
	switch(link){
		case identity:
			return z;
		case logit:
			return 1.0 / (1.0 + exp(-z));
		case log:
			return log(z);
		default:
			return z; //default to  identity if all cases are non to be found
	}
}

double ApplyInverseLinkFunction(double z, LinkFunctionType link)
{
	switch(link){
		case identity:
			return z;
		case logit:
			return log(z / (1.0 - z));
		case log:
			return exp(z);
		default:
			return z;
	}
}

//your regularly scheduled program

GLM* CreateGLM(int features, LinkFunctionType link, DistributionType distribution)
{

	GLM *model = (*GLM)malloc(sizeof(GLM));
	model->weights = CreateMatrix(features, 1);
	model->bias = 0.0;
	model->link = link;
	model->distribution = distribution;
	return model;
}


void TrainGLM(GLM *model, Matrix *X, Matrix *Y, double learning_rate, int epochs)
{
	int m = X->rows; //gives out the rows value

	for(int epoch=0; epoch < epochs; epoch++){
		Matrix *linear_combinations = MultiplyMatrix(X, model->weights); //computes the Xw
		Matrix *predictions = CreateMatrix(linear_combinations->rows, 1);
		//must apply inverse link to make predictions
		for(int i=0; i < linear_combinations->rows; i++){
			double z = linear_combinations->data[i][0] + model->bias;
			predictions->data[i][0] = ApplyLinkFunction(z, model->link);
		}

		Matrix *errors = MatrixSubtract(predictions, Y);

		//using gradient descent on weights
		Matrix *grad_w = MultiplyMatrix(TransposeMatrix(X), errors);
		grad_w = ScalarMultiply(grad_w, 1.0/m);

		model->weights = MatrixSubtract(model->weights, ScalarMultiply(grad_w, learning_rate));
		model->bias -= learning_rate * 1.0/m * MatrixAdd(errors);

		//free matrices or sort of acts like a destructor to the weights
		FreeMatrix(linear_combinations);
		FreeMatrix(predictions);
		FreeMatrix(errors);
		FreeMatrix(grad_w);
	}
}



Matrix* PredictGLM(GLM *model, Matrix *X)
{
	GLM *linear_combinations = MultiplyMatrix(X, model->weights);
	GLM *predictions = CreateMatrix(linear_combinations->rows, 1);

	for(int i=0; i < linear_combinations->rows; i++){
		double z = linear_combinations->data[i][0] + model->bias;
		predictions->data[i][0] = ApplyInverseLinkFunction(z, model->link);
	}
	FreeMatrix(linear_combinations);
	return predictions;
}


//da real destructor
void FreeGLM(GLM *model)
{
	FreeMatrix(model->weights)
	free(model);
}
