// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 6
#define NUM_MISSOES 5
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;


void atribuirMissao(char* destino, const char* missoes[], int totalMissoes, const char* corJogador);
int verificarMissao(const char* missao, const char* corJogador, Territorio* mapa, int tamanhoMapa);
void exibirMissao(const char* missao);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanhoMapa);
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2);
void limparBuffer();

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

    void atribuirMissao(char* destino, const char* missoes[], int totalMissoes, const char* corJogador) {
        int missaoValida = 0;
        do {
            int indiceSorteado = rand() % totalMissoes;
            strcpy(destino, missoes[indiceSorteado]);

            if ((strcmp(destino, "Eliminar todas as tropas do jogador Azul") == 0 && strcmp(corJogador, "Azul") == 0) ||
                (strcmp(destino, "Eliminar todas as tropas do jogador Vermelho") == 0 && strcmp(corJogador, "Vermelho") == 0)) {
                missaoValida = 0;
            } else {
                missaoValida = 1;
            }
        } while (!missaoValida);
    }

    void exibirMissao(const char* missao) {
        printf("    -> \"%s\"\n", missao);
    }

    void exibirMapa(Territorio* mapa, int tamanhoMapa) {
        printf("\nMAPA ATUAL:\n");
        for (int i = 0; i < tamanhoMapa; i++) {
            printf("ID: %d | Territorio: %-10s | Cor: %-9s | Tropas: %d\n",
                i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
        }
    }

    void atacar(Territorio* atacante, Territorio* defensor) {
        printf("\n*** ATAQUE: %s (%d tropas) ataca %s (%d tropas) ***\n",
            atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
            
        int dadoAtacante = rand() % 6 + 1;
        int dadoDefensor = rand() % 6 + 1;

        printf("Rolagem de dados -> Atacante: %d | Defensor: %d\n", dadoAtacante, dadoDefensor);

        if (dadoAtacante > dadoDefensor) {
            printf("VITORIA DO ATACANTE! %s conquistou %s.\n", atacante->nome, defensor->nome);
            strcpy(defensor->cor, atacante->cor);
            int tropasMovidas = (atacante->tropas - 1) / 2;
            if (tropasMovidas < 1) tropasMovidas = 1;
            
            defensor->tropas = tropasMovidas;
            atacante->tropas -= tropasMovidas;
            
        } else {
            printf("VITORIA DO DEFENSOR! %s perdeu o ataque e uma tropa.\n", atacante->nome);
            atacante->tropas--;
        }
    }

    int verificarMissao(const char* missao, const char* corJogador, Territorio* mapa, int tamanhoMapa) {
        if (strcmp(missao, "Conquistar 4 territorios no total") == 0) {
            int count = 0;
            for (int i = 0; i < tamanhoMapa; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0) {
                    count++;
                }
            }
            return count >= 4;
        }
        
        if (strcmp(missao, "Eliminar todas as tropas do jogador Vermelho") == 0) {
            for (int i = 0; i < tamanhoMapa; i++) {
                if (strcmp(mapa[i].cor, "Vermelho") == 0) return 0;
            }
            return 1;
        }

        if (strcmp(missao, "Eliminar todas as tropas do jogador Azul") == 0) {
            for (int i = 0; i < tamanhoMapa; i++) {
                if (strcmp(mapa[i].cor, "Azul") == 0) return 0;
            }
            return 1;
        }

        if (strcmp(missao, "Conquistar o territorio 'Brasil'") == 0) {
            for (int i = 0; i < tamanhoMapa; i++) {
                if (strcmp(mapa[i].nome, "Brasil") == 0 && strcmp(mapa[i].cor, corJogador) == 0) {
                    return 1;
                }
            }
        }

        if (strcmp(missao, "Ter 10 tropas em um unico territorio") == 0) {
            for (int i = 0; i < tamanhoMapa; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 10) {
                    return 1;
                }
            }
        }

        return 0;
    }

    void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2) {
        printf("\nLiberando memoria alocada...\n");
        free(mapa);
        free(missaoJogador1);
        free(missaoJogador2);
        printf("Memoria liberada com sucesso.\n");
    }

    void limparBuffer() {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    srand(time(NULL));
    
    // Cores dos jogadores
    const char* JOGADOR_1_COR = "Azul";
    const char* JOGADOR_2_COR = "Vermelho";

    // 1. Criação do vetor de missões
    const char* missoes[NUM_MISSOES] = {
        "Conquistar 4 territorios no total",
        "Eliminar todas as tropas do jogador Vermelho",
        "Eliminar todas as tropas do jogador Azul",
        "Conquistar o territorio 'Brasil'",
        "Ter 10 tropas em um unico territorio"
    };

    // Alocação de memória para o mapa de territórios
    Territorio* mapa = (Territorio*) malloc(NUM_TERRITORIOS * sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1;
    }

    // Inicialização dos territórios
    strcpy(mapa[0].nome, "Brasil");     strcpy(mapa[0].cor, "Azul");     mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "Argentina");  strcpy(mapa[1].cor, "Azul");     mapa[1].tropas = 3;
    strcpy(mapa[2].nome, "Peru");       strcpy(mapa[2].cor, "Azul");     mapa[2].tropas = 2;
    strcpy(mapa[3].nome, "Alemanha");   strcpy(mapa[3].cor, "Vermelho"); mapa[3].tropas = 5;
    strcpy(mapa[4].nome, "Franca");     strcpy(mapa[4].cor, "Vermelho"); mapa[4].tropas = 3;
    strcpy(mapa[5].nome, "Espanha");    strcpy(mapa[5].cor, "Vermelho"); mapa[5].tropas = 2;
    
    // 3. Armazenamento da missão de cada jogador (alocação dinâmica)
    char* missaoJogador1 = (char*) malloc(100 * sizeof(char));
    char* missaoJogador2 = (char*) malloc(100 * sizeof(char));
     if (missaoJogador1 == NULL || missaoJogador2 == NULL) {
        printf("Erro ao alocar memoria para as missoes!\n");
        free(mapa);
        return 1;
    }

    // 2. Sorteio da missão para cada jogador (com validação)
    atribuirMissao(missaoJogador1, missoes, NUM_MISSOES, JOGADOR_1_COR);
    atribuirMissao(missaoJogador2, missoes, NUM_MISSOES, JOGADOR_2_COR);

    // Exibição condicional: mostra a missão apenas uma vez no início
    printf("--- BEM-VINDO AO WAR ESTRUTURADO ---\n\n");
    printf("Jogador 1 (%s), sua missao secreta e:\n", JOGADOR_1_COR);
    exibirMissao(missaoJogador1);
    printf("\n");

    printf("Jogador 2 (%s), sua missao secreta e:\n", JOGADOR_2_COR);
    exibirMissao(missaoJogador2);
    printf("\n--- O JOGO COMECou! ---\n\n");

    int turno = 1;
    int vencedor = 0;

    // Loop principal do jogo
    while (vencedor == 0) {
        exibirMapa(mapa, NUM_TERRITORIOS);
        
        const char* corJogadorAtual = (turno % 2 != 0) ? JOGADOR_1_COR : JOGADOR_2_COR;
        int numJogadorAtual = (turno % 2 != 0) ? 1 : 2;

        printf("\n--- TURNO %d: Vez do Jogador %d (%s) ---\n", turno, numJogadorAtual, corJogadorAtual);

        int idAtacante, idDefensor;
        printf("Digite o ID do territorio de ataque: ");
        scanf("%d", &idAtacante);
        limparBuffer();

        printf("Digite o ID do territorio de defesa: ");
        scanf("%d", &idDefensor);
        limparBuffer();

        // Validações do ataque
        if (idAtacante < 0 || idAtacante >= NUM_TERRITORIOS || idDefensor < 0 || idDefensor >= NUM_TERRITORIOS) {
            printf("ID de territorio invalido! Tente novamente.\n\n");
            continue;
        }
        if (strcmp(mapa[idAtacante].cor, corJogadorAtual) != 0) {
            printf("Ataque invalido! Voce so pode atacar com seus proprios territorios.\n\n");
            continue;
        }
        if (strcmp(mapa[idDefensor].cor, corJogadorAtual) == 0) {
            printf("Ataque invalido! Voce nao pode atacar seus proprios territorios.\n\n");
            continue;
        }
        if (mapa[idAtacante].tropas <= 1) {
            printf("Ataque invalido! O territorio de ataque precisa ter mais de 1 tropa.\n\n");
            continue;
        }

        atacar(&mapa[idAtacante], &mapa[idDefensor]);

        // 5. Verificação da missão
        if (numJogadorAtual == 1) {
            if (verificarMissao(missaoJogador1, JOGADOR_1_COR, mapa, NUM_TERRITORIOS)) {
                vencedor = 1;
            }
        } else {
            if (verificarMissao(missaoJogador2, JOGADOR_2_COR, mapa, NUM_TERRITORIOS)) {
                vencedor = 2;
            }
        }
        
        turno++;
        printf("\n--------------------------------------\n");
    }

    // Declara o vencedor
    printf("\n\n--- FIM DE JOGO! ---\n");
    printf("O Jogador %d (%s) cumpriu sua missao e venceu o jogo!\n", vencedor, (vencedor == 1) ? JOGADOR_1_COR : JOGADOR_2_COR);
    exibirMapa(mapa, NUM_TERRITORIOS);
    
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---



    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
