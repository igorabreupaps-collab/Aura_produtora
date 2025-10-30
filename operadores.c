// operadores.c - Versão FINAL com ALOCAÇÃO DINÂMICA, .txt, Criptografia e CORREÇÕES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operadores.h"

#define MAX_OPERADORES 50
#define NOME_ARQUIVO_OPERADORES "operadores.txt"
#define CHAVE_CIFRA 5

// --- VARIÁVEIS DINÂMICAS ---
static Operador* listaDeOperadores = NULL;
static int numOperadores = 0;
static int capacidadeOperadores = 0;

// Funções "privadas"
static void limparBuffer_op() { int c; while ((c = getchar()) != '\n' && c != EOF); }
static void lerString_op(char* dest, int size) { if (fgets(dest, size, stdin) != NULL) { dest[strcspn(dest, "\n")] = 0; } }
static void pausarSistema_op() { printf("\nPressione Enter para continuar..."); limparBuffer_op(); }
static void limparTela_op() { system("cls || clear"); }
static void exibirCabecalho_op(const char* titulo) { printf("============================================\n   %s\n============================================\n\n", titulo); }

static void transformar_senha(char* senha, int chave) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha[i] = senha[i] + chave;
    }
}

static int buscarOperadorPorCodigo(int codigo) {
    for (int i = 0; i < numOperadores; i++) {
        if (listaDeOperadores[i].codigo == codigo && listaDeOperadores[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}

// --- Funções de Arquivo ---
void carregarOperadores() {
    FILE* f = fopen(NOME_ARQUIVO_OPERADORES, "r");
    if (f == NULL) { 
        numOperadores = 0; 
        capacidadeOperadores = 0;
        listaDeOperadores = NULL;
        return; 
    }
    
    char linha[256];
    int totalArquivo = 0;
    if (fgets(linha, sizeof(linha), f) != NULL) { sscanf(linha, "totalOperadores: %d", &totalArquivo); }

    if (totalArquivo > 0) {
        capacidadeOperadores = totalArquivo;
        listaDeOperadores = (Operador*) malloc(capacidadeOperadores * sizeof(Operador));
        if (listaDeOperadores == NULL) { printf("Erro fatal de memoria!\n"); exit(1); }
    } else {
        numOperadores = 0;
        capacidadeOperadores = 0;
        listaDeOperadores = NULL;
        fclose(f);
        return;
    }

    numOperadores = 0;
    for (int i = 0; i < totalArquivo; i++) {
        Operador* op = &listaDeOperadores[numOperadores];
        fgets(linha, sizeof(linha), f); // Separador
        fgets(linha, sizeof(linha), f); sscanf(linha, "codigo: %d", &op->codigo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "ativo: %d", &op->ativo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "nome: %[^\n]", op->nome);
        fgets(linha, sizeof(linha), f); sscanf(linha, "usuario: %[^\n]", op->usuario);
        fgets(linha, sizeof(linha), f); sscanf(linha, "senha: %[^\n]", op->senha);
        numOperadores++;
    }
    fclose(f);
}

void salvarOperadores() {
    FILE* f = fopen(NOME_ARQUIVO_OPERADORES, "w");
    if (f == NULL) { perror("Erro ao salvar operadores"); return; }
    fprintf(f, "totalOperadores: %d\n", numOperadores);
    for (int i = 0; i < numOperadores; i++) {
        Operador op = listaDeOperadores[i];
        fprintf(f, "---\n");
        fprintf(f, "codigo: %d\n", op.codigo);
        fprintf(f, "ativo: %d\n", op.ativo);
        fprintf(f, "nome: %s\n", op.nome);
        fprintf(f, "usuario: %s\n", op.usuario);
        fprintf(f, "senha: %s\n", op.senha);
    }
    fclose(f);
}

// --- Funções CRUD e Menu ---
void adicionarOperador() {
    if (numOperadores == capacidadeOperadores) {
        capacidadeOperadores = (capacidadeOperadores == 0) ? 5 : capacidadeOperadores * 2;
        Operador* temp = (Operador*) realloc(listaDeOperadores, capacidadeOperadores * sizeof(Operador));
        if (temp == NULL) { printf("Erro fatal de memoria!\n"); return; }
        listaDeOperadores = temp;
    }
    
    Operador* op = &listaDeOperadores[numOperadores];
    op->codigo = (numOperadores > 0) ? listaDeOperadores[numOperadores - 1].codigo + 1 : 1;
    op->ativo = 1;
    
    limparTela_op();
    exibirCabecalho_op("CADASTRAR NOVO OPERADOR");
    printf("Codigo gerado: %d\n\n", op->codigo);
    printf("Nome completo: ");
    lerString_op(op->nome, 100);
    printf("Usuario (para login): ");
    lerString_op(op->usuario, 50);
    printf("Senha: ");
    lerString_op(op->senha, 50);
    transformar_senha(op->senha, CHAVE_CIFRA);
    numOperadores++;
    salvarOperadores();
    printf("\nOperador cadastrado com sucesso!\n");
    pausarSistema_op();
}

void listarOperadores() {
    limparTela_op();
    exibirCabecalho_op("LISTA DE OPERADORES ATIVOS");
    int ativos = 0;
    for (int i = 0; i < numOperadores; i++) {
        if(listaDeOperadores[i].ativo == 1) {
            Operador op = listaDeOperadores[i];
            printf("----------------------------------------\n");
            printf("Codigo: %d\n", op.codigo);
            printf("Nome: %s\n", op.nome);
            printf("Usuario: %s\n", op.usuario);
            printf("Senha: ********\n");
            ativos++;
        }
    }
    if (ativos == 0) { printf("Nenhum operador ativo cadastrado.\n"); }
    printf("----------------------------------------\n");
    pausarSistema_op();
}

void editarOperador() {
    int codigo, indice;
    char buffer[100];
    limparTela_op();
    exibirCabecalho_op("EDITAR OPERADOR");
    printf("Digite o codigo do operador que deseja editar: ");
    lerString_op(buffer, 10);
    codigo = atoi(buffer);
    indice = buscarOperadorPorCodigo(codigo);

    if (indice == -1) { printf("\nErro: Operador com o codigo %d nao encontrado!\n", codigo); } 
    else {
        Operador* op = &listaDeOperadores[indice];
        printf("\nEditando operador: %s (%s)\n", op->nome, op->usuario);
        printf("Deixe em branco e pressione Enter para manter o valor atual.\n\n");
        printf("Nome [%s]: ", op->nome);
        lerString_op(buffer, 100);
        if (strlen(buffer) > 0) strcpy(op->nome, buffer);
        printf("Usuario [%s]: ", op->usuario);
        lerString_op(buffer, 50);
        if (strlen(buffer) > 0) strcpy(op->usuario, buffer);
        printf("Nova Senha (deixe em branco para nao alterar): ");
        lerString_op(buffer, 50);
        if (strlen(buffer) > 0) {
            transformar_senha(buffer, CHAVE_CIFRA);
            strcpy(op->senha, buffer);
        }
        salvarOperadores();
        printf("\nOperador atualizado com sucesso!\n");
    }
    pausarSistema_op();
}

void excluirOperador() {
    int codigo, indice;
    char buffer[10];
    limparTela_op();
    exibirCabecalho_op("EXCLUIR OPERADOR");
    printf("Digite o codigo do operador que deseja excluir: ");
    lerString_op(buffer, 10);
    codigo = atoi(buffer);
    indice = buscarOperadorPorCodigo(codigo);

    if (indice == -1) { printf("\nErro: Operador com o codigo %d nao encontrado!\n", codigo); } 
    else {
        printf("\nOperador encontrado: %s\n", listaDeOperadores[indice].nome);
        printf("Tem certeza que deseja excluir este operador? (s/n): ");
        lerString_op(buffer, 10);
        if (buffer[0] == 's' || buffer[0] == 'S') {
            listaDeOperadores[indice].ativo = 0;
            salvarOperadores();
            printf("\nOperador excluido com sucesso!\n");
        } else {
            printf("\nOperacao cancelada.\n");
        }
    }
    pausarSistema_op();
}

void menuOperadores() {
    int opcao;
    char buffer[10];
    do {
        limparTela_op();
        exibirCabecalho_op("GESTAO DE OPERADORES DO SISTEMA");
        printf("1. Adicionar Operador\n2. Listar Operadores\n3. Editar Operador\n4. Excluir Operador\n0. Voltar\n\n");
        printf("Escolha uma opcao: ");
        lerString_op(buffer, 10);
        opcao = atoi(buffer);
        switch (opcao) {
            case 1: adicionarOperador(); break;
            case 2: listarOperadores(); break;
            case 3: editarOperador(); break;
            case 4: excluirOperador(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausarSistema_op();
        }
    } while (opcao != 0);
}

int verificarCredenciais(const char* usuario, const char* senha) {
    char senha_digitada_cifrada[50];
    strcpy(senha_digitada_cifrada, senha);
    transformar_senha(senha_digitada_cifrada, CHAVE_CIFRA);

    if (numOperadores == 0) {
        if (strcmp(usuario, "admin") == 0 && strcmp(senha, "admin") == 0) {
            printf("\nLogin inicial de administrador detectado.\n");
            return 1;
        }
    }
    
    for (int i = 0; i < numOperadores; i++) {
        Operador op = listaDeOperadores[i];
        if (op.ativo == 1) {
            if (strcmp(op.usuario, usuario) == 0 && strcmp(op.senha, senha_digitada_cifrada) == 0) {
                return 1;
            }
        }
    }
    return 0; // <-- CORREÇÃO: Adicionado o return para o caso de falha
}

void liberarMemoriaOperadores() {
    if (listaDeOperadores != NULL) {
        free(listaDeOperadores);
        listaDeOperadores = NULL;
    }
}