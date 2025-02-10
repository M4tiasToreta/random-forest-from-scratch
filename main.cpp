#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "train.h"

int main()
{
    srand(time(NULL));
    DataPoint data[MAX_ROWS];
    DataPoint train[MAX_ROWS], test[MAX_ROWS];
    int num_samples, train_size, test_size;

    if (read_csv("data.csv", data, &num_samples, MAX_COLS))
    {
        return 1;
    }

    printf("Total de amostras: %d\n", num_samples);

    split_data(data, num_samples, train, test, &train_size, &test_size); // aqui tem que mudar para colocar aleatório nos samples de train and test

    printf("Amostras de treino: %d, Amostras de teste: %d\n", train_size, test_size);

    RandomForest *random_forest = train_model(train, train_size, MAX_COLS - 1);

    int *prediction = (int *)malloc(sizeof(test_size));
    prediction = predict(random_forest, test, test_size);
    for (int b = 0; b < test_size; b++)
    {
        printf("%d", train[b].label);
    }
    printf("\n");
    printf("------------------- prediction ---------------\n");
    printf("Test:      ");
    for (int b = 0; b < test_size; b++)
    {
        printf("%d", test[b].label);
    }
    printf("\nPrediction:");
    for (int b = 0; b < test_size; b++)
    {
        printf("%d", prediction[b]);
    }
    printf("\n");

    evaluate(prediction, test, test_size);
    free(prediction);
    free_random_forest(random_forest);
    printf("\n\n");
    return 0;
}
