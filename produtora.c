// produtora.c - Versão FINAL com base no seu código e I/O em .txt formatado

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produtora.h"

#define NOME_ARQUIVO "produtora.txt" // <-- ALTERADO

// Protótipos de funções "privadas" a este arquivo
void limparBuffer();
void lerString(char *destino, int tamanho);
void limparTela();
void pausarSistema();
void exibirCabecalho(const char* titulo);
void salvarDados(const Produtora* p);
void exibirDados(const Produtora* p);
void excluirDadosProdutora(Produtora* p);
void editarDadosGerais(Produtora* p);
void editarDadosEndereco(Produtora* p);
void editarDadosContato(Produtora* p);
void editarDadosFinanceiros(Produtora* p);

// --- Implementação das Funções Auxiliares ---
void limparBuffer() { int c; while ((c = getchar()) != '\n' && c != EOF); }
void lerString(char *d, int t) { if(fgets(d,t,stdin)){size_t l=strlen(d);if(l>0&&d[l-1]=='\n'){d[l-1]='\0';}}}
void limparTela() { 
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void pausarSistema() { printf("\nPressione Enter para continuar..."); limparBuffer(); }
void exibirCabecalho(const char* t) { printf("============================================\n   %s\n============================================\n\n", t); }


// --- Implementação das Funções de Arquivo ---

// ALTERADO PARA MODO TEXTO (.txt) - Salva com rótulos
void salvarDados(const Produtora* p) {
    FILE* arquivo = fopen(NOME_ARQUIVO, "w");
    if (arquivo == NULL) { perror("Erro ao salvar dados da produtora"); pausarSistema(); return; }

    fprintf(arquivo, "dadosCadastrados: %d\n", p->dadosCadastrados);
    fprintf(arquivo, "nomeFantasia: %s\n", p->nomeFantasia);
    fprintf(arquivo, "razaoSocial: %s\n", p->razaoSocial);
    fprintf(arquivo, "cnpj: %s\n", p->cnpj);
    fprintf(arquivo, "inscricaoEstadual: %s\n", p->inscricaoEstadual);
    fprintf(arquivo, "telefone: %s\n", p->telefone);
    fprintf(arquivo, "email: %s\n", p->email);
    fprintf(arquivo, "nomeResponsavel: %s\n", p->nomeResponsavel);
    fprintf(arquivo, "telefoneResponsavel: %s\n", p->telefoneResponsavel);
    fprintf(arquivo, "cep: %s\n", p->endereco.cep);
    fprintf(arquivo, "logradouro: %s\n", p->endereco.logradouro);
    fprintf(arquivo, "numero: %s\n", p->endereco.numero);
    fprintf(arquivo, "complemento: %s\n", p->endereco.complemento);
    fprintf(arquivo, "bairro: %s\n", p->endereco.bairro);
    fprintf(arquivo, "cidade: %s\n", p->endereco.cidade);
    fprintf(arquivo, "estado: %s\n", p->endereco.estado);
    fprintf(arquivo, "margemLucroPadrao: %f\n", p->margemLucroPadrao);
    
    fclose(arquivo);
}

// ALTERADO PARA MODO TEXTO (.txt) - Lê dados com rótulos
int carregarDados(Produtora* p) {
    FILE* arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) { 
        p->dadosCadastrados = 0;
        return 0; 
    }

    char linha[256];

    // Lê e processa cada linha com seu rótulo específico
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "dadosCadastrados: %d", &p->dadosCadastrados);

    if (p->dadosCadastrados == 0) {
        fclose(arquivo);
        return 0;
    }

    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "nomeFantasia: %[^\n]", p->nomeFantasia);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "razaoSocial: %[^\n]", p->razaoSocial);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "cnpj: %[^\n]", p->cnpj);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "inscricaoEstadual: %[^\n]", p->inscricaoEstadual);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "telefone: %[^\n]", p->telefone);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "email: %[^\n]", p->email);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "nomeResponsavel: %[^\n]", p->nomeResponsavel);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "telefoneResponsavel: %[^\n]", p->telefoneResponsavel);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "cep: %[^\n]", p->endereco.cep);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "logradouro: %[^\n]", p->endereco.logradouro);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "numero: %[^\n]", p->endereco.numero);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "complemento: %[^\n]", p->endereco.complemento);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "bairro: %[^\n]", p->endereco.bairro);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "cidade: %[^\n]", p->endereco.cidade);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "estado: %[^\n]", p->endereco.estado);
    fgets(linha, sizeof(linha), arquivo); sscanf(linha, "margemLucroPadrao: %f", &p->margemLucroPadrao);

    fclose(arquivo);
    return 1;
}

