#ifndef COMPLEMENT_NB_H
#define COMPLEMENT_NB_H

#include "matrix.h"
#include<stdio.h>
#include<stdlib.h>


typedef struct {
	int n_features;
	int n_classes;
	Matrix *class_priori;
	Matrix **feature_probs;
} ComplementNaiveBayes;

ComplementNaiveBayes *CreateComplementNB(int n_classes, int n_features);
void TrainComplementNaiveBayes(ComplementNaiveBayes *model, Matrix *X, Matrix *Y);
int PredictComplementNB(ComplementNaiveBayes *model, double *samples);
Matrix *PredictComplementNB(ComplementNaiveBayes *model, Matrix *X);
void FreeComplementNaiveBayes(ComplementNaiveBayes *model);


#endif //COMPLEMENT_NB_H
