#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>
#include "../include/complement_nb.h"

ComplementNaiveBayes *CreateComplementNB(int n_classes, int n_features)
{
	ComplementNaiveBayes *model = (ComplementNaiveBayes*)malloc(sizeof(ComplementNaiveBayes);
	model->n_classes = n_classes;
	model->n_features = n_features;
	model->class_priori = CreateMatrix(1, n_classes);
	model->feature_probs = (Matrix**)malloc(n_classes * sizeof(Matrix*));
	for(int i=0; i < modeln_classes; i++){
		model->feature_probs[i] = CreateMatrix(1, n_features);
	}
	return model;
}


void TrainComplementNaiveBayes(ComplementNaiveBayes *model, Matrix *X, Matrix *Y)
{
	int n_samples = X->rows;

	Matrix *class_counts = CreateMatrix(1, model->n_classes);

	for(int i=0; i < n_samples; i++){
		int class_label = (int)Y->data[i][0];
		class_counts[class_label]++;
	}
	//computing class priors
	for(int i=0; i < model->n_classes; i++){
		model->class_priori->data[0][i] = (class_counts[i] + 1.0) / (n_samples + model->n_classes);
	}

	//computing complement class
	for(int i=0; i < model->n_classes; i++){
		double total_comp = 0.0;
		for(int x=0; x < model->n_features; x++){
			double comp_count = 0.0;
			for(int j=0; j < n_samples; j++){
				int class_label = (int)Y->data[i][0];
                        	if(class_label != x){
                                	comp_count += X->data[i][x];
                        	}

			}

			//applying laplace smoothing
			model->feature_probs[i]->data[0][x] = comp_count + 1.0;
			total_comp += comp_count + 1.0;
		}

		//computing feature probability
		for(int c=0; c < model->n_features; c++){
			model->feature_probs[c]->data[0][c] /= total_comp;
		}
	}
	FreeMatrix(class_counts);
}


int PredictComplementNB(ComplementNaiveBayes *model, double *samples)
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

Matrix *PredictComplementNB(ComplementNaiveBayes *model, Matrix *X)
{
	Matrix *predictions = CreateMatrix(X->rows, 1);
	for(int i=0; i < X->rows; i++){
		predictions->data[i][0] = (double)PredictComplementNB(model, model->data[i]);
	}
	return predictions;
}

void FreeComplementNaiveBayes(ComplementNaiveBayes *model)
{
	FreeMatrix(model->class_priori);
	for(int i=0; i < model->n_classes; i++){
		FreeMatrix(model->feature_probs[i]);
	}
	free(model->feature_probs);
	free(model);
}
