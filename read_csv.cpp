#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROWS 1000
#define MAX_COLS 10
#define TRAIN_SPLIT 0.8

// Estrutura para armazenar os dados
typedef struct {
    double features[MAX_COLS - 1];
    int label;
} DataPoint;

// Função para ler um CSV
int read_csv(const char *filename, DataPoint data[], int *num_samples, int num_features) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    
    char line[1024];
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < MAX_ROWS) {
        char *token = strtok(line, ",");
        int col = 0;
        
        while (token && col < num_features) {
            if (col < num_features - 1) {
                data[row].features[col] = atof(token);
            } else {
                data[row].label = atoi(token);
            }
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    
    fclose(file);
    *num_samples = row;
    return 0;
}


void split_data(DataPoint data[], int num_samples, DataPoint train[], DataPoint test[], int *train_size, int *test_size) {
    int train_count = (int)(num_samples * TRAIN_SPLIT);
    int test_count = num_samples - train_count;
    
    for (int i = 0; i < train_count; i++) {
        train[i] = data[i];
    }
    for (int i = 0; i < test_count; i++) {
        test[i] = data[train_count + i];
    }
    
    *train_size = train_count;
    *test_size = test_count;
}

int main() {
    DataPoint data[MAX_ROWS];
    DataPoint train[MAX_ROWS], test[MAX_ROWS];
    int num_samples, train_size, test_size;

    if (read_csv("data.csv", data, &num_samples, 5)) {
        return 1;
    }
    
    printf("Total de amostras: %d\n", num_samples);
    
    split_data(data, num_samples, train, test, &train_size, &test_size);
    
    printf("Amostras de treino: %d, Amostras de teste: %d\n", train_size, test_size);
    
    return 0;
}
