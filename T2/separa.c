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

/*
    Opções de entrada:
    • l: herói da vez;
    • x: quantidade "ótima" de conflitos;
    • numH: número total de heróis;
    • group[]: vetor com os heróis do grupo com o 1 herói;
    • groupOpt[]: vetor com os heróis do grupo com o 1 herói do caso "ótimo"
    • inp: input que contém os vetores de afinidades e conflitos
*/
void backtracking(int l, int *x, int numH, int group[], int groupOpt[], input_t *inp) {
    int valido = 0;
    int conflitos = 0;

    if(l == numH) {
        // !Tem algum grupo vazio?
        for(int i = 0; i < numH - 1; i++) {
            if(group[i] != group[i + 1]) {
                valido = 1; // Valido == 1 quer dizer que existem herois nos dois grupos
            }
        }

        // Se tem heroi nos dois grupos entra, senão segue o baile
        if(valido) {
            // !Todos os que possuem afinidade estão no mesmo grupo? 
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->aNum; j++)
                    if((inp->aPairs->i[j] == (i + 1)) && group[(inp->aPairs->j[j]) - 1] != group[i])
                        valido = 0; // Valido == 0 quer dizer que existem herois com afinidade em grupos diferentes
        }

        // Se passou pelo for acima e continuou com válido == 1 então é valido
        if(valido) {
            // Calcula a quantidade de conflitos presente no grupo
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->cNum; j++)
                    if((inp->cPairs->i[j] == (i + 1)) && group[(inp->cPairs->j[j]) - 1] == group[i])
                        conflitos++;

            // Verifica se a solução é melhor ou nem
            int cont = 0;
            if(conflitos < *x || *x == -1) {
                *x = conflitos;
                groupOpt[0] = group[0];
                cont++;
                for(int i = 1; i < numH; i++)
                     if(group[i] == group[0]) {
                        groupOpt[cont] = i+1;
                        cont++;
                     }
            }
        }
    } else {
        group[l] = 1;
        backtracking(l + 1, x, numH, group, groupOpt, inp);
        group[l] = 0;
        backtracking(l + 1, x, numH, group, groupOpt, inp);
    }
}

/*
    Opções de entrada:
    • l: herói da vez;
    • x: quantidade "ótima" de conflitos;
    • numH: número total de heróis;
    • group[]: vetor com os heróis do grupo com o 1 herói;
    • groupOpt[]: vetor com os heróis do grupo com o 1 herói do caso "ótimo"
    • inp: input que contém os vetores de afinidades e conflitos
*/
void backtrackingViabilidade(int l, int *x, int numH, int group[], int groupOpt[], input_t *inp) {
    int valido = 0;
    int conflitos = 0;
    int caminho = -1;
    int restricao = 0;

    if(l == numH) {
        // !Tem algum grupo vazio?
        for(int i = 0; i < numH - 1; i++) {
            if(group[i] != group[i + 1]) {
                valido = 1; // Valido == 1 quer dizer que existem herois nos dois grupos
            }
        }

        if(valido) {
            for(int i = 0; i < numH; i++) {
                printf("[%d] ", group[i]);
            }
            printf("\n");

            // Calcula a quantidade de conflitos presente no grupo
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->cNum; j++)
                    if((inp->cPairs->i[j] == (i + 1)) && group[(inp->cPairs->j[j]) - 1] == group[i])
                        conflitos++;

            // Verifica se a solução é melhor ou nem
            int cont = 0;
            if(conflitos < *x || *x == -1) {
                *x = conflitos;
                groupOpt[0] = group[0];
                cont++;
                for(int i = 1; i < numH; i++) {
                    if(group[i] == group[0]) {
                        groupOpt[cont] = i+1;
                        cont++;
                    }
                }
            }
        }
    } else {
        caminho = -1;
        for (int j = 0; j < inp->aNum; j++) {
            if((inp->aPairs->i[j] == l+1) && (inp->aPairs->j[j] < l+1)) {
                caminho = group[inp->aPairs->j[j] - 1];
                restricao = 1;
            } else if((inp->aPairs->j[j] == l+1) && (inp->aPairs->i[j] < l+1)) {
                caminho = group[inp->aPairs->i[j] - 1];
                restricao = 1;
            }
        }
        

        if(restricao) {
            group[l] = caminho;
            backtrackingViabilidade(l + 1, x, numH, group, groupOpt, inp);
        } else {
            group[l] = 1;
            backtrackingViabilidade(l + 1, x, numH, group, groupOpt, inp);
            group[l] = 0;
            backtrackingViabilidade(l + 1, x, numH, group, groupOpt, inp);
        }
    }
}

