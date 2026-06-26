#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloom.h"

// 3 funcoes de hash faceis de entender desenvolvidas para o trabalho
unsigned int hash_tipo1(const char* str) {
    unsigned int h = 0;
    while (*str) {
        h = h * 31 + *str++;
    }
    return h;
}

unsigned int hash_tipo2(const char* str) {
    unsigned int h = 7;
    while (*str) {
        h = (h << 5) - h + *str++;
    }
    return h;
}

unsigned int hash_tipo3(const char* str) {
    unsigned int h = 0;
    for (int i = 0; *str; i++) {
        h += (*str++) * (i + 1);
    }
    return h;
}

FiltroBloom* criar_bloom() {
    FiltroBloom* b = (FiltroBloom*)malloc(sizeof(FiltroBloom));
    b->tam_bits = TAMANHO_BLOOM;
    int tam_bytes = (TAMANHO_BLOOM / 8) + 1;
    b->bytes = (unsigned char*)calloc(tam_bytes, sizeof(unsigned char));
    return b;
}

// acende o bit na marra usando operacao binaria
void ligar_bit(unsigned char* v, int pos) {
    v[pos / 8] |= (1 << (pos % 8));
}

// pega o valor do bit
int ler_bit(unsigned char* v, int pos) {
    return (v[pos / 8] & (1 << (pos % 8))) != 0;
}

void inserir_bloom(FiltroBloom* b, const char* usuario) {
    unsigned int p1 = hash_tipo1(usuario) % b->tam_bits;
    unsigned int p2 = hash_tipo2(usuario) % b->tam_bits;
    unsigned int p3 = hash_tipo3(usuario) % b->tam_bits;

    ligar_bit(b->bytes, p1);
    ligar_bit(b->bytes, p2);
    ligar_bit(b->bytes, p3);
}

int checar_bloom(FiltroBloom* b, const char* usuario) {
    unsigned int p1 = hash_tipo1(usuario) % b->tam_bits;
    unsigned int p2 = hash_tipo2(usuario) % b->tam_bits;
    unsigned int p3 = hash_tipo3(usuario) % b->tam_bits;

    if (!ler_bit(b->bytes, p1)) return 0;
    if (!ler_bit(b->bytes, p2)) return 0;
    if (!ler_bit(b->bytes, p3)) return 0;

    return 1; // talvez exista
}

void liberar_bloom(FiltroBloom* b) {
    free(b->bytes);
    free(b);
}
