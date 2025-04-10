#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/meanshift.h"
#include "../include/matrix.h"

static double EuclideanDistance(double *a, double *b, int d)
{
	double sum = 0.0;
	for(int i=0; i < d; i++){
		double diff = a[i] - b[i];
		sum += diff * diff;
	}
	return sqrt(sum);
}

static void ShiftPoint(double *point, Matrix *X, int n_features, double bandwidth, double *new_point)
{
	int count = 0;
	for(int i=0; i < n_features; i++){
		new_point[i] = 0.0;
	}

	for(int x=0; x < X->rows; x++){
		double *x = &X->data[x * X->cols];
		double dist = EuclideanDistance(point, x, n_features);

		if(dist < bandwidth){
			for(int i=0; i < n_features; i++){
				new_point[i] += x[i];
			}
			count++;
		}
		if(count > 0){
			for(int i=0; i < n_features; i++){
				new_point[i] /= count;
			}
		}
	}
}

MeanShift* CreateMeanShift(double bandwidth, int max_iter, double tol)
{
	MeanShift *model = (MeanShift*)malloc(sizeof(MeanShift));
	model->bandwidth = bandwidth;
	model->max_iter = max_iter;
	model->tol = tol;
	model->centroids = NULL; //set during training
	return model;
}

void TrainMeanShift(MeanShift *model, Matrix *X)
{
	int n_samples = X->rows;
	int n_features = X->cols;

	Matrix *centroids = CreateMatrix(n_samples, n_features);

	for(int i=0; i < n_samples; i++){
		double *point = &X->data[i * n_features];
		double *shifted = (double*)malloc(sizeof(double) * n_features);

		for(int x=0; x < n_features; x++){
			shifted[x] = point[x];
		}

		for(int j=0; j < model->max_iter; j++){
			double *prev = (double*)malloc(sizeof(double) * n_features);

			for(int n=0; n < n_features; n++){
				prev[n] = shifted[n];
			}

			ShiftPoint(shifted, X, n_features, model->bandwidth, shifted);

			double movement = EuclideanDistance(prev, shifted, n_features);

			free(prev);
			if(movement < model->tol) break;
		}

		for(int j=0; j < n_features; j++){
			centroids->data[i * n_features + j] = shifted[j];
		}

		free(shifted);
	}

	model->centroids = centroids;
}

int PredictMeanShift(MeanShift *model, Matrix *X)
{
	double min_dist = INFINITY;
	int best_index = -1;

	for(int i=0; i < model->centroids->rows; i++){
		double *cent = &model->centroids->data[i * model->centroids->cols];
		double dist = EuclideanDistance(X->data, cent, X->cols);

		if(dist < min_dist){
			min_dist = dist;
			best_index = i;
		}
	}
	return best_index;
}

void FreeMeanShift(MeanShift *model)
{
	if(model){
		if(model->centroids){
			FreeMatrix(model->centroids);
		}
		free(mode);
	}
}
