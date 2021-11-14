#ifndef __GERAL__
#define __GERAL__

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

//Função que cuida da pontuação do player
void pontuacao (bool *);

//Função que destroi todo o entorno do jogador
void localiza_explosao (t_objeto*, t_objeto *, t_player );

//Trata do tempo do jogo, não é uma função perfeita, mas funciona razoavelmente bem
void trata_tempo (int *, int );

//Pega o último score do arquivo
void pega_ultimo_score (FILE *);

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