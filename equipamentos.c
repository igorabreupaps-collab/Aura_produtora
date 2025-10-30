// equipamentos.c - Versão FINAL com ALOCAÇÃO DINÂMICA e funções completas

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipamentos.h"

#define NOME_ARQUIVO_EQUIPAMENTOS "equipamentos.txt"

// --- VARIÁVEIS DINÂMICAS ---
static Equipamento* listaDeEquipamentos = NULL;
static int numEquipamentos = 0;
static int capacidadeEquipamentos = 0;

// Funções "privadas"
static void limparBuffer_eq() { int c; while ((c = getchar()) != '\n' && c != EOF); }
static void lerString_eq(char* dest, int size) { if (fgets(dest, size, stdin) != NULL) { dest[strcspn(dest, "\n")] = 0; } }
static void pausarSistema_eq() { printf("\nPressione Enter para continuar..."); limparBuffer_eq(); }
static void limparTela_eq() { system("cls || clear"); }
static void exibirCabecalho_eq(const char* titulo) { printf("============================================\n   %s\n============================================\n\n", titulo); }

static int buscarEquipamentoPorCodigo(int codigo) {
    for (int i = 0; i < numEquipamentos; i++) {
        if (listaDeEquipamentos[i].codigo == codigo && listaDeEquipamentos[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}

// --- Funções de Arquivo ---
void carregarEquipamentos() {
    FILE* f = fopen(NOME_ARQUIVO_EQUIPAMENTOS, "r");
    if (f == NULL) { 
        numEquipamentos = 0; 
        capacidadeEquipamentos = 0;
        listaDeEquipamentos = NULL;
        return; 
    }
    
    char linha[256];
    int totalArquivo = 0;
    if (fgets(linha, sizeof(linha), f) != NULL) { sscanf(linha, "totalEquipamentos: %d", &totalArquivo); }

    if (totalArquivo > 0) {
        capacidadeEquipamentos = totalArquivo;
        listaDeEquipamentos = (Equipamento*) malloc(capacidadeEquipamentos * sizeof(Equipamento));
        if (listaDeEquipamentos == NULL) { printf("Erro fatal de memoria!\n"); exit(1); }
    } else {
        numEquipamentos = 0;
        capacidadeEquipamentos = 0;
        listaDeEquipamentos = NULL;
        fclose(f);
        return;
    }

    numEquipamentos = 0;
    for (int i = 0; i < totalArquivo; i++) {
        Equipamento* eq = &listaDeEquipamentos[numEquipamentos];
        fgets(linha, sizeof(linha), f); // Separador
        fgets(linha, sizeof(linha), f); sscanf(linha, "codigo: %d", &eq->codigo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "ativo: %d", &eq->ativo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "descricao: %[^\n]", eq->descricao);
        fgets(linha, sizeof(linha), f); sscanf(linha, "categoria: %[^\n]", eq->categoria);
        fgets(linha, sizeof(linha), f); sscanf(linha, "estoque: %d", &eq->quantidadeEstoque);
        fgets(linha, sizeof(linha), f); sscanf(linha, "custo: %f", &eq->precoCusto);
        fgets(linha, sizeof(linha), f); sscanf(linha, "locacao: %f", &eq->valorLocacao);
        numEquipamentos++;
    }
    fclose(f);
}

void salvarEquipamentos() {
    FILE* f = fopen(NOME_ARQUIVO_EQUIPAMENTOS, "w");
    if (f == NULL) { perror("Erro ao salvar equipamentos"); return; }
    fprintf(f, "totalEquipamentos: %d\n", numEquipamentos);
    for (int i = 0; i < numEquipamentos; i++) {
        Equipamento eq = listaDeEquipamentos[i];
        fprintf(f, "---\n");
        fprintf(f, "codigo: %d\n", eq.codigo);
        fprintf(f, "ativo: %d\n", eq.ativo);
        fprintf(f, "descricao: %s\n", eq.descricao);
        fprintf(f, "categoria: %s\n", eq.categoria);
        fprintf(f, "estoque: %d\n", eq.quantidadeEstoque);
        fprintf(f, "custo: %f\n", eq.precoCusto);
        fprintf(f, "locacao: %f\n", eq.valorLocacao);
    }
    fclose(f);
}

// --- Funções CRUD e Menu ---
void adicionarEquipamento() {
    if (numEquipamentos == capacidadeEquipamentos) {
        capacidadeEquipamentos = (capacidadeEquipamentos == 0) ? 5 : capacidadeEquipamentos * 2;
        Equipamento* temp = (Equipamento*) realloc(listaDeEquipamentos, capacidadeEquipamentos * sizeof(Equipamento));
        if (temp == NULL) {
            printf("Erro fatal de memoria ao realocar!\n");
            pausarSistema_eq();
            return;
        }
        listaDeEquipamentos = temp;
    }
    
    Equipamento* eq = &listaDeEquipamentos[numEquipamentos];
    eq->codigo = (numEquipamentos > 0) ? listaDeEquipamentos[numEquipamentos - 1].codigo + 1 : 1;
    eq->ativo = 1;
    
    char buffer[20];
    limparTela_eq();
    exibirCabecalho_eq("CADASTRAR NOVO EQUIPAMENTO");
    printf("Codigo gerado: %d\n\n", eq->codigo);

    printf("Descricao (Ex: Caixa de Som Ativa): ");
    lerString_eq(eq->descricao, 100);
    printf("Categoria (Ex: Sonorizacao, Iluminacao): ");
    lerString_eq(eq->categoria, 50);

    printf("Quantidade em estoque: ");
    lerString_eq(buffer, 10);
    eq->quantidadeEstoque = atoi(buffer);

    printf("Preco de Custo (valor de aquisicao): ");
    lerString_eq(buffer, 20);
    eq->precoCusto = atof(buffer);

    printf("Valor da Locacao (diaria): ");
    lerString_eq(buffer, 20);
    eq->valorLocacao = atof(buffer);

    numEquipamentos++;
    salvarEquipamentos();
    printf("\nEquipamento cadastrado com sucesso!\n");
    pausarSistema_eq();
}

void listarEquipamentos() {
    limparTela_eq();
    exibirCabecalho_eq("LISTA DE EQUIPAMENTOS ATIVOS");
    int equipamentosAtivos = 0;
    for (int i = 0; i < numEquipamentos; i++) {
        if(listaDeEquipamentos[i].ativo == 1) {
            Equipamento eq = listaDeEquipamentos[i];
            printf("----------------------------------------\n");
            printf("Codigo: %d\n", eq.codigo);
            printf("Descricao: %s\n", eq.descricao);
            printf("Categoria: %s\n", eq.categoria);
            printf("Estoque: %d unid.\n", eq.quantidadeEstoque);
            printf("Preco de Custo: R$ %.2f\n", eq.precoCusto);
            printf("Valor da Locacao: R$ %.2f\n", eq.valorLocacao);
            equipamentosAtivos++;
        }
    }
    if (equipamentosAtivos == 0) { printf("Nenhum equipamento ativo cadastrado.\n"); }
    printf("----------------------------------------\n");
    pausarSistema_eq();
}

void editarEquipamento() {
    int codigo, indice;
    char buffer[100];

    limparTela_eq();
    exibirCabecalho_eq("EDITAR EQUIPAMENTO");
    printf("Digite o codigo do equipamento que deseja editar: ");
    lerString_eq(buffer, 10);
    codigo = atoi(buffer);

    indice = buscarEquipamentoPorCodigo(codigo);

    if (indice == -1) {
        printf("\nErro: Equipamento com o codigo %d nao encontrado!\n", codigo);
    } else {
        Equipamento* eq = &listaDeEquipamentos[indice];
        printf("\nEditando equipamento: %s\n", eq->descricao);
        printf("Deixe em branco e pressione Enter para manter o valor atual.\n\n");

        printf("Descricao [%s]: ", eq->descricao);
        lerString_eq(buffer, 100);
        if (strlen(buffer) > 0) strcpy(eq->descricao, buffer);

        printf("Categoria [%s]: ", eq->categoria);
        lerString_eq(buffer, 50);
        if (strlen(buffer) > 0) strcpy(eq->categoria, buffer);
        
        printf("Quantidade em estoque [%d]: ", eq->quantidadeEstoque);
        lerString_eq(buffer, 10);
        if (strlen(buffer) > 0) eq->quantidadeEstoque = atoi(buffer);

        printf("Preco de Custo [%.2f]: ", eq->precoCusto);
        lerString_eq(buffer, 20);
        if (strlen(buffer) > 0) eq->precoCusto = atof(buffer);

        printf("Valor da Locacao [%.2f]: ", eq->valorLocacao);
        lerString_eq(buffer, 20);
        if (strlen(buffer) > 0) eq->valorLocacao = atof(buffer);

        salvarEquipamentos();
        printf("\nEquipamento atualizado com sucesso!\n");
    }
    pausarSistema_eq();
}

void excluirEquipamento() {
    int codigo, indice;
    char buffer[10];

    limparTela_eq();
    exibirCabecalho_eq("EXCLUIR EQUIPAMENTO");
    printf("Digite o codigo do equipamento que deseja excluir: ");
    lerString_eq(buffer, 10);
    codigo = atoi(buffer);
    
    indice = buscarEquipamentoPorCodigo(codigo);

    if (indice == -1) {
        printf("\nErro: Equipamento com o codigo %d nao encontrado ou ja excluido!\n", codigo);
    } else {
        printf("\nEquipamento encontrado: %s\n", listaDeEquipamentos[indice].descricao);
        printf("Tem certeza que deseja excluir (desativar) este equipamento? (s/n): ");
        lerString_eq(buffer, 10);

        if (buffer[0] == 's' || buffer[0] == 'S') {
            listaDeEquipamentos[indice].ativo = 0;
            salvarEquipamentos();
            printf("\nEquipamento excluido com sucesso!\n");
        } else {
            printf("\nOperacao cancelada.\n");
        }
    }
    pausarSistema_eq();
}

void menuEquipamentos() {
    int opcao;
    char buffer[10];
    do {
        limparTela_eq();
        exibirCabecalho_eq("GESTAO DE RECURSOS E EQUIPAMENTOS");
        printf("1. Adicionar Equipamento\n");
        printf("2. Listar Equipamentos\n");
        printf("3. Editar Equipamento\n");
        printf("4. Excluir Equipamento\n");
        printf("0. Voltar ao Menu Principal\n\n");
        printf("Escolha uma opcao: ");
        lerString_eq(buffer, 10);
        opcao = atoi(buffer);

        switch (opcao) {
            case 1: adicionarEquipamento(); break;
            case 2: listarEquipamentos(); break;
            case 3: editarEquipamento(); break;
            case 4: excluirEquipamento(); break;
            case 0: break;
            default:
                printf("Opcao invalida!\n");
                pausarSistema_eq();
        }
    } while (opcao != 0);
}

void liberarMemoriaEquipamentos() {
    if (listaDeEquipamentos != NULL) {
        free(listaDeEquipamentos);
        listaDeEquipamentos = NULL;
    }
}