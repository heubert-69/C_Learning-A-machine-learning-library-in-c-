#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H


#include<stdlib.h>
#include<stdio.h>
#include "matrix.h"
#include "../CHAID/include/chaid.h"

typedef struct {
	CHAID  **trees
	int n_trees;
} RandomForest;




RandomForest *CreateRandomForest(Matrix *features, int *labels, int samples, n_features, int n_trees, int max_features);
int ClassifyRandomForest(RandomForest *node, Matrix *features, int sample_index);
void FreeRandomForest(RandomForest *node);




#endif //RANDOM_FOREST_H
