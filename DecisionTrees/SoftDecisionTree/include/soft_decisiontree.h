#ifndef SOFT_DECISIONTREE_H
#define SOFT_DECISIONTREE_H

#include<stdlib.h>
#include<stdio.h>
#include "matrix.h"

typedef struct SoftTreeNode {
	int is_leaf;
	double *weights;
	double bias;
	double prediction;
	struct SoftTreeNode *left;
	struct SoftTreeNode *right;
} SoftTreeNode;

typedef struct SoftTree {
	SoftTreeNode **root;
	int max_depth;
	double temperature; //control variable for softening the trees decision
}

SoftTree *TrainSoftTree(Matrix *features, double *labels, int samples, int n_features, int max_depth, double temp);
double PredictSoftTree(SoftTree *model, Matrix *features, int sample_index);
void FreeSoftTree(SoftTree *model);

#endif //SOFT_DECISIONTREE_H


