#ifndef C5_H
#define C5_H


#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"


typedef struct {
	int label;
	int feature;
	struct C5 *left;
	struct C5 *right; 
} C5;



C5* BuildC5(Matrix *features, int *labels, int samples, int n_features);
int ClassifyC5(C5 *node, Matrix *features, int sample_index);
void FreeC5(C5 *node);
double Entrophy(int *labels, int samples);
double InformationGain(Matrix *features, int *labels, int feature_index, double threshold, int samples);
int BestFeatureToSplit(Matrix *features, int *labels, int samples, int features, double best_threshold);

#endif //C5_H
