#include "../include/bgc.h"
#include<stdlib.h>
#include<stdio.h>
#include<time.h>



//sampling the dataset
void Bootstrap_sample(Matrix *X, int *Y, Matrix *X_sample, int* Y_sample, int samples)
{
	for(int i=0; i < samples; i++){
		int idx = rand() % samples; //random index selection for sampling
		for(int x=0; x < X->cols; x++){
			X_sample->data[i * X->cols + x] = X->data[idx * X->cols + x];
		}
		Y_sample = Y[idx];
	}
}



BGC* CreateBGC(int n_estimators, int max_depth)
{
	BGC *model = (BGC*)malloc(sizeof(BGC));
	model->trees = (DecisionTree**)malloc(sizeof(DecisionTree));
	model->n_estimators = n_estimators;
	model->max_depth = max_depth;
	return model;
}


void TrainBGC(BGC *model, Matrix *X, int *Y, int samples)
{
	Matrix *X_sample = CreateMatrix(samples, X->cols);
	int *Y_sample = (int*)calloc(samples, sizeof(int));

	for(int i=0; i < model->n_estimators; i++){
		Bootstrap_sample(X, Y, X_sample, Y_sample, samples);
		model->trees[i] = BuildTree(X_sample, Y_sample, samples, model->max_depth);
	}

	FreeMatrix(X_sample);
	free(Y_sample);
}

int PredictBGC(BGC *model, Matrix *X, int sample_index)
{
	int *votes = (int*)calloc(model->);
	for(int i=0; i < model->n_estimators; i++){
		n_votes = Classify(model->trees[i], X, sample_index);
		votes[n_votes]++;
	}

	int final_pred = (votes[0] > votes[1])? 0 : 1; //majority vote???? 
	free(votes);
	return votes;
}


void FreeBGC(BGC *model)
{
	for(int i=0; i < model->n_estimators; i++){
		FreeTree(model->trees[i]);
	}
	free(model->trees);
	free(model);
}
