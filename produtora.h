#ifndef PRODUTORA_H
#define PRODUTORA_H

typedef struct {
    char cep[10];
    char logradouro[100];
    char numero[10];
    char complemento[50];
    char bairro[50];
    char cidade[50];
    char estado[3];
} Endereco;

typedef struct {
    int dadosCadastrados; 
    char nomeFantasia[100];
    char razaoSocial[100];
    char inscricaoEstadual[20];
    char cnpj[19];
    Endereco endereco;
    char telefone[16];
    char email[100];
    char nomeResponsavel[100];
    char telefoneResponsavel[16];
    float margemLucroPadrao;
} Produtora;

// Funções públicas do módulo
int carregarDados(Produtora* p);
void menuProdutora(Produtora* p);
void menuEditar(Produtora* p); // Usada para o primeiro cadastro e para a edição

#endif // PRODUTORA_H