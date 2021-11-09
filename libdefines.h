#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#ifndef __DEFINES__
#define __DEFINES__

#define FPS 60.0                                            //Taxa de atualização do jogo
#define DELAY_MORTE_JOGADOR 20                              //Delay para a verificação da morte do jogador
#define ATUALIZACAO_PEDRAS 10                               //Delay para a atualização das posições das pedras

#define MULT_DISPLAY 2                                      //Número para multiplicar o buffer para preencher formar o display
#define LARGURA_BUFFER 640                                  //Largura do buffer
#define ALTURA_BUFFER 352                                   //Altura do buffer
#define CORRECAO_DISPLAY 10                                 //Para deixar uma sobra no display para a HUD
#define LARGURA_DISPLAY (LARGURA_BUFFER * MULT_DISPLAY)     //Largura do display (640 * 2)
#define ALTURA_DISPLAY (ALTURA_BUFFER * MULT_DISPLAY)       //Altura do display (352 * 2)

#define PONTUACAO_MINIMA_F1 10                              //Pontuação mínima no começo da fase
#define DIAMANTES_MINIMOS_F1 12                             //Diamantes mínimos para passar de fase

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
#define PAREDE 0                                            //Codificação para a parede externa
#define NADA 1                                          
#define TIJOLO 2                                            //Codificação para os tijolos internos
#define PLAYER 3                                            //Codificação para o player
#define ROCHA 4                                             //Codificação para a rochas do mapa
#define DIAMANTE 5                                          //Codificação para os diamantes do mapa
#define PORTA 6                                             //Codificação da porta do mapa
#define TERRA 7                                             //Codificação para as terras do mapa

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
    bool movimentando;
} t_objeto;

//Struct que comanda todos os itens do mapa
//Qual item é e se ele esta na tela
typedef struct t_mapa{
    int na_tela, item;
} t_mapa;

//Struct que comanda alguns delays pelo jogo
typedef struct t_contaFrames
{
    int atualizacao_rocha;
    int evento_morte;
    int vitoria;
} t_contaFrames;

enum estados_t {MENU, JOGANDO, FIMDAPARTIDA, GAMEOVER, TERMINOU};     //Possíveis estados do jogo
enum estados_t estado_jogo;

t_sprites sheet;                                            //Sprites de todos os itens do mapa
t_mapa **mapa;                                              //Matriz do mapa

ALLEGRO_TIMER *timer;                                       //Temporizador 
ALLEGRO_EVENT_QUEUE *fila;                                  //fila de eventos
ALLEGRO_DISPLAY *display;                                   //Tela do jogo
ALLEGRO_BITMAP *buffer;                                     //Bitmap para um pré print dos itens do jogo
ALLEGRO_FONT *font;                                         //fonte de texto

int pontos;                                                 //Pontuação do jogador
int contador_diamantes;                                     //contador de diamantes
int diamante_minimos;                                       //Diamantes mínimos necessários para mudar a pontuação
int pontuacao_minima;                                       //pontuação mínima inicial 

#endif