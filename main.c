#include <stdio.h>
#include <stdlib.h>

#include "libDefines.h"

#include "lib_init_dest.h"
#include "lib_geral.h"
#include "lib_graficos.h"
#include "lib_move.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

enum estados_t estado_jogo;

t_sprites sheet;                                            //Sprites de todos os itens do mapa
t_mapa **mapa;                                              //Matriz do mapa

ALLEGRO_TIMER *timer;                                       //Temporizador 
ALLEGRO_EVENT_QUEUE *fila;                                  //fila de eventos
ALLEGRO_DISPLAY *display;                                   //Tela do jogo
ALLEGRO_BITMAP *buffer;                                     //Bitmap para um pré print dos itens do jogo
ALLEGRO_FONT *font;                                         //fonte de texto
ALLEGRO_SAMPLE *sample_coin;
ALLEGRO_SAMPLE *sample_explosion;
ALLEGRO_SAMPLE *sample_select;

int morreu;

int tempo;
int score;                                                  //última pontuação do jogador
int pontos;                                                 //Pontuação do jogador
int contador_diamantes;                                     //contador de diamantes
int diamante_minimos;                                       //Diamantes mínimos necessários para mudar a pontuação
int pontuacao_minima;

int main () {

    FILE *arq;
    t_objeto *diamantes;
    t_objeto *rochas;
    t_player player;
    ALLEGRO_EVENT evento;
    int largura_mapa, altura_mapa;
    unsigned char key[ALLEGRO_KEY_MAX];

    //Inicializa todas a varíaveis usadas no jogo
    arq = fopen ("./resources/score.txt", "a+");
    if (!arq){
        perror ("Não foi possível inicializar o arquivo score.txt");
        exit (1);
    }
    inicializa_variaveis (&largura_mapa, &altura_mapa, &player);
    rochas = aloca_vetor_objeto (N_ROCHAS);                         //Aloca espaço na memória para um vetor t_objeto que cuidadará das rochas do mapa
    diamantes = aloca_vetor_objeto (N_DIAMANTES);                   //Aloca espaço na memória para um vetor t_objeto que cuidadará dos diamantes do mapa

    //Inicializa ambas estruturas com as coordenadas iniciais dos 
    inicializa_vetor_objetos (rochas, largura_mapa, altura_mapa, ROCHA);
    inicializa_vetor_objetos (diamantes, largura_mapa, altura_mapa, DIAMANTE);

    //Resgistra os eventos que serão necessário para o jogo
    al_register_event_source (fila, al_get_keyboard_event_source());
    al_register_event_source (fila, al_get_display_event_source(display));
    al_register_event_source (fila, al_get_timer_event_source(timer));

    pega_ultimo_score (arq);

    //inicia o contador
    al_start_timer (timer);
    
    //inicializa as posições do vetor key com 0 
    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));

    //Inicia o jogo com o estado menu
    estado_jogo = MENU;

    //Continua nesse laço enquanto o jogo não terminou
    while (estado_jogo != TERMINOU) {
        //Escolhe uma função de acordo com o estado do jogo
        switch (estado_jogo) {
            case MENU        : estado_menu      (key, &evento); break;
            case JOGANDO     : estado_jogando   (arq, player, rochas, diamantes, largura_mapa, altura_mapa, key, &evento); break;
            case FIMDAPARTIDA: estado_standby   (&evento, key); break;
            case GAMEOVER    : estado_gameover  (&evento, key); break;
            default: break;
        }
    }
    //Desaloca os t_objeto*
    destroi_tudo ();
    free (diamantes);
    free (rochas);
    fclose (arq);
    return 0;

}