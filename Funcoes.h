/******************************************************************************

Interface do módulo Funções
Autor: Miguel Angelo Gonzaga Marques, Mateus Augusto do Nascimento Affonso
Data: 15/07/2023

Descrição:  Esse módulo cobre apenas as funções auxiliares, isto é, funções em
            que seu código se repete várias vezes ao longo do main(), e por isso
            foram separadas em funções para tornar o código mais limpo.

*******************************************************************************/

#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/*=============================DEFINIÇÃO DAS FUNÇÕES=============================*/

void limpar_buffer(void);
void limpar_tela(void);
int ctoi(char c);
void preencher_matriz(int** matriz);
int** alocar_matriz(int linhas, int colunas);
bool strcknl(char* string);

#endif