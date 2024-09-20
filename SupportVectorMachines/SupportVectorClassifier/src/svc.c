#include<stdlib.h>
#include<stdio.h>
#include<cmath.h>
#include "../include/matrix.h"
#include "../include/svc.h"

//math helper function
double RBF(Matrix *x_1, Matrix *x_2, double gamma)
{
	double sum = 0.0;

	for(int i=0; i < x_1->cols; i++){
		double diff = x_1->data[0][i] - x_2->data[0][i];
		sum += diff * diff;
	}
	return exp(-gamma * sum);
}


SupportVectorClassifier* CreateSVC(int features, double gamma, double C, double learning_rate, int epochs)
{
	SupportVectorClassifier *model = (*SupportVectorClassifier)malloc(sizeof(SupportVectorClassifier));
	model->support_vectors = NULL;
	model->dual_coeffs = NULL;
	model->bias = 0;
	model->gamma = gamma;
	model->C = C;
	model->learning_rate = learning_rate;
	model->epochs = epochs;
	model->n_support_vector = 0;
	return model;
}


void TrainSVC(SupportVectorClassifier *model, Matrix *X, Matrix *Y)
{
	int samples = X->rows;
	int features = X->cols;

	model->support_vectors = CreateMatrix(samples, features);
	model->dual_coeffs = CreateMatrix(samples, 1);

	for(int i=0; i < samples; i++){
		for(int x=0; x < features; x++){
			model->support_vectors->data[i][x] = X->data[i][x];
		}
	}

	//Gradient Descent Time
	for(int epoch=0; epoch < model->epochs; epoch){
		for(int i=0; i < samples; i++){
			Matrix *x_ith = CreateMatrix(1, features);
			for(int x=0; x < features; x++){
				set_value(x_ith, 0, x, X->data[i][j]);
			}
			double y_ith = Y->data[i][0];
			double decision = 0.0;

			//computing the decision for all alphas computed in the support vectors
			for(int j=0; j < samples; j++){
				Matrix *yx_ith = CreateMatrix(1, features);
				for(int k=0; k < features; k++){
					set_value(yx_ith, 0, k, X->data[j][k]);
				}

				double alpha = model->dual_coeffs->data[j][0];
				double y_alpha = Y->data[j][0];
				double kernel_val = RBF(x_ith, yx_ith, model->gamma);


				decision += alpha * y_alpha * kernel_val;

				FreeMatrix(yx_ith);
			}

			double grad_w = 1 - y_ith * decision;
			double alpha_new = model->dual_coeffs->data[i][0];

			//applying the box constraint method
			if(alpha_new < 0){
				alpha_new = 0;
			} else if(alpha_new > model->C){
				alpha_new = model->C;
			}

			//update the alpha betch
			model->dual_coeffs->data[i][0] = alpha_new;

			FreeMatrix(x_ith);
		}
	}
	model->n_support_vectors = samples;
}


void PredictSVC(SupportVectorClassifier *model, Matrix *X)
{

	double decision = 0.0;

	for(int i=0; i < model->support_vectors->rows; i++){
		Matrix *support_vectors = CreateMatrix(1, model->support_vectors->cols);
		for(int x=0; x < model->support_vectors->cols; x++){
			set_value(support_vectors, 0, x, model->support_vectors->data[i][x]);
		}

		double kernel_val = RBF(support_vectors, X, model->gamma);
		decision += model->dual_coeffs->data[i][0] * kernel_val;

		FreeMatrix(support_vectors);
	}

	decision += model->bias;
	return decision >= 0 ? 1 : -1;
}

void FreeSVC(SupportVectorClassifier *model)
{
	FreeMatrix(model->support_vectors);
	FreeMatrix(model->dual_coeffs);
	free(model);
}
