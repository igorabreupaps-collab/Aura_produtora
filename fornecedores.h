// fornecedores.h - Versão com ponteiros para alocação dinâmica

#ifndef FORNECEDORES_H
#define FORNECEDORES_H

#include "produtora.h" // Inclui para poder usar a struct Endereco

typedef struct {
    int codigo;
    char nomeFantasia[100];
    char razaoSocial[100];
    char cnpjCpf[19];
    Endereco endereco;
    char telefone[16];
    char tipoServico[100];
    int ativo;
} Fornecedor;

// Funções públicas
void carregarFornecedores();
void menuFornecedores();
void liberarMemoriaFornecedores(); // <-- NOVA FUNÇÃO PARA LIMPEZA DE MEMÓRIA

#endif // FORNECEDORES_H