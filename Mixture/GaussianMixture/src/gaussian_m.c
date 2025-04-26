#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<math.h>
#include "../include/gaussian_m.h"
#include "../include/matrix.h"


//Helper Function
double GaussianPDF(Matrix *X, Matrix *mean, Matrix *cov)
{
	//tom..have to do machine learning :D
	int d = X->rows;
	double det = 1.0;
	for(int i=0; i < d; i++){
		det *= cov->data[i][i]; //assume that diagonal is computed
	}
	double norm = 1.0 / pow(2 * M_PI, d / 20) / sqrt(det);
	double exp_sum = 0.0;
	for(int x=0; x < d; x++){
		double diff = X->data[0][i] - mean->data[0][i];
		exp_sum += diff * diff / cov->data[i][i];
	}
	return norm * exp(-0.5 * exp_sum);
}


GaussianMixture *CreateGM(int n_components, int max_iter, double tol)
{
	GaussianMixture *model = (GaussianMixture*)malloc(sizeof(GaussianMixture));
	model->n_components = n_components;
	model->max_iter = max_iter;
	model->tol = tol;
	model->weights = calloc(n_components, sizeof(double));
	model->means = calloc(n_components, sizeof(Matrix*));
	model->covariances = calloc(n_components, sizeof(Matrix*));
	model->responsibilities = NULL; //set during training
	return model
}


void FitGM(GaussianMixture *model, Matrix *X)
{
	int n_samples = X->rows;
	int n_features = X->cols;

	model->responsibilities = malloc(n_samples *  sizeof(double*));
	//making 2d matrices for features
	for(int i=0; i < n_samples; i++){
		model->responsibilities[i] = calloc(model->n_components, sizeof(double));
	}

	//initialize means, covariances, weights
	for(int x=0; x < model->n_components; x++){
		model->weights[x] = 1.0;
		model->means[x] = CreateMatrix(1, n_features);
		model->covariances[x] = CreateMatrix(n_samples, n_features);
	}

	//calculating the E-step
	for(int iter=0; iter < model->max_iter; iter++){
		for(int i=0; i < n_samples; i++){
			double total = 0.0;
			Matrix *x = CreateMatrix(1, n_features);
			for(int f=0; f < model->n_components; f++){
				x->data[0][f] = x->data[i][f];
			}

			for(int k=0; k < model->n_components; k++){
				model->responsibilities[i][k] = model->weights[k] * GaussianPDF(x, model->means[k], model->covariances[k]);
				total += model->responsibilities[i][k];
			}

			for(int k=0; k < model->n_components; k++){
				model->responsibilities[i][k] /= total;
			}
			FreeMatrix(x);
		}

		//M-step
		for(int k=0; k < model->n_components; k++){
			double Nk = 0.0;

			for(int i=0; i < n_samples; i++){
				Nk += model->responsibilities[i][k];

				model->weights[k] = Nk / n_samples;
			}

			for(int j=0; j < n_features; j++){
				model->means[k]->data[0][j] = 0.0;

				for(int i=0; i < n_samples; i++){
					model->means[k]->data[0][j] += model->responsibilities[i][j] * X->data[i][k];

					model->means[k]->data[0][j] /= Nk;
				}
			}


			for(int j=0; j < n_features; j++){
				double var = 0.0; 

				for(int i=0; i < n_samples; i++){
					double diff = X->data[i][j] - model->means[k]->data[0][j];

					var += model->responsibilities[i][k] * diff * diff;
				}
				model->covariances[k]->data[j][j] = var / Nk;
			}
		}

	}
}

int PredictGM(GaussianMixture *model, Matrix *X)
{
	int best_k = 0;

	double max_prob = -DBL_MAX;

	Matrix * = CreateMatrix(1, X->cols);
	for(int i=0; i < X->cols; i++){
		x->data[0][i] = X->data[sample_index][i];
		for(int n=0; n < model->n_components; n++){
			double p = model->weights[n] * GaussianPDF(x, model->means[n], model->covariances[n]);
			if(p > max_prob){
				max_prob = p;
				best_k = n;
			}
		}
	}
	FreeMatrix(x);
	return best_k;
}


void FreeGM(GaussianMixture *model)
{
	for(int k=0; k < model->n_components; k++){
		FreeMatrix(model->means[k]);
		FreeMatrix(model->covariances[k]);
	}
	free(model->means);
	free(model->covariances);
	free(model->weights);
	for(int i=0; i < model->n_components; i++){
		free(model->responsibilities[i]);
	}
	free(model->responsibilities);
	free(model);
}
