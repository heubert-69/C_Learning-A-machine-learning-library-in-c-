#ifndef OBLIQUE_H
#define OBLIQUE_H

#include<stdio.h>
#include<stdlib.h>
#include "matrix.h"

typedef struct ObliqueTree {
	int is_leaf;
	double *coefficients; //linear coefficients to replace single feature splits
	double threshold;
	double prediction;
	struct ObliqueTree *left;
	struct ObliqueTree *right;
} ObliqueTree;

typedef struct ObliqueTrees {
	ObliqueTree *root;
	int max_depth;
} ObliqueTrees;

ObliqueTree *BuildObliqueTree(Matrix *features, double *labels, int *indices, int samples, int n_features, int depth, int max_depth);
double PredictObliqueTree(ObliqueTree *model, Matrix *features, int sample_index);
void FreeObliqueTree(ObliqueTree *model);

#endif //OBLIQUE_H
