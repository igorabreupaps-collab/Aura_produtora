#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "produtora.h"
#include "clientes.h"
#include "equipe_interna.h"
#include "equipamentos.h"
#include "fornecedores.h"
#include "operadores.h"

// Funções auxiliares do main
void limparBuffer_main() { int c; while ((c = getchar()) != '\n' && c != EOF); }
void pausarSistema_main() { printf("\nPressione Enter para continuar..."); limparBuffer_main(); }
void limparTela_main() { system("cls || clear"); }
void lerString_main(char* dest, int size) { if (fgets(dest, size, stdin) != NULL) { dest[strcspn(dest, "\n")] = 0; } }

// TELA DE LOGIN
int fazerLogin() {
    char usuario[50];
    char senha[50];
    int tentativas = 3;

    while (tentativas > 0) {
        limparTela_main();
        printf("============================================\n");
        printf("   SISTEMA DE GESTAO DE EVENTOS - LOGIN\n");
        printf("============================================\n\n");
        
        printf("Usuario: ");
        lerString_main(usuario, 50);

        printf("Senha: ");
        lerString_main(senha, 50);

        if (verificarCredenciais(usuario, senha)) {
            printf("\nLogin bem-sucedido!\n");
            pausarSistema_main();
            return 1;
        } else {
            tentativas--;
            if (tentativas > 0) {
                 printf("\nUsuario ou senha invalidos! Voce tem mais %d tentativa(s).\n", tentativas);
            } else {
                 printf("\nUsuario ou senha invalidos!\n");
            }
            pausarSistema_main();
        }
    }

    printf("\nNumero maximo de tentativas atingido. O programa sera encerrado.\n");
    pausarSistema_main();
    return 0; // <-- CORREÇÃO: Adicionado o return para o caso de falha.
}

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    system("chcp 65001 > nul");

    Produtora* empresa;
    int opcao = -1;
    
    empresa = (Produtora*) malloc(sizeof(Produtora));
    if (empresa == NULL) {
        printf("Erro fatal: Falha ao alocar memoria para a produtora.\n");
        return 1;
    }
    memset(empresa, 0, sizeof(Produtora));

    printf("Inicializando sistema...\n");
    carregarDados(empresa);
    carregarClientes();
    carregarEquipe();
    carregarEquipamentos();
    carregarFornecedores();
    carregarOperadores();
    printf("Modulos carregados.\n");
    pausarSistema_main();
    
    if (fazerLogin() == 0) {
        // Libera a memória de todos os módulos que já foram alocados
        free(empresa);
        liberarMemoriaClientes();
        liberarMemoriaEquipe();
        liberarMemoriaEquipamentos();
        liberarMemoriaFornecedores();
        liberarMemoriaOperadores();
        return 1;
    }

    do {
        limparTela_main();
        printf("==================================================\n");
        if(empresa->dadosCadastrados) { 
             printf("   SISTEMA DE GESTAO DE EVENTOS - %s\n", empresa->nomeFantasia);
        } else {
             printf("   SISTEMA DE GESTAO DE EVENTOS - (PRODUTORA NAO CADASTRADA)\n");
        }
        printf("==================================================\n\n");
        printf("1. Gestao da Produtora\n2. Gestao de Clientes\n3. Gestao de Equipe Interna\n4. Gestao de Equipamentos\n5. Gestao de Fornecedores\n6. Gestao de Operadores\n0. Sair\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer_main();

        switch (opcao) {
            case 1: menuProdutora(empresa); break;
            case 2: menuClientes(); break;
            case 3: menuEquipe(); break;
            case 4: menuEquipamentos(); break;
            case 5: menuFornecedores(); break;
            case 6: menuOperadores(); break;
            case 0: printf("\nSaindo do sistema...\n"); break;
            default: printf("\nOpcao invalida!\n"); pausarSistema_main();
        }
    } while (opcao != 0);

    // LIBERAÇÃO DE TODA A MEMÓRIA ALOCADA NO FINAL DO PROGRAMA
    printf("Liberando memoria...\n");
    free(empresa);
    liberarMemoriaClientes();
    liberarMemoriaEquipe();
    liberarMemoriaEquipamentos();
    liberarMemoriaFornecedores();
    liberarMemoriaOperadores();

    return 0;
}