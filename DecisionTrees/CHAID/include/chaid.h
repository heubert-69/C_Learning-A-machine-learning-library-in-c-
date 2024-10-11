#ifndef CHAID_H
#define CHAID_H

#include "matrix.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

typedef struct {
	int feature;
	int label;
	struct TreeNode left;
	struct TreeNode right;
} CHAID;


CHAID* CreateCHAID(Matrix *features, int *labels, int samples, int n_features);
void FreeCHAID(CHAID *node);
int ClassifyCHAID(CHAID *node, Matrix *features, int sample_index);
double ChiSquaredTest(int *observed, int *expected, int size);
int BestFeatureToSplit(Matrix *features, int *labels, int samples, int n_features);




#endif //CHAID_h
