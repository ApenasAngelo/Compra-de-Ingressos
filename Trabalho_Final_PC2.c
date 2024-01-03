/******************************************************************************

Programa: Trabalho_Pratico_PC2.c
Autor: Miguel Angelo Gonzaga Marques, Mateus Augusto do Nascimento Affonso
Data: 15/07/2023
Descrição:  O sistema permite que os usuários comprem ingressos de filmes,
            selecionem poltronas desejadas e recebam informações sobre o
            valor a ser pago. O sistema aplica descontos para ingressos de
            meia-entrada para estudantes e para clientes do Banco Itasil.

*******************************************************************************/

#include "Funcoes.h"
#include "Estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*=============================DEFINIÇÃO DAS FUNÇÕES=============================*/

void exibir_mapa_sala(int** sala);
int selecionar_filme(void);
INGRESSO* solicitar_quantidade_ingressos(int** sala, int* ingressos_disponiveis_arg, CODIGOS_ITASIL** codigos_itasil_utilizados, CODIGOS_ESTUDANTE** codigos_estudante_utilizados);
bool aplicar_desconto_meia_entrada(CODIGOS_ESTUDANTE** codigos_utilizados);
int verificar_carteira_estudante(char* numero_carteira, CODIGOS_ESTUDANTE** codigos_utilizados);
void adicionar_codigo_estudante(int codigo, CODIGOS_ESTUDANTE** codigos_utilizados);
bool aplicar_desconto_cliente_itasil(CODIGOS_ITASIL** codigos_utilizados);
int verificar_codigo_itasil(int codigo, CODIGOS_ITASIL** codigos_utilizados);
void adicionar_codigo_itasil(int codigo, CODIGOS_ITASIL** codigos_utilizados);
int calcular_valor_total(INGRESSO* valores_ingressos);
void agradecer_pedido(int valor_total);
void marcar_poltronas(int filme, int** sala, INGRESSO** structIngresso);

/*==================================FUNÇÃO MAIN==================================*/

int main(void){
    int filme, valor_total;
    char reiniciar;
    CODIGOS_ITASIL* codigos_itasil_utilizados_sala_1 = NULL;
    CODIGOS_ITASIL* codigos_itasil_utilizados_sala_2 = NULL;
    CODIGOS_ITASIL* codigos_itasil_utilizados_sala_3 = NULL;
    CODIGOS_ESTUDANTE* codigos_estudante_utilizados_sala_1 = NULL;
    CODIGOS_ESTUDANTE* codigos_estudante_utilizados_sala_2 = NULL;
    CODIGOS_ESTUDANTE* codigos_estudante_utilizados_sala_3 = NULL;
    SALAS* salas = (SALAS*)malloc(sizeof(SALAS));
    INGRESSO* ingressos;

    salas->ingressos_restantes_sala1 = 400;
    salas->ingressos_restantes_sala2 = 400;
    salas->ingressos_restantes_sala3 = 400;

    salas->sala1 = alocar_matriz(40, 10);
    salas->sala2 = alocar_matriz(40, 10);
    salas->sala3 = alocar_matriz(40, 10);

    preencher_matriz(salas->sala1);
    preencher_matriz(salas->sala2);
    preencher_matriz(salas->sala3);

    while(true){
        limpar_tela();
        filme = selecionar_filme();

        if (filme == 0)
            break;

        switch (filme){
            case 1:
                ingressos = solicitar_quantidade_ingressos(salas->sala1, &salas->ingressos_restantes_sala1, &codigos_itasil_utilizados_sala_1, &codigos_estudante_utilizados_sala_1);
                marcar_poltronas(filme, salas->sala1, &ingressos);
                break;
            case 2:
                ingressos = solicitar_quantidade_ingressos(salas->sala2, &salas->ingressos_restantes_sala2, &codigos_itasil_utilizados_sala_2, &codigos_estudante_utilizados_sala_2);
                marcar_poltronas(filme, salas->sala2, &ingressos);
                break;
            case 3:
                ingressos = solicitar_quantidade_ingressos(salas->sala3, &salas->ingressos_restantes_sala3, &codigos_itasil_utilizados_sala_3, &codigos_estudante_utilizados_sala_3);
                marcar_poltronas(filme, salas->sala3, &ingressos);
                break;
        }

        valor_total = calcular_valor_total(ingressos);

        agradecer_pedido(valor_total);

        printf("\nDeseja realizar um novo pedido? (S/N): ");
        reiniciar = getchar();
        limpar_buffer();

        if (reiniciar != 'S' && reiniciar != 's')
            break;
    }

    return 0;
}

/*====================================FUNÇÕES====================================*/

