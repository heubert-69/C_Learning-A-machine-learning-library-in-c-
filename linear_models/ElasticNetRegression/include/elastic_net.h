#ifndef ELASTIC_NET_H
#define ELASTIC_NET_H


#include<stdio.h>
#include<stdlib.h>
#include<cmath>



typedef struct {

	Matrix *weights;
	double alpha;
	double bias;
	double l1; //regularization to prevent overfitting

} ElasticNetRegresssion;


ElasticNetRegression* CreateNetRegression(int features, double alpha);
void TrainNetRegression(ElasticNetRegression *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
Matrix* PredictNetRegression(ElasticNetRegression *model, Matrix *X)

//acts as a destructor
void FreeNetRegression(ElasticNetRegression *model);


#endif //ELASTIC_NET_H
