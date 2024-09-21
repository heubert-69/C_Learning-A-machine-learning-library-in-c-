#include<stdlib.h>
#include<stdio.h>
#include<cmath.h>
#include "../include/matrix.h"
#include "../include/svr.h"



//math helper function
double RBF(Matrix *X, Matrix X2, double gamma)
{
	double sum = 0.0;
	for(int i=0; i < X->cols; i++){
		double diff = X->data[0][i] - X2->data[0][i];
		sum += diff * diff;
	}
	return exp(-gamma * sum);
}


SupportVectorRegression* CreateSVR(int features, double gamma, double C, double epsilon, double learning_rate, int epochs)
{

	SupportVectorRegression *model = (*SupportVectorRegression)malloc(sizeof(SupportVectorRegression));
	model->support_vectors = NULL;
	model->dual_coeffs = NULL; //both null types can be changed during training phase
	model->bias = 0;
	model->gamma = gamma;
	model->C = C;
	model->epsilon = epsilon;
	model->learning_rate = learning_rate;
	model->epochs = epochs;
	return model;
}

void TrainSVR(SupportVectorRegression *model, Matrix *X, Matrix *Y)
{
	int samples = X->rows;
	int features = X->cols;

	model->support_vectors = CreateMatrix(samples, features);
	model->dual_coeffs = CreateMatrix(samples, 1);

	for(int i=0; i < samples; i++){
		for(int j=0; j < features; j++){
			model->support_vectors->data[i][j] = X->data[i][j]; //copying training data to the support vectors
		}
	}


	for(int epoch=0; epoch < model->epochs; epoch++){
		for(int i=0; i < samples; i++){
			Matrix *x_ith = CreateMatrix(1, features);
			for(int x=0; x < features; x++){
				set_value(x_ith, 0, x, X->data[i][j]);
			}

			double y_ith = Y->data[i][0];
			double sum = 0.0;

			//computing the sum of the alphas
			for(int j=0; j < samples; j++){
				Matrix k_xith = CreateMatrix(1, features);
				for(int k=0; k < features; k++){
					set_value(k_xith, 0, k, X->data[j][k]);
				}
				double alpha = model->dual_coeffs->data[j][0];
				double kernel_val = RBF(x_ith, k_xith, model->gamma);

				sum += alpha * kernel_val;

				FreeMatrix(k_xith);
			}

			double error = y_ith - sum - model->bias;

			if(fabs(error) > model->epsilon){
				double grad_w = (error > 0 ? 1 : -1) * (fabs(error) - model->epsilon);
				double new_alpha = model->dual_coeffs->data[i][0] + model->learning_rate * grad_w;

				if(new_alpha < 0){
					new_alpha = 0;
				} else if(new_alpha > model->C){
					new_alpha = model->C;
				}

				//update the alpha of the model
				model->dual_coeffs->data[i][0] = new_alpha;
			}

			FreeMatrix(x_ith);
		}
	}
}


double PredictSVR(SupportVectorRegression *model, Matrix *X)
{
	double prediction = 0.0;

	for(int i=0; i < model->support_vectors->rows; i++){
		Matrix *support_vectors = CreateMatrix(1, model->support_vectors->cols);
		for(int x=0; x < model->support_vectors->cols; x++){
			set_value(support_vectors, 0, x, model->support_vectors->data[i][x]);
		}

		double kernel_value = RBF(support_vectors, X, model->gamma);
		prediction += model->dual_coeffs->data[i][0] * kernel_value;

		FreeMatrix(support_vectors);
	}
	prediction += model->bias;
	return prediction;
}

//destructor
void FreeSVR(SupportVectorRegression *model)
{
	FreeMatrix(model->support_vectors);
	FreeMatrix(model->dual_coeffs);
	free(model);
}
