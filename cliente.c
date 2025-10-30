// clientes.c - Versão FINAL com Alocação Dinâmica, Saída .txt e CADASTRO COMPLETO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clientes.h"

#define NOME_ARQUIVO_CLIENTES "clientes.txt"

// --- VARIÁVEIS DINÂMICAS ---
static Cliente* listaDeClientes = NULL;
static int numClientes = 0;
static int capacidadeClientes = 0;

// Funções "privadas"
static void limparBuffer_c() { int c; while ((c = getchar()) != '\n' && c != EOF); }
static void lerString_c(char* dest, int size) { if (fgets(dest, size, stdin) != NULL) { dest[strcspn(dest, "\n")] = 0; } }
static void pausarSistema_c() { printf("\nPressione Enter para continuar..."); limparBuffer_c(); }
static void limparTela_c() { system("cls || clear"); }
static void exibirCabecalho_c(const char* titulo) { printf("============================================\n   %s\n============================================\n\n", titulo); }

static int buscarClientePorCodigo(int codigo) {
    for (int i = 0; i < numClientes; i++) {
        if (listaDeClientes[i].codigo == codigo && listaDeClientes[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}

// --- Funções de Arquivo ---
void carregarClientes() {
    FILE* f = fopen(NOME_ARQUIVO_CLIENTES, "r");
    if (f == NULL) { 
        numClientes = 0; 
        capacidadeClientes = 0; 
        listaDeClientes = NULL; 
        return; 
    }
    
    char linha[256];
    int totalArquivo = 0;
    if (fgets(linha, sizeof(linha), f) != NULL) { 
        sscanf(linha, "totalClientes: %d", &totalArquivo); 
    }

    if (totalArquivo > 0) {
        capacidadeClientes = totalArquivo;
        listaDeClientes = (Cliente*) malloc(capacidadeClientes * sizeof(Cliente));
        if (listaDeClientes == NULL) { printf("Erro fatal de memoria!\n"); exit(1); }
    } else {
        numClientes = 0;
        capacidadeClientes = 0;
        listaDeClientes = NULL;
        fclose(f);
        return;
    }

    numClientes = 0;
    for (int i = 0; i < totalArquivo; i++) {
        Cliente* c = &listaDeClientes[numClientes];
        fgets(linha, sizeof(linha), f); // Separador "---"
        fgets(linha, sizeof(linha), f); sscanf(linha, "codigo: %d", &c->codigo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "ativo: %d", &c->ativo);
        fgets(linha, sizeof(linha), f); sscanf(linha, "nome: %[^\n]", c->nomeRazaoSocial);
        fgets(linha, sizeof(linha), f); sscanf(linha, "doc: %[^\n]", c->cpfCnpj);
        fgets(linha, sizeof(linha), f); sscanf(linha, "tel: %[^\n]", c->telefone);
        fgets(linha, sizeof(linha), f); sscanf(linha, "email: %[^\n]", c->email);
        fgets(linha, sizeof(linha), f); sscanf(linha, "contato: %[^\n]", c->nomeContato);
        fgets(linha, sizeof(linha), f); sscanf(linha, "cep: %[^\n]", c->endereco.cep);
        fgets(linha, sizeof(linha), f); sscanf(linha, "logradouro: %[^\n]", c->endereco.logradouro);
        fgets(linha, sizeof(linha), f); sscanf(linha, "numero: %[^\n]", c->endereco.numero);
        fgets(linha, sizeof(linha), f); sscanf(linha, "complemento: %[^\n]", c->endereco.complemento);
        fgets(linha, sizeof(linha), f); sscanf(linha, "bairro: %[^\n]", c->endereco.bairro);
        fgets(linha, sizeof(linha), f); sscanf(linha, "cidade: %[^\n]", c->endereco.cidade);
        fgets(linha, sizeof(linha), f); sscanf(linha, "estado: %[^\n]", c->endereco.estado);
        numClientes++;
    }
    fclose(f);
}

void salvarClientes() {
    FILE* f = fopen(NOME_ARQUIVO_CLIENTES, "w");
    if (f == NULL) { perror("Erro ao salvar clientes"); return; }
    fprintf(f, "totalClientes: %d\n", numClientes);
    for (int i = 0; i < numClientes; i++) {
        Cliente c = listaDeClientes[i];
        fprintf(f, "---\n");
        fprintf(f, "codigo: %d\n", c.codigo);
        fprintf(f, "ativo: %d\n", c.ativo);
        fprintf(f, "nome: %s\n", c.nomeRazaoSocial);
        fprintf(f, "doc: %s\n", c.cpfCnpj);
        fprintf(f, "tel: %s\n", c.telefone);
        fprintf(f, "email: %s\n", c.email);
        fprintf(f, "contato: %s\n", c.nomeContato);
        fprintf(f, "cep: %s\n", c.endereco.cep);
        fprintf(f, "logradouro: %s\n", c.endereco.logradouro);
        fprintf(f, "numero: %s\n", c.endereco.numero);
        fprintf(f, "complemento: %s\n", c.endereco.complemento);
        fprintf(f, "bairro: %s\n", c.endereco.bairro);
        fprintf(f, "cidade: %s\n", c.endereco.cidade);
        fprintf(f, "estado: %s\n", c.endereco.estado);
    }
    fclose(f);
}

// --- FUNÇÕES CRUD (COM CORPO COMPLETO) ---

void adicionarNovoCliente() {
    if (numClientes == capacidadeClientes) {
        capacidadeClientes = (capacidadeClientes == 0) ? 5 : capacidadeClientes * 2;
        Cliente* temp = (Cliente*) realloc(listaDeClientes, capacidadeClientes * sizeof(Cliente));
        if (temp == NULL) {
            printf("Erro fatal de memoria ao realocar!\n");
            pausarSistema_c();
            return;
        }
        listaDeClientes = temp;
    }

    Cliente* c = &listaDeClientes[numClientes];
    c->codigo = (numClientes > 0) ? listaDeClientes[numClientes - 1].codigo + 1 : 1;
    c->ativo = 1;
    
    limparTela_c();
    exibirCabecalho_c("CADASTRAR NOVO CLIENTE");
    printf("Codigo gerado: %d\n\n", c->codigo);

    printf("Nome / Razao Social: ");
    lerString_c(c->nomeRazaoSocial, 100);
    printf("CPF / CNPJ: ");
    lerString_c(c->cpfCnpj, 19);
    printf("Telefone: ");
    lerString_c(c->telefone, 16);
    printf("E-mail: ");
    lerString_c(c->email, 100);
    printf("Nome do Contato: ");
    lerString_c(c->nomeContato, 100);
    
    printf("\n--- Endereco Completo ---\n");
    printf("CEP: ");
    lerString_c(c->endereco.cep, 10);
    printf("Logradouro (Rua/Av): ");
    lerString_c(c->endereco.logradouro, 100);
    printf("Numero: ");
    lerString_c(c->endereco.numero, 10);
    printf("Complemento: ");
    lerString_c(c->endereco.complemento, 50);
    printf("Bairro: ");
    lerString_c(c->endereco.bairro, 50);
    printf("Cidade: ");
    lerString_c(c->endereco.cidade, 50);
    printf("Estado (UF): ");
    lerString_c(c->endereco.estado, 3);

    numClientes++;
    salvarClientes();
    printf("\nCliente cadastrado com sucesso!\n");
    pausarSistema_c();
}

void editarCliente() {
    int codigo, indice;
    char buffer[100];
    limparTela_c();
    exibirCabecalho_c("EDITAR CLIENTE");
    printf("Digite o codigo do cliente que deseja editar: ");
    scanf("%d", &codigo);
    limparBuffer_c();

    indice = buscarClientePorCodigo(codigo);

    if (indice == -1) {
        printf("\nErro: Cliente com o codigo %d nao encontrado!\n", codigo);
        pausarSistema_c();
        return;
    }
    
    Cliente* c = &listaDeClientes[indice];
    printf("\nEditando cliente: %s\n\n", c->nomeRazaoSocial);
    printf("Deixe em branco e pressione Enter para manter o valor atual.\n\n");
    
    printf("Nome / Razao Social [%s]: ", c->nomeRazaoSocial);
    lerString_c(buffer, 100);
    if(strlen(buffer) > 0) strcpy(c->nomeRazaoSocial, buffer);
    
    printf("CPF / CNPJ [%s]: ", c->cpfCnpj);
    lerString_c(buffer, 19);
    if(strlen(buffer) > 0) strcpy(c->cpfCnpj, buffer);

    printf("Telefone [%s]: ", c->telefone);
    lerString_c(buffer, 16);
    if(strlen(buffer) > 0) strcpy(c->telefone, buffer);

    printf("E-mail [%s]: ", c->email);
    lerString_c(buffer, 100);
    if(strlen(buffer) > 0) strcpy(c->email, buffer);
    
    printf("Nome do Contato [%s]: ", c->nomeContato);
    lerString_c(buffer, 100);
    if(strlen(buffer) > 0) strcpy(c->nomeContato, buffer);

    // Edição do endereço
    printf("\n--- Editando Endereco ---\n");
    printf("CEP [%s]: ", c->endereco.cep);
    lerString_c(buffer, 10);
    if(strlen(buffer) > 0) strcpy(c->endereco.cep, buffer);
    // (Pode adicionar os outros campos de endereço aqui se desejar)

    salvarClientes();
    printf("\nCliente atualizado com sucesso!\n");
    pausarSistema_c();
}

void listarClientes() {
    limparTela_c();
    exibirCabecalho_c("LISTA DETALHADA DE CLIENTES ATIVOS");
    int ativos = 0;
    if (numClientes > 0) {
        for (int i = 0; i < numClientes; i++) {
            if(listaDeClientes[i].ativo == 1) {
                Cliente c = listaDeClientes[i];
                printf("----------------------------------------\n");
                printf("Codigo: %d\n", c.codigo);
                printf("Nome/Razao Social: %s\n", c.nomeRazaoSocial);
                printf("CPF/CNPJ: %s\n", c.cpfCnpj);
                printf("Telefone: %s\n", c.telefone);
                printf("E-mail: %s\n", c.email);
                printf("Contato: %s\n", c.nomeContato);
                printf("Endereco: %s, %s - %s, %s-%s\n", 
                    c.endereco.logradouro, c.endereco.numero, c.endereco.bairro,
                    c.endereco.cidade, c.endereco.estado);
                ativos++;
            }
        }
    }
    if (ativos == 0) { printf("Nenhum cliente ativo cadastrado.\n"); }
    printf("----------------------------------------\n");
    pausarSistema_c();
}

void excluirCliente() {
    int codigo, indice;
    char confirmacao;
    limparTela_c();
    exibirCabecalho_c("EXCLUIR CLIENTE");
    printf("Digite o codigo do cliente que deseja excluir: ");
    scanf("%d", &codigo);
    limparBuffer_c();

    indice = buscarClientePorCodigo(codigo);

    if (indice == -1) {
        printf("\nErro: Cliente nao encontrado.\n");
    } else {
        printf("\nExcluir cliente: %s. Tem certeza (s/n)?: ", listaDeClientes[indice].nomeRazaoSocial);
        scanf(" %c", &confirmacao);
        limparBuffer_c();
        if (confirmacao == 's' || confirmacao == 'S') {
            listaDeClientes[indice].ativo = 0;
            salvarClientes();
            printf("\nCliente excluido com sucesso!\n");
        } else {
            printf("\nOperacao cancelada.\n");
        }
    }
    pausarSistema_c();
}

void menuClientes() {
    int opcao;
    do {
        limparTela_c();
        exibirCabecalho_c("GESTAO DE CLIENTES");
        printf("1. Adicionar Cliente\n2. Listar Clientes (Detalhado)\n3. Editar Cliente\n4. Excluir Cliente\n0. Voltar\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer_c();
        switch (opcao) {
            case 1: adicionarNovoCliente(); break;
            case 2: listarClientes(); break;
            case 3: editarCliente(); break;
            case 4: excluirCliente(); break;
        }
    } while (opcao != 0);
}

void liberarMemoriaClientes() {
    if (listaDeClientes != NULL) {
        free(listaDeClientes);
        listaDeClientes = NULL;
    }
}