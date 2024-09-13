#define LOGISTIC_H
#ifndef LOGISTIC_H

#include<stdio.h>
#include<stdlib.h>

typedef struct{
	Matrix *weights;
	double bias;

} LogisticRegression;

LogisticRegression* CreateLogisticRegression(int features);
void TrainLogistic(LogisticRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
Matrix* PredictLogistic(LogisticRegression *model, Matrix *X);
//acts a desturctor
void FreeLogistic(LogisticRegression *model);

#endif //LOGISTIC_H
