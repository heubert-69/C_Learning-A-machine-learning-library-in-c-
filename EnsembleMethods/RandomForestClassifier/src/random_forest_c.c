#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include "../include/random_forest_c.h"

RandomForestClassifier *CreateRandomForestClassifier(int n_trees, int max_depth, int min_samples_split, double feature_fraction)
{
	RandomForestClassifier *model = (RandomForestClassifier*)malloc(sizeof(RandomForestClassifier));
	model->n_trees = n_trees;
	model->max_depth = max_depth;
	model->min_samples_split = min_samples_split;
	model->feature_fraction = feature_fraction;
	model->trees = (DecisionTree**)malloc(n_trees * sizeof(DecisionTree*));
	for(int i=0; i < n_trees; i++){
		model->trees[i] = NULL;
	}
	srand(time(NULL));
	return model;
}

void TrainRandomForestClassifier(RandomForestClassifier *model, Matrix *features, double *labels, int n_classes)
{
	int n_samples = features->rows;
	int n_features = features->cols;


	for(int i=0; i < model->n_trees; i++){
		Matrix *bootstrap_features = CreateMatrix(n_samples, n_features);
		int *bootstrap_labels = (int*)malloc(n_samples * sizeof(int));

		for(int x=0; x < n_samples; x++){
			int sample_index = rand() % n_samples;
			for(int k=0; k < n_features; k++){
				set_value(bootstrap_features, x, k, get_value(features, sample_index, k));
			}
			bootstrap_labels[x] = labels[sample_index];
		}
		model->trees[i] = Build_Tree(bootstrap_features, bootstrap_labels, n_samples, n_features);

		FreeMatrix(bootstrap_features);
		free(bootstrap_labels);
	}
}

int TrainRandomForestClassifier(RandomForestClassifier *model, Matrix *X, int n_classes, int sample_index)
{
	int *votes = (int*)calloc(model->n_trees, sizeof(int));

	for(int i=0; i < model->n_trees; i++){
		if(model->trees[i]){
			int prediction = Classify(model->trees[i], features, sample_index);
			votes[prediction]++;
		}
	}

	int max_votes = 0, predicted_class = -1;

	for(int x=0; x < n_classes; x++){
		if(votes[x] > max_votes){
			max_votes = votes[x];
			predicted_class = x;
		}
	}

	free(votes);
	return predicted_class;
}


void FreeRandomForestClassifier(RandomForestClassifier *model)
{
	for(int i=0; i < model->n_trees; i++){
		FreeTree(model->trees[i]);
	}
	free(model->trees);
	free(model);
}
