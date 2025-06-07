🧠 C Machine Learning Library

A full-featured, modular machine learning library written in C, using your custom matrix library. This library includes a wide range of algorithms for classification, regression, clustering, and density estimation.


📦 Features

🧮 Linear Models

LinearRegression

RidgeRegression

LassoRegression

LogisticRegression

ElasticNet


🌳 Decision Trees

DecisionTreeClassifier (ID3, CART, C5, CHAID)

DecisionTreeRegressor

ObliqueDecisionTree

SoftDecisionTree


🌲 Ensemble Methods

RandomForestClassifier, RandomForestRegressor

ExtraTreesClassifier, ExtraTreesRegressor

GradientBoostingClassifier, GradientBoostingRegressor

HistGradientBoostingClassifier, HistGradientBoostingRegressor

AdaBoostClassifier, AdaBoostRegressor

BaggingClassifier, BaggingRegressor

VotingClassifier, VotingRegressor

StackingClassifier, StackingRegressor

XGBoost (simplified)

LightGBM (simplified)

CatBoost (with ordered boosting and categorical support)


🧪 Naive Bayes

GaussianNB

MultinomialNB

BernoulliNB

ComplementNB

CategoricalNB


🧍 Neighbors

KNeighborsClassifier, KNeighborsRegressor

RadiusNeighborsClassifier, RadiusNeighborsRegressor

KDTree (for fast neighbor search)


🌌 Clustering

KMeans, MiniBatchKMeans

DBSCAN

OPTICS

SpectralClustering

AgglomerativeClustering, FeatureAgglomeration

MeanShift

AffinityPropagation


🧬 Mixture Models

GaussianMixture

BayesianGaussianMixture



🗂️ Directory Structure

mlc/
├── models/
│   ├── linear/                # Linear models
│   ├── tree/                  # Tree models
│   ├── ensemble/              # Random forest, GBoost, etc.
│   ├── bayes/                 # Naive Bayes models
│   ├── neighbors/             # KNN, Radius, KDTree
│   ├── cluster/               # KMeans, DBSCAN, etc.
│   └── mixture/               # GMM, Bayesian GMM
├── matrix/
│   ├── matrix.h
│   └── matrix.c
├── utils/                     # Distance metrics, sampling, etc.
├── include/                   # Shared headers
└── main.c                     # Example usage




🚀 How to Use

1. Include Headers

#include "matrix/matrix.h"
#include "models/linear/linear_regression.h"
#include "models/ensemble/random_forest.h"
#include "models/cluster/kmeans.h"

2. Create a Model

LinearRegression *lr = create_linear_regression();
RandomForestClassifier *rf = create_random_forest(10, max_depth);
KMeans *kmeans = create_kmeans(n_clusters, n_features);

3. Train the Model

train_linear_regression(lr, X_train, y_train);
train_random_forest_classifier(rf, X_train, y_train);
train_kmeans(kmeans, X_train, n_iters);

4. Make Predictions

double prediction = predict_linear_regression(lr, X_test, i);
int label = predict_random_forest_classifier(rf, X_test, i);
int cluster = predict_kmeans(kmeans, X_test, i);

5. Free Memory

free_linear_regression(lr);
free_random_forest_classifier(rf);
free_kmeans(kmeans);
free_matrix(X_train);
free_matrix(y_train);




🛠️ Requirements

GCC or Clang compiler

Standard C99+

Your custom matrix.h library with basic matrix operations (create, destroy, get, set, transpose, dot)





🧪 Example: Linear Regression

#include "matrix/matrix.h"
#include "models/linear/linear_regression.h"

int main() {
    Matrix *X = load_matrix("features.csv");
    Matrix *y = load_matrix("targets.csv");

    LinearRegression *lr = create_linear_regression();
    train_linear_regression(lr, X, y);
    
    double pred = predict_linear_regression(lr, X, 0);
    printf("Prediction: %.4f\n", pred);

    free_linear_regression(lr);
    free_matrix(X);
    free_matrix(y);
    return 0;
}




🧩 Extending the Library

You can easily extend this library by:

Adding new models (models/)

Implementing different distance metrics (utils/distance.h)

Using your matrix operations for efficient computation



📚 Documentation

Each model folder contains:

A .h file with the struct and function prototypes

A .c file with full implementation

Comments for each method



📌 License

MIT License