void calculaTriangulo(int round, int first, int daVez, int *cC, input_t *inp) {    
    if(round == 3) {
        for(int i = 0; i < inp->cNum; i++) {
            if(inp->cPairs->i[i] == daVez && inp->cPairs->j[i] == first) {
                *cC = *cC + 1;
                return;
            } else if(inp->cPairs->j[i] == daVez && inp->cPairs->i[i] == first) {
                *cC = *cC + 1;
                return;
            }
        }
    } else {
        for(int i = 0; i < inp->cNum; i++) {
            if(inp->cPairs->i[i] == daVez && inp->cPairs->j[i] > daVez)
                calculaTriangulo(round + 1, first, inp->cPairs->j[i], cC, inp);
            else if(inp->cPairs->j[i] == daVez && inp->cPairs->i[i] > daVez)
                calculaTriangulo(round + 1, first, inp->cPairs->i[i], cC, inp);
        }
    }
}

/*
    Opções de entrada:
    • l: herói da vez;
    • x: quantidade "ótima" de conflitos;
    • numH: número total de heróis;
    • group[]: vetor com os heróis do grupo com o 1 herói;
    • groupOpt[]: vetor com os heróis do grupo com o 1 herói do caso "ótimo"
    • inp: input que contém os vetores de afinidades e conflitos
*/
void backtrackingOtimalidade(int l, int *x, int numH, int group[], int groupOpt[], input_t *inp) {
    int valido = 0;
    int conflitos = 0;

    if(l == numH) {
        // !Tem algum grupo vazio?
        // for(int i = 0; i < numH - 1; i++) {
        //     if(group[i] != group[i + 1]) {
        //         valido = 1; // Valido == 1 quer dizer que existem herois nos dois grupos
        //     }
        // }

        // Se tem heroi nos dois grupos entra, senão segue o baile
        // if(valido) {
        //     // !Todos os que possuem afinidade estão no mesmo grupo? 
        //     for(int i = 0; i < numH; i++)
        //         for(int j = 0; j < inp->aNum; j++)
        //             if((inp->aPairs->i[j] == (i + 1)) && group[(inp->aPairs->j[j]) - 1] != group[i])
        //                 valido = 0; // Valido == 0 quer dizer que existem herois com afinidade em grupos diferentes
        // }

        // Se passou pelo for acima e continuou com válido == 1 então é valido
        // if(valido) {
            // for(int i = 0; i < numH; i++) {
            //     printf("[%d] ", group[i]);
            // }
            // printf("\n");

            // Calcula a quantidade de conflitos presente no grupo
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->cNum; j++)
                    if((inp->cPairs->i[j] == (i + 1)) && group[(inp->cPairs->j[j]) - 1] == group[i])
                        conflitos++;

            // Verifica se a solução é melhor ou nem
            int cont = 0;
            if(conflitos < *x || *x == -1) {
                *x = conflitos;
                groupOpt[0] = group[0];
                cont++;
                for(int i = 1; i < numH; i++)
                     if(group[i] == group[0]) {
                        groupOpt[cont] = i+1;
                        cont++;
                     }
            }

            // printf("Entrei\n");
            // for(int i = 0; i < l; i++) {
            //     printf("[%d] ", group[i]);
            // }
            // printf("\n");
            // printf("O número de conflitos eh: %d\n", calculaTriangulo(l, group, inp));

            // printf("Conflitos: %d\n", conflitos);
        // }
    } else {
        if(l == 3) {
            printf("Entrei\n");
            for(int i = 0; i < l; i++) {
                printf("[%d] ", group[i]);
            }
            printf("\n");
            int conf = 0;
            // Calcular os conflitos até então
            for(int i = 0; i < l; i++)
                for(int j = 0; j < inp->cNum; j++) 
                    if((inp->cPairs->i[j] == (i+1)) && (inp->cPairs->j[j] < (l+1)) && (group[i] == group[inp->cPairs->j[j] - 1]))
                        conf++;
            printf("Conf antes %d\n", conf);

            for(int i = l; i < inp->hNum; i++)
                calculaTriangulo(1, i, i, &conf, inp);

            printf("O número de conflitos depois eh: %d\n", conf);
        }
        group[l] = 1;
        backtrackingOtimalidade(l + 1, x, numH, group, groupOpt, inp);
        group[l] = 0;
        backtrackingOtimalidade(l + 1, x, numH, group, groupOpt, inp);
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
    int escolhas[100], x = -1, escolhasOpt[100];

    inputHandler(argc, argv, input);

    //backtracking(0, &x, input->hNum, escolhas, escolhasOpt, input);
    //backtrackingViabilidade(0, &x, input->hNum, escolhas, escolhasOpt, input);
    backtrackingOtimalidade(0, &x, input->hNum, escolhas, escolhasOpt, input);

    printf("x: %d\n", x);
    for(int i = 0; i < input->hNum; i++) {
        printf("%d ", escolhasOpt[i]);
        if(escolhasOpt[i + 1] == 0)
            i = input->hNum;
    }
    printf("\n");
    
    // X é o grupo ótimo
    // minC(X, l, n) { 
    // l => altura da arvore
    // n => quantidade de herois
    // if l == n
    //     {   
    //         // Afinidades:
    //         // !Todos os que possuem afinidade estão no mesmo grupo? 
    //         // !Tem algum grupo vazio?
    //             {
    //                 // Otimalidade
    //                 NumCAtual < NumCOpt
    //                     // Deu boa, faz a troca
    //                     // Salvar OptX 
    //             }
    //     }
    // else 
    //     {
    //         // Coloca no Grupo A
    //         // Não coloca no Grupo A
    //     }
    // }
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

// Calcular triângulos
    // for(int i = l; i < inp->hNum - 2; i++) {
    //     printf("Entrei no triângulo\n");
    //     for(int j = 0; j < inp->cNum; j++) {
    //         // Verifica se o cara tem conflito com alguém
    //         if(((inp->cPairs->i[j] == (i+1)) && (inp->cPairs->j[j] > (i+1))) || ((inp->cPairs->j[j] == (i+1)) && (inp->cPairs->i[j] > (i+1)))) { 
    //             for(int k = (j + 1); k < inp->cNum; k++) {
    //                 // Verifica se o cara tem conflito com mais alguém
    //                 if(((inp->cPairs->i[k] == (i+1)) && (inp->cPairs->j[k] > (i+1))) || ((inp->cPairs->j[k] == (i+1)) && (inp->cPairs->i[k] > (i+1)))) { 
    //                     for(int n = 0; n < inp->cNum; n++) {
    //                         // Verifica se os dois caras com quem o primeiro cara tem conflito possuem conflito entre si
    //                         // 1° Caso: ambos elementos em j
    //                         if(((((inp->cPairs->i[n]) == (inp->cPairs->j[j])) && (inp->cPairs->i[n] != (i+1))) 
    //                         && (((inp->cPairs->j[n]) == (inp->cPairs->j[k])) && (inp->cPairs->j[n] != (i+1))))
    //                         || (((((inp->cPairs->i[n]) == (inp->cPairs->j[k])) && (inp->cPairs->i[n] != (i+1))) 
    //                         && (((inp->cPairs->j[n]) == (inp->cPairs->j[j])) && (inp->cPairs->j[n] != (i+1)))))) {
    //                             conf++;
    //                         } 
    //                         // 2° Caso: ambos elementos em i
    //                         else if((((inp->cPairs->i[n]) == (inp->cPairs->i[j])) && (inp->cPairs->i[n] != (i+1))) && (((inp->cPairs->j[n]) == (inp->cPairs->i[k])) && (inp->cPairs->j[n] != (i+1)))) {
    //                             conf++;
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // printf("Conf depois %d\n", conf);

    // return conf;