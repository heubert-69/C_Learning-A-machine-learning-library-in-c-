#include<stdlib.h>
#include<stdio.h>
#include<float.h>
#include<math.h>
#include "../include/bernoulli_nb.h"


BernoulliNaiveBayes *CreateBernoulliNB(int n_features, int n_classes)
{
	BernoulliNaiveBayes *model = (BernoulliNaiveBayes*)malloc(sizeof(BernoulliNaiveBayes));
	model->n_features = n_features;
	model->n_classes = n_classes;
	model->class_priori = CreateMatrix(1, n_classes);
	model->feature_probs = (Matrix**)malloc(n_classes * sizeof(Matrix*));
	for(int i=0; i < n_classes; i++){
		model->feature_probs[i] = CreateMatrix(1, n_features);
	}

	return model;
}

void TrainBernoulliNaiveBayes(BernoulliNaiveBayes *model, Matrix *X, Matrix *Y)
{
	int n_samples = X->rows;


	Matrix *class_counts = CreateMatrix(1, model->n_classes);
	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		class_counts->data[0][class_label]++;

	}

	//computing class priors
	for(int i=0; i < model->n_classes; i++){
		model->class_priori->data[0][i] = (class_counts->data[0][i] + 1);
	}

	//computing feature probabilities
	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		for(int x=0; x < model->n_features; x++){
			model->feature_probs[class_label]->data[0][x] += X->data[i][x];
		}
	}

	for(int i=0; i < model->n_classes; i++){
		for(int x=0; x < model->n_features; x++){
			model->feature_probs[i]->data[0][x] = (model->feature_probs[i]>data[0][x] + 1) / (class_counts->data[0][i] + 2);
		}
	}

	FreeMatrix(class_counts);
}


int PredictBernoulliNB(BernoulliNaiveBayes *model, double *samples)
{
	int best_class = -1;
	double best_log_prob = -DBL_MAX;


	for(int i=0; i < model->n_classes; i++){
		double log_prob = log(model->class_priori->data[0][i]);

		for(int x=0; x < model->n_features; x++){
			if(sample[x] == 1){
				log_prob += log(model->feature_probs[i]->data[0][x]);
			} else {
				log_prob += log(1.0 - model->feature_probs[i]->data[0][x]);
			}
		}

		if(log_prob > best_log_prob){
			best_log_prob = log_prob;
			best_class = i;
		}
	}
	return best_class;
}

Matrix *PredictBernoulliNB(BernoulliNaiveBayes *model, Matrix *X)
{
	Matrix *predictions = CreateMatrix(X->rows, 1);
	for(int i=0; i < X->rows; i++){
		predictions->data[i][0]->PredictBernoulliNB(model, model->data[i]);
	}
	return predictions;
}

void FreeBernoulliNaiveBayes(BernoulliNaiveBayes *model)
{
	FreeMatrix(model->class_priori);
	for(int i=0; i < model->n_classes; i++){
		FreeMatrix(model->feature_probs[i]);
	}
	free(model->feature_probs);
	free(model);
}
