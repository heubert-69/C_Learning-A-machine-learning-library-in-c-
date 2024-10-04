#ifndef CART_H
#define CART_H


#include<stdio.h>
#include<stdlib.h>
#include "matrix.h"

typedef struct {
	int label;
	int feature;
	double threshold;
	struct TreeNode *left_node;
	struct TreeNode *right_node;
} CARTClassifier;



CARTClassifier* BuildCART(Matrix *features, int *features, int *labels, int samples, int features);
int ClassifyCART(CARTClassifier *node, Matrix *features, int sample_index);
void FreeCART(CARTClassifier *node);
double GiniImpurity(int *labels, int samples);
double BestSplitGini(Matrix *features, int *labels, int feature_index, double *best_threshold, int samples);


#endif //CART_H
