#include<stdlib.h>
#include<stdio.h>
#include<float.c>
#include<math.h>
#include<time.h>
#include "../include/minikmeans.h"


//Helper function to calculate euclidean distance
static double EuclideanDistance(double *a, double *b, int d)
{
	double sum = 0.0;
	for(int i=0; i < d; i++){
		double diff = a[i] - b[i];
		sum += diff * diff;
	}
	return sqrt(sum);
}


MiniBatchKMeans *CreateMiniKMeans(int k, int max_iters, int minibatch_size, int d)
{
	MiniBatchKMeans *model = (MiniBatchKMeans*)malloc(sizeof(MiniBatchKMeans));
	model->k = k;
	model->max_iters = max_iters;
	model->minibatch_size = minibatch_size;
	model->d = d;
	model->counts = (int*)calloc(k, sizeof(int));

	//randomly initialize centroids to the points
	srand((unsigned int)time(NULL));
	for(int i=0; i < k; i++){
		//pick a random value for each centroid
		for(int x=0; x < d; x++){
			set_value(model->centroids, i, x, (double)rand() / RAND_MAX);
		}
	}
	return model;
}


int *TrainMiniBatchKMeans(MiniBatchKMeans *model, Matrix *X)
{
	int n_samples = X->rows;
	int d = X->cols;
	int batch_size = model->minibatch_size;


	int *batch_indices = (*int)malloc(batch_size * sizeof(int));

	//sampling minibatch
	for(int i=0; i < model->max_iters; i++){
		for(int x=0; x < batch_size; i++){
			batch_indices[x] = rand() % n_samples;
		}

		//assign the sample to the nearest centroid and update that centroid
		for(int j=0; j < batch_size; j++){
			int idx = batch_indices[j];
			double sample = X->data[i];
			double min_dist = DBL_MAX;
			int best_cluster = -1;

			for(int c=0; c < model->k; c++){
				double dist = EuclideanDistance(sample, model->centroids->data[c], d);
				if(dist < min_dist){
					min_dist = dist;
					best_cluster = c;
				}
			}
			//increment the counts array for the best cluster
			model->counts[best_cluster]++
			int count = model->counts[best_cluster];
			//updating centroid using the incremental mean update

			for(int k=0; k < d; k++){
				double old_value = get_value(model->centroids, best_cluster, k);
				double sample_value = sample[j];
				double new_value = old_value + (sample_value - old_value) / count;
				set_value(model->centroids, best_cluster, k, new_value);
			}
		}
	}
	free(batch_indices);
}

int *PredictMiniBatchKMeans(MiniBatchKMeans *model, double *sample)
{
	int n_samples = X->rows;
	int *labels = (int*)malloc(n_samples * sizeof(int));
	for(int i=0; i < n_samples; i++){
		labels[i] = PredictMiniBatchKMeans(model, X->data[i]);
	}
	return labels;
}


//for consistency and definition purposes to protect data leaks
int PredictMiniBatchKMeans(MiniBatchKMeans *model, double *sample)
{
	return PredictMiniBatchKMeans(model, sample);
}

//for getting a batch of samples
Matrix *PredictMiniBatchKMeansSamples(MiniBatchKMeans *model, Matrix *X)
{
	Matrix *labels_matrix = CreateMatrix(X->rows, 1);
	for(int i=0; i < X->rows; i++){
		int cluster = PredictMiniBatchKMeans(model, X->data);
		set_value(labels_matrix, i, 0, (double)cluster);
	}
	return labels_matrix;
}

void FreeMiniBatchKMeans(MiniBatchKMeans *model)
{
	if(model){
		if(model->centroids){
			FreeMatrix(model->centroids);
		}
		if(model->counts){
			free(model->counts);
		}
	}
	free(model);
}
