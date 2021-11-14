#ifndef __GRFC__
#define __GRFC__

//Desenha no buffer bitmap
void disp_pre_draw ();

//Desenha no display
//Estica o buffer
void disp_post_draw ();

//Função que printa o mapa
void desenha_mapa (t_objeto *, t_objeto * , int , int , t_player * , int , int );

//Desenha a pontução e o progresso do jogador
void desenha_hud ();

//Desenhas as mesagens do menu
void desenha_menu ();

//Printa as mensagens do menu ajuda e printa o easter egg caso o tempo necessário para isso passe
void desenha_ajuda (int , int , int *, bool* );

//Desenha as mensagens do estado stand by
void desenha_standby ();

//Desenha as mensagens do estado game over
void desenha_gameover ();

//Função feita para animar o player
void anima_jogador (bool * , int *, t_player *);

//Função feita para trocar as sprites dos diamantes 
void anima_diamantes (int *, int *);

//Função que anima as explosões
void anima_explosao (int *, int *);

#endif