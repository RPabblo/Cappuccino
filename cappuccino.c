#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <ctype.h>

#define COR1 4
#define COR2 14
#define COR3 2
#define COR4 1
#define CORPADRAO 15

void mostrar_regras(void);
int alocaMatrizes(int x, int y, int(**aptr)[x][y], int(**aptr2)[x][y]);
int preencheMatrizes(int x, int y, int tabuleiro[x][y], int identificador[x][y]);
void exibeTabuleiro(int x, int y, int tabuleiro[x][y], int identificador[x][y]);
void textoColorido(int valor, int jogador);
int posicaoMatriz(int *x, int *y);
int validaPeca(int x, int y, int identificador[x][y], int jogador, int i, int j);
int moverPeca(int x, int y, int tabuleiro[x][y], int identificador[x][y], int jogador, int i_inicial, int j_inicial, int *j1, int *j2, int *j3, int *j4);
int trocaTurno(int x, int y, int tabuleiro[x][y], int identificador[x][y], int *jog_atual, int *j1, int *j2, int *j3, int *j4, int *j1_isolado, int *j2_isolado, int *j3_isolado, int *j4_isolado);
int verificaVitoria(int j1, int j2, int j3, int j4);


int main(void)
{
    // *Jogo cappucino* //

    // *Inicialização das matrizes*//
    int i = 0, j = 0, jog_atual = 1, x = 5, y = 5;
    int j1 = 5, j2 = 5, j3 = 5, j4 = 5;
    int j1_isolado = 0, j2_isolado = 0, j3_isolado = 0, j4_isolado = 0;
    
    printf("\n      CAPUCCINO\n    =-=-=-=-=-=-=\n");
    mostrar_regras();

    int (*aptr)[x][y];
    int (*aptr2)[x][y];

    if(alocaMatrizes(x, y, &aptr, &aptr2) == 1) 
        return 1;
    preencheMatrizes(x, y, *aptr, *aptr2);

    // *Loop principal, onde a maior parte do jogo acontecerá* //
    for(;;){
        printf("Vez do jogador ");
        textoColorido(jog_atual, jog_atual);
        switch(jog_atual) {
            case 1: {printf(": Vermelho\n\n"); break;}
            case 2: {printf(": Amarelo\n\n"); break;}
            case 3: {printf(": Verde\n\n"); break;}
            case 4: {printf(": Azul\n\n"); break;}
            default: return 1;
        }

        exibeTabuleiro(x, y, *aptr, *aptr2);
        for(;;) {
            if(posicaoMatriz(&i, &j) == 10) 
                printf("Posicao fora do tabuleiro!\n");
            else {
                if(validaPeca(x, y, *aptr2, jog_atual, i, j) == 0) {
                    if(moverPeca(x, y, *aptr, *aptr2, jog_atual, i, j, &j1, &j2, &j3, &j4) == 0)
                        break;
                }
            }
        }
        if(trocaTurno(x, y, *aptr, *aptr2, &jog_atual, &j1, &j2, &j3, &j4, &j1_isolado, &j2_isolado, &j3_isolado, &j4_isolado) == 1)
            break;
    }

    free(aptr);
    free(aptr2);

    return 0;
}


void mostrar_regras(void)
{
    char pergunta;
    for(;;){
        printf("Deseja ler as regras? (S/N): ");
        scanf("%c", &pergunta);
        getchar();

        pergunta = toupper(pergunta);
        if(pergunta == 'S') {
            printf("\n      REGRAS\n    =-=-=-=-=-=\n\n");
            printf(" Cappuccino e um jogo em um tabuleiro 5 x 5, onde 4 jogadores com 5 pecas cada\n"
            "disputam a vitoria. Os cantos e o meio do tabuleiro nao possuem pecas.\n"
            " As pecas se movimentam como o rei no xadres, sendo oito direcoes possiveis.\n\n"

            " Cada jogador pode capturar um copo ou pilha adjacente a sua,\n"
            "desde que a pilha a ser capturada seja menor ou igual a sua.\n"
            " Se, por exemplo, o copo no topo da pilha for AZUL, o jogador 4 a controla.\n\n"

            " Se todas as pilhas adjacentes a uma pilha estiverem vazias ou forem maiores, essa pilha esta isolada.\n"
            " Se todas as pilhas de um jogador estiverem isoladas, o jogador nao pode jogar.\n\n"

            " Ao fim do jogo, quando todos os jogadores estiverem impossibilitados de jogar,\n"
            " vence quem tiver a MAIOR SOMA DE PILHAS do jogo!\n\n");

            printf("    =-=-=-=-=-=-=\n      BOA SORTE!\n    =-=-=-=-=-=-=\n\n");
            break;
        }
        else if(pergunta == 'N') {
            printf("\n    =-=-=-=-=-=-=\n      BOA SORTE!\n    =-=-=-=-=-=-=\n\n");
            break;
        }
    }
}

