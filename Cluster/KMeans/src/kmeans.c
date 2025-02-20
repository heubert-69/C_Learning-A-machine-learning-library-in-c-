#include "../include/kmeans.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<float.h>


//helper function..
//computing euclidean distance
static double EuclideanDistance(double *X, double *Y, int d)
{
	double dist = 0.0;
	for(int i=0; i < d; i++){
		double diff = X[i] - Y[i];
		dist += diff * diff;
	}
	return dist;
}

KMeans *CreateKMeans(int k, int max_iters, double tol)
{
	KMeans *model = (KMeans*)malloc(sizeof(KMeans));
	model->k = k;
	model->max_iters = max_iters;
	model->tol = tol;
	model->centroids = NULL;
	return model;
}


int *TrainKMeans(KMeans *model, Matrix *X)
{
	int n_samples = X->rows;
	int n_features = X->cols;
	int k = model->k;


	//allocate memory for cluster assignments
	int *labels = (int*)malloc(n_samples * sizeof(int));
	int *old_labels = (int*)malloc(n_samples * sizeof(int));

	//initialize the centroids
	model->centroids = CreateMatrix(k, n_features);
	for(int i=0; i < k; i++){
		int idx = rand() % n_samples;
		for(int x=0; x < n_features; x++){
			model->centroids->data[i][x] = X[idx][x];
		}
	}

	int iter = 0;
	double diff = 0.0;
	do {
		//assigning each point to the nearest centroid
		for(int i=0; i < n_samples; i++){
			double min_dist = -DBL_MAX;
			int best_cluster = -1;
			for(int x=0; x < k; x++){
				double dist = EuclideanDistance(X->data[i], model->centroids[x], n_features);
				if(dist < min_dist){
					min_dist = dist;
					best_cluster = x;
				}
			}
			labels[i] = best_cluster;
		}

		diff = 0.0;
		//check convergence if assignments hasnt been changed
		for(int i=0; i < n_samples; i++){
			if(labels[i] != old_labels[i]){
				diff += 1.0;
			}
			old_labels[i] = labels[i];
		}

		//if nothing has changed we break the loop
		if(diff == 0.0){
			break;
		}

		//recompute centroid as the mean centroids
		//reset the centroids
		for(int i=0; i < k; i++){
			for(int x=0; x < n_features; x++){
				model->centroids->data[i][x] = 0.0;
			}
		}

		int *counts = (int*)calloc(k, sizeof(int));
		for(int x=0; x < n_samples; x++){
			int c = labels[x];
			counts[c]++;
			for(int i=0; i < n_features; i++){
				model->centroids->data[c][i] += X->data[x][i];
			}
		}

		for(int i=0; i < k; i++){
			if(counts[i] > 0){
				for(int x=0; x < n_features; x++){
					model->centroids->data[i][x] /= counts[c];
				}
			}
		}
		free(counts);

		iter++;
	} while(iter < model->max_iter);

	free(old_labels);
	return labels;
}

int *PredictKMeans(KMeans *model, Matrix *X)
{
	int n_samples = X->rows;
	int *labels = (int*)malloc(n_samples * sizeof(int));

	for(int i=0; i < n_samples; i++){
		double min_dist = -DBL_MAX;
		int best_cluster = -1;
		for(int x=0; x < model->k; x++){
			double dist = EuclideanDistance(X->data[i], model->centroids->data[x], X->cols);
			if(dist < min_dist){
				min_dist = dist;
				best_cluster = x;
			}
		}
		labels[i] = best_cluster;
	}
	return labels;
}


void FreeKMeans(KMeans *model)
{
	if(model){
		if(model->centroids){
			FreeMatrix(model->centroids);
		}
		free(model);
	}
}
