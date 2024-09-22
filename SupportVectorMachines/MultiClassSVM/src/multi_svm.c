#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/multi_svm.h"


MultiClassSVM* CreateMultiSVM(int classes, int features, double learning_rate, double regularization, int epochs)
{

	MultiClassSVM *model = (*MultiClassSVM)malloc(sizeof(MultiClassSVM));
	model->weights = CreateMatrix(classes, features);
	model->learning_rate = learning_rate;
	model->regularization = regularization;
	model->epochs = epochs;

	//initializaing random weights
	for(int i=0; i < classes; i++){
		for(int x=0; x < features; x++){
			set_value(model->weights, i, x, ((double)rand()/RAND_MAX)-0.5f);
		}
	}
	return model;
}

void TrainMultiClassSVM(MultiClassSVM *model, Matrix *X, Matrix *Y)
{

	int samples = X->rows;
	int features = X->cols;
	int classes = model->weights->rows;

	for(int epoch=0; epoch < model->epochs; epoch++){
		for(int i=0; i < samples; i++){
			//get the ith sample from the features
			Matrix *x_ith = CreateMatrix(1, features);
			for(int x=0; x < features; x++){
				set_value(x_ith, 0, x, X->data[i][x]);
			}
			int true_class = (int) Y->data[i][0];
			//iterates through the classes of the model
			for(int c=0; c < classes; c++){
				double score = 0.0;

				for(int j=0; j < features; j++){
					//computing the dot product
					score += model->weights->data[c][j] * x_ith->data[0][j];
				}

				//computing the hinge loss function
				if(c == true_class){
					if(score < 1){
						for(int j=0; j < features; j++){
							model->weights->data[c][j] += model->learning_rate * (x_ith->data[0][j] - model->regularization * model->weights->data[c][j]);

						}
					} else {
						for(int i=0; i < features; i++){
							model->weights->data[c][i] += model->learning_rate * (-x_ith->data[0][i] - model->regularization * model->weights->data[c][i]);

						}
					} else {
						if(score >= 1){
							continue; //proceed if the score is greater than or equal to one
						} else {
							for(int x=0; x < features; x++){
								model->weights->data[c][x] += model->learning_rate * (-x_ith->data[0][x] - model->regularization * model->weights->data[c][x]);
							}
						}
					}
				}
			}
			FreeMatrix(x_ith);
		}
	}
}

int PredictMultiClass(MultiClassSVM *model, Matrix *X)
{
	int classes = model->weights->rows;
	int features = model->weights->cols;
	int best_class = -1;
	int best_score = -INFINITY;

	//computing the best score for each class and the greatest will be replaced in the best_score variable
	for(int c=0; c < classes; c++){
		double score = 0.0

		//compute da dot product
		for(int i=0; i < features; i++){
			score += model->weights->data[c][i] * X->data[0][i];
		}

		if(score > best_score){
			best_score = score;
			best_class = c
		}
	}
	return best_class;
}

void FreeMultiClass(MultiClassSVM *model)
{
	FreeMatrix(model->weights);
	free(model);
}
