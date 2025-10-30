// equipamentos.h - Versão com ponteiros para alocação dinâmica

#ifndef EQUIPAMENTOS_H
#define EQUIPAMENTOS_H

typedef struct {
    int codigo;
    char descricao[100];
    char categoria[50];
    int quantidadeEstoque;
    float precoCusto;
    float valorLocacao;
    int ativo;
} Equipamento;

// Funções públicas
void carregarEquipamentos();
void menuEquipamentos();
void liberarMemoriaEquipamentos(); // <-- NOVA FUNÇÃO PARA LIMPEZA DE MEMÓRIA

#endif // EQUIPAMENTOS_H