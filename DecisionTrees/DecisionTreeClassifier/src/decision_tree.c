#include<stdlib.h>
#include<stdio.h>
#include "../include/matrix.h"
#include "../include/decision_tree.h"



double Entrophy(int *labels, int samples)
{
	int count_0 = 0, int count_1 = 0;

	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++};
		else{count_1++;}
	}

	double p0 = (double)count_0/samples;
	double p1 = (double)count_1/samples;

	if(count_0 == 0 ||  count_1 == 0){return 0;}

	return -(p0 * log2(p0) + p1 * log2(p1));
}


double InformationGain(Matrix *features, int *labels, int feature_index, int samples)
{
	int left_count = 0, right_count = 0; //counting feature from left to right

	int *left_labels = (*int)malloc(samples * sizeof(int));
	int *right_labels = (*int)malloc(samples * sizeof(int));

	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, i, feature_label);
		if(feature_value == 0){
			left_labels[left_count++] = labels[i];
		} else {
			right_labels[right_count++] = labels[i];
		}
	}


	double ent_left = Entrophy(left_labels, left_count);
	double ent_right = Entrophy(right_labels, right_count);
	double total_ent = Entrophy(labels, samples);

	double gain = total_ent - (left_count / (double)samples) * ent_left + (count_right / (double)samples * ent_right);

	free(left_labels);
	free(right_labels);

	return gain;
}

double FeatureToSplit(int *labels, int *labels, int samples, int features)
{
	int best_feature = 0;
	int best_gain = -1;

	for(int i=0; i < features; i++){
		double gain = InformationGain(features, labels, i, samples);
		if(gain > best_gain){
			best_gain = gain;
			best_feature = i;
		}
	}
	return best_feature;
}



DecisionTree *BuildTree(Matrix *features, int* labels, int samples, int features)
{
	int count_0 = 0, count_1 = 0;
	for(int i=0; i < samples; i++){
		if(labels[i] == 0){count_0++;}
		else{count_1++;}
	}

	//if samples are in the same class put them in the same leaf node
	if(count_0 == samples  || count_1 == samples){
		DecisionTree *leaf = (*DecisionTree)malloc(sizeof(DecisionTree));
		leaf->feature = -1; //will be initialized during the classification
		leaf->label = (count_0 == samples) ? 0 : 1;
		leaf->left = NULL;
		leaf->right = NULL;
		return leaf;
	}

	int best_feature = FeatureToSplit(features, labels, samples, features);

	DecisionTree *node = (*DecisionTree)malloc(sizeof(DecisionTree));
	node->feature =  best_feature;
	node->label = NULL;
	node->right = NULL;

	//spliting the dataset
	int *left_nodes = (*int)malloc(samples * sizeof(int));
	int *right_nodes = (*int)malloc(samples * sizeof(int));

	int count_left = 0, count_right = 0;

	for(int i=0; i < samples; i++){
		double feature_value = get_value(features, i, best_feature);
		if(feature_value == 0){
			left_indices[count_left++] = i;
		} else {right_indices[count_right++];}
	}

	Matrix *left_features = CreateMatrix(count_left, features);
	Matrix *right_features = CreateMatrix(count_right, features);

	int *left_labels = (*int)malloc(count_left * sizeof(int));
	int *right_labels = (*int)malloc(count_right * sizeof(int));

	for(int i=0; i < count_right; i++){
		for(int j=0; j < features; j++){
			set_value(right_features, i, j, get_value(features, right_indices[i], j));
		}
		right_labels[i] = labels[right_indices[i]];
	}

	node->left = BuildTree(left_features, left_labels, count_left, features);
	node->right = BuildTree(right_features, right_labels, count_right, features);

	free(left_indices);
	free(right_indices);
	free(left_labels);
	free(right_labels);
	FreeMatrix(left_features);
	FreeMatrix(right_features);

	return node;
}



int Classify(DecisionTree *node, Matrix *features, int sample_index)
{
	if(node->feature == -1){
		return node->label;
	}

	double feature_value = get_value(features, sample_index, node->feature);
	if(feature_value == 0){
		return Classify(node->left, features, sample_index);
	} else {return Classify(node->right, features, sample_index);}

}

void FreeTree(DecisionTree *node)
{
	if(node == NULL){return;}
	FreeTree(node->left);
	FreeTree(node->right);
	free(node);
}
