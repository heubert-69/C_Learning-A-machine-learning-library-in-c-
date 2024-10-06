#ifndef ID3_H
#define ID3_H

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "matrix.h"

typedef struct {
	int label;
	int feature;
	struct TreeNode *left;
	struct TreeNode *right;
} ID3;


ID3* BuildID3(Matrix *features, int *labels, int samples, int features);
int ClassifyID3(Matrix *node, Matrix *features, int sample_index);
void FreeID3(ID3 *node);
double Entrophy(int *labels, int samples);
double InformationGain(Matrix *features, int *labels, int feature_index, int samples);
int BestFeatureToSplit(Matrix *features, int *labels, int samples, int n_features);

#endif //ID3_H
