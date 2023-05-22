/*
    Trabalho desenvolvido pelo discente Yuri Junqueira Tobias
    Data: 11/05/2023
    Docente: André Guedes
    Disciplina: Otimização - CI1238
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct producao_t{
    int p; // quantidade de produtos
    int c; // quantidade de compostos
    float *vendaP; // vetor com o valor de venda dos produtos
    float *precoC; // vetor com o preco de compra dos compostos
    float *limiteC; // vetor com o limite diario de consumo dos compostos
    float *lucro; // vetor com o lucro de cada produto
    float **quantidadeCP; // matriz com a quantidade de uso de composto por produto
} producao_t;

void leEntrada(producao_t *p) {
    int i, j;

    scanf("%d%d", &p->p, &p->c);

    p->vendaP = malloc(sizeof(float) * p->p);
    p->precoC = malloc(sizeof(float) * p->c);
    p->limiteC = malloc(sizeof(float) * p->c);
    p->lucro = malloc(sizeof(float) * p->p);

    p->quantidadeCP = malloc(sizeof(float*) * p->p);
    for(i = 0; i < p->p; i++)
        p->quantidadeCP[i] = malloc(sizeof(float) * p->c);

    for(i = 0; i < p->p; i++)
        scanf("%f", &p->vendaP[i]);

    for(i = 0; i < p->c; i++)
        scanf("%f%f", &p->precoC[i], &p->limiteC[i]);

    for(i = 0; i < p->p; i++) {
        for(j = 0; j < p->c; j++) {
            scanf("%f", &p->quantidadeCP[i][j]);
        }
    }
    
    return;
}

void calculaLucros(producao_t *p) {
    int i, j;
    float soma = 0;

    for(i = 0; i < p->p; i++) {
        for(j = 0; j < p->c; j++)
            soma = soma + (p->precoC[j] * p->quantidadeCP[i][j]);

        p->lucro[i] = p->vendaP[i] - soma;
        soma = 0;
    }

    return;
}

void imprimeLPSolve(producao_t *p) {
    int i, j;

    printf("max: ");
    for(i = 0; i < p->p; i++)
        printf("+ %.1fx%d ", p->lucro[i], (i+1));
    printf(";\n\n");

    for(i = 0; i < p->c; i++) {
        for(j = 0; j < p->p; j++) {
            printf("+ %.1fx%d ", p->quantidadeCP[j][i], (j+1));
        }
        printf("<= %.1f ;\n", p->limiteC[i]);
    }

    return;
}

void deletaProducao(producao_t *p) {
    free(p->vendaP);
    free(p->precoC);
    free(p->limiteC);
    free(p->lucro);
    for(int i = 0; i < p->p; i++)
        free(p->quantidadeCP[i]);
    free(p->quantidadeCP);
    free(p);

    return;
}

int main() {
    producao_t *p = malloc(sizeof(producao_t));

    leEntrada(p);
    calculaLucros(p);
    imprimeLPSolve(p);
    deletaProducao(p);

    return 0;
}