/******************************************************************************

Interface do módulo Estruturas
Autor: Miguel Angelo Gonzaga Marques, Mateus Augusto do Nascimento Affonso
Data: 15/07/2023

Descrição:  A estrutura INGRESSO armazena os valores de todos os tipos de
            ingressos que o cinema pode conter, sendo esses o total a ser
            comprado e suas subdivisões, como os sem desconto, com desconto
            de meia entrada, e com desconto do banco Itasil.

            As estruturas CODIGOS_ITASIL e CODIGOS_ESTUDANTE tem o mesmo
            propósito, que é criar uma lista encadeada para armazenar os códigos
            já utilizados de suas respectivas formas de desconto.

            A estrutura SALAS armazena a matriz que representa os lugares da sala,
            com os valores que representam se a sala foi ocupada ou não. E também
            armazena a quantidade de ingressos ainda disponiveis para cada sala.

*******************************************************************************/

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H


/*==============================ESTRUTURAS DE DADOS==============================*/

typedef struct _INGRESSO {
    unsigned int ingressos_totais;
    unsigned int ingressos_normais;
    unsigned int ingressos_meia;
    unsigned int ingressos_banco;
} INGRESSO;

typedef struct _CODIGOS_ITASIL {
    unsigned long int codigo_itasil;
    struct _CODIGOS_ITASIL* prox;
} CODIGOS_ITASIL;

typedef struct _CODIGOS_ESTUDANTE {
    unsigned long int codigo_estudante;
    struct _CODIGOS_ESTUDANTE* prox;
} CODIGOS_ESTUDANTE;

typedef struct _SALAS {
    int** sala1;
    int** sala2;
    int** sala3;
    int ingressos_restantes_sala1;
    int ingressos_restantes_sala2;
    int ingressos_restantes_sala3;
} SALAS;

#endif