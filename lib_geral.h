#ifndef __GERAL__
#define __GERAL__

//Função que cuida da pontuação do player
void pontuacao (bool *);

//Função que destroi todo o entorno do jogador
void localiza_explosao (t_objeto*, t_objeto *, t_player );

//Trata do tempo do jogo, não é uma função perfeita, mas funciona razoavelmente bem
void trata_tempo (int *, int );

//Pega o último score do arquivo
void pega_ultimo_score (FILE *);

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

#endif