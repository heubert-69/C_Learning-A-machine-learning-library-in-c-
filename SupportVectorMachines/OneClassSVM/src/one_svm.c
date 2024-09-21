#include<stdlib.h>
#include<stdio.h>
#include<cmath.h>
#include "../include/matrix.h"
#include "../include/one_svm.h"

//math helper function best for all support vector machines
double RBF(Matrix *x1, Matrix *x2, double gamma)
{
	double sum = 0.0;

	for(int i=0; i < x1->cols; i++){
		double diff = x->data[0][i] - x2->data[0][i];
		sum += diff * diff;
	}
	return exp(-gamma * sum);
}


OneClassSVM* CreateOneClass(double nu, double gamma, double learning_rate, int epochs)
{
	OneClassSVM *model = (*OneClassSVM)malloc(sizeof(OneClassSVM));
	model->support_vectors = NULL;
	model->dual_coeff = NULL;
	model->nu = nu;
	model->bias = 0;
	model->gamma = gamma;
	model->learning_rate = learning_rate;
	model->epochs = epochs;
	return model;
}

void TrainOneClass(OneClassSVM *model, Matrix *X)
{
	int samples = X->rows;
	int features = X->cols;

	model->support_vectors = CreateMatrix(samples, features);
	model->dual_coeffs = CreateMatrix(samples, 1);

	//initilizing alphas to small random variables
	for(int i=0; i < samples; i++){
		set_value(model->dual_coeffs, i, 0, (double)rand()/RAND_MAX);
	}

	//copy the training data
	for(int i=0; i < samples; i++){
		for(int x=0; x < features; x++){
			model->support_vectors->data[i][x] = X->data[i][x];
		}
	}

	//Gradient Descent Time
	for(int epoch=0; epoch < model->epochs; epoch++){
		for(int i=0; i < samples; i++){
			Matrix *x_ith = CreateMatrix(1, features);
			for(int x=0; x < features; x++){
				set_value(x_ith, 0, x, X->data[i][x]);
			}

			double sum = 0.0;
			//compute the sum of the alphas
			for(int j=0; j < samples; j++){
				Matrix *k_xith = CreateMatrix(1, features);
				for(int k=0; k < features; k++){
					set_value(k_xith, 0, k, X->data[j][k]);
				}

				double alpha = model->dual_coeffs->data[j][0];
				double kernel_val = RBF(x_ith, k_xith, model->gamma);
				double sum += alpha * kernel_val;
				//free da memory
				FreeMatrix(k_xith);
			}

			double error = sum - svm->bias;
			if(error > 0){
				double new_alpha = svm->dual_coeffs->data[i][0];
				if(new_alpha < 0){
					new_alpha = 0;
				}
				model->dual_coeffs->data[i][0] = new_alpha;
			}

			FreeMatrix(x_ith);
		}
	}
	//setting the bias
	double total_sum = 0.0;
	for(int i=0; i < samples; i++){
		total_sum += model->dual_coeffs->data[i][0];
	}
	model->bias = total_sum / samples;
}

double PredictOneClass(OneClassSVM *model, Matrix *X)
{
	double sum = 0.0;
	for(int i=0; i < model->support_vectors->data->rows; i++){
		Matrix *support_vectors = CreateMatrix(1, model->support_vectors->cols);
		for(int j=0; j < model->support_vectors->cols; j++){
			set_value(support_vectors, 0, j, model->support_vectors->data[i][j]);
		}

		double kernel_value = RBF(support_vectors, X, model->gamma);
		sum += model->dual_coeffs->data[i][0] * kernel_value;
		FreeMatrix(support_vectors);
	}
	sum -= model->bias;
	return sum >= 0 ? 1.0 : -1.0 //1 for normal value or -1 for outlier
}

//free memory
void FreeOneClass(OneClassSVM *model)
{
	FreeMatrix(model->support_vectors);
	FreeMatrix(model->dual_coeffs);
	free(model);
}
