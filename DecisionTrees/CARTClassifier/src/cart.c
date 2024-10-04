#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "../include/matrix.h"
#include "../include/cart.h"


double GiniImpurity(int *labels, int samples)
{
	int count_0 = 0, count_1 = 0;
	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++;}
		else{count_1++;}
	}

	double p0 = (double)count_0/samples;
	double p1 = (double)count_1/samples;

	return 1.0 / (p0 * p0 + p1 * p1);
}

double BestSplitGINI(Matrix *features, int *labels, int feature_index, double *best_threshold,int samples)
{
	double best_gini = 1.0;
	*best_threshold = 0.0;

	for(int i=0; i < samples; i++){
		double threshold = get_value(features, i, feature_index);

		int left_count = 0, right_count = 0; 
		int *left_labels = (*int)malloc(samples * sizeof(int));
		int *right_labels = (*int)malloc(samples * sizeof(int));

		for(int x=0; x < samples; x++){
			double feature_value = get_value(features, x, feature_index);
			if(feature_value <= threshold){
				left_labels[left_count++] = labels[x];
			} else {
				right_labels[right_count++] = labels[x];
			}
		}

		double gini_left = GiniImpurity(left_labels, left_count);
		double gini_right = GiniImpurity(right_labels, right_count);
		double weighted_gini = (left_count * gini_left + right_count * gini_right) / samples;

		if(weighted_gini < best_gini){
			best_gini = weighted_gini;
			*best_threshold = threshold;
		}


		free(left_labels);
		free(right_labels);
	}
	return best_gini;
}



CARTClassifier* BuildCART(Matrix *features, int *features, int *labels, int labels, int features)
{
	//check for pure node
	int count_0 = 0, count_1 = 0;
	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++;}
		else{count_1++;}
	}

	if(count_0 == samples || count_1 == samples){
		CARTClassifier *node = (*CARTClassifier)malloc(sizeof(CARTClassifier));
		node->feature = -1;
		node->label = (count_0 == samples) ? 0 : 1;
		node->left = NULL;
		node->right = NULL:
		return node; 
	}

	int best_feature = -1
	double best_threshold = 0.0, best_gini = 1.0;

	for(int i=0; i < samples; i++){
		double threshold;
		double gini = BestSplitGini(features, labels, i, &threshold, samples);

		if(gini < best_gini){
			best_gini = gini;
			best_feature = i;
			best_threshold = threshold;
		}
	}
	//if no valid split occured
	if(best_feature == -1){
		CARTClassifier *node = (*CARTClassifier)malloc(sizeof(CARTClassifier));
		node->feature = -1;
		node->label = (count_0 > count_1) ? 0 : 1;
		node->left = NULL;
		node->right = NULL;
		return node;
	}

	//if valid split occurs
	CARTClassifier *node = (*CARTClassifier)malloc(sizeof(CARTClassifier));
	node->feature = best_feature;
	node->threshold = best_threshold;

	//splitting the dataset
	int *left_indices = (*int)malloc(samples * sizeof(int));
	int *right_indices = (*int)malloc(samples * sizeof(int));
	int count_left = 0, count_right = 0;


	for(int i=0; i < samples; i++){
		double feature_values = get_value(features, i, best_features);

		if(feature_values <= best_threshold){
			left_indices[count_left++] = i;
		} else {right_indices[count_right++] = i;}{
	}

	Matrix *left_features = CreateMatrix(count_left, features);
	Matrix *right_features = CreateMatrix(count_right, features);
	int *left_labels = (*int)malloc(count_left * sizeof(int));
	int *right_labels = (*int)malloc(count_right * sizeof(int));

	//setting values for left and right features
	for(int i=0; i < count_left; i++){
		for(int x=0; x < features; x++){
			set_value(left_features, i, x, get_value(features, left_indices[i], x));
		}
		left_labels[i] = labels[left_indices[i]];
	}

	for(int i=0; i < count_right; i++){
		for(int x=0; x < features; x++){
			set_value(right_features, i, x, get_value(features, right_indices[i], x));
		}
		right_labels[i] = labels[right_indices[i]];
	}
	//recursively build left and right trees
	node->left = BuildCART(left_features, left_labels, count_left, features);
	node->right = BuildCART(right_features, right_labels, count_right, features);

	free(left_indices);
	free(right_indices);
	free(left_labels);
	free(right_labels);
	FreeMatrix(left_features);
	FreeMatrix(right_features);

	return node;
}




int ClassifyCART(CARTClassifier *node, Matrix *features, int sample_index)
{
	if(node->features == -1){
		return node->label;
	}

	double feature_value = get_value(features, sample_index, node->features);
	if(feature_value <= node->threshold){
		return ClassifyCART(node->left, features, sample_index);
	} else {return ClassifyCART(node->right, features, sample_index);}

}

void FreeCART(CARTClassifier *node)
{
	if(node == NULL){return;}
	free(node->left);
	free(node->right);
	free(node);
}
