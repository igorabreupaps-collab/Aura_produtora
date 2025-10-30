// operadores.h - Versão com ponteiros para alocação dinâmica

#ifndef OPERADORES_H
#define OPERADORES_H

typedef struct {
    int codigo;
    char nome[100];
    char usuario[50];
    char senha[50];
    int ativo;
} Operador;

// Funções públicas
void carregarOperadores();
void menuOperadores();
int verificarCredenciais(const char* usuario, const char* senha);
void liberarMemoriaOperadores(); // <-- NOVA FUNÇÃO PARA LIMPEZA DE MEMÓRIA

#endif // OPERADORES_H