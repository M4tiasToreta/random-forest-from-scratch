#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_csv.h"

typedef struct TreeNode {
    int feature_index;         // feature to split on
    double threshold;          // threshold value for the split
    int prediction;            // if leaf node, prediction (0 or 1)
    struct TreeNode *left;     // left subtree pointer
    struct TreeNode *right;    // right subtree pointer
    int is_leaf;
} TreeNode;

typedef struct {
    TreeNode **nodes;
    int n_trees;
} RandomForest;

int random_int(int low, int high) {
    return low + rand() % (high - low + 1);
}

double gini_index(DataPoint *samples, int n_samples) {
    if (n_samples == 0)
        return 0.0;

    int count0 = 0, count1 = 0;
    for (int i = 0; i < n_samples; i++) {
        if (samples[i].label == 0)
            count0++;
        else
            count1++;
    }
    double p0 = (double)count0 / n_samples;
    double p1 = (double)count1 / n_samples;
    return 1.0 - (p0 * p0 + p1 * p1);
}

void split_dataset(DataPoint *samples, int n_samples, int feature, double threshold,
                   DataPoint **left, int *n_left, DataPoint **right, int *n_right) {
    DataPoint *left_samples = (DataPoint *)malloc(n_samples * sizeof(DataPoint));
    DataPoint *right_samples = (DataPoint *)malloc(n_samples * sizeof(DataPoint));
    int left_count = 0, right_count = 0;

    for (int i = 0; i < n_samples; i++) {
        if (samples[i].features[feature] < threshold) {
            left_samples[left_count++] = samples[i];
        } else {
            right_samples[right_count++] = samples[i];
        }
    }

    *left = left_samples;
    *right = right_samples;
    *n_left = left_count;
    *n_right = right_count;
}

void find_best_split(DataPoint *samples, int n_samples, int n_features,
                     int *best_feature, double *best_threshold, double *best_gini) {
    *best_gini = 1.0;
    *best_feature = -1;
    *best_threshold = 0.0;

    int feature = random_int(0, n_features); // CHANGE HERE FOR A MORE INTELLIGENT SPLIT

    double min_val = samples[0].features[feature];
    double max_val = samples[0].features[feature];
    for (int i = 0; i < n_samples; i++) {
        if (samples[i].features[feature] < min_val)
            min_val = samples[i].features[feature];
        if (samples[i].features[feature] > max_val)
            max_val = samples[i].features[feature];
    }

    int num_thresholds = 10; // ?? MAKE SURE THIS MAKES SENSE
    for (int i = 0; i < num_thresholds; i++) {
        double threshold = min_val + i * (max_val - min_val) / (num_thresholds - 1);
        DataPoint *left = NULL, *right = NULL;
        int n_left = 0, n_right = 0;
        split_dataset(samples, n_samples, feature, threshold, &left, &n_left, &right, &n_right);
        double gini_left = gini_index(left, n_left);
        double gini_right = gini_index(right, n_right);
        double weighted_gini = (n_left * gini_left + n_right * gini_right) / n_samples;
        if (weighted_gini < *best_gini) {
            *best_gini = weighted_gini;
            *best_feature = feature;
            *best_threshold = threshold;
        }
        free(left);
        free(right);
    }
}

TreeNode* build_tree(DataPoint train[], int depth, int max_depth, int n_samples, int n_features, int min_registers_per_split){
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->left = node->right = NULL;
    node->is_leaf = 0;
    int best_feature;
    double best_threshold;
    double best_gini;

    if (depth >= max_depth || n_samples < min_registers_per_split || gini_index(train, n_samples) == 0.0) {
        int count0 = 0, count1 = 0;
        for (int i = 0; i < n_samples; i++) {
            if (train[i].label == 0) count0++; else count1++;
        }
        node->is_leaf = 1;
        // Set prediction to the majority class.
        node->prediction = (count1 > count0) ? 1 : 0;
        return node;
    }
    find_best_split(train, n_samples, n_features, &best_feature, &best_threshold, &best_gini);

    if (best_feature == -1) {
        int count0 = 0, count1 = 0;
        for (int i = 0; i < n_samples; i++) {
            if (train[i].label == 0)
                count0++;
            else
                count1++;
        }
        node->is_leaf = 1;
        node->prediction = (count1 > count0) ? 1 : 0;
        return node;
    }

    node->feature_index = best_feature;
    node->threshold = best_threshold;
    DataPoint *left_samples = NULL, *right_samples = NULL;
    int n_left = 0, n_right = 0;
    split_dataset(train, n_samples, best_feature, best_threshold, &left_samples, &n_left, &right_samples, &n_right);

    node->left = build_tree(left_samples, depth + 1, max_depth, n_left, n_features, min_registers_per_split);
    node->right = build_tree(right_samples, depth + 1, max_depth, n_right, n_features, min_registers_per_split);

    free(left_samples);
    free(right_samples);
    return node;
}

void free_tree(TreeNode *node) {
    if (node == NULL)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

void free_random_forest(RandomForest *forest) {
    for (int i = 0; i < forest->n_trees; i++) {
        free_tree(forest->nodes[i]);
    }
    free(forest->nodes);
    free(forest);
}


RandomForest* train_model(DataPoint train[], int n_samples, int n_features, int min_registers_per_split=15, int max_depth=10, int n_trees=5) {
    RandomForest *random_forest = (RandomForest *)malloc(sizeof(RandomForest));
    random_forest->n_trees = n_trees;
    random_forest->nodes = (TreeNode * *)malloc(n_trees * sizeof(TreeNode*));

    for (int i = 0; i < n_trees; i++) {
        DataPoint *bootstrap = (DataPoint *)malloc(n_samples * sizeof(DataPoint));
        for (int j = 0; j < n_samples; j++) {
            int index = random_int(0, n_samples - 1); // valores podem se repetir aqui. Talvez um tunning para não se repetirem?
            bootstrap[j] = train[index];
        }
        random_forest->nodes[i] = build_tree(bootstrap, 0, max_depth, n_samples, n_features, min_registers_per_split);
        free(bootstrap);
    }
    return random_forest;
}

int predict_tree(TreeNode *node, DataPoint sample) {
    if (node->is_leaf)
        return node->prediction;
    if (sample.features[node->feature_index] < node->threshold)
        return predict_tree(node->left, sample);
    else
        return predict_tree(node->right, sample);
}

int make_final_decision(int* trees_decisions, int num_nodes){
    int sum=0;
    for (int i=0; i<num_nodes; i++){
        sum += trees_decisions[i];
    }
    return (sum * 2 >= num_nodes) ? 1 : 0;
}

int* predict(RandomForest *random_forest, DataPoint samples[], int n_prediction_sample) {
    int* final_decisions = (int*)malloc(n_prediction_sample * sizeof(int));
    int trees_decisions[random_forest->n_trees];
    for (int j = 0; j<n_prediction_sample; j++) {
        for (int i = 0; i < random_forest->n_trees; i++) {
            int pred = predict_tree(random_forest->nodes[i], samples[j]);
            trees_decisions[i] = pred;
        }
        final_decisions[j] = make_final_decision(trees_decisions, random_forest->n_trees);
    }
    return final_decisions;
}

int evaluate(int *prediction, DataPoint test_sample[], int test_size) {
    int count_correct=0;
    for (int i=0; i<test_size; i++) {
        // printf("prediction %d sample %d\n", prediction[i], test_sample[i].label);
        if (prediction[i]==test_sample[i].label) {
            count_correct++;
        }
    }
    printf("%d\n", count_correct);
    printf("Accurancy: %f\n", (float)count_correct/test_size);
    return 0;
}