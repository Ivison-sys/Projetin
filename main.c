#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    fogo,
    agua,
    eletricidade,
    planta
} Elemento;

typedef struct {
    int id;
    char nome[30];
    Elemento tipo;
    int xp;
    int ataque;
} Pokemon;

typedef struct {
    char nome[30];
    char cpf[14];
    int idade;
    Pokemon *pokemons;
    int qtdPokemons;
    int nivel;
} Treinador;

int qtdTreinadores = 0;

const char* get_nome_elemento(Elemento e) {
    switch(e) {
        case fogo: return "Fogo";
        case agua: return "Agua";
        case eletricidade: return "Eletricidade";
        case planta: return "Planta";
        default: return "";
    }
}

int buscar_treinador(Treinador* t, char* cpf) {
    for (int i = 0; i < qtdTreinadores; i++) {
        if (strcmp(t[i].cpf, cpf) == 0) return i;
    }
    return -1;
}

void cadastrarTreinador(Treinador **treinadores) {
    char nome[30], cpf[14];
    int idade;
    if (scanf("%s %s %d", nome, cpf, &idade) != 3) return;
    if (buscar_treinador(*treinadores, cpf) != -1) return;

    *treinadores = (Treinador*) realloc(*treinadores, (qtdTreinadores + 1) * sizeof(Treinador));
    if (*treinadores == NULL) exit(1);

    Treinador *novo = &(*treinadores)[qtdTreinadores];
    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    novo->idade = idade;
    novo->pokemons = NULL;
    novo->qtdPokemons = 0;
    novo->nivel = 0;
    qtdTreinadores++;
}

void cadastrarPokemon(Treinador *treinadores) {
    char cpf[14], nome[30];
    int id, xp, atq, elem;
    if (scanf("%s %d %s %d %d %d", cpf, &id, nome, &xp, &atq, &elem) != 6) return;

    int idx = buscar_treinador(treinadores, cpf);
    if (idx == -1) return;

    for (int i = 0; i < treinadores[idx].qtdPokemons; i++) {
        if (treinadores[idx].pokemons[i].id == id) return;
    }

    treinadores[idx].pokemons = (Pokemon*) realloc(treinadores[idx].pokemons, (treinadores[idx].qtdPokemons + 1) * sizeof(Pokemon));
    Pokemon *p = &treinadores[idx].pokemons[treinadores[idx].qtdPokemons];
    p->id = id;
    strcpy(p->nome, nome);
    p->xp = xp;
    p->ataque = atq;
    p->tipo = (Elemento)elem;

    treinadores[idx].qtdPokemons++;
    treinadores[idx].nivel += (2 * xp + atq);
}

void removerTreinador(Treinador **treinadores) {
    char cpf[14];
    if (scanf("%s", cpf) != 1) return;
    int idx = buscar_treinador(*treinadores, cpf);
    if (idx == -1) return;

    free((*treinadores)[idx].pokemons);
    for (int i = idx; i < qtdTreinadores - 1; i++) {
        (*treinadores)[i] = (*treinadores)[i + 1];
    }

    qtdTreinadores--;
    if (qtdTreinadores > 0) {
        *treinadores = (Treinador*) realloc(*treinadores, (qtdTreinadores) * sizeof(Treinador));
    } else {
        free(*treinadores);
        *treinadores = NULL;
    }
}

void atualizarPokemon(Treinador *treinadores) {
    char cpf[14], nome[30];
    int id_p, xp, atq, elem;
    if (scanf("%s %d %s %d %d %d", cpf, &id_p, nome, &xp, &atq, &elem) != 6) return;

    int t_idx = buscar_treinador(treinadores, cpf);
    if (t_idx == -1) return;

    for (int i = 0; i < treinadores[t_idx].qtdPokemons; i++) {
        if (treinadores[t_idx].pokemons[i].id == id_p) {
            treinadores[t_idx].nivel -= (2 * treinadores[t_idx].pokemons[i].xp + treinadores[t_idx].pokemons[i].ataque);
            
            strcpy(treinadores[t_idx].pokemons[i].nome, nome);
            treinadores[t_idx].pokemons[i].xp = xp;
            treinadores[t_idx].pokemons[i].ataque = atq;
            treinadores[t_idx].pokemons[i].tipo = (Elemento)elem;
            
            treinadores[t_idx].nivel += (2 * xp + atq);
            break;
        }
    }
}

// Comando 3: Listar com "Classificação atual" e Ranking
void listarTreinadores(Treinador *treinadores) {
    if (qtdTreinadores == 0) {
        printf("Classificação atual\n");
        return;
    }

    // Ordenação Treinadores (Nível)
    for (int i = 0; i < qtdTreinadores - 1; i++) {
        for (int j = 0; j < qtdTreinadores - i - 1; j++) {
            if (treinadores[j].nivel < treinadores[j+1].nivel) {
                Treinador temp = treinadores[j];
                treinadores[j] = treinadores[j+1];
                treinadores[j+1] = temp;
            }
        }
    }

    // Ordenação Pokémons (Força)
    for (int i = 0; i < qtdTreinadores; i++) {
        for (int j = 0; j < treinadores[i].qtdPokemons - 1; j++) {
            for (int k = 0; k < treinadores[i].qtdPokemons - j - 1; k++) {
                int f1 = 2 * treinadores[i].pokemons[k].xp + treinadores[i].pokemons[k].ataque;
                int f2 = 2 * treinadores[i].pokemons[k+1].xp + treinadores[i].pokemons[k+1].ataque;
                if (f1 < f2) {
                    Pokemon tempP = treinadores[i].pokemons[k];
                    treinadores[i].pokemons[k] = treinadores[i].pokemons[k+1];
                    treinadores[i].pokemons[k+1] = tempP;
                }
            }
        }
    }

    // Saída com a frase solicitada
    printf("Classificação atual\n");
    for (int i = 0; i < qtdTreinadores; i++) {
        printf("T: %s, CPF: %s, Nivel: %d\n", treinadores[i].nome, treinadores[i].cpf, treinadores[i].nivel);
        for (int j = 0; j < treinadores[i].qtdPokemons; j++) {
            Pokemon *p = &treinadores[i].pokemons[j];
            printf("  P: %d, %s, %d, %d, %s\n", p->id, p->nome, p->xp, p->ataque, get_nome_elemento(p->tipo));
        }
    }
}

int main() {
    Treinador *treinadores = NULL;
    int comando;

    while (scanf("%d", &comando) != EOF && comando != 0) {
        switch (comando) {
            case 1: cadastrarTreinador(&treinadores); break;
            case 2: cadastrarPokemon(treinadores); break;
            case 3: listarTreinadores(treinadores); break;
            case 4: removerTreinador(&treinadores); break;
            case 5: atualizarPokemon(treinadores); break;
        }
    }

    for (int i = 0; i < qtdTreinadores; i++) free(treinadores[i].pokemons);
    free(treinadores);
    return 0;
}