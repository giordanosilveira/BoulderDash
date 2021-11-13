#ifndef __MACRO__
#define __MACRO__

//#include "libDefines.h"
//#include "lib_micro.h"

//inicializa as variáveis globais do jogo
void inicializa_variaveis (int *, int *, t_player *);

//Função que controla o menu do jogo
void estado_menu (unsigned char *, ALLEGRO_EVENT *);

//Função que aparece quando o jogador pede ajuda
void estado_ajuda (unsigned char *, ALLEGRO_EVENT* );

//Estado que controla os acontecimentos do jogo
void estado_jogando (FILE *, t_player , t_objeto * , t_objeto* , int , int , unsigned char * , ALLEGRO_EVENT *);

//Função que cuida do estado stand by
void estado_standby (ALLEGRO_EVENT* , unsigned char *);

//Função que cuida do estado game over
void estado_gameover (ALLEGRO_EVENT* , unsigned char *);

//Destroi algumas estruturas do jogo
void destroi_tudo ();

#endif
