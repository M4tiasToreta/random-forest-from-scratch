#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_csv.h"
// #include "train.h"


int main() {
    DataPoint data[MAX_ROWS];
    DataPoint train[MAX_ROWS], test[MAX_ROWS];
    int num_samples, train_size, test_size;
    int prediction[114];

    if (read_csv("data.csv", data, &num_samples, MAX_COLS)) {
        return 1;
    }

    printf("Total de amostras: %d\n", num_samples);

    split_data(data, num_samples, train, test, &train_size, &test_size);

    printf("Amostras de treino: %d, Amostras de teste: %d\n", train_size, test_size);

    // train_model(train, train_size, MAX_COLS);
    // prediction = predict(test);
    // evaluate(prediction, test);
    return 0;
}
