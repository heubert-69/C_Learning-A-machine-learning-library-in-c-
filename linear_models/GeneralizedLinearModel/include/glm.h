#ifndef GLM_H
#define GLM_H


#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"

typedef enum {
	identity, //identity link aka linear regression
	logit, //logit link aka logistic regression
	log //log link aka poisson regression
} LinkFunctionType;


typedef enum {
	normal, //assumes a normal distribution
	bernoulli, //assumes a binary outcome or a logistic regression
	poisson //counts data or a poisson regression
} DistributionType;

typedef struct {
	Matrix *weights;
	double bias;
	LinkFunctionType link;
	DistributionType distribution;
} GLM;


GLM* CreateGLM(int features, LinkFunctionType link, DistributionType distribution);
void TrainGLM(GLM *model, Matrix *X, Matrix *Y, double learning_rate, int epochs);
Matrix* PredictGLM(GLM *model, Matrix *X);
void FreeGLM(GLM *model);

//math functions to link functions and do derivatives
double ApplyLinkFunction(double z, LinkFunctionType link);
double ApplyInverseLinkFunction(double z, LinkFunctionType link);


#endif //GLM_H
