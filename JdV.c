#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define USERNAME_SIZE 50
#define PASSWORD_SIZE 12

    // ==== ESTRUTURA PARA USUÁRIOS ====
    typedef struct
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    int wins; // número de vitórias
} User;

// ==== VARIÁVEIS GLOBAIS (PARA SIMPLICIDADE) ====
User *users = NULL;   // Ponteiro para array dinâmico de usuários
int userCount = 0;    // Quantos usuários já cadastramos
int userCapacity = 0; // Capacidade atual do array de usuários

// ==== CONSTANTES DO JOGO DA VELHA ====
#define ROWS 3
#define COLS 3

// ==== FUNÇÕES DE INICIALIZAÇÃO / FINALIZAÇÃO ====
void inicializarListaUsuarios();

// ==== FUNÇÕES DE CADASTRO/LOGIN ====
int criarLogin();
int logarUsuario();
void exibirRanking();
void ordenarRanking();

// ==== FUNÇÕES DO JOGO ====
char jogarPartidaComLogin(int indexP1, int indexP2);
char jogarPartidaSemLogin();
void inicializarTabuleiro(char tabuleiro[ROWS][COLS]);
void exibirTabuleiro(const char tabuleiro[ROWS][COLS]);
bool fazerJogada(char tabuleiro[ROWS][COLS], int linha, int coluna, char jogador);
bool verificarVitoria(const char tabuleiro[ROWS][COLS], char jogador);
bool verificarEmpate(const char tabuleiro[ROWS][COLS]);

int main()
{
    // Primeiro passo: inicializar a lista dinâmica de usuários
    inicializarListaUsuarios();

    int opcao;
    do
    {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1. Criar login\n");
        printf("2. Jogar com login (P1 e P2)\n");
        printf("3. Jogar sem login\n");
        printf("4. Ranking de vencedores\n");
        printf("0. Sair\n");
        printf("==========================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // consumir eventual '\n' pendente

        switch (opcao)
        {
        case 1:
            criarLogin();
            break;

        case 2:
        {
            printf("\n=== JOGAR COM LOGIN ===\n");
            printf("Para o Jogador X:\n");
            int indexP1 = logarUsuario();
            if (indexP1 < 0)
            {
                printf("Falha no login do Jogador X. Retornando ao menu.\n");
                break;
            }

            printf("\nPara o Jogador O:\n");
            int indexP2 = logarUsuario();
            if (indexP2 < 0)
            {
                printf("Falha no login do Jogador O. Retornando ao menu.\n");
                break;
            }

            char vencedor = jogarPartidaComLogin(indexP1, indexP2);
            if (vencedor == 'X')
            {
                users[indexP1].wins++;
                printf("\n%s (X) venceu a partida!\n", users[indexP1].username);
            }
            else if (vencedor == 'O')
            {
                users[indexP2].wins++;
                printf("\n%s (O) venceu a partida!\n", users[indexP2].username);
            }
            else
            {
                printf("\nA partida terminou em empate!\n");
            }
            break;
        }

        case 3:
        {
            printf("\n=== JOGAR SEM LOGIN ===\n");
            char vencedor = jogarPartidaSemLogin();
            if (vencedor == 'X')
            {
                printf("\nO jogador X venceu a partida!\n");
            }
            else if (vencedor == 'O')
            {
                printf("\nO jogador O venceu a partida!\n");
            }
            else
            {
                printf("\nA partida terminou em empate!\n");
            }
            break;
        }

        case 4:
            exibirRanking();
            break;

        case 0:
            printf("\nSaindo...\n");
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }

    } while (opcao != 0);

    // Desalocando a memória usada pelo array de usuários
    free(users);
    users = NULL;

    return 0;
}

// ======================================================
//      FUNÇÃO PARA INICIALIZAR A LISTA DE USUÁRIOS
// ======================================================
void inicializarListaUsuarios()
{
    // Definimos uma capacidade inicial (pode ser 2, 5, etc.)
    userCapacity = 2;
    userCount = 0;

    // Alocando espaço para userCapacity usuários
    users = (User *)malloc(userCapacity * sizeof(User));
    if (users == NULL)
    {
        printf("Erro ao alocar memoria para usuarios.\n");
        exit(1); // encerra o programa se falhar
    }
}

// ======================================================
//               FUNÇÕES DE CADASTRO/LOGIN
// ======================================================

// Cria um novo usuário (username e senha). Retorna -1 se falhar ou índice do novo usuário.
int criarLogin()
{
    printf("\n=== CRIAR LOGIN ===\n");

    // Verifica se precisamos aumentar a capacidade antes de ler o novo username
    if (userCount == userCapacity)
    {
        // dobramos a capacidade
        userCapacity *= 2;
        User *temp = (User *)realloc(users, userCapacity * sizeof(User));
        if (temp == NULL)
        {
            printf("Erro ao realocar memoria para usuarios.\n");
            return -1;
        }
        users = temp;
    }

    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    char confpassword[PASSWORD_SIZE];
    printf("Digite um nome de usuario: ");
    fgets(username, USERNAME_SIZE, stdin);
    printf("Digite uma senha: ");
    fgets(password, PASSWORD_SIZE, stdin);
    printf("Confirme a sua senha: ");
    fgets(confpassword, PASSWORD_SIZE, stdin);
    username[strcspn(username, "\n")] = '\0'; // remover \n
    password[strcspn(password, "\n")] = '\0'; // remover \n
    confpassword[strcspn(confpassword, "\n")] = '\0'; // remover \n

    // Verificar se já existe
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Esse nome de usuario ja existe. Tente outro.\n");
            return -1;
        }
    }

    if (password == confpassword)
    {
        // Armazenar no array global
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].wins = 0;

    userCount++;
    printf("Usuario criado com sucesso!\n");

        // Retorna o índice do novo usuário
    return (userCount - 1);
    } else if (password != confpassword)
    {
        printf("Senhas não são iguais");
        return -1;
    }





}

