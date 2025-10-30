// equipe_interna.h - Versão com ponteiros para alocação dinâmica

#ifndef EQUIPE_INTERNA_H
#define EQUIPE_INTERNA_H

typedef struct {
    int codigo;
    char nome[100];
    char cpf[15];
    char funcao[50];
    float valorDiariaHora;
    int ativo;
} Funcionario;

// Funções públicas
void carregarEquipe();
void menuEquipe();
void liberarMemoriaEquipe(); // <-- NOVA FUNÇÃO PARA LIMPEZA DE MEMÓRIA

#endif // EQUIPE_INTERNA_H