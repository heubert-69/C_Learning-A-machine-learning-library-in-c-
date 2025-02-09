#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>
#include "../include/multinomial_nb.h"

MultinomialNaiveBayes *CreateMNB(int n_classes, int n_features)
{
	MultinomialNaiveBayes *model = (MultinomialNaiveBayes*)malloc(sizeof(MultinomialNaiveBayes));
	model->n_classes = n_classes;
	model->n_features = n_features;
	model->class_priori = CreateMatrix(1, n_classes);
	model->feature_counts = (Matrix **)malloc(n_classes * sizeof(Matrix*));
	model->feature_probs = (Matrix **)malloc(n_classes * sizeof(Matrix*));
	//for creating 2d matrices
	for(int i=0; i < n_classes; i++){
		model->feature_counts[i] = CreateMatrix(1, n_classes);
		model->feature_probs[i] = CreateMatrix(1, n_features);
	}

	return model;
}

void TrainMNB(MultinomialNaiveBayes *model, Matrix *X, Matrix *Y)
{
	int n_samples = X->rows;
	Matrix *class_counts = CreateMatrix(1, model->n_classes);

	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		class_counts->data[0][class_label]++;
	}

	//compute class priors
	for(int i=0; i < model->n_classes; i++){
		model->class_priori->data[0][i] = (class_counts->data[0][i] + 1) / (n_samples + model->n_classes);
	}

	//computing feature counts per class
	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		for(int x=0; x < model->n_features; x++){
			model->feature_counts[class_label]->data[0][x] += X->data[i][x];
		}
	}

	//computing Feature probabilities
	for(int i=0; i < model->n_classes; i++){
		double total_count = 0.0;
		for(int x=0; x < model->n_features; x++){
			total_count += model->feature_counts[i]->data[0][x] + 1; //laplace smoothing
		}
		for(int j=0; j < model->n_features; j++){
			model->feature_probs[i]->data[0][j] = (model->feature_counts[i]->data[0][j] + 1) / total_count;
		}
	}

	FreeMatrix(class_counts);
}

int PredictMNB(MultinomialNaiveBayes *model, Matrix *X, int sample_index)
{
	int best_class = -1;
	double best_log_prob = -DBL_MAX;

	for(int i=0; i < model->n_classes; i++){
		double log_prob = log(model->class_priori->data[0][i]);
		for(int j=0; j < model->n_features; j++){
			log_prob += sample[j] * log(model->feature_probs[i]->data[0][j]);
		}

		if(log_prob > best_log_prob){
			best_log_prob = log_prob;
			best_class = i;
		}
	}
	return best_class;
}

Matrix *PredictSingleMNB(MultinomialNaiveBayes *model, Matrix *X)
{
	Matrix *predictions = CreateMatrix(X->rows, 1);
	for(int i=0; i < X->rows; i++){
		predictions->data[i][0] = PredictMNB(model, X->data[i]);
	}
	return predictions;
}

void FreeMNB(MultinomialNaiveBayes *model)
{
	FreeMatrix(model->class_priori);
	for(int i=0; i < model->n_classes; i++){
		FreeMatrix(model->feature_counts[i]);
		FreeMatrix(model->feature_probs[i]);
	}
	free(model->feature_counts);
	free(model->feature_probs);
	free(model);
}
