// clientes.h - Versão com ponteiros para alocação dinâmica

#ifndef CLIENTES_H
#define CLIENTES_H

#include "produtora.h" // Para usar a struct Endereco

typedef struct {
    int codigo;
    char nomeRazaoSocial[100];
    char cpfCnpj[19];
    Endereco endereco;
    char telefone[16];
    char email[100];
    char nomeContato[100];
    int ativo;
} Cliente;

// Funções públicas
void carregarClientes();
void menuClientes();
void liberarMemoriaClientes(); // <-- NOVA FUNÇÃO

#endif // CLIENTES_H