// Tenta logar usuário buscando apenas pelo nome e senha
int logarUsuario()
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    printf("Digite seu nome de usuario: ");
    fgets(username, USERNAME_SIZE, stdin);
    printf("Digite sua senha: ");
    fgets(password, PASSWORD_SIZE, stdin);
    username[strcspn(username, "\n")] = '\0';
    password[strcspn(password, "\n")] = '\0';

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) 
        {
            printf("Login bem-sucedido!\n");
            return i;
        }
    }

    printf("Usuario nao encontrado.\n");
    return -1;
}

// Exibe o ranking de todos os usuários cadastrados, ordenando por número de vitórias desc
void exibirRanking()
{
    if (userCount == 0)
    {
        printf("\nNenhum usuario cadastrado ainda.\n");
        return;
    }

    ordenarRanking();

    printf("\n=== RANKING DE VENCEDORES ===\n");
    for (int i = 0; i < userCount; i++)
    {
        printf("%d) %s - %d vitorias\n", i + 1, users[i].username, users[i].wins);
    }
    printf("\n");
}

// Ordena o array global de usuários em ordem decrescente de vitórias (bubble sort simples)
void ordenarRanking()
{
    // Bubble sort apenas para exemplo. Em projetos maiores, use algoritmos mais eficientes.
    for (int i = 0; i < userCount - 1; i++)
    {
        for (int j = 0; j < userCount - i - 1; j++)
        {
            if (users[j].wins < users[j + 1].wins)
            {
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
}

// ======================================================
//               FUNÇÕES DO JOGO DA VELHA
// ======================================================

// Joga uma partida exigindo que P1 e P2 já estejam logados (indices). Retorna 'X' se X ganhar, 'O' se O ganhar ou 'D' se empatar
char jogarPartidaComLogin(int indexP1, int indexP2)
{
    char tabuleiro[ROWS][COLS];
    inicializarTabuleiro(tabuleiro);

    char jogadorAtual = 'X';
    bool partidaRodando = true;
    char vencedor = 'D'; // 'D' para empate por padrão

    while (partidaRodando)
    {
        exibirTabuleiro(tabuleiro);

        int linha, coluna;

        if (jogadorAtual == 'X')
        {
            printf("%s (X), escolha a linha [0..2]: ", users[indexP1].username);
            scanf("%d", &linha);
            printf("Escolha a coluna [0..2]: ");
            scanf("%d", &coluna);
        }
        else
        {
            printf("%s (O), escolha a linha [0..2]: ", users[indexP2].username);
            scanf("%d", &linha);
            printf("Escolha a coluna [0..2]: ");
            scanf("%d", &coluna);
        }

        // Validação simples
        if (linha < 0 || linha >= ROWS || coluna < 0 || coluna >= COLS)
        {
            printf("\nPosicao invalida! Tente novamente.\n");
            continue;
        }

        // Tenta fazer jogada
        if (fazerJogada(tabuleiro, linha, coluna, jogadorAtual))
        {
            // Verifica vitória
            if (verificarVitoria(tabuleiro, jogadorAtual))
            {
                exibirTabuleiro(tabuleiro);
                vencedor = jogadorAtual;
                partidaRodando = false;
            }
            // Verifica empate
            else if (verificarEmpate(tabuleiro))
            {
                exibirTabuleiro(tabuleiro);
                vencedor = 'D'; // empate
                partidaRodando = false;
            }
            else
            {
                // Alterna jogador
                jogadorAtual = (jogadorAtual == 'X') ? 'O' : 'X';
            }
        }
        else
        {
            printf("\nPosicao ja ocupada! Tente novamente.\n");
        }
    }
    return vencedor;
}

// Joga uma partida sem login (sem registrar vitórias). Retorna 'X', 'O' ou 'D'
char jogarPartidaSemLogin()
{
    char tabuleiro[ROWS][COLS];
    inicializarTabuleiro(tabuleiro);

    char jogadorAtual = 'X';
    bool partidaRodando = true;
    char vencedor = 'D'; // assume empate se ninguém vencer

    while (partidaRodando)
    {
        exibirTabuleiro(tabuleiro);

        int linha, coluna;

        printf("Jogador %c, escolha a linha [0..2]: ", jogadorAtual);
        scanf("%d", &linha);
        printf("Escolha a coluna [0..2]: ");
        scanf("%d", &coluna);

        // Validação simples
        if (linha < 0 || linha >= ROWS || coluna < 0 || coluna >= COLS)
        {
            printf("\nPosicao invalida! Tente novamente.\n");
            continue;
        }

        // Tenta fazer jogada
        if (fazerJogada(tabuleiro, linha, coluna, jogadorAtual))
        {
            // Verifica vitória
            if (verificarVitoria(tabuleiro, jogadorAtual))
            {
                exibirTabuleiro(tabuleiro);
                vencedor = jogadorAtual;
                partidaRodando = false;
            }
            // Verifica empate
            else if (verificarEmpate(tabuleiro))
            {
                exibirTabuleiro(tabuleiro);
                vencedor = 'D'; // empate
                partidaRodando = false;
            }
            else
            {
                // Alterna jogador
                jogadorAtual = (jogadorAtual == 'X') ? 'O' : 'X';
            }
        }
        else
        {
            printf("\nPosicao ja ocupada! Tente novamente.\n");
        }
    }
    return vencedor;
}

// Inicializa todas as casas do tabuleiro com espaço em branco
void inicializarTabuleiro(char tabuleiro[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            tabuleiro[i][j] = ' ';
        }
    }
}

// Mostra o tabuleiro no console
void exibirTabuleiro(const char tabuleiro[ROWS][COLS])
{
    printf("\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf(" %c ", tabuleiro[i][j]);
            if (j < COLS - 1)
                printf("|");
        }
        printf("\n");
        if (i < ROWS - 1)
        {
            printf("---+---+---\n");
        }
    }
    printf("\n");
}

// Marca a jogada se a posicao estiver vazia, caso contrário retorna false
bool fazerJogada(char tabuleiro[ROWS][COLS], int linha, int coluna, char jogador)
{
    if (tabuleiro[linha][coluna] == ' ')
    {
        tabuleiro[linha][coluna] = jogador;
        return true;
    }
    return false;
}

// Verifica se o jogador atual ganhou
bool verificarVitoria(const char tabuleiro[ROWS][COLS], char jogador)
{
    // Linhas
    for (int i = 0; i < ROWS; i++)
    {
        if (tabuleiro[i][0] == jogador &&
            tabuleiro[i][1] == jogador &&
            tabuleiro[i][2] == jogador)
        {
            return true;
        }
    }

    // Colunas
    for (int j = 0; j < COLS; j++)
    {
        if (tabuleiro[0][j] == jogador &&
            tabuleiro[1][j] == jogador &&
            tabuleiro[2][j] == jogador)
        {
            return true;
        }
    }

    // Diagonais
    if (tabuleiro[0][0] == jogador &&
        tabuleiro[1][1] == jogador &&
        tabuleiro[2][2] == jogador)
    {
        return true;
    }
    if (tabuleiro[0][2] == jogador &&
        tabuleiro[1][1] == jogador &&
        tabuleiro[2][0] == jogador)
    {
        return true;
    }

    return false;
}

// Verifica se todas as posições estão ocupadas (e não houve vitória)
bool verificarEmpate(const char tabuleiro[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (tabuleiro[i][j] == ' ')
            {
                return false;
            }
        }
    }
    return true;
}