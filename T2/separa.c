/*
    Trabalho desenvolvido pelo discente Yuri Junqueira Tobias
    Data: 09/06/2023
    Docente: André Guedes
    Disciplina: Otimização - CI1238
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct sets_t {
    int *i;
    int *j;
}sets_t;

typedef struct input_t {
    int hNum; // Number of heroes
    int cNum; // Number of conflicts
    int aNum; // Number of affinities
    sets_t *cPairs; // Set of conflict pairs
    sets_t *aPairs; // Set of affinity pairs
}input_t;

void allocateInput(input_t *inp) {
    inp->cPairs = malloc(sizeof(sets_t));
    inp->cPairs->i = malloc(sizeof(int)*inp->cNum);
    inp->cPairs->j = malloc(sizeof(int)*inp->cNum);
    inp->aPairs = malloc(sizeof(sets_t));
    inp->aPairs->i = malloc(sizeof(int)*inp->aNum);
    inp->aPairs->j = malloc(sizeof(int)*inp->aNum);
}

/* 
    Opções de entrada:
    • default: todos os cortes estão ativos e a função limitante é a criada por nós discentes;
    • "-a": usa a função limitante fornecida pelo(s) docente(s);
    • "-f": desliga os cortes de viabilidade;
    • "-o": desliga os cortes de otimalidade;
*/
void inputHandler(int argNum, char *args[], input_t *inp) {
    
    /* Invalid inputs */
    if (argNum > 4) {
        fprintf(stderr, "Too many arguments\nUsage: %s -a -f -o\nWhere '-a', '-f' and '-o' are optional\n", args[0]);
        exit(1);
    }

    for(int i = 1; i < argNum; i++) {
        if(args[i][1] != 97 && args[i][1] != 102 && args[i][1] != 111) {
            fprintf(stderr, "Invalid argument\nUsage: %s -a -f -o\nWhere '-a', '-f' and '-o' are optional\n", args[0]);
            exit(2);
        }
    }

    /* Read numbers */
    scanf("%d%d%d", &inp->hNum, &inp->cNum, &inp->aNum);

    allocateInput(inp);
    for(int i = 0; i < inp->cNum; i++) {
        scanf("%d%d", &inp->cPairs->i[i], &inp->cPairs->j[i]);
    }

    for(int i = 0; i < inp->aNum; i++) {
        scanf("%d%d", &inp->aPairs->i[i], &inp->aPairs->j[i]);
    }
}

void deleteInput(input_t *inp) {
    free(inp->cPairs->i);
    free(inp->cPairs->j);
    free(inp->aPairs->i);
    free(inp->aPairs->j);
    free(inp->cPairs);
    free(inp->aPairs);
    free(inp);
}

int main(int argc, char *argv[]) {
    input_t *input = malloc(sizeof(input_t));
    
    inputHandler(argc, argv, input);
    // Here hoes the whole logic
    // for(int i = 0; i < input->cNum; i++) {
    //     printf("%d,%d\n", input->cPairs->i[i], input->cPairs->j[i]);
    // }
    // for(int i = 0; i < input->aNum; i++) {
    //     printf("%d,%d\n", input->aPairs->i[i], input->aPairs->j[i]);
    // }
    deleteInput(input);

    return 0;
}