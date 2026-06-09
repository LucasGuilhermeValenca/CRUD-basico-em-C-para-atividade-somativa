#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numeroConta;
    char nome[50];
    float saldo;
    int ativo; 
} Cliente;

void cadastrarCliente(FILE *arquivo);
void consultarCliente(FILE *arquivo);
void atualizarSaldo(FILE *arquivo);
void removerCliente(FILE *arquivo);
void listarClientes(FILE *arquivo);

int main() {
    FILE *arquivo;
    int opcao;

    arquivo = fopen("clientes.dat", "rb+");

    if (arquivo == NULL) {
        arquivo = fopen("clientes.dat", "wb+");

        if (arquivo == NULL) {
            printf("Erro ao criar arquivo.\n");
            return 1;
        }
    }

    do {
        printf("\n===== Banco do Riquinho =====\n");
        printf("1. Cadastrar cliente\n");
        printf("2. Consultar cliente\n");
        printf("3. Atualizar saldo\n");
        printf("4. Encerrar conta\n");
        printf("5. Listar clientes\n");
        printf("6. Repetir listagem usando rewind()\n");
        printf("7. Encerrar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarCliente(arquivo);
                break;
            case 2:
                consultarCliente(arquivo);
                break;
            case 3:
                atualizarSaldo(arquivo);
                break;
            case 4:
                removerCliente(arquivo);
                break;
            case 5:
                listarClientes(arquivo);
                break;
            case 6:
                rewind(arquivo);
                printf("\nLeitura reposicionada para o inicio.\n");
                listarClientes(arquivo);
                break;
            case 7:
                printf("Encerrando sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 7);

    fclose(arquivo);
    return 0;
}

void cadastrarCliente(FILE *arquivo) {
    Cliente cliente;
    int posicao;

    printf("\nPosicao do registro (0, 1, 2...): ");
    scanf("%d", &posicao);

    printf("Numero da conta: ");
    scanf("%d", &cliente.numeroConta);

    getchar(); 

    printf("Nome: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

    printf("Saldo inicial: ");
    scanf("%f", &cliente.saldo);

    cliente.ativo = 1;

   
    fseek(arquivo, posicao * sizeof(Cliente), SEEK_SET);
    fwrite(&cliente, sizeof(Cliente), 1, arquivo);

    printf("Cliente cadastrado com sucesso na posicao %d!\n", posicao);
}

void consultarCliente(FILE *arquivo) {
    Cliente cliente;
    int conta;
    int encontrou = 0;

    printf("\nDigite o Numero da conta para consulta: ");
    scanf("%d", &conta);

    rewind(arquivo); // Volta para o inicio para buscar, n entendi mt bem como funciona

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        
        if (cliente.ativo == 1 && cliente.numeroConta == conta) {
            printf("\n===== CLIENTE ENCONTRADO =====");
            printf("\nConta: %d\n", cliente.numeroConta);
            printf("Nome : %s\n", cliente.nome);
            printf("Saldo: %.2f\n", cliente.saldo);
            printf("---------------------------\n");
            encontrou = 1;
            break; 
        }
    }

    if (!encontrou) {
        printf("Conta nao encontrada ou inativa.\n");
    }
}

void atualizarSaldo(FILE *arquivo) {
    Cliente cliente;
    int conta;
    float novoSaldo;
    int encontrou = 0;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arquivo);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo == 1 && cliente.numeroConta == conta) {
            printf("Saldo atual: %.2f\n", cliente.saldo);
            printf("Novo saldo: ");
            scanf("%f", &novoSaldo);

            cliente.saldo = novoSaldo;

            fseek(arquivo, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo);

            printf("Saldo atualizado com sucesso!\n");
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) {
        printf("Conta nao encontrada.\n");
    }
}

void removerCliente(FILE *arquivo) {
    Cliente cliente;
    int conta;
    int encontrou = 0;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arquivo);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo == 1 && cliente.numeroConta == conta) {
            cliente.ativo = 0;

            fseek(arquivo, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, arquivo);

            printf("Conta encerrada com sucesso!\n");
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) {
        printf("Conta nao encontrada.\n");
    }
}

void listarClientes(FILE *arquivo) {
    Cliente cliente;
    int encontrouCliente = 0; 

    rewind(arquivo);

    printf("\n===== CLIENTES CADASTRADOS =====\n");

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo == 1) {
            printf("Conta: %d\n", cliente.numeroConta);
            printf("Nome : %s\n", cliente.nome);
            printf("Saldo: %.2f\n", cliente.saldo);
            printf("--------------------------\n");
            encontrouCliente = 1; 
        }
    }

    if (!encontrouCliente) {
        printf("Nenhum cliente cadastrado.\n");
        printf("--------------------------\n");
    }
}
