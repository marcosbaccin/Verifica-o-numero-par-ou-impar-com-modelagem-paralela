#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <ctime>

//variaveis globais farao acesso a gpu
int* array_rand, * array_result;

//kernel --> GPU
__global__ void kernelDosParcas(int n, int* array_rand, int* array_result){
    int i = threadIdx.x;
    while(i<n){
        array_result[i] = array_rand[i] % 2;
        i += blockDim.x;
    }
}

//codigo --> CPU
int main(){
    cudaDeviceReset(); // resetar cudacore
    int * d_array_rand, * d_array_result; // variaveis do lado da gpu
    int n = 6000;
    int tamanho = n* sizeof(int); //vetor para locacao de memoria

    //malloc do array_rand e array_result da cpu
    array_rand = (int *)malloc(tamanho);
    array_result = (int *)malloc(tamanho);

    //malloc da gpu
    cudaMalloc((void**)&d_array_rand, tamanho);
    cudaMalloc((void**)&d_array_result, tamanho);

    //inicializar valores do array_rand
    int maior = 989;
    int menor = 10;
    for (int i = 0; i < n; i++) {
        array_rand[i] = rand() % maior + menor;
    }

    //mover dados da cpu para gpu
    cudaMemcpy(d_array_rand, array_rand, tamanho, cudaMemcpyHostToDevice);

    //processamento
    kernelDosParcas <<<6, 1000>>>(n, d_array_rand, d_array_result);

    //saida de dados
    cudaMemcpy(array_result, d_array_result, tamanho, cudaMemcpyDeviceToHost);

    printf("\n Resultado: \n");
    for (int i = 0; i < n; i++){
        
        printf("%d, %d \n", array_rand[i], array_result[i]);
    }
    
    cudaFree(d_array_rand); cudaFree(d_array_result);
    return 0;
}
