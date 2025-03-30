#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

int N,T,counter=0;
int **A = NULL, **B = NULL, **C = NULL;

int **mem_alloc(){
    int i,j;
    int **table;

    table = (int**) malloc(N*sizeof(int*)); //rows
    if (!table){
        printf("Error while allocating memory!\n");
        exit(0);
    }

    for(i = 0; i < N; i++){
        table[i] = (int*) malloc(N*sizeof(int)); //columns
        if (!table[i]){
        printf("Error while allocating memory!\n");
        exit(0);
        }
    }

    return table;
    
}

void **fillArray(int **array){
    int i, j;

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            array[i][j] = 1 + rand()%10;
        }
    }
    srand(time(NULL));
}

void *worker(void *data){  //εκτελείται από κάθε thread
    int i, j;
    int a = N/T;
    
    if (counter == 1){ //if at thread 1
        for (i = 0; i < a;i++){  //rows
            for(j = 0; j < N;j++){ //columns
                C[i][j] = A[i][j] - B[i][j];
            }
        }
    } else {
        for (i = a;i < a*counter;i++){
            for(j = 0; j < N;j++){
                C[i][j] = A[i][j] - B[i][j];
            }
        }
    }
    
}

void **printArray(int **array){
    int i, j;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%d\t", array[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    N = atoi(argv[1]); //διαστάσεις
    T = atoi(argv[2]); //αριθμός threads
    pthread_t t[T];
    struct timeval start, end;
    double ts_usec,ts, te_usec,te, tt;
    
    if (N%T != 0){
        printf("T must be submultiple of N\n");
        exit(0);
    } else {
        A = mem_alloc();
        B = mem_alloc();
        fillArray(A);
        fillArray(B);
        if (N < 15) {
            printf("Matrix 1:\n");
            printArray(A);
            printf("Matrix 2:\n");
            printArray(B);
        }

        gettimeofday(&start, NULL);

        C = mem_alloc();

        //threads
        for (int i = 0; i < T; i++) {
            counter++;
            pthread_create(&t[i], NULL, worker, (void*)NULL);
            pthread_join(t[i], NULL);
        }

        gettimeofday(&end, NULL);

        ts_usec = (double) start.tv_usec/1000000; //millisec to sec
        ts = ts_usec + start.tv_sec;
        te_usec = (double) end.tv_usec/1000000;  //millisec to sec
        te = te_usec + end.tv_sec;
        tt = te - ts; //total time in sec

        //printing final table
        if (N < 15){
            printf("Matrix 3:\n");
            printArray(C);
            printf("Number of threads:%d Execution Time:%f sec\n", counter, tt);
        }
    }
    
    return 0;

}