#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define N 3

void *hilo(void *arg){

    // Generar dos numeros aleatorios entre 0 y 10
    float a = ((float)rand() / RAND_MAX) * 10.0f;
    float b = ((float)rand() / RAND_MAX) * 10.0f;

    // Reservar memoria para devolver la suma
    float *suma = malloc(sizeof(float));

    if(suma == NULL){
        perror("Error reservando memoria");
        return NULL;
    }

    *suma = a + b;

    printf("a = %.2f, b = %.2f, suma = %.2f\n", a, b, *suma);

    return (void *)suma;
}

int main(){

    srand(time(NULL));

    pthread_t hilos[N];
    void *suma_parcial;
    float suma_total = 0.0f;
    int error;

    // Crear los N hilos
    for(int i = 0; i < N; i++){

        error = pthread_create(&hilos[i], NULL, hilo, NULL);

        if(error != 0){
            fprintf(stderr, "Error creando hilo: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a los hilos y recoger sus resultados
    for(int i = 0; i < N; i++){

        error = pthread_join(hilos[i], &suma_parcial);

        if(error != 0){
            fprintf(stderr, "Error esperando hilo: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }

        if(suma_parcial == NULL){
            fprintf(stderr, "El hilo %d no pudo devolver su suma\n", i);
            exit(EXIT_FAILURE);
        }

        suma_total += *(float *)suma_parcial;

        free(suma_parcial);
    }

    printf("\nLa suma total es: %.2f\n", suma_total);

    return EXIT_SUCCESS;
}