/******************************************************************************

Implementação do módulo Funções
Autor: Miguel Angelo Gonzaga Marques, Mateus Augusto do Nascimento Affonso
Data: 15/07/2023

Descrição:  Esse módulo cobre apenas as funções auxiliares, isto é, funções em
            que seu código se repete várias vezes ao longo do main(), e por isso
            foram separadas em funções para tornar o código mais limpo.

*******************************************************************************/

#include "Funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/*====================================FUNÇÕES====================================*/

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {

    }
}

void limpar_tela(void){
    #ifdef _WIN32
        system("cls");      /*Limpa a tela no Windows*/
    #else
        system("clear");    /*Limpa a tela em sistemas baseados em Unix*/
    #endif
}

int ctoi(char c){       /*Função que converte de char para int*/
    int x = c - '0';
    return x;
}

void preencher_matriz(int** matriz){
    int x, y;

    for (x = 0; x < 10; x++) {      /*Preenche toda a matriz com 0 (valor de cadeira disponivel)*/
        for (y = 0; y < 40; y++)
            matriz[x][y] = 0;
    }
}

int** alocar_matriz(int linhas, int colunas) {
    int** matriz;
    int i;

    /*Alocação de memoria para as linhas*/
    matriz = (int**)malloc(linhas * sizeof(int*));

    /*Alocação de memoria para as colunas*/
    for (i = 0; i < linhas; i++) {
        matriz[i] = (int*)malloc(colunas * sizeof(int));
    }

    return matriz;
}

bool strcknl(char* string){
    int i;

    for (i = 0; string[i]; i++){        /*Verifica se a string possui '\n'*/
        if(string[i] == '\n')
            return false;
    }
    return true;
}