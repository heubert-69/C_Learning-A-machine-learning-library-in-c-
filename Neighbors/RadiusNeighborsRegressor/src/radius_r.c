#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/radius_r.h"

//helper function for euclidean distance
static double EuclideanDistance(double *p1, double *p2, int dim)
{
	double dist = 0.0;
	for(int i=0; i < dim; i++){
		double diff = p1[i] - p2[i];
		dist += diff * diff;
	}
	return sqrt(dist);
}

RadiusNeighborsRegressor *CreateRadiusRegressor(double radius)
{
	RadiusNeighborsRegressor *model = (RadiusNeighborsRegressor*)malloc(sizeof(RadiusNeighborsRegressor));
	model->features = NULL; //set during training
	model->labels = NULL; //set during training
	model->radius = radius;
}

void TrainRadiusNeighborsRegressor(RadiusNeighborsRegressor *model, Matrix *X, Matrix *Y)
{
	model->features = X;
	model->labels = Y;
}

double PredictSingleRadiusNeighborsRegressor(RadiusNeighborsRegressor *model, Matrix *X, int sample_index)
{
	double weighted_sum = 0.0;
	double weight_sum = 0.0; //for each matrix it will be added together then computed to form weighted sum;

	for(int i=0; i < model->features->rows; i++){
		double dist = EuclideanDistance(model->features->data[i], X->data[sample_index], model->features->cols);
		if(dist <= model->radius){
			double weight = (dist == 0) ? 1.0 : 1.0 / dist; //handlig division by zero
			weighted_sum += weight * model->labels->data[i][0];
			weight_sum += weight;
		}
	}
	return (weight_sum > 0) ? (weighted_sum / weight_sum): 0.0;
}

Matrix *PredictRadiusNeighborsRegressor(RadiusNeighborsRegressor *model, Matrix *X)
{
	Matrix *predictions = (X->rows, 1);
	for(int i=0; i < X->rows; i++){
		predictions->data[i][0] = PredictSingleRadiusNeighborsRegressor(model, X, i);
	}
	return predictions;
}

void FreeRadiusRegressor(RadiusNeighborsRegressor *model)
{
	if(model){
		FreeMatrix(model->features);
	}
	if(model->labels){
		FreeMatrix(model->labels);
	}
	free(model);
}
