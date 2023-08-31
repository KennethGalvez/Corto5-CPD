#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void par_qsort(int *data, int lo, int hi)
{
    if (lo > hi)
        return;
    int l = lo;
    int h = hi;
    int p = data[(hi + lo) / 2];

    while (l <= h)
    {
        while ((data[l] - p) < 0)
            l++;
        while ((data[h] - p) > 0)
            h--;
        if (l <= h)
        {
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++;
            h--;
        }
    }
    // recursive call
    par_qsort(data, lo, h);
    par_qsort(data, l, hi);
}

int main()
{
    int N;

    printf("Cantidad de numeros a generar: ");
    scanf("%d", &N);

    int *x = (int *)malloc(N * sizeof(int));
    if (!x)
    {
        perror("Error allocating memory for x");
        exit(1);
    }

    srand(time(NULL)); // Seed random number 

    FILE *csv_file = fopen("input.csv", "w");
    if (!csv_file)
    {
        perror("Error creating CSV file");
        exit(1);
    }

    for (int j = 0; j < N; j++)
    {
        int num;
        do
        {
            num = rand() % 100000; // Generate numeros randoms entre 0 y 999999
        } while (j > 0 && num == x[j - 1]); // Checquear por duplicados

        x[j] = num;
        fprintf(csv_file, "%d\n", x[j]);
    }

    fclose(csv_file);

    // Leer los numeros del CSV 
    csv_file = fopen("input.csv", "r");
    if (!csv_file)
    {
        perror("Error abrienod CSV");
        exit(1);
    }

    for (int j = 0; j < N; j++)
    {
        if (fscanf(csv_file, "%d", &x[j]) != 1)
        {
            perror("Error leyendo el CSV ");
            exit(1);
        }
    }

    fclose(csv_file);

    clock_t start_time = clock();

    par_qsort(x, 0, N - 1);

    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Escribir los numeros sorteados
    csv_file = fopen("output.csv", "w");
    if (!csv_file)
    {
        perror("Error creando CSV");
        exit(1);
    }

    for (int j = 0; j < N; j++)
    {
        fprintf(csv_file, "%d\n", x[j]);
    }

    fclose(csv_file);

    free(x);

    printf("Sortiado y Escritura Completa.\n");
    printf("Tiempo: %lf segundos\n", execution_time);

    return 0;
}
