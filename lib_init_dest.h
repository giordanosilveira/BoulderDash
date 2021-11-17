#ifndef __INITDEST__
#define __INITDEST__

//Função que reseta o jogo
void reseta_jogo (t_player *player);

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

//inicializa as variáveis globais do jogo
void inicializa_variaveis (int *, int *, t_player *);

//Destroi algumas estruturas do jogo
void destroi_tudo ();

#endif