int alocaMatrizes(int x, int y, int(**aptr)[x][y], int(**aptr2)[x][y])
{
    *aptr = malloc(sizeof (**aptr) );
    *aptr2 = malloc(sizeof(**aptr2));

    //* Se a alocação falhar, retorna 1, tanto aqui quanto na main e o programa encerra *//
    if(*aptr == NULL || *aptr2 == NULL) {
        printf("Erro ao alocar matrizes.\n");
        return 1; }
    return 0;
}

int preencheMatrizes(int x, int y, int tabuleiro[x][y], int identificador[x][y])
{
    // *A matriz tabuleiro guarda o tamanho das pilhas no tabuleiro.* //
    // *É ela que aparece na tela durante o jogo* //
    // *A matriz identificador guarda o numero da pilha corresponde aos jogadores, de 1 a 4*//
    int n, i, j, cont, j1 = 0, j2 = 0, j3 = 0, j4 = 0;
    srand((unsigned) time(0));

    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            // *Os cantos e o centro não possuem peças; portanto, recebem valor 0 nas matrizes.* //
            if(i == 0 && j == 0)      {tabuleiro[i][j] = 0; identificador[i][j] = 0;}
            else if(i == 0 && j == 4) {tabuleiro[i][j] = 0; identificador[i][j] = 0;}
            else if(i == 2 && j == 2) {tabuleiro[i][j] = 0; identificador[i][j] = 0;}
            else if(i == 4 && j == 0) {tabuleiro[i][j] = 0; identificador[i][j] = 0;}
            else if(i == 4 && j == 4) {tabuleiro[i][j] = 0; identificador[i][j] = 0;}
            else {
                cont = 0;
                while(cont != 1) {
                    n = (rand() %4) + 1;
                    switch (n) {
                        case 1: {
                            if(j1 < 5)
                            {   identificador[i][j] = 1;
                                j1 += 1;
                                cont += 1; }
                            break; 
                        } 
                        case 2: {
                            if(j2 < 5)
                            {   identificador[i][j] = 2;
                                j2 += 1;
                                cont += 1; }
                            break;
                        }
                        case 3: {
                            if(j3 < 5)
                            {   identificador[i][j] = 3;
                                j3 += 1;
                                cont += 1; }
                            break;
                        }
                        case 4: {
                            if(j4 < 5)
                            {   identificador[i][j] = 4;
                                j4 += 1;
                                cont += 1; }
                            break;
                        }
                        default:
                            break;
                    }
                    tabuleiro[i][j] = 1;
                }
            }
        }
    }
}

void exibeTabuleiro(int x, int y, int tabuleiro[x][y], int identificador[x][y])
{
    printf("   A   B   C   D   E\n");
    for(int i = 0; i < x; i++)
    {
        printf("%d ", i + 1);
        for(int j = 0; j < y; j++)
        {
            if(i == 0 && j == 0)      printf("   |");
            else if(i == 4 && j == 0) printf("   |");
            else if(i == 2 && j == 2) printf("   |");
            else if(i == 0 && j == 4) ;
            else if(i == 4 && j == 4) ;
            else {
                printf(" ");
                if(j == 4) {
                textoColorido(tabuleiro[i][j], identificador[i][j]); 
                printf("  "); 
                }
                else {
                    textoColorido(tabuleiro[i][j], identificador[i][j]);
                    printf(" |"); 
                }
            }
        }
        if(i != 4)
            printf("\n  ---.---.---.---.---\n");
    }
}

void textoColorido(int valor, int jogador) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(jogador) {
        case 0: {printf(" "); return;}
        case 1: SetConsoleTextAttribute(hConsole, COR1); break;
        case 2: SetConsoleTextAttribute(hConsole, COR2); break;
        case 3: SetConsoleTextAttribute(hConsole, COR3); break;
        case 4: SetConsoleTextAttribute(hConsole, COR4); break; 
    }
    printf("%d", valor);
    SetConsoleTextAttribute(hConsole, CORPADRAO);
}

