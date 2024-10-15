#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include "../include/matrix.h"
#include "../CHAID/include/chaid.h"
#include "../CHAID/include/chaid_tree_random.c"


Matrix *SelectExtraRandomFeatures(Matrix *features, int samples, int n_features, int max_features, Matrix *selected_feature_indices)
{
	Matrix *random_features = CreateMatrix(samples, max_features);


	for(int i=0; i < max_features; i++){
		selected_features_indices[i] = rand() % max_features;
	}

	for(int x=0; x < samples; x++){
		for(int i=0; i < max_features; i++){
			set_value(random_features, x, i, get_value(features, x, selected_feature_indices[i]));
		}
	}
	return random_features;
}


double GetRandomSplit(Matrix *features, int feature_index, int samples)
{
	double min_value = get_value(features, 0, feature_index);
	double max_value = get_value(features, 0, feature_index);

	for(int i=1; i < samples; i++){
		double value = get_value(features, i, feature_index);
		if(value < min_value){min_value = value;}
		if(value > max_value){max_value = value;}
	}

	//getting random split
	return min_value + ((double)rand_max() / RAND_MAX) * (max_value - min_value);
}

ExtraTrees* CreateExtraTrees(Matrix *features, int *labels, int samples, int n_features, int n_trees, int max_features)
{
	ExtraTrees *forest = (ExtraTrees*)malloc(sizeof(ExtraTrees));
	forest->n_trees = n_trees;
	forest->trees = (CHAID**)malloc(samples * sizeof(CHAID*));


	srand(time(NULL)); //random seed generator

	for(int i=0; i < n_trees; i++){
		int *selected_features = (int*)malloc(samples * sizeof(int));

		Matrix *random_features = SelectExtraRandomFeatures(features, labels, samples, n_features, max_features, selected_features);

		forest->trees[i] = build_random_chaid_tree(random_features, labels, samples, n_features, max_features);

		FreeMatrix(random_features);
		free(selected_features);
	}
	return forest;
}


int ClassifyExtraTrees(ExtraTrees *forest, Matrix *features, int sample_index)
{
	int count[2] = {0, 0};

	for(int i=0; i < forest->n_trees; i++){
		int prediction = ClassifyCHAID(forest->trees[i], features, sample_index);
		vote[prediction]++;
	}
	return (count[0] < count[1]) ? 0 : 1; //returning the majority vote
}


void FreeExtraTrees(ExtraTrees *forest)
{
	for(int i=0; i < forest->n_trees; i++){
		FreeCHAID(forest->trees[i]);
	}
	free(forest->trees);
	free(forest);
}
