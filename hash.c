#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Funcao hash djb2 simplificada
unsigned int calcular_hash(const char* str, int tam_tabela) {
    unsigned long hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash % tam_tabela;
}

TabelaHash* criar_tabela(int tam) {
    TabelaHash* h = (TabelaHash*)malloc(sizeof(TabelaHash));
    h->tamanho = tam;
    h->qtd_elementos = 0;
    h->dados = (No**)calloc(tam, sizeof(No*));
    return h;
}

int inserir_hash(TabelaHash* hash, const char* usuario) {
    if (buscar_hash(hash, usuario)) {
        return 0; // ja existe
    }

    unsigned int pos = calcular_hash(usuario, hash->tamanho);
    No* novo = (No*)malloc(sizeof(No));
    strcpy(novo->usuario, usuario);

    novo->proximo = hash->dados[pos];
    hash->dados[pos] = novo;
    hash->qtd_elementos++;
    return 1;
}

int buscar_hash(TabelaHash* hash, const char* usuario) {
    unsigned int pos = calcular_hash(usuario, hash->tamanho);
    No* atual = hash->dados[pos];

    while (atual != NULL) {
        if (strcmp(atual->usuario, usuario) == 0) {
            return 1; // achou
        }
        atual = atual->proximo;
    }
    return 0; // nao achou
}

void liberar_hash(TabelaHash* hash) {
    for (int i = 0; i < hash->tamanho; i++) {
        No* atual = hash->dados[i];
        while (atual != NULL) {
            No* aux = atual;
            atual = atual->proximo;
            free(aux);
        }
    }
    free(hash->dados);
    free(hash);
}
