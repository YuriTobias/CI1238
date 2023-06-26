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

void calculaFigura(int round, int roundMax, int first, int daVez, int *cC, input_t *inp) {
    if(round == roundMax) {
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
            if(inp->cPairs->i[i] == daVez && inp->cPairs->j[i] > daVez) {
                (roundMax == 5) ? calculaFigura(round + 1, 5, first, inp->cPairs->j[i], cC, inp) : calculaFigura(round + 1, 3, first, inp->cPairs->j[i], cC, inp);
            }
                
            else if(inp->cPairs->j[i] == daVez && inp->cPairs->i[i] > daVez)
                (roundMax == 5) ? calculaFigura(round + 1, 5, first, inp->cPairs->i[i], cC, inp) : calculaFigura(round + 1, 3, first, inp->cPairs->i[i], cC, inp);
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
void backtracking(int l, int *x, int numH, int group[], int groupOpt[], input_t *inp, int *qNodo) {
    int valido = 0;
    int conflitos = 0;
    *qNodo = *qNodo + 1;

    if(l == numH) {
        // !Tem algum grupo vazio?
        for(int i = 0; i < numH - 1; i++) {
            if(group[i] != group[i + 1]) {
                valido = 1; // Valido == 1 quer dizer que existem herois nos dois grupos
            }
        }

        // Se passou pelo for acima e continuou com válido == 1 então é valido
        if(valido) {
            // !Todos os que possuem afinidade estão no mesmo grupo? 
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->aNum; j++)
                    if(((inp->aPairs->i[j] == (i + 1)) && group[(inp->aPairs->j[j]) - 1] != group[i]) || ((inp->aPairs->j[j] == (i + 1)) && group[(inp->aPairs->i[j]) - 1] != group[i]))
                        return;

            // Calcula a quantidade de conflitos presente no grupo
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->cNum; j++)
                    if(((inp->cPairs->i[j] == (i + 1)) && (group[(inp->cPairs->j[j]) - 1] == group[i]) && (inp->cPairs->j[j] > (i + 1))) 
                    || ((inp->cPairs->j[j] == (i + 1)) && (group[(inp->cPairs->i[j]) - 1] == group[i]) && (inp->cPairs->i[j] > (i + 1))))
                        conflitos++;

            // Verifica se a solução é melhor ou nem
            int cont = 0;
            if(conflitos < *x || *x == -1) {
                for(int i = 0; i < numH; i++)
                    groupOpt[i] = 0;

                *x = conflitos;
                groupOpt[0] = 1;
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
        group[l] = 1;
        backtracking(l + 1, x, numH, group, groupOpt, inp, qNodo);
        group[l] = 0;
        backtracking(l + 1, x, numH, group, groupOpt, inp, qNodo);
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
void backtrackingViabilidade(int l, int *x, int numH, int group[], int groupOpt[], input_t *inp, int *qNodo) {
    int valido = 0;
    int conflitos = 0;
    int caminho = -1;
    int restricao = 0;
    *qNodo = *qNodo + 1;

    if(l == numH) {
        // !Tem algum grupo vazio?
        for(int i = 0; i < numH - 1; i++) {
            if(group[i] != group[i + 1]) {
                valido = 1; // Valido == 1 quer dizer que existem herois nos dois grupos
            }
        }

        if(valido) {
            // Calcula a quantidade de conflitos presente no grupo
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->cNum; j++)
                    if(((inp->cPairs->i[j] == (i + 1)) && (group[(inp->cPairs->j[j]) - 1] == group[i]) && (inp->cPairs->j[j] > (i + 1))) 
                    || ((inp->cPairs->j[j] == (i + 1)) && (group[(inp->cPairs->i[j]) - 1] == group[i]) && (inp->cPairs->i[j] > (i + 1))))
                        conflitos++;

            // Verifica se a solução é melhor ou nem
            int cont = 0;
            if(conflitos < *x || *x == -1) {
                for(int i = 0; i < numH; i++)
                    groupOpt[i] = 0;

                *x = conflitos;
                groupOpt[0] = 1;
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
            backtrackingViabilidade(l + 1, x, numH, group, groupOpt, inp, qNodo);
        } else {
            group[l] = 1;
            backtrackingViabilidade(l + 1, x, numH, group, groupOpt, inp, qNodo);
            group[l] = 0;
            backtrackingViabilidade(l + 1, x, numH, group, groupOpt, inp, qNodo);
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
void backtrackingOtimalidade(int l, int *x, int numH, int group[], int groupOpt[], input_t *inp, int *qNodo) {
    int valido = 0;
    int conflitos = 0;
    *qNodo = *qNodo + 1;

    if(l == numH) {
        // !Tem algum grupo vazio?
        for(int i = 0; i < numH - 1; i++) {
            if(group[i] != group[i + 1]) {
                valido = 1; // Valido == 1 quer dizer que existem herois nos dois grupos
            }
        }

        //Se passou pelo for acima e continuou com válido == 1 então é valido
        if(valido) {
            // !Todos os que possuem afinidade estão no mesmo grupo? 
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->aNum; j++)
                    if(((inp->aPairs->i[j] == (i + 1)) && group[(inp->aPairs->j[j]) - 1] != group[i]) || ((inp->aPairs->j[j] == (i + 1)) && group[(inp->aPairs->i[j]) - 1] != group[i]))
                        return;  

            // Calcula a quantidade de conflitos presente no grupo
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->cNum; j++)
                    if(((inp->cPairs->i[j] == (i + 1)) && (group[(inp->cPairs->j[j]) - 1] == group[i]) && (inp->cPairs->j[j] > (i + 1))) 
                    || ((inp->cPairs->j[j] == (i + 1)) && (group[(inp->cPairs->i[j]) - 1] == group[i]) && (inp->cPairs->i[j] > (i + 1))))
                        conflitos++;

            // Verifica se a solução é melhor ou nem
            int cont = 0;
            if(conflitos < *x || *x == -1) {
                for(int i = 0; i < numH; i++)
                    groupOpt[i] = 0;

                *x = conflitos;
                groupOpt[0] = 1;
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
        for(int i = 0; i < l; i++)
            for(int j = 0; j < inp->cNum; j++) 
                if((((inp->cPairs->i[j] == (i+1)) && (inp->cPairs->j[j] < (l+1)) && (group[i] == group[inp->cPairs->j[j] - 1])) 
                || ((inp->cPairs->j[j] == (i+1)) && (inp->cPairs->i[j] < (l+1)) && (group[i] == group[inp->cPairs->i[j] - 1]))))
                    conflitos++;

        for(int i = l; i < inp->hNum; i++) {
            calculaFigura(1, 5, i, i, &conflitos, inp);
            calculaFigura(1, 3, i, i, &conflitos, inp);
        }

        if(conflitos > *x && *x != -1)
            return;
            
        group[l] = 1;
        backtrackingOtimalidade(l + 1, x, numH, group, groupOpt, inp, qNodo);
        group[l] = 0;
        backtrackingOtimalidade(l + 1, x, numH, group, groupOpt, inp, qNodo);
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
    • a == 0 -> default (usa a nossa), o == 0 -> sem corte de otimalidade e f == 0 -> sem corte de viabilidade
*/
void separaGrupos(int l, int *x, int numH, int group[], int groupOpt[], input_t *inp, int *qNodo, int a, int o, int f) {
    int valido = 0, conflitos = 0, caminho = -1, restricao = 0;
    *qNodo = *qNodo + 1;

    if(l == numH) {
        // !Tem algum grupo vazio?
        for(int i = 0; i < numH - 1; i++) {
            if(group[i] != group[i + 1]) {
                valido = 1; // Valido == 1 quer dizer que existem herois nos dois grupos
            }
        }

        // Se passou pelo for acima e continuou com válido == 1 então é valido
        if(valido) {
            // !Todos os que possuem afinidade estão no mesmo grupo? 
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->aNum; j++)
                    if(((inp->aPairs->i[j] == (i + 1)) && group[(inp->aPairs->j[j]) - 1] != group[i]) || ((inp->aPairs->j[j] == (i + 1)) && group[(inp->aPairs->i[j]) - 1] != group[i]))
                        return;

            // Calcula a quantidade de conflitos presente no grupo
            for(int i = 0; i < numH; i++)
                for(int j = 0; j < inp->cNum; j++)
                    if(((inp->cPairs->i[j] == (i + 1)) && (group[(inp->cPairs->j[j]) - 1] == group[i]) && (inp->cPairs->j[j] > (i + 1))) 
                    || ((inp->cPairs->j[j] == (i + 1)) && (group[(inp->cPairs->i[j]) - 1] == group[i]) && (inp->cPairs->i[j] > (i + 1))))
                        conflitos++;

            // Verifica se a solução é melhor ou nem
            int cont = 0;
            if(conflitos < *x || *x == -1) {
                printf("Entrei %d %d %d\n", l, *x, conflitos);
                for(int i = 0; i < l; i++) {
                    printf("[%d] ", group[i]);
                }
                printf("\n");
                for(int i = 0; i < numH; i++)
                    groupOpt[i] = 0;

                *x = conflitos;
                groupOpt[0] = 1;
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
        if(f && !o) {
            for (int j = 0; j < inp->aNum; j++) {
                if((inp->aPairs->i[j] == l+1) && (inp->aPairs->j[j] < l+1)) {
                    caminho = group[inp->aPairs->j[j] - 1];
                    restricao = 1;
                } else if((inp->aPairs->j[j] == l+1) && (inp->aPairs->i[j] < l+1)) {
                    caminho = group[inp->aPairs->i[j] - 1];
                    restricao = 1;
                }
            }
        } else if(o && !f) {
            for(int i = 0; i < l; i++)
                for(int j = 0; j < inp->cNum; j++) 
                    if((((inp->cPairs->i[j] == (i+1)) && (inp->cPairs->j[j] < (l+1)) && (group[i] == group[inp->cPairs->j[j] - 1])) 
                    || ((inp->cPairs->j[j] == (i+1)) && (inp->cPairs->i[j] < (l+1)) && (group[i] == group[inp->cPairs->i[j] - 1]))))
                        conflitos++;

            for(int i = l; i < inp->hNum; i++) {
                if(!a)
                    calculaFigura(1, 5, i, i, &conflitos, inp);
                calculaFigura(1, 3, i, i, &conflitos, inp);
            }

            if(conflitos > *x && *x != -1)
                return;
        } else if (f && o) {
            for (int j = 0; j < inp->aNum; j++) {
                if((inp->aPairs->i[j] == l+1) && (inp->aPairs->j[j] < l+1)) {
                    caminho = group[inp->aPairs->j[j] - 1];
                    restricao = 1;
                } else if((inp->aPairs->j[j] == l+1) && (inp->aPairs->i[j] < l+1)) {
                    caminho = group[inp->aPairs->i[j] - 1];
                    restricao = 1;
                }
            }

            for(int i = 0; i < l; i++)
                for(int j = 0; j < inp->cNum; j++) 
                    if((((inp->cPairs->i[j] == (i+1)) && (inp->cPairs->j[j] < (l+1)) && (group[i] == group[inp->cPairs->j[j] - 1])) 
                    || ((inp->cPairs->j[j] == (i+1)) && (inp->cPairs->i[j] < (l+1)) && (group[i] == group[inp->cPairs->i[j] - 1]))))
                        conflitos++;

            for(int i = l; i < inp->hNum; i++) {
                if(!a)
                    calculaFigura(1, 5, i, i, &conflitos, inp);
                calculaFigura(1, 3, i, i, &conflitos, inp);
            }

            if(conflitos > *x && *x != -1)
                return;
        }

        if(f) {
            if(restricao) {
                group[l] = caminho;
                separaGrupos(l + 1, x, numH, group, groupOpt, inp, qNodo, a, o, f);
            } else {
                group[l] = 1;
                separaGrupos(l + 1, x, numH, group, groupOpt, inp, qNodo, a, o, f);
                group[l] = 0;
                separaGrupos(l + 1, x, numH, group, groupOpt, inp, qNodo, a, o, f);
            }
        } else {
            group[l] = 1;
            separaGrupos(l + 1, x, numH, group, groupOpt, inp, qNodo, a, o, f);
            group[l] = 0;
            separaGrupos(l + 1, x, numH, group, groupOpt, inp, qNodo, a, o, f);
        }
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
    int escolhas[100], x = -1, escolhasOpt[100], quant = 0;

    inputHandler(argc, argv, input);

    printf("Quant: %d\n", quant);
    //backtracking(0, &x, input->hNum, escolhas, escolhasOpt, input, &quant);
    //backtrackingViabilidade(0, &x, input->hNum, escolhas, escolhasOpt, input, &quant);
    //backtrackingOtimalidade(0, &x, input->hNum, escolhas, escolhasOpt, input, &quant);
    separaGrupos(0, &x, input->hNum, escolhas, escolhasOpt, input, &quant, 1, 1, 1);
    printf("Quant: %d\n", quant);

    printf("x: %d\n", x);
    for(int i = 0; i < input->hNum; i++) {
        printf("%d ", escolhasOpt[i]);
        if(escolhasOpt[i + 1] == 0)
            i = input->hNum;
    }
    printf("\n");
    
    deleteInput(input);

    return 0;
}
