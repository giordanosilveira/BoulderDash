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

/*--------------------------------------------------------------------Declaração das variáveis--------------------------------------------------------------------------------*/
enum estados_t estado_jogo;

t_objeto *rochas;
t_objeto *diamantes;
t_sprites sheet;                                            //Sprites de todos os itens do mapa
t_mapa **mapa;                                              //Matriz do mapa

ALLEGRO_TIMER *timer;                                       //Temporizador 
ALLEGRO_EVENT_QUEUE *fila;                                  //fila de eventos
ALLEGRO_DISPLAY *display;                                   //Tela do jogo
ALLEGRO_BITMAP *buffer;                                     //Bitmap para um pré print dos itens do jogo
ALLEGRO_FONT *font;                                         //fonte de texto
ALLEGRO_SAMPLE *sample_coin;                                //Sample que toca quando o jogador pega uma moeda
ALLEGRO_SAMPLE *sample_explosion;                           //Sample que toca quando o jogador morre
ALLEGRO_SAMPLE *sample_pause;      

int morreu;                                                 //Variável que verifca se o jogador morreu

int tempo;                                                  //Variável que controla o tempo do jogo
int score;                                                  //última pontuação do jogador
int pontos;                                                 //Pontuação do jogador
int contador_diamantes;                                     //contador de diamantes
int diamante_minimos;                                       //Diamantes mínimos necessários para mudar a pontuação
int pontuacao_minima;                                       //Pontução que o jogador recebe a cada diamante


/*----------------------------------------------------------------------------------Função main--------------------------------------------------------------------------------*/


int main () {

    FILE *arq;
    t_player player;
    ALLEGRO_EVENT evento;
    int largura_mapa, altura_mapa;
    unsigned char key[ALLEGRO_KEY_MAX];

    //Abre o arquivo score e testa para ver se ele abriu corretamente
    arq = fopen ("./resources/score.txt", "a+");
    if (!arq){
        perror ("Não foi possível inicializar o arquivo score.txt");
        exit (1);
    }

    //Inicializa todas a varíaveis usadas no jogo
    inicializa_variaveis (&largura_mapa, &altura_mapa, &player);

    //Inicializa ambas estruturas com as coordenadas iniciais dos 
    inicializa_vetor_objetos (rochas, largura_mapa, altura_mapa, ROCHA);
    inicializa_vetor_objetos (diamantes, largura_mapa, altura_mapa, DIAMANTE);

    //Resgistra os eventos que serão necessário para o jogo
    al_register_event_source (fila, al_get_keyboard_event_source());
    al_register_event_source (fila, al_get_display_event_source(display));
    al_register_event_source (fila, al_get_timer_event_source(timer));

    pega_ultimo_score (arq);                                        //Pega o último melhor score do jogador

    al_start_timer (timer);                                         //inicia o contador
     
    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));       //inicializa as posições do vetor key com 0       

    estado_jogo = MENU;                                             //Inicia o jogo com o estado menu

    //Continua nesse laço enquanto o jogo não terminou
    while (estado_jogo != TERMINOU) {
        //Escolhe uma função de acordo com o estado do jogo
        switch (estado_jogo) {
            case MENU        : estado_menu      (key, &evento); break;
            case JOGANDO     : estado_jogando   (arq, player, largura_mapa, altura_mapa, key, &evento); break;
            case FIMDAPARTIDA: estado_standby   (&player,&evento, key); break;
            case GAMEOVER    : estado_gameover  (&evento, key); break;
            default: break;
        }
    }
    if (pontos + tempo > score) {
        fprintf (arq, "%d\n", pontos + tempo);
    }
    //Desaloca os t_objeto*
    destroi_tudo ();
    fclose (arq);
    return 0;

}