void exibir_mapa_sala(int** sala){
    int i, j;

    limpar_tela();
    printf("Mapa da Sala:\n");
    printf("==========================================\n");
    printf("___| A   B   C   D   E   F   G   H   I   J\n");

    for (i = 0; i < 40; i++) {      /*Loop para exibir as linhas*/
        printf("%02d | ", i+1);

        for (j = 0; j < 10; j++) {  /*Loop para exibir as colunas*/

            if (sala[i][j] == 1)    /*Verifica se o lugar esta ocupado*/
                printf("X ");
            else
                printf("0 ");
            
            if (j < 9)
                printf("- ");
        }
        printf("\n");
    }
    printf("===================TELA===================\n");
    printf("\n");
}

int selecionar_filme(void){
    int filme;

    while (true){
        limpar_tela();

        printf("=== Selecione o Filme ===\n");
        printf("1 - Velozes e Furiosos 137\n");
        printf("2 - The Flash\n");
        printf("3 - Transformers\n");
        printf("0 - Encerrar\n");

        printf("Digite o numero da sala do filme desejado (1, 2, 3 ou 0): ");
        scanf("%d", &filme);
        limpar_buffer();

        if (filme >= 0 && filme <= 3)       /*Verifica se o número do filme é válido*/
            return filme;
        else {
            printf("Filme invalido! Digite um numero valido.\n");

        }
    }
}

INGRESSO* solicitar_quantidade_ingressos(int** sala, int* ingressos_disponiveis_arg, CODIGOS_ITASIL** codigos_itasil_utilizados, CODIGOS_ESTUDANTE** codigos_estudante_utilizados){
    INGRESSO* ingressos = (INGRESSO*)malloc(sizeof(INGRESSO));
    unsigned int ingressos_totais = 0;
    unsigned int ingressos_normais = 0;
    unsigned int ingressos_meia = -1;
    unsigned int ingressos_banco = -1;
    int i = 0, invalid = false, ingressos_disponiveis = *ingressos_disponiveis_arg;

    /*Solicitação da quantidade total de ingressos*/
    limpar_tela();
    exibir_mapa_sala(sala);
    while (!(ingressos_totais > 0 && ingressos_totais <= ingressos_disponiveis)){
        if (invalid){
            printf("Quantidade invalida! Digite um numero valido entre 1 e %d.\n", ingressos_disponiveis);
            invalid = false;
        }

        printf("Digite a quantidade de ingressos desejada: ");
        scanf("%u", &ingressos_totais);
        limpar_buffer();

        if (!(ingressos_totais > 0 && ingressos_totais <= ingressos_disponiveis))
            invalid = true;
    }
    ingressos_normais = ingressos_totais;

    /*Solicitação da quantidade de meia-entradas*/
    limpar_tela();
    while (!(ingressos_meia >= 0 && ingressos_meia <= ingressos_normais)){
        if (invalid){
            printf("Quantidade invalida! Digite um numero valido entre 0 e %d.\n", ingressos_normais);
            invalid = false;
        }

        printf("Digite a quantidade de ingressos com meia-entrada desejada: ");
        scanf("%u", &ingressos_meia);
        limpar_buffer();

        if (!(ingressos_meia >= 0 && ingressos_meia <= ingressos_normais))
            invalid = true;
    }

    /*Verificação dos ingressos de meia entrada*/
    for (i = 0; i < ingressos_meia; i++){
        if (!aplicar_desconto_meia_entrada(codigos_estudante_utilizados))
            ingressos_meia -= 1;
    }

    ingressos_normais -= ingressos_meia;

    /*Solicitação da quantidade de descontos do banco Itasil*/
    limpar_tela();
    while (!(ingressos_banco >= 0 && ingressos_banco <= ingressos_normais)){
        if (invalid){
            printf("Quantidade invalida! Digite um numero valido entre 0 e %d.\n", ingressos_normais);
            invalid = false;
        }

        printf("Digite a quantidade de ingressos com desconto Itasil desejada: ");
        scanf("%u", &ingressos_banco);
        limpar_buffer();

        if (!(ingressos_banco >= 0 && ingressos_banco <= ingressos_normais))
            invalid = true;
    }

    /*Verificação dos ingressos de clientes do banco Itasil*/
    for (i = 0; i < ingressos_banco; i++){
        if (!aplicar_desconto_cliente_itasil(codigos_itasil_utilizados))
            ingressos_banco -= 1;
    }

    *ingressos_disponiveis_arg -= ingressos_totais;

    ingressos_normais -= ingressos_banco;

    ingressos->ingressos_totais = ingressos_totais;
    ingressos->ingressos_normais = ingressos_normais;
    ingressos->ingressos_meia = ingressos_meia;
    ingressos->ingressos_banco = ingressos_banco;

    return ingressos;
}

bool aplicar_desconto_meia_entrada(CODIGOS_ESTUDANTE** codigos_utilizados){
    char numero_carteira[10];
    int verificacao, i;
    int invalid = 0;

    while (true){
        for(i = 0; i < 10; i++)
            numero_carteira[i] = '0';
        
        limpar_tela();

        switch(invalid){
            case 1:
                printf("Codigo invalido! Digite um codigo valido ou 0 para cancelar o desconto.\n");
                invalid = 0;
                break;
            
            case -1:
                printf("Codigo ja utilizado! Digite um codigo valido ou 0 para cancelar o desconto.\n");
                invalid = 0;
                break;
        }
        printf("Digite o numero da carteira de estudante (ou 0 para cancelar o desconto): ");
        fgets(numero_carteira, sizeof(numero_carteira), stdin);

        if (strcknl(numero_carteira))
            limpar_buffer();

        if (strlen(numero_carteira) == 2 && numero_carteira[0] == '0')      /*Verifica se o usuário digitou apenas 0*/
            return false;

        verificacao = (verificar_carteira_estudante(numero_carteira, codigos_utilizados));
        switch(verificacao){
            case 1:
                return true;        /*Retorna verdadeiro se o número da carteira é válido*/

            case 0:
                invalid = 1;        /*Marca como código inválido*/
                break;

            case -1:
                invalid = -1;       /*Marca como código já utilizado*/
                break;
        }
    }
}

int verificar_carteira_estudante(char* numero_carteira, CODIGOS_ESTUDANTE** codigos_utilizados){
    int numero_carteira_int[5], codigo, i, soma = 0;
    int achados = 0;
    CODIGOS_ESTUDANTE* pt_aux = *codigos_utilizados;

    if (strlen(numero_carteira) != 6)
        return false;
    
    for (i = 0; i < 5; i++){
        numero_carteira_int[i] = ctoi(numero_carteira[i]);      /*Converte cada dígito do número da carteira para um inteiro*/ 
        if (i != 4)
            soma += numero_carteira_int[i];                     /*Calcula a soma dos primeiros 4 dígitos*/
    }
    codigo = atoi(numero_carteira);

    while (pt_aux != NULL){                                     /*Verifica se o código já foi achado*/
        if (codigo == pt_aux->codigo_estudante)
            achados++;

        pt_aux = pt_aux->prox;
    }
    if (achados > 0)
        return -1;

    soma = soma % 10;                                           /*Calcula o dígito verificador da carteira*/
    if (soma == numero_carteira_int[4]){
        adicionar_codigo_estudante(codigo, codigos_utilizados);
        return 1;
    } else
        return 0;
}

