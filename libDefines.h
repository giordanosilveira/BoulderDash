#ifndef __DEFINES__
#define __DEFINES__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#define FPS 60.0                                            //Taxa de atualização do jogo
#define DELAY_MORTE_JOGADOR 20                              //Delay para a verificação da morte do jogador
#define ATUALIZACAO_OBJETO 10                               //Delay para a atualização das posições das pedras
#define DELAY_EXPLOSAO 40                                   //Delay para a troca de sprites das explosões
#define DELAY_ANIMACAO 84                                   //Delay para a troca de sprites do jogador
#define DELAY_ANIMACAO_DIAMANTE 160                         //Delay para a troca de sprites dos diamantes
#define DELAY_EASTER_EGG 1500                               //Tempo para aparecer o easter egg
#define TEMPORIZADOR 60                                     //Responsavel pelo tempo do jogo
#define TEMPO_TOTAL 120                                     //Tempo que o jogador tem para passar de fase 

#define MULT_DISPLAY 2                                      //Número para multiplicar o buffer para preencher formar o display
#define CORRECAO_DISPLAY 23                                 //Para deixar uma sobra no display para a HUD
#define LARGURA_BUFFER 640                                  //Largura do buffer
#define ALTURA_BUFFER ( 352 + CORRECAO_DISPLAY)             //Altura do buffer
#define LARGURA_DISPLAY (LARGURA_BUFFER * MULT_DISPLAY)     //Largura do display (640 * 2)
#define ALTURA_DISPLAY (ALTURA_BUFFER * MULT_DISPLAY)       //Altura do display (352 * 2)

#define PONTUACAO_MINIMA_F1 10                              //Pontuação mínima no começo da fase
#define DIAMANTES_MINIMOS_F1 12                             //Diamantes mínimos para passar de fase

#define N_VARIACOES_EXPLOSAO 8                              //Número de sprites de explosão no sprite Boulder_dash.png
#define N_VARIACOES_DIAMANTE 8                              //Número de sprites de diamantes no sprite Boulder_dash.png
#define N_VARIACOES_PLAYER 21                               //Número de sprites do player no sprite Boulder_dash.png
#define N_ITENS_MAPA 6                                      //Número de sprites de itens do mapa no sprite Boulder_dash.png
#define ALTURA_BITMAP_PLAYER 15                             //Altura do sprite do player
#define LARGURA_BITMAP_PLAYER 14                            //Largura do sprite do player
#define ALTURA_BITMAP_BLOCO 16                              //Altura dos sprites dos blocos do mapa 
#define LARGURA_BITMAP_BLOCO 16                             //Largura dos sprites dos blocos do mapa

#define COORD_X_PORTA 16                                    //Coordenada da porta (linha)
#define COORD_Y_PORTA 38                                    //Coordenada da porta (coluna)
#define N_DIAMANTES 19                                      //Número de diamantes pelo mapa (previamente contados)
#define N_ROCHAS 105                                        //Número de rochas do mapa previamentes contadas
#define VAGO2 -2                                            //Codificação para dizer que a posição da matriz está ocupada por um diamante                
#define VAGO -1                                             //Codificação para dizer que a posição da matriz está ocupada por uma rocha
#define PAREDE 0                                            //Codificação para a parede externa
#define NADA 1                                              //Codificação para dizer que não tem nada na casa da matriz
#define TIJOLO 2                                            //Codificação para os tijolos internos
#define PLAYER 3                                            //Codificação para o player
#define ROCHA 4                                             //Codificação para a rochas do mapa
#define DIAMANTE 5                                          //Codificação para os diamantes do mapa
#define PORTA 6                                             //Codificação da porta do mapa
#define TERRA 7                                             //Codificação para as terras do mapa
#define EXPLODIDO 8                                         //Codificação para dizer que a posição da matriz está ocupada por uma explosão

#define ESQUERDA -1                                         //Codificação quando o player/objeto vai para a esquerda
#define DIREITA 1                                           //Codificação quando o player/objeto vai para a direita
#define CIMA -1                                             //Codificação quando o player/objeto vai para a cima
#define BAIXO 1                                             //Codificação quando o player/objeto vai para a baixo
#define PLAYER_COORD_X 2                                    //Posição inicial do player no mapa (linha)
#define PLAYER_COORD_Y 3                                    //Posição inicial do player no mapa (coluna)

#define KEY_SEEN 1                                            
#define KEY_RELEASED 2

//Struct que comanda todos os sprites do mapa
typedef struct t_sprites {
    ALLEGRO_BITMAP* bitmap_sprites;
    ALLEGRO_BITMAP* player[21];
    ALLEGRO_BITMAP* mapa[6];
    ALLEGRO_BITMAP* explosao[8]; 
    ALLEGRO_BITMAP* diamante[8]; 
} t_sprites;

//Struct que comanda o player
typedef struct t_player {
    int tipo;
    int coord_x, coord_y;
    int direcao;
} t_player;

//Struct que comanda os obejtos que tem "física" no mapa 
typedef struct t_objeto {
    int coord_x, coord_y;
    int q_obj;
    bool movimentando;
    bool morto;
} t_objeto;

//Struct que comanda todos os itens do mapa
//Qual item é e se ele esta na tela
typedef struct t_mapa{
    int na_tela, item, codificacao;
} t_mapa;

//Struct que comanda alguns delays pelo jogo
typedef struct t_contaFrames
{
    int atualizacao_objeto;
    int evento_morte;
    int animacao_player;
    int animacao_diamante;
    int morte;
    int tempo;
} t_contaFrames;

enum estados_t {MENU, JOGANDO, FIMDAPARTIDA, GAMEOVER, TERMINOU};     //Possíveis estados do jogo
extern enum estados_t estado_jogo;

extern t_sprites sheet;                                            //Sprites de todos os itens do mapa
extern t_mapa **mapa;                                              //Matriz do mapa

extern ALLEGRO_TIMER *timer;                                       //Temporizador 
extern ALLEGRO_EVENT_QUEUE *fila;                                  //fila de eventos
extern ALLEGRO_DISPLAY *display;                                   //Tela do jogo
extern ALLEGRO_BITMAP *buffer;                                     //Bitmap para um pré print dos itens do jogo
extern ALLEGRO_FONT *font;                                         //fonte de texto
extern ALLEGRO_SAMPLE *sample_coin;
extern ALLEGRO_SAMPLE *sample_explosion;
extern ALLEGRO_SAMPLE *sample_select;

extern int morreu;

extern int tempo;
extern int score;                                                  //última pontuação do jogador
extern int pontos;                                                 //Pontuação do jogador
extern int contador_diamantes;                                     //contador de diamantes
extern int diamante_minimos;                                       //Diamantes mínimos necessários para mudar a pontuação
extern int pontuacao_minima;

#endif