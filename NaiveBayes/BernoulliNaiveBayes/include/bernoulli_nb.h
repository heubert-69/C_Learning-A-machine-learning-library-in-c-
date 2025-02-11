#ifndef BERNOULLI_NB_H
#define BERNOULLI_NB_H


#include "matrix.h"
#include<stdlib.h>
#include<stdio.h>


typedef struct {
	int n_features;
	int n_classes;
	Matrix **feature_probs;
	Matrix *class_priori;
} BernoulliNaiveBayes;


BernoulliNaiveBayes *CreateBernoulliNB(int n_features, int n_classes);
void TrainBernoulliNaiveBayes(BernoulliNaiveBayes *model, Matrix *X, Matrix *Y);
int PredictBernoulliNB(BernoulliNaiveBayes *model, double *samples);
Matrix *PredictBernoulliNB(BernoulliNaiveBayes *model, Matrix *X);
void FreeBernoulliNaiveBayes(BernoulliNaiveBayes *model);



#endif //BERNOULLI_NB_H


