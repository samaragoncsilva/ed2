#ifndef BLOOM_H
#define BLOOM_H

// Tamanho do vetor de bits (m) definido via define, bem estilo de aluno
#define TAMANHO_BLOOM 900000

typedef struct {
    unsigned char* bytes;
    int tam_bits;
} FiltroBloom;

FiltroBloom* criar_bloom();
void inserir_bloom(FiltroBloom* b, const char* usuario);
int checar_bloom(FiltroBloom* b, const char* usuario);
void liberar_bloom(FiltroBloom* b);

#endif
