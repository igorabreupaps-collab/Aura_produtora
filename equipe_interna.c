

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipe_interna.h"

#define NOME_ARQUIVO_EQUIPE "equipe.txt" // <-- USANDO .TXT

// --- VARIÁVEIS DINÂMICAS ---
static Funcionario* listaDeFuncionarios = NULL;
static int numFuncionarios = 0;
static int capacidadeEquipe = 0;

// Funções "privadas"
static void limparBuffer_e() { int c; while ((c = getchar()) != '\n' && c != EOF); }
static void lerString_e(char* dest, int size) { 
    if (fgets(dest, size, stdin) != NULL) {
        dest[strcspn(dest, "\n")] = 0;
    }
}
static void pausarSistema_e() { printf("\nPressione Enter para continuar..."); limparBuffer_e(); }
static void limparTela_e() { system("cls || clear"); }
static void exibirCabecalho_e(const char* titulo) { printf("============================================\n   %s\n============================================\n\n", titulo); }

static int buscarFuncionarioPorCodigo(int codigo) {
    for (int i = 0; i < numFuncionarios; i++) {
        if (listaDeFuncionarios[i].codigo == codigo && listaDeFuncionarios[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}

// --- FUNÇÕES DE ARQUIVO ATUALIZADAS PARA .TXT ---
void carregarEquipe() {
    FILE* f = fopen(NOME_ARQUIVO_EQUIPE, "r");
    if (f == NULL) { 
        numFuncionarios = 0; 
        capacidadeEquipe = 0;
        listaDeFuncionarios = NULL;
        return; 
    }
    
    char linha[256];
    int totalArquivo = 0;
    if (fgets(linha, sizeof(linha), f) != NULL) { sscanf(linha, "totalFuncionarios: %d", &totalArquivo); }

    if (totalArquivo > 0) {
        capacidadeEquipe = totalArquivo;
        listaDeFuncionarios = (Funcionario*) malloc(capacidadeEquipe * sizeof(Funcionario));
        if (listaDeFuncionarios == NULL) { printf("Erro fatal de memoria!\n"); exit(1); }
    } else {
        numFuncionarios = 0;
        capacidadeEquipe = 0;
        listaDeFuncionarios = NULL;
        fclose(f);
        return;
    }

    numFuncionarios = 0;
    for (int i = 0; i < totalArquivo; i++) {
        Funcionario* func = &listaDeFuncionarios[numFuncionarios];
        fgets(linha, sizeof(linha), f); // Separador
        fgets(linha, sizeof(linha), f); sscanf(linha, "codigo: %d", &func->codigo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "ativo: %d", &func->ativo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "nome: %[^\n]", func->nome);
        fgets(linha, sizeof(linha), f); sscanf(linha, "cpf: %[^\n]", func->cpf);
        fgets(linha, sizeof(linha), f); sscanf(linha, "funcao: %[^\n]", func->funcao);
        fgets(linha, sizeof(linha), f); sscanf(linha, "valorDiariaHora: %f", &func->valorDiariaHora);
        numFuncionarios++;
    }
    fclose(f);
}

void salvarEquipe() {
    FILE* f = fopen(NOME_ARQUIVO_EQUIPE, "w");
    if (f == NULL) { perror("Erro ao salvar equipe"); return; }
    fprintf(f, "totalFuncionarios: %d\n", numFuncionarios);
    for (int i = 0; i < numFuncionarios; i++) {
        Funcionario func = listaDeFuncionarios[i];
        fprintf(f, "---\n");
        fprintf(f, "codigo: %d\n", func.codigo);
        fprintf(f, "ativo: %d\n", func.ativo);
        fprintf(f, "nome: %s\n", func.nome);
        fprintf(f, "cpf: %s\n", func.cpf);
        fprintf(f, "funcao: %s\n", func.funcao);
        fprintf(f, "valorDiariaHora: %f\n", func.valorDiariaHora);
    }
    fclose(f);
}

// --- FUNÇÕES CRUD E MENU (COM CORREÇÃO DO BUG) ---
void adicionarFuncionario() {
    if (numFuncionarios == capacidadeEquipe) {
        capacidadeEquipe = (capacidadeEquipe == 0) ? 5 : capacidadeEquipe * 2;
        Funcionario* temp = (Funcionario*) realloc(listaDeFuncionarios, capacidadeEquipe * sizeof(Funcionario));
        if (temp == NULL) {
            printf("Erro fatal de memoria ao realocar!\n");
            pausarSistema_e();
            return;
        }
        listaDeFuncionarios = temp;
    }
    
    Funcionario* f = &listaDeFuncionarios[numFuncionarios];
    f->codigo = (numFuncionarios > 0) ? listaDeFuncionarios[numFuncionarios - 1].codigo + 1 : 1;
    f->ativo = 1;
    
    limparTela_e();
    exibirCabecalho_e("CADASTRAR NOVO FUNCIONARIO");
    printf("Codigo gerado: %d\n\n", f->codigo);

    printf("Nome: ");
    lerString_e(f->nome, 100);
    printf("CPF: ");
    lerString_e(f->cpf, 15);
    printf("Funcao (Ex: Produtor, Tecnico de Som): ");
    lerString_e(f->funcao, 50);

    // --- CORREÇÃO APLICADA AQUI ---
    char valor_buffer[20];
    printf("Valor da diaria/hora: ");
    lerString_e(valor_buffer, 20);
    f->valorDiariaHora = atof(valor_buffer); // atof() converte a string para float

    numFuncionarios++;
    salvarEquipe();
    printf("\nFuncionario cadastrado com sucesso!\n");
    pausarSistema_e();
}

void listarFuncionarios() {
    limparTela_e();
    exibirCabecalho_e("LISTA DE FUNCIONARIOS ATIVOS");
    int funcionariosAtivos = 0;
    for (int i = 0; i < numFuncionarios; i++) {
        if(listaDeFuncionarios[i].ativo == 1) {
            Funcionario f = listaDeFuncionarios[i];
            printf("----------------------------------------\n");
            printf("Codigo: %d\n", f.codigo);
            printf("Nome: %s\n", f.nome);
            printf("CPF: %s\n", f.cpf);
            printf("Funcao: %s\n", f.funcao);
            printf("Valor Diaria/Hora: R$ %.2f\n", f.valorDiariaHora);
            funcionariosAtivos++;
        }
    }
    if (funcionariosAtivos == 0) { printf("Nenhum funcionario ativo cadastrado.\n"); }
    printf("----------------------------------------\n");
    pausarSistema_e();
}

void editarFuncionario() {
    int codigo, indice;
    char buffer[100];

    limparTela_e();
    exibirCabecalho_e("EDITAR FUNCIONARIO");
    printf("Digite o codigo do funcionario que deseja editar: ");
    lerString_e(buffer, 10); // Lendo como texto para segurança
    codigo = atoi(buffer);

    indice = buscarFuncionarioPorCodigo(codigo);

    if (indice == -1) { 
        printf("\nErro: Funcionario com o codigo %d nao encontrado!\n", codigo); 
    } else {
        Funcionario* f = &listaDeFuncionarios[indice];
        printf("\nEditando funcionario: %s\n", f->nome);
        printf("Deixe em branco e pressione Enter para manter o valor atual.\n\n");

        printf("Nome [%s]: ", f->nome);
        lerString_e(buffer, 100);
        if (strlen(buffer) > 0) strcpy(f->nome, buffer);

        printf("CPF [%s]: ", f->cpf);
        lerString_e(buffer, 15);
        if (strlen(buffer) > 0) strcpy(f->cpf, buffer);
        
        printf("Funcao [%s]: ", f->funcao);
        lerString_e(buffer, 50);
        if (strlen(buffer) > 0) strcpy(f->funcao, buffer);

        printf("Valor Diaria/Hora [%.2f]: ", f->valorDiariaHora);
        lerString_e(buffer, 20);
        if (strlen(buffer) > 0) f->valorDiariaHora = atof(buffer);

        salvarEquipe();
        printf("\nFuncionario atualizado com sucesso!\n");
    }
    pausarSistema_e();
}

void excluirFuncionario() {
    int codigo, indice;
    char buffer[10];

    limparTela_e();
    exibirCabecalho_e("EXCLUIR FUNCIONARIO");
    printf("Digite o codigo do funcionario que deseja excluir: ");
    lerString_e(buffer, 10); // Lendo como texto
    codigo = atoi(buffer);

    indice = buscarFuncionarioPorCodigo(codigo);

    if (indice == -1) { 
        printf("\nErro: Funcionario com o codigo %d nao encontrado ou ja excluido!\n", codigo); 
    } else {
        printf("\nFuncionario encontrado: %s\n", listaDeFuncionarios[indice].nome);
        printf("Tem certeza que deseja excluir (desativar) este funcionario? (s/n): ");
        lerString_e(buffer, 10);
        
        if (buffer[0] == 's' || buffer[0] == 'S') {
            listaDeFuncionarios[indice].ativo = 0;
            salvarEquipe();
            printf("\nFuncionario excluido com sucesso!\n");
        } else {
            printf("\nOperacao cancelada.\n");
        }
    }
    pausarSistema_e();
}

void menuEquipe() {
    int opcao;
    char buffer[10];
    do {
        limparTela_e();
        exibirCabecalho_e("GESTAO DE EQUIPE INTERNA");
        printf("1. Adicionar Funcionario\n");
        printf("2. Listar Funcionarios\n");
        printf("3. Editar Funcionario\n");
        printf("4. Excluir Funcionario\n");
        printf("0. Voltar ao Menu Principal\n\n");
        printf("Escolha uma opcao: ");
        lerString_e(buffer, 10);
        opcao = atoi(buffer);

        switch (opcao) {
            case 1: adicionarFuncionario(); break;
            case 2: listarFuncionarios(); break;
            case 3: editarFuncionario(); break;
            case 4: excluirFuncionario(); break;
            case 0: break;
            default:
                printf("Opcao invalida!\n");
                pausarSistema_e();
        }
    } while (opcao != 0);
}

void liberarMemoriaEquipe() {
    if (listaDeFuncionarios != NULL) {
        free(listaDeFuncionarios);
        listaDeFuncionarios = NULL;
    }
}