#ifndef __MOVE__
#define __MOVE__

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

//Função responsável para atualizar os objetos do mapa
void atualiza_objetos_mapa (FILE *, int *, t_objeto * , t_objeto *, int *);


#endif