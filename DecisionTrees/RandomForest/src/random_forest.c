#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../include/random_forest.h"
#include "../include/matrix.h"



Matrix* SelectRandomFeatures(Matrix *features, int samples, int num_features, int max_features, int *selected_features_indices)
{
	//creating a matrix for random feature selection
	Matrix *random_features = CreateMatrix(samples, max_features);

	for(int i=0; i < max_features; i++){
		selected_features_indices[i] = rand() % num_features;
	}

	for(int i=0; i < samples; i++){
		for(int x=0; x < max_features; x++){
			set_value(random_features, i, x, get_value(features, i, selected_features_indices[x]));
		}
	}

	return random_features;
}



//create the random forest
RandomForest *CreateRandomForest(Matrix *features, int *labels, int samples, n_features, int n_trees, int max_features)
{
	RandomForest *forest = (*RandomForest)malloc(sizeof(RandomForest));

	forest->n_trees = n_trees;
	forest->trees = (**CHAID)malloc(n_trees * sizeof(*CHAID));

	srand(time(NULL)); //to make sure to generate random value and dosent let the values repeat during iteration

	for(int i=0; i < n_trees; i++){
		int *selected_features = (*int)malloc(max_features * sizeof(int));

		//select random subsets of the features
		Matrix *random_features = SelectRandomFeatures(features, samples, n_features, max_features, selected_features);


		forest->trees[i] = CreateCHAID(random_features, labels, samples, max_features);


		FreeMatrix(random_features);
		free(selected_features);
	}
	return forest;
}



int ClassifyRandomForest(RandomForest *node, Matrix *features, int sample_index)
{
	int votes[2] = {0, 0}; //will be inputed values after model classification


	//prediction
	for(int i=0; i < forest->n_trees; i++){
		int  prediction = ClassifyCHAID(forest->trees[i], features, sample_index);
		votes[prediction]++;
	}

	//returns the max vote
	return (votes[0] > votes[1]) ? 0 : 1;
}



void FreeRandomForest(RandomForest *node)
{
	for(int i=0; i < node->n_trees; i++){
		FreeCHAID(node->trees[i]);
	}
	free(node->trees);
	free(node);
}
