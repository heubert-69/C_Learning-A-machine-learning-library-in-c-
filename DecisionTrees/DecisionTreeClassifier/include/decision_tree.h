#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include<stdlib.h>
#include<stdio.h>
#include "matrix.h"



typedef struct {
	int feature;
	int label;
	struct TreeNode *left;
	struct TreeNode *right;
} DecisionTree;

DecisionTree *Build_Tree(Matrix* features, int *labels, int samples, int features);
int Classify(DecisionTree *node, Matrix *features, int sample);
void FreeTree(DecisionTree *node);
double Entrophy(int *labels, int samples);
double InformationGain(Matrix *features, int *labels, int feature_index, int samples);
int FeatureToSplit(Matrix *features, int *labels, int samples, int features);


#endif //DECISION_TREE_H
