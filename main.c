#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W 15      // largura (odd)
#define H 11      // altura (odd)

int maze[H][W];     // 0 = parede, 1 = caminho
int player_r = 1;     // posição inicial do jogador
int player_c = 1;
int exit_r = H - 2;
int exit_c = W - 2;

// gera um número aleatório entre 0 e n-1
static inline int rand_range(int n) { return rand() % n; }

// troca valores (usado para shuffle)
static void swap_int(int *a, int *b) { int t = *a; *a = *b; *b = t; }

// embaralha um vetor de 4 inteiros (Fisher-Yates)
static void shuffle4(int arr[4]) {
    for (int i = 3; i > 0; --i) {
        int j = rand_range(i + 1);
        swap_int(&arr[i], &arr[j]);
    }
}

// DFS backtracker para geração de labirinto
static void carve(int r, int c) {
    maze[r][c] = 1;

    int dirs[4][2] = { {2,0}, {-2,0}, {0,2}, {0,-2} };
    int order[4] = {0,1,2,3};
    shuffle4(order);

    for (int idx = 0; idx < 4; ++idx) {
        int dr = dirs[order[idx]][0];
        int dc = dirs[order[idx]][1];
        int nr = r + dr;
        int nc = c + dc;

        if (nr > 0 && nr < H-1 && nc > 0 && nc < W-1 && maze[nr][nc] == 0) {
            // quebre a parede entre o atual e o vizinho
            maze[r + dr/2][c + dc/2] = 1;
            carve(nr, nc);
        }
    }
}

// imprime o labirinto no console
static void print_maze() {
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            char ch;
            if (r == player_r && c == player_c) {
                ch = '@';               // jogador
            } else if (r == exit_r && c == exit_c) {
                ch = 'E';               // saída
            } else if (maze[r][c] == 0) {
                ch = '#';               // parede
            } else {
                ch = ' ';               // caminho
            }
            putchar(ch);
        }
        putchar('\n');
    }
}

// verifica se a posição é caminho (dentro do labirinto)
static int is_path(int r, int c) {
    if (r < 0 || r >= H || c < 0 || c >= W) return 0;
    return maze[r][c] == 1;
}

int main(void) {
    srand((unsigned)time(NULL));

    // inicializa o labirinto com paredes
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            maze[i][j] = 0;

    // gera o labirinto a partir de (1,1)
    maze[1][1] = 1;
    carve(1, 1);

    // garante que a saída é caminho
    maze[exit_r][exit_c] = 1;

    // começa no (1,1)
    player_r = 1;
    player_c = 1;

    printf("Labirinto Gerado com DFS (labirinto simples em ASCII)\n");
    printf("Use WASD para mover. Chegue até a saída 'E'. Digite 'q' para sair.\n");
    printf("Aperte Enter após cada movimento.\n\n");

    while (1) {
        print_maze();
        if (player_r == exit_r && player_c == exit_c) {
            printf("Parabéns! Você saiu do labirinto.\n");
            break;
        }

        printf("Movimento (W/A/S/D) ou 'q' para sair: ");
        int ch = getchar();
        // descartar possível newline do buffer
        while (getchar() != '\n') { /* apenas consome até newline */ }

        if (ch == 'q' || ch == 'Q') {
            printf("Saindo do jogo.\n");
            break;
        }

        int nr = player_r;
        int nc = player_c;
        if (ch == 'w' || ch == 'W') { nr = player_r - 1; }
        else if (ch == 'a' || ch == 'A') { nc = player_c - 1; }
        else if (ch == 's' || ch == 'S') { nr = player_r + 1; }
        else if (ch == 'd' || ch == 'D') { nc = player_c + 1; }
        else {
            printf("Comando inválido. Use W/A/S/D ou q.\n");
            continue;
        }

        if (is_path(nr, nc)) {
            player_r = nr;
            player_c = nc;
        } else {
            printf("Movimento inválido (parede). Tente novamente.\n");
        }
        // limpa eventual linha em excesso (se houver)
    }

    return 0;
}