// --- Funções de Menu e Edição (exatamente como no seu código) ---

void menuProdutora(Produtora* p) {
    int opcao;
    do {
        limparTela();
        exibirCabecalho("GESTAO DA PRODUTORA");
        printf("1. Cadastrar Dados da Produtora\n");
        printf("2. Editar Dados da Produtora\n");
        printf("3. Visualizar Dados da Produtora\n");
        printf("4. Excluir Dados da Produtora\n");
        printf("0. Voltar ao Menu Principal\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: if (p->dadosCadastrados == 1) { printf("\nDados ja cadastrados. Use 'Editar'.\n"); pausarSistema(); } else { menuEditar(p); } break;
            case 2: if (p->dadosCadastrados == 0) { printf("\nNenhum dado cadastrado. Use 'Cadastrar'.\n"); pausarSistema(); } else { menuEditar(p); } break;
            case 3: if (p->dadosCadastrados == 0) { printf("\nNenhum dado para visualizar.\n"); pausarSistema(); } else { exibirDados(p); } break;
            case 4: if (p->dadosCadastrados == 0) { printf("\nNenhum dado para excluir.\n"); pausarSistema(); } else { excluirDadosProdutora(p); } break;
        }
    } while (opcao != 0);
}

void excluirDadosProdutora(Produtora* p) {
    char confirmacao;
    limparTela();
    exibirCabecalho("EXCLUIR DADOS DA PRODUTORA");
    printf("ATENCAO: Esta acao apagara todos os dados da produtora do sistema.\n");
    printf("Voce tem certeza que deseja continuar? (s/n): ");
    scanf(" %c", &confirmacao);
    limparBuffer();
    if (confirmacao == 's' || confirmacao == 'S') {
        memset(p, 0, sizeof(Produtora));
        p->dadosCadastrados = 0;
        salvarDados(p);
        printf("\nDados da produtora excluidos com sucesso.\n");
    } else {
        printf("\nOperacao cancelada.\n");
    }
    pausarSistema();
}

void exibirDados(const Produtora* p) {
    limparTela();
    exibirCabecalho("DADOS DA PRODUTORA");
    printf("Nome Fantasia: %s\n", p->nomeFantasia);
    printf("Razao Social: %s\n", p->razaoSocial);
    printf("CNPJ: %s\n", p->cnpj);
    printf("Inscricao Estadual: %s\n", p->inscricaoEstadual);
    printf("\n--- Endereco ---\n");
    printf("%s, %s - %s\n", p->endereco.logradouro, p->endereco.numero, p->endereco.bairro);
    printf("%s - %s, CEP: %s\n", p->endereco.cidade, p->endereco.estado, p->endereco.cep);
    printf("\n--- Contato ---\n");
    printf("Telefone Principal: %s\n", p->telefone);
    printf("E-mail Principal: %s\n", p->email);
    printf("Responsavel: %s (%s)\n", p->nomeResponsavel, p->telefoneResponsavel);
    printf("\n--- Financeiro ---\n");
    printf("Margem de Lucro Padrao: %.2f%%\n", p->margemLucroPadrao);
    pausarSistema();
}

void menuEditar(Produtora* p) {
    int opcao;
    do {
        limparTela();
        exibirCabecalho("CADASTRO/EDICAO DE DADOS");
        printf("1. Dados Gerais\n2. Endereco\n3. Contato\n4. Financeiro\n0. Voltar\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        switch (opcao) {
            case 1: editarDadosGerais(p); break;
            case 2: editarDadosEndereco(p); break;
            case 3: editarDadosContato(p); break;
            case 4: editarDadosFinanceiros(p); break;
        }
    } while (opcao != 0);
}

