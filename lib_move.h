#ifndef __MOVE__
#define __MOVE__

//Função que testa a colisão do jogador com os objetos e toma as ações necessárias dependendo de onde o jogador bateu
void colisao (FILE*, int , int , int *, int *, int *, int *, bool *);

//Função que interpreta as teclas que o jogador aperta
int controle (FILE *, ALLEGRO_EVENT *, int , int *, int *, int *, bool* );

//Função que move as pedras e os diamante conforme as regras do jogo
//Os itens ou vão para baixo, ou se movem em L caso o item de baixo seja uma pedra ou um diamante
void gravidade (t_objeto *, t_objeto *, int , int , int *, int *, bool *);

//Função que verifica qual objeto vai receber "gravidade"
void deslizamento (FILE*, int );

//Função responsável para atualizar os objetos do mapa
//É uma atualização por "x" ALLEGRO_EVENT_TIMER
void atualiza_objetos_mapa (FILE *, int *, int *);


#endif