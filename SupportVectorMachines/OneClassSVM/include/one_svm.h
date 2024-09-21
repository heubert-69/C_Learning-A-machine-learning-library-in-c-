#ifndef ONE_SVM_H
#define ONE_SVM_H

#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"


typedef struct {
	Matrix *support_vectors;
	Matrix *dual_coeffs;
	double bias;
	double nu;
	double gamma;
	double learning_rate;
	int epochs;
} OneClassSVM;


OneClassSVM* CreateOneClass(double nu, double gamma, double learing_rate, int epochs);
void TrainOneClass(OneClassSVM *model, Matrix *X);
double PredictOneClass(OneClassSVM *model, Matrix *X);
void FreeOneClass(OneClassSVM *model);


#endif //ONE_SVM_H