void editarDadosGerais(Produtora* p) {
    char buffer[100];
    limparTela();
    exibirCabecalho("EDITAR DADOS GERAIS");
    printf("Preencha os dados. Campos com * sao obrigatorios.\n\n");
    do {
        printf("Nome Fantasia*: ");
        lerString(buffer, 100);
        if (strlen(buffer) > 0) {
            strcpy(p->nomeFantasia, buffer);
            p->dadosCadastrados = 1; 
        }
        if (strlen(p->nomeFantasia) == 0 && p->dadosCadastrados == 0) {
            printf("Erro: Nome Fantasia e obrigatorio no primeiro cadastro.\n");
        }
    } while (strlen(p->nomeFantasia) == 0 && p->dadosCadastrados == 0);
    printf("Razao Social: ");
    lerString(buffer, 100);
    if (strlen(buffer) > 0) strcpy(p->razaoSocial, buffer);
    printf("CNPJ: ");
    lerString(buffer, 19);
    if (strlen(buffer) > 0) strcpy(p->cnpj, buffer);
    printf("Inscricao Estadual: ");
    lerString(buffer, 20);
    if (strlen(buffer) > 0) strcpy(p->inscricaoEstadual, buffer);
    salvarDados(p);
    printf("\nDados gerais atualizados com sucesso!\n");
    pausarSistema();
}

void editarDadosEndereco(Produtora* p) {
    char buffer[100];
    limparTela();
    exibirCabecalho("EDITAR ENDERECO");
    printf("Deixe em branco e pressione Enter para manter o valor atual.\n\n");
    printf("CEP [%s]: ", p->endereco.cep);
    lerString(buffer, 10);
    if (strlen(buffer) > 0) strcpy(p->endereco.cep, buffer);
    printf("Logradouro (Rua/Av) [%s]: ", p->endereco.logradouro);
    lerString(buffer, 100);
    if (strlen(buffer) > 0) strcpy(p->endereco.logradouro, buffer);
    printf("Numero [%s]: ", p->endereco.numero);
    lerString(buffer, 10);
    if (strlen(buffer) > 0) strcpy(p->endereco.numero, buffer);
    printf("Complemento [%s]: ", p->endereco.complemento);
    lerString(buffer, 50);
    if (strlen(buffer) > 0) strcpy(p->endereco.complemento, buffer);
    printf("Bairro [%s]: ", p->endereco.bairro);
    lerString(buffer, 50);
    if (strlen(buffer) > 0) strcpy(p->endereco.bairro, buffer);
    printf("Cidade [%s]: ", p->endereco.cidade);
    lerString(buffer, 50);
    if (strlen(buffer) > 0) strcpy(p->endereco.cidade, buffer);
    printf("Estado (UF) [%s]: ", p->endereco.estado);
    lerString(buffer, 3);
    if (strlen(buffer) > 0) strcpy(p->endereco.estado, buffer);
    salvarDados(p);
    printf("\nEndereco atualizado com sucesso!\n");
    pausarSistema();
}

void editarDadosContato(Produtora* p) {
    char buffer[100];
    limparTela();
    exibirCabecalho("EDITAR DADOS DE CONTATO");
    printf("Deixe em branco e pressione Enter para manter o valor atual.\n\n");
    printf("Telefone Principal [%s]: ", p->telefone);
    lerString(buffer, 16);
    if (strlen(buffer) > 0) strcpy(p->telefone, buffer);
    printf("E-mail Principal [%s]: ", p->email);
    lerString(buffer, 100);
    if (strlen(buffer) > 0) strcpy(p->email, buffer);
    printf("Nome do Responsavel [%s]: ", p->nomeResponsavel);
    lerString(buffer, 100);
    if (strlen(buffer) > 0) strcpy(p->nomeResponsavel, buffer);
    printf("Telefone do Responsavel [%s]: ", p->telefoneResponsavel);
    lerString(buffer, 16);
    if (strlen(buffer) > 0) strcpy(p->telefoneResponsavel, buffer);
    salvarDados(p);
    printf("\nDados de contato atualizados com sucesso!\n");
    pausarSistema();
}

void editarDadosFinanceiros(Produtora* p) {
    char buffer[10];
    float novaMargem;
    limparTela();
    exibirCabecalho("EDITAR DADOS FINANCEIROS");
    printf("Deixe em branco para manter o valor atual.\n\n");
    do {
        printf("Margem de Lucro Padrao (ex: 25.5) [%.2f%%]: ", p->margemLucroPadrao);
        lerString(buffer, 10);
        if (strlen(buffer) > 0) {
            novaMargem = atof(buffer);
            if (novaMargem >= 0) {
                p->margemLucroPadrao = novaMargem;
                break;
            } else {
                printf("Erro: A margem de lucro nao pode ser um valor negativo.\n");
            }
        } else {
            break;
        }
    } while (1);
    salvarDados(p);
    printf("\nDados financeiros atualizados com sucesso!\n");
    pausarSistema();
}