int posicaoMatriz(int *x, int *y)
{
    // *Função que tranforma input letra e numero em posiçoes do tabuleiro* //
    // *Ex.: B3  é tranformada em (2, 1)* //
    char * input = malloc(3);

    printf("\n\nPosicao de sua peca (Letra/Numero): ");
    scanf("%[^\n]s", input);
    getchar();
    
    if(isalpha(!input[0]))
        return 10;
    char chari = toupper(input[0]);

    if(chari == 'A')      *y = 0;
    else if(chari == 'B') *y = 1;
    else if(chari == 'C') *y = 2;
    else if(chari == 'D') *y = 3;
    else if(chari == 'E') *y = 4;
    else {free(input); return 10;}

    char charj = input[1];
    if(charj == '1')      *x = 0;
    else if(charj == '2') *x = 1;
    else if(charj == '3') *x = 2;
    else if(charj == '4') *x = 3;
    else if(charj == '5') *x = 4;
    else {free(input); return 10;}
    
    free(input);
}

int validaPeca(int x, int y, int identificador[x][y], int jogador, int i, int j)
{
    // *Verifica se a posição da peça escolhida pertence ao jogador da vez* //
    if(identificador[i][j] == jogador) 
        return 0;
    else 
        return 1;
}

int moverPeca(int x, int y, int tabuleiro[x][y], int identificador[x][y], int jogador, int i_inicial, int j_inicial, int *j1, int *j2, int *j3, int *j4)
{
    // *Função que recebe um input numérico de 1 a 9 e tranforma em coordenadas para as matrizes*//
    int input, i_dest, j_dest;

    printf("\n7    8    9\n\n");
    printf("4         6\n\n");
    printf("1    2    3\n\n");
    printf("Direcao de destino: ");
    scanf("%d", &input);
    getchar();

    switch (input) {
        case 1: {
            i_dest = i_inicial + 1; j_dest = j_inicial - 1; break; }
        case 2: {
            i_dest = i_inicial + 1; j_dest = j_inicial; break; }
        case 3: {
            i_dest = i_inicial + 1; j_dest = j_inicial + 1; break; }
        case 4: {
            i_dest = i_inicial; j_dest = j_inicial - 1; break; }
        case 5: {
            printf("Posicao de destino igual a inicial.\n"); return 1; }
        case 6: {
            i_dest = i_inicial; j_dest = j_inicial + 1; break; }
        case 7: {
            i_dest = i_inicial - 1; j_dest = j_inicial - 1; break; }
        case 8: {
            i_dest = i_inicial - 1; j_dest = j_inicial; break; }
        case 9: {
            i_dest = i_inicial - 1; j_dest = j_inicial + 1; break; }    
        default: {printf("Posicao invalida.\n"); return 1;}
    }

    if(i_dest < 0 || i_dest > 4) {
        printf("Posicao final fora do tabuleiro.\n"); return 1; }
    if(j_dest < 0 || j_dest > 4) {
        printf("Posicao final fora do tabuleiro.\n"); return 1; }
    
    if(identificador[i_dest][j_dest] == 0) {
        printf("Posicao vazia.\n"); return 1; }

    if(tabuleiro[i_dest][j_dest] > tabuleiro[i_inicial][j_inicial]) {
        printf("A pilha do destino eh maior que a sua :(\n"); return 1; }
    
    // *Se todas as condições anteriores forem falsas, o movimento é válido e ocorrerá * //
    switch (identificador[i_dest][j_dest]) {
        case 1: {*j1 -= 1; break;}
        case 2: {*j2 -= 1; break;}
        case 3: {*j3 -= 1; break;}
        case 4: {*j4 -= 1; break;}
        default: break; 
    }

    tabuleiro[i_dest][j_dest] += tabuleiro[i_inicial][j_inicial];
    tabuleiro[i_inicial][j_inicial] = 0;
    identificador[i_dest][j_dest] = jogador;
    identificador[i_inicial][j_inicial] = 0;

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
    return 0;
}

