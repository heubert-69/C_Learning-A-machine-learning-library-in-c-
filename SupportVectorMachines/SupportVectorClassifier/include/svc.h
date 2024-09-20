#ifndef SVC_H
#define SVC_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"

typedef struct {

	Matrix *support_vectors;
	Matrix *dual_coeffs;
	double bias;
	double gamma;
	double C;
	double learning_rate;
	int epochs;
	int n_support_vector;

} SupportVectorClassifier;



SupportVectorClassifier* CreateSVC(int features, double gamma, double C, double learning_rate, int epochs);
void TrainSVC(SupportVectorClassifier* model, Matrix *X, Matrix *Y);
void PredictSVC(SupportVectorModel *model, Matrix *X);
void FreeSVC(SupportVectorClassifier *model);


#endif //SVC_H
