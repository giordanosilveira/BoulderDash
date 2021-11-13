#ifndef __MICRO__
#define __MICRO__

//#include "libDefines.h"

//Função que aloca um matriz do tipo mapa
//Retorno o ponteiro da matriz
t_mapa ** aloca_matriz_mapa (int , int );

//Função que aloca um vetor do tipo objeto
//Retorno o ponteiro da vetor
t_objeto *aloca_vetor_objeto (int );

//Função que testa se uma variável ALLEGRO foi alocada
void testa_inicializacao (bool , const char *);

//Função que carrega o mapa de uma arquivo para a memória
void carrega_mapa (int *, int *);

//Função que pega uma sub-bitmap de uma bitmap maior, é passada as coordenadas da sub-bitmap e as dimensões dela
//É retornada a sub-bitmap 
ALLEGRO_BITMAP* sprite_grab(int , int , int , int );

//Carrega as sprites de uma arquivo para a memória
void inicializa_sprites ();

//Destrói todas as sprites do jogo
void destruindo_sprites ();

//Função que inicializa o tipo objeto
void inicializa_vetor_objetos (t_objeto *, int , int , int );

//Função que testa a colisão do jogador com os objetos e toma as ações necessárias dependendo de onde o jogador bateu
void colisao (FILE*, t_objeto* , t_objeto * , int , int , int *, int *, int *, int *, bool *);

//Função que interpreta as teclas que o jogador aperta
int controle (FILE *, t_objeto* , t_objeto* , ALLEGRO_EVENT *, int , int *, int *, int *, bool* );

//Função que move as pedras e os diamante conforme as regras do jogo
//Os itens ou vão para baixo, ou se movem em L caso o item de baixo seja uma pedra
//É uma atualização por x ALLEGRO_EVENT_TIMER
void gravidade (t_objeto *, t_objeto *, int , int , int *, int *, bool *);

//Função que trata da atualização dos objetos
void deslizamento (FILE*, t_objeto * , t_objeto *, int );

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

//Função que cuida da pontuação do player
void pontuacao (bool *);

//Função feita para animar o player
void anima_jogador (bool * , int *, t_player *);

//Função feita para trocar as sprites dos diamantes 
void anima_diamantes (int *, int *);

//Função responsável para atualizar os objetos do mapa
void atualiza_objetos_mapa (FILE *, int *, t_objeto * , t_objeto *, int *);

//Função que destroi todo o entorno do jogador
void localiza_explosao (t_objeto*, t_objeto *, t_player );

//Função que anima as explosões
void anima_explosao (int *, int *);

//Trata do tempo do jogo, não é uma função perfeita, mas funciona razoavelmente bem
void trata_tempo (int *, int );

//Desenha as mensagens do estado stand by
void desenha_standby ();

//Desenha as mensagens do estado game over
void desenha_gameover ();

//Pega o último score do arquivo
void pega_ultimo_score (FILE *);

#endif
