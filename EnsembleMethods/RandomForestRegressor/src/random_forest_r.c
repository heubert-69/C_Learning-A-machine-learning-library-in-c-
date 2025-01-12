#include<math.h>
#include<stdlib.h>
#include<time.h>
#include "../include/random_forest_r.h"

RandomForestRegressor *CreateForestRegressor(int n_trees, int max_depth, int samples_split, double feature_fraction)
{
	RandomForestRegressor *model = (RandomForestRegressor*)malloc(sizeof(RandomForestRegressor);)
	model->trees = trees;
	model->max_depth = max_depth;
	model->samples_split = samples_split;
	model->feature_fraction = feature_fraction;
	model->n_trees = n_trees;
	model->trees = (DecisionTree**)malloc(n_trees * sizeof(DecisionTree));
	for(int i=0; i < n_trees; i++){
		model->trees[i] = NULL; //will be set during training
	}
	srand(time(NULL));
	return model;
}


void TrainForestRegressor(RandomForestRegressor *model, Matrix *X, double *labels)
{
	int n_samples = X->rows;
	int n_features = X->cols;

	for(int i=0; i < model->n_trees; i++){
		Matrix *bootstrap_features = CreateMatrix(n_samples, n_features);
		int *bootstrap_labels = (int*)malloc(n_samples * sizeof(int));
		for(int j=0; j < n_samples; j++){
			int sample_index = rand() % n_samples
			for(int k=0; k < n_features; k++){
				set_value(bootstrap_features, j, k, get_value(X, sample_index, k));
			}
			bootstrap_labels[j] = labels[sample_index];
		}

		model->trees[i] = Build_Tree(bootstrap_features, bootstrap_labels, n_samples, n_features);

		FreeMatrix(bootstrap_features);
		free(bootstrap_labels);
	}
}

double PredictForestRegressor(RandomForestRegressor *model, Matrix *X, int sample_index)
{
	double prediction = 0.0;
	for(int i=0; i < model->n_trees; i++){
		if(model->trees[i]){
			prediction += Classiy(model->trees[i], features, sample_index);
		}
	}
	return prediction / model->n_trees;
}


void FreeForestRegressor(RandomForestRegressor *model)
{
	for(int i=0; i < model->n_trees; i++){
		if(model->trees[i]){
			FreeTree(model->trees[i]);
		}
	}
	free(model->trees);
	free(model);
}
