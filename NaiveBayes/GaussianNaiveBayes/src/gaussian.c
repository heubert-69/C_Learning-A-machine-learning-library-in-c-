#include<stdlib.h>
#include<math.h>
#include<float.h>
#include "../include/gaussian.h"

GaussianNaiveBayes *CreateGaussianNaiveBayes(int n_classes, int n_features)
{
	GaussianNaiveBayes *model = (GaussianNaiveBayes*)malloc(sizeof(GaussianNaiveBayes));
	model->n_classes = n_classes;
	model->n_features = n_features;
	model->means = (Matrix**)malloc(n_classes * sizeof(Matrix*));
	model->variance = (Matrix**)malloc(n_classes * sizeof(Matrix*));
	for(int i=0; i < n_classes; i++){
		model->means[i] = CreateMatrix(1, n_features);
		model->variance[i] = CreateMatrix(1, n_features);
	}
	return model;
}

void TrainGaussianNaiveBayes(GaussianNaiveBayes *model, Matrix *X, Matrix *Y)
{
	int n_samples = X->rows;
	//counting the number of clases for each class feature
	Matrix *class_count = CreateMatrix(1, model->n_classes);
	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		class_count->data[0][class_label]++;
	}

	//compute class priors (probabilities :D)
	for(int i=0; i < n_samples; i++){
		model->class_priori->data[i][0] = class_count[0][i] / n_samples;
	}

	//computing the mean and variance for each class
	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		for(int x=0; x < model->n_features; x++){
			model->means[class_label]->data[0][x] += X->data[i][x];
		}
	}

	for(int i=0; i < model->n_classes; i++){
		for(int x=0; x < model->n_features; x++){
			model->means[i]->data[0][x] /= class_count->data[0][x];
		}
	}


	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		for(int x=0; x < model->n_features; x++){
			double diff = X->data[i][x] - model->means[class_label]->data[0][x];
			model->variance[class_label]->data[0][x] += diff * diff;
		}
	}

	for(int i=0; i < model->n_classes; i++){
		for(int x=0; x < model->n_features; x++){
			model->variance[i]->data[0][x] /= class_count->data[0][x];
		}
	}
	FreeMatrix(class_count);
}


int PredictNaiveBayes(GaussianNaiveBayes *model, double *samples)
{
	int best_log = -1;
	double best_prob = -DBL_MAX; //setting to negative infinity until the number is specified

	for(int i=0; i < model->n_classes; i++){
		double log_prob = log(model->class_priori->data[0][i]);
		for(int x=0; x < model->n_features; x++){
			double mean = model->means[i]->data[0][x];
			double var = model->variance[i]->data[0][x];
			double diff = samples[x];

			//calculating the probs
			log_prob -= 0.5 * log(2.0 * M_PI * var);
			log_prob -= (diff * diff) / (2.0 * var);
		}
		if(log_prob > best_prob){
			best_prob = log_prob;
			best_log = i;
		}
	}
	return best_log;
}

Matrix *PredictGaussianNaiveBayes(GaussianNaiveBayes *model, Matrix *X)
{
	Matrix *predictions = (X->rows, 1);
	for(int i=0; i < X->rows; i++){
		predictions->data[i][0] = PredictNaiveBayes(model, X->data[i]);
	}
	return predictions;
}

void FreeGaussianNaiveBayes(GaussianNaiveBayes *model)
{
	FreeMatrix(model->class_priori);
	for(int i=0; i < model->n_classes; i++){
		FreeMatrix(model->means[i]);
		FreeMatrix(model->variance[i]);
	}
	free(model->mean);
	free(model->variance);
	free(model);
}