int trocaTurno(int x, int y, int tabuleiro[x][y], int identificador[x][y], int *jog_atual, int *j1, int *j2, int *j3, int *j4, int *j1_isolado, int *j2_isolado, int *j3_isolado, int *j4_isolado)
{
    // *Função que passa a vez para outro jogador* //
    int i, j, i_inicial, j_inicial, i_dest, j_dest;
    int ok = 0, cont = 0, qtd_de_zero = 0;
    int j1_0 = 0, j2_0 = 0, j3_0 = 0, j4_0 = 0;

    for(i_inicial = 0; i_inicial < x; i_inicial++)
    {
        for(j_inicial = 0; j_inicial < y; j_inicial++)
        {
            cont = 0;
            qtd_de_zero = 0;

            // *Se a posição contiver uma peça válida, fará a validação das peças adjacentes* //
            // *em todas as direções válidas* //
            if(identificador[i_inicial][j_inicial] != 0)
            {
                for(i = -1; i < 2; i++)
                {
                    for(j = -1; j < 2; j++)
                    {
                        if(i == 0 && j == 0)
                            ;
                        else {
                            i_dest = i_inicial + i;
                            j_dest = j_inicial + j;
                            
                            // *Aqui as posições fora do tabuleiro não serão levadas em consideração na contagem.* //
                            if(i_dest >= 0 && i_dest <= 4) {
                                if(j_dest >= 0 && j_dest <= 4) {
                                    cont += 1;
                                    if(identificador[i_dest][j_dest] == 0 || tabuleiro[i_dest][j_dest] > tabuleiro[i_inicial][j_inicial]) 
                                        qtd_de_zero += 1;
                                }
                            }
                        }
                    }
                }
                
                // *Se todas as peças adjacentes forem vazias ou a pilha for maior, * //
                // *a peça não poderá se movimentar* //
                if(qtd_de_zero == cont) {
                    switch (identificador[i_inicial][j_inicial]) {
                        case 1: {j1_0 += 1; break;}
                        case 2: {j2_0 += 1; break;}
                        case 3: {j3_0 += 1; break;}
                        case 4: {j4_0 += 1; break;}
                        default: break;
                    }
                }
            }
        }
    }

    // *Se todas as peças de um jogador não puderem se movimentar, o jogador estará "isolado"* //
    if(*j1 == j1_0 || *j1 == 0) {printf("Jogador 1 incapaz de jogar.\n"); *j1_isolado = 1;}
    if(*j2 == j2_0 || *j2 == 0) {printf("Jogador 2 incapaz de jogar.\n"); *j2_isolado = 1;}
    if(*j3 == j3_0 || *j3 == 0) {printf("Jogador 3 incapaz de jogar.\n"); *j3_isolado = 1;}
    if(*j4 == j4_0 || *j4 == 0) {printf("Jogador 4 incapaz de jogar.\n"); *j4_isolado = 1;}

    if(verificaVitoria(*j1_isolado, *j2_isolado, *j3_isolado, *j4_isolado) == 1) 
    {
        int tmp;
        int maior, j1_pontos = 0, j2_pontos = 0, j3_pontos = 0, j4_pontos = 0;

        // *Soma os pontos dos jogadores* //
        for(i = 0; i < x; i++)
        {
            for(j = 0; j < y; j++)
            {
                if(identificador[i][j] != 0) {
                    switch (identificador[i][j]) {
                    case 1: {j1_pontos += tabuleiro[i][j]; break;}
                    case 2: {j2_pontos += tabuleiro[i][j]; break;}
                    case 3: {j3_pontos += tabuleiro[i][j]; break;}
                    case 4: {j4_pontos += tabuleiro[i][j]; break;}
                    default: break;
                    }
                }
            }
        }

        // *Ordena em ordem decrescente as pontuações dos jogadores em um array* //
        int a[] = {j1_pontos, j2_pontos, j3_pontos, j4_pontos};

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                if (a[j] < a[i]) {
                    tmp = a[i];
                    a[i] = a[j];
                    a[j] = tmp;
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            printf("Em %do lugar, com %d pontos: ", i + 1, a[i]);
            if(a[i] == j1_pontos) printf("Jogador 1!\n");
            else if(a[i] == j2_pontos) printf("Jogador 2!\n");   
            else if(a[i] == j3_pontos) printf("Jogador 3!\n");   
            else if(a[i] == j4_pontos) printf("Jogador 4!\n");
        }

        for(i = 0; i < 23; i++)
            printf("=-");
        printf("\n");

        return 1;
    }

    // *Se ainda não há vitória, agora sim, passa a vez para um jogador que possa movimentar suas peças* //
    while(ok == 0) {
        switch(*jog_atual) {
            case 1: {
                if(*j2_isolado == 0) 
                    ok = 1; 
                *jog_atual += 1;
                break;
            }
            case 2: {
                if(*j3_isolado == 0) 
                    ok = 1; 
                *jog_atual += 1;
                break;
            }
            case 3: {
                if(*j4_isolado == 0) 
                    ok = 1; 
                *jog_atual += 1;
                break;
            }
            case 4: {
                if(*j1_isolado == 0) 
                    ok = 1; 
                *jog_atual = 1;
                break;
            }
            default:
                break;
        }
    }
    return 0;
}

int verificaVitoria(int j1, int j2, int j3, int j4)
{
    int cont = 0;
    if(j1 == 1) cont += 1;
    if(j2 == 1) cont += 1;
    if(j3 == 1) cont += 1;
    if(j4 == 1) cont += 1;

    if(cont == 4) {
        printf("\n\n=-=-=-=-=-=-=-=\n  FIM DE JOGO\n=-=-=-=-=-=-=-=\n");
        return 1;
    }
    else return 0;
}

// *Autor: Rwann Pabblo* //