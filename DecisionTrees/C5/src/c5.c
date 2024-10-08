#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/c5.h"




double Entrophy(int *labels, int samples)
{
	int count_0 = 0, count_1 = 0;
	for(int i=0; i < samples; i++){
		if(labels[i] == 0) {count_0++;}
		else{count_1++;}
	}

	double p0 = (double)count_0/samples;
	double p1 = (double)count_1/samples;

	if(count_0 == 0 || count_1 == 1){return 0.0;}
	return -(p0 * log2(p0) + p1 * log2(p1));
}



double InformationGain(Matrix *features, int *labels, int *feature_index, double threshold, int samples)
{
	int left_count = 0, right_count = 0;
	int *left_labels = (*int)malloc(samples * sizeof(int));
	int *right_labels = (*int)malloc(samples * sizeof(int));


	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, i, feature_index);
		if(feature_value <= threshold){
			left_labels[left_count++] = labels[i];
		} else {
			right_labels[right_count++] = labels[i];
		}
	}

	double ent_left = Entrophy(left_labels, left_count);
	double ent_right = Entrophy(right_labels, right_count);

	double total_ent = Entrophy(labels, samples);
	double gain = total_ent - (left_count / (double)samples) * ent_left + (right_count/(double)samples);

	free(left_labels);
	free(right_labels);

	return gain;

}


int BestFeatureToSplit(Matrix *features, int *labels, int samples, int features, double best_threshold)
{
	int best_feature = -1;
	int best_gain = -1;

	for(int i=0; i < features; i++){
		double *thresholds = (*double)malloc(samples * sizeof(double));
		int unique = 0;

		for(int x=0; x < samples; x++){
			thresholds[unique++] = get_value(features, x, i);
		}

		//sort the values in thresholds
		for(int k=0; k < unique; k++){
			for(int n=k+1; n < unique; n++){
				if(thresholds[k] > thresholds[n]){
					double temp = thresholds[k];
					thresholds[k] = thresholds[n];
					tresholds[k] = temp;
				}
			}

		}


		//evaluating each thresholds
		for(int x=0; x < unique; x++){
			double threshold = (threholds[x-1] + thresholds[x]) / 2.0;
			double gain = InformationGain(features, labels, i, threshold, samples); 
			if(gain > best_gain){
				best_gain = gain;
				best_feature = i;
				*best_threshold = threshold;
			}
		}
		free(thresholds)
	}
	return best_feature;
}


C5* BuildC5(Matrix *features, int *labels, int samples, int n_features)
{
	int count_0 = 0, count_1 = 0;
	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++;}
		else{count_1++;}
	}



	//initilizing the leaf node if count is similar to samples
	if(count_0 == samples || count_1 == 0){
		C5* node = (*C5)malloc(sizeof(C5));
		node->feature = -1;
		node->label = (count_0 == num_samples) ? 0 : 1;
		node->left = NULL:
		node->right = NULL; 
		return node;
	}

	//finding the best feature to split on
	double best_threshold = 0.0;
	int best_feature = BestFeatureToSplit(features, labels, samples, n_features);

	C5* node = (*C5)malloc(sizeof(C5));
	node->feature = best_feature;
	node->threshold = best_threshold

	//separating features based on best threshold and feature
	int *left_labels = (*int)malloc(samples * sizeof(int));
	int *right_labels = (*int)malloc(samples * sizeof(int));

	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, i, best_feature);
		if(feature_value <= best_threshold){
			left_labels[count_left++] = labels[i];
		} else {
			right_labels[count_right++] = labels[i]
		}
	}


	Matrix *left_features = CreateMatrix(count_left, n_features);
	Matrix *right_features = CreateMatrix(count_right, n_features);

	for(int i=0; i < count_left; i++){
		for(int x=0; x < n_features; x++){
			set_value(left_features, i, x, get_value(features, i, x)); //copies from the original feature matrix
		}
	}

	for(int i=0; i < count_left; i++){
                for(int x=0; x < n_features; x++){
                        set_value(right_features, i, x, get_value(features, i, x)); //copies from the original feature matrix
                }
        }

	//recursively build the tree
	node->left = BuildC5(left_features, left_labels, count_left, n_features);
	node->right = BuildC5(right_features, right_labels, count_right, n_features);

	free(left_labels);
	free(right_labels);
	FreeMatrix(right_features);
	FreeMatrix(left_features);

	return node;
}


int ClassifyC5(C5 *node, Matrix *features, int sample_index)
{
	if(node->feature == 0){
		return node->label;
	}

	double feature_value = get_value(features, sample_index, node->feature);
	if(feature_value <= node->threshold){
		return ClassifyC5(node->left, features, sample_index);
	} else {
		return ClassifyC5(node->right, features, sample_index);
	}

}

void FreeC5(C5 *node)
{
	if(node == NULL){return;}
	FreeC5(node->left);
	FreeC5(node->right);
	free(node);
}
