#ifndef MULTINOMIAL_NB_H
#define MULTINOMIAL_NB_H

typedef struct {
	int n_classes;
	int n_features;
	Matrix **class_priori;
	Matrix **feature_counts;
	Matrix **feature_probs;
} MultinomialNaiveBayes;


MultinomialNaiveBayes *CreateMNB(int n_classes, int n_features);
void TrainMNB(MultinomialNaiveBayes *model, Matrix *X, Matrix *Y);
int PredictMNB(MultinomialNaiveBayes *model, Matrix *X, int sample_index);
Matrix *PredictSingleMNB(MultinomialNaiveBayes *model, Matrix *X);
void FreeMNB(MultinomialNaiveBayes *model);

#endif //MULTINOMIAL_NB_H


