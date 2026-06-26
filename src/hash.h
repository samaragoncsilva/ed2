#ifndef HASH_H
#define HASH_H

// No da lista encadeada para tratar colisao
typedef struct No {
    char usuario[12];
    struct No* proximo;
} No;

// Estrutura da tabela hash
typedef struct {
    No** dados;
    int tamanho;
    int qtd_elementos;
} TabelaHash;

TabelaHash* criar_tabela(int tam);
unsigned int calcular_hash(const char* str, int tam_tabela);
int inserir_hash(TabelaHash* hash, const char* usuario);
int buscar_hash(TabelaHash* hash, const char* usuario);
void liberar_hash(TabelaHash* hash);

#endif