void adicionar_codigo_estudante(int codigo, CODIGOS_ESTUDANTE** codigos_utilizados) {
    CODIGOS_ESTUDANTE *aux;
    CODIGOS_ESTUDANTE *novo = (CODIGOS_ESTUDANTE *)malloc(sizeof(CODIGOS_ESTUDANTE));
       
    if (novo){
        novo->codigo_estudante = codigo;        /*Atribui o código da carteira à estrutura*/
        novo->prox = NULL;

        if (*codigos_utilizados == NULL){
            *codigos_utilizados = novo;         /*Inicia a estrutura com o primeiro código*/
        } else {
            aux = *codigos_utilizados;
            while(aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = novo;                   /*Adiciona a nova estrutura ao final da lista de códigos utilizados*/
        }    
    } else {
        printf("Erro ao alocar memoria!\n");
    }       
}

bool aplicar_desconto_cliente_itasil(CODIGOS_ITASIL** codigos_utilizados){
    unsigned long int codigo_itasil;
    char codigo_itasil_string[11];
    int invalid = 0, verificacao, i;

    while(true) {
        limpar_tela();
        codigo_itasil = 1;

        switch(invalid){
            case 1:
                printf("Codigo invalido! Digite um codigo valido ou 0 para cancelar o desconto.\n");
                invalid = 0;
                break;
            
            case -1:
                printf("Codigo ja utilizado! Digite um codigo valido ou 0 para cancelar o desconto.\n");
                invalid = 0;
                break;
        }

        printf("Digite o codigo de cliente Itasil (ou 0 para cancelar o desconto): ");

        fgets(codigo_itasil_string, sizeof(codigo_itasil_string), stdin);           /*Le o código digitado pelo usuário como string*/
        if (strcknl(codigo_itasil_string)) 
            limpar_buffer();

        codigo_itasil_string[strcspn(codigo_itasil_string, "\n")] = '\0';

        for (i = 0; codigo_itasil_string[i]; i++){
            if(!(57 >= codigo_itasil_string[i] && codigo_itasil_string[i] >= 48))   /*Verifica se algum caracter é uma letra*/
                invalid = 1;
        }

        switch (invalid){
            case 1:
                verificacao = 0;
                break;

            case 0:
                if (strlen(codigo_itasil_string) != 0)              /*Converte o código Itasil para um número inteiro caso a string não seja vazia*/
                    codigo_itasil = atoi(codigo_itasil_string);
        
                if (codigo_itasil == 0)
                    return false;

                verificacao = verificar_codigo_itasil(codigo_itasil, codigos_utilizados);
                break;
        }

        switch(verificacao){
            case 1:
                return true;        /*Retorna verdadeiro se o código Itasil é válido*/

            case 0:
                invalid = 1;        /*Marca como código inválido*/
                break;

            case -1:
                invalid = -1;       /*Marca como código já utilizado*/
                break;
        }
    }
}

int verificar_codigo_itasil(int codigo, CODIGOS_ITASIL** codigos_utilizados){
    int achados = 0;
    CODIGOS_ITASIL* pt_aux = *codigos_utilizados;

    while (pt_aux != NULL){
        if (codigo == pt_aux->codigo_itasil)
            achados++;

        pt_aux = pt_aux->prox;
    }
    if (achados > 0) {
        return -1;
    } else if (codigo % 341 == 0 && codigo % 1 == 0){
        adicionar_codigo_itasil(codigo, codigos_utilizados);
        return 1;
    } else {
        return 0;
    }
}

void adicionar_codigo_itasil(int codigo, CODIGOS_ITASIL** codigos_utilizados) {
    CODIGOS_ITASIL *aux;
    CODIGOS_ITASIL *novo = (CODIGOS_ITASIL *)malloc(sizeof(CODIGOS_ITASIL));
       
    if (novo){
        novo->codigo_itasil = codigo;           /*Atribui o código Itasil à estrutura*/
        novo->prox = NULL;

        if (*codigos_utilizados == NULL){
            *codigos_utilizados = novo;         /*Inicia a estrutura com o primeiro código*/
        } else {
            aux = *codigos_utilizados;
            while(aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = novo;                   /*Adiciona a nova estrutura ao final da lista de códigos utilizados*/
        }    
    } else {
        printf("Erro ao alocar memoria!\n");
    }       
}

int calcular_valor_total(INGRESSO* valores_ingressos){
    int valor_total;

    valor_total = valores_ingressos->ingressos_normais * 20;

    if (valores_ingressos->ingressos_meia > 0)
        valor_total += valores_ingressos->ingressos_meia * 10;

    if (valores_ingressos->ingressos_banco > 0)
        valor_total += valores_ingressos->ingressos_banco * 14;
    
    return valor_total;
}

void agradecer_pedido(int valor_total){
    limpar_tela();
    printf("=== Pedido Realizado ===\n");
    printf("Valor Total: R$ %d.00\n", valor_total);
    printf("Obrigado por comprar conosco!\n");
}

void marcar_poltronas(int filme, int** sala, INGRESSO** structIngresso){
    INGRESSO* ingressos = *structIngresso;
    int i, fileira, coluna, quantidade_ingressos = ingressos->ingressos_totais;
    char poltrona[10], fileira_string[3];
    bool loop = true;

    for (i = 0; i< quantidade_ingressos; i++){
        limpar_tela();
        exibir_mapa_sala(sala);
        printf("Ingresso %d/%d", i+1, quantidade_ingressos);

        loop = true;
        while(loop){
            fileira = 0;
            coluna = 0;

            printf("\nSelecione a poltrona para o ingresso (01-40 A-J): ");
            fgets(poltrona, sizeof(poltrona), stdin);
            if (strcknl(poltrona))
            limpar_buffer();

            fileira_string[0] = poltrona[0];
            fileira_string[1] = poltrona[1];
            fileira_string[2] = '\n';

            fileira = (atoi(fileira_string)) - 1;       /*Converte a fileira para um número inteiro*/
            coluna = (int)poltrona[3];                  /*Converte a coluna para um número inteiro*/

            if ((65 <= coluna && coluna <= 74))         /*Transforma o código ASCII da letra em case insensitive*/
                coluna -= 65;
            else if ((97 <= coluna && coluna <= 106))
                coluna -= 97;

            if (0 <= fileira && fileira < 40 && 0 <= coluna && coluna < 10 && poltrona[2] == ' ' && strlen(poltrona) == 5){
                if (sala[fileira][coluna] == 0){
                    sala[fileira][coluna] = 1;
                    loop = false;
                } else
                    printf("Poltrona ja ocupada! Selecione outra poltrona.");
            } else
                printf("Poltrona invalida! Selecione uma poltrona valida.");
        }
    }
}