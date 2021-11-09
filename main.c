#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#define FPS 60.0                                            //Taxa de atualização do jogo
#define DELAY_MORTE_JOGADOR 20                              //Delay para a verificação da morte do jogador
#define ATUALIZACAO_OBJETO 10                               //Delay para a atualização das posições das pedras
#define DELAY_ANIMACAO 84
#define DELAY_ANIMACAO_DIAMANTE 160
#define DELAY_EASTER_EGG 1500

#define MULT_DISPLAY 2                                      //Número para multiplicar o buffer para preencher formar o display
#define CORRECAO_DISPLAY 23                                 //Para deixar uma sobra no display para a HUD
#define LARGURA_BUFFER 640                                  //Largura do buffer
#define ALTURA_BUFFER ( 352 + CORRECAO_DISPLAY)             //Altura do buffer
#define LARGURA_DISPLAY (LARGURA_BUFFER * MULT_DISPLAY)     //Largura do display (640 * 2)
#define ALTURA_DISPLAY (ALTURA_BUFFER * MULT_DISPLAY)       //Altura do display (352 * 2)

#define PONTUACAO_MINIMA_F1 10                              //Pontuação mínima no começo da fase
#define DIAMANTES_MINIMOS_F1 12                             //Diamantes mínimos para passar de fase

#define N_VARIACOES_DIAMANTE 8                              //Número de sprites de diamantes no sprite Boulder_dash.png
#define N_VARIACOES_PLAYER 21                               //Número de sprites do player no sprite Boulder_dash.png
#define N_ITENS_MAPA 6                                      //Número de sprites de itens do mapa no sprite Boulder_dash.png
#define ALTURA_BITMAP_PLAYER 15                             //Altura do sprite do player
#define LARGURA_BITMAP_PLAYER 14                            //Largura do sprite do player
#define ALTURA_BITMAP_BLOCO 16                              //Altura dos sprites dos blocos do mapa 
#define LARGURA_BITMAP_BLOCO 16                             //Largura dos sprites dos blocos do mapa

#define COORD_X_PORTA 16                                    //Coordenada da porta (linha)
#define COORD_Y_PORTA 38                                    //Coordenada da porta (coluna)
#define N_DIAMANTES 19                                      //Número de diamantes pelo mapa (previamente contados)
#define N_ROCHAS 105                                        //Número de rochas do mapa previamentes contadas
#define PAREDE 0                                            //Codificação para a parede externa
#define NADA 1                                          
#define TIJOLO 2                                            //Codificação para os tijolos internos
#define PLAYER 3                                            //Codificação para o player
#define ROCHA 4                                             //Codificação para a rochas do mapa
#define DIAMANTE 5                                          //Codificação para os diamantes do mapa
#define PORTA 6                                             //Codificação da porta do mapa
#define TERRA 7                                             //Codificação para as terras do mapa

#define ESQUERDA -1                                         //Codificação quando o player/objeto vai para a esquerda
#define DIREITA 1                                           //Codificação quando o player/objeto vai para a direita
#define CIMA -1                                             //Codificação quando o player/objeto vai para a cima
#define BAIXO 1                                             //Codificação quando o player/objeto vai para a baixo
#define PLAYER_COORD_X 2                                    //Posição inicial do player no mapa (linha)
#define PLAYER_COORD_Y 3                                    //Posição inicial do player no mapa (coluna)

#define KEY_SEEN 1                                            
#define KEY_RELEASED 2

//Struct que comanda todos os sprites do mapa
typedef struct t_sprites {
    ALLEGRO_BITMAP* bitmap_sprites;
    ALLEGRO_BITMAP* player[21];
    ALLEGRO_BITMAP* mapa[6];
    ALLEGRO_BITMAP* diamante[8]; 
} t_sprites;

//Struct que comanda o player
typedef struct t_player {
    int tipo;
    int coord_x, coord_y;
    int direcao;
} t_player;

//Struct que comanda os obejtos que tem "física" no mapa 
typedef struct t_objeto {
    int coord_x, coord_y;
    int q_obj;
    bool movimentando;
    bool morto;
    int codificacao;
} t_objeto;

//Struct que comanda todos os itens do mapa
//Qual item é e se ele esta na tela
typedef struct t_mapa{
    int na_tela, item, codificacao;
} t_mapa;

//Struct que comanda alguns delays pelo jogo
typedef struct t_contaFrames
{
    int atualizacao_objeto;
    int evento_morte;
    int animacao_player;
    int animacao_diamante;
    int vitoria;
} t_contaFrames;

enum estados_t {MENU, JOGANDO, FIMDAPARTIDA, GAMEOVER, TERMINOU};     //Possíveis estados do jogo
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


int pontos;                                                 //Pontuação do jogador
int contador_diamantes;                                     //contador de diamantes
int diamante_minimos;                                       //Diamantes mínimos necessários para mudar a pontuação
int pontuacao_minima;

t_mapa ** aloca_matriz_mapa (int largura, int altura) {

    t_mapa **matriz;
    
    //Bloco: aloca a matriz e testa para ver se alocou
    matriz = malloc (sizeof(t_mapa*) * altura);
    if (! matriz) {
        perror ("Error");
        fprintf (stderr, "Não foi possivel alocar espaço para os ponteiros da matriz de pixel\n");
        exit (1);
    }                                                                   //Fim do bloco

    //Bloco: aloca espaço para a matriz de linhas contíguas (método 3 na wiki do Maziero)
    matriz[0] = malloc (largura * altura * sizeof(t_mapa));
    if (! matriz[0]) {
        perror ("Error");
        fprintf (stderr, "Não foi possivel alocar espaço para a matriz\n");
        exit (1);
    }                                                                   //Fim do bloco

    //Arruma os ponteiros
    int i;
    for (i = 0; i < altura; i++)                                        
        matriz[i] = matriz[0] + i*largura;

    return matriz;

}

t_objeto *aloca_vetor_objeto (int tam_vetor) {

    t_objeto *vetor;

    //Bloco: aloca o vetor e testa para ver se alocou
    vetor = malloc (sizeof(t_objeto)*tam_vetor);
    if (! vetor) {
        perror ("Não foi possível alocar espaço para o vetor de rochas");
        exit (1);
    }                                                                   //Fim do bloco

    return vetor;

}

void testa_inicializacao (bool test, const char *descricao) {

    //Teste para ver se inicializou corretamente
    if (test)
        return;

    //Caso não tenha inicializado, printa a mensagem e sai do programa
    fprintf (stderr, "Não foi possível inicializar %s\n", descricao);
    exit (1);

}

void carrega_mapa (int *largura, int *altura) {
    
    FILE *arquivo;

    //Bloco: Abre o arquivo e testa para ver se for aberto corretamente
    arquivo = fopen ("./resources/mapa.dat", "r");
    if (! arquivo) {
        perror ("Não foi possível abrir o arquivo mapa.dat");
        exit (1);
    }                                                                   //Fim do bloco
    
    fscanf (arquivo, "%d %d", largura, altura);                         //Lendo as dimensões da matriz

    mapa = aloca_matriz_mapa (*largura, *altura);                       //Aloca espaço para a matriz na memória

    int item;
    int k = 0;

    //Bloco: inicializa as estruturas da matriz
    for (int i = 0; i < *altura; i++) {
        for (int j = 0; j < *largura; j++) {
            fscanf (arquivo, "%d ", &item);
            mapa[i][j].item = item;
            mapa[i][j].na_tela = 1;
            if (item == DIAMANTE) {
                mapa[i][j].codificacao = k;
                k++;
            }
        }
    }                                                                   //Fim do bloco

    fclose (arquivo);

}

ALLEGRO_BITMAP* sprite_grab(int coord_x_inicial, int coord_y_inicial, int largura_sprite, int altura_sprite) {
    
    //Seleciona uma sub-sprite da imagem grande
    //Todos os valores foram previamente calculados usando o software 'gimp'
    //al_create_sub_bitmap é facilmente explicável
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap (sheet.bitmap_sprites, coord_x_inicial, coord_y_inicial, largura_sprite, altura_sprite);
    testa_inicializacao(sprite, " o sub-sprite");
    return sprite;
    
}

void inicializa_sprites () {

    //Carrega a imagem principal 
    sheet.bitmap_sprites = al_load_bitmap ("./resources/Boulder_dash.png");
    testa_inicializacao (sheet.bitmap_sprites, "a imagem Boulder_dash.png");

    //Bloco: Carrega as sprites das variações do player
    sheet.player[0] = sprite_grab (1, 1, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[1] = sprite_grab (17, 1, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[2] = sprite_grab (33, 1, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[3] = sprite_grab (49, 1, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[4] = sprite_grab (65, 1, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[5] = sprite_grab (81, 1, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[6] = sprite_grab (97, 1, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);

    sheet.player[7] = sprite_grab (2, 17, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[8] = sprite_grab (16, 17, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[9] = sprite_grab (33, 17, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[10] = sprite_grab (48, 17, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[11] = sprite_grab (64, 17, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[12] = sprite_grab (81, 17, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[13] = sprite_grab (97, 17, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);

    sheet.player[14] = sprite_grab (0, 33, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[15] = sprite_grab (17, 33, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[16] = sprite_grab (34, 33, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[17] = sprite_grab (50, 33, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[18] = sprite_grab (66, 33, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[19] = sprite_grab (82, 33, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
    sheet.player[20] = sprite_grab (96, 33, LARGURA_BITMAP_PLAYER, ALTURA_BITMAP_PLAYER);
                                                                    //Fim do bloco

    //Carrega as variações dos sprites do mapa
    //Contas facilmente explicáveis 
    for (int i = 0; i < N_ITENS_MAPA; i++){
        sheet.mapa[i] = sprite_grab (i*LARGURA_BITMAP_BLOCO, 48, LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    }

    int k = 0;
    //Carrega as variações dos diamantes do mapa
    //Contas facilmente explicáveis
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            sheet.diamante[k] = sprite_grab ( j*LARGURA_BITMAP_BLOCO, (i*ALTURA_BITMAP_BLOCO) + 64, LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
            k++;
        }
    }
}

void destruindo_sprites () {

    //Desaloca a bitmap principal    
    al_destroy_bitmap (sheet.bitmap_sprites);

    //Percorre o vetor de bitmap e desaloca cada sprite
    for (int i = 0; i < N_VARIACOES_PLAYER; i++)
        al_destroy_bitmap (sheet.player[i]);

    //Percorre o vetor de bitmap e desaloca cada sprite
    for (int i = 0; i < N_ITENS_MAPA; i++)
        al_destroy_bitmap (sheet.mapa[i]);

    //Percorre o vetor de bitmap e desaloca cada sprite
    for (int i = 0; i < N_VARIACOES_DIAMANTE; i++)
        al_destroy_bitmap (sheet.diamante[i]);

}

//Função que inicializa o tipo objeto
void inicializa_vetor_objetos (t_objeto *vetor, int largura_mapa, int altura_mapa, int ITEM) {
    
    int i = 0;

    //Bloco: percorre a matriz do mapa
    for (int j = 0; j < altura_mapa; j++){
        for (int l = 0; l < largura_mapa; l++) {
            //Se o item do mapa for igual ao ITEM passado carrega as informações dele na memória
            if (mapa[j][l].item == ITEM ){ 
                vetor[i].coord_x = j;
                vetor[i].coord_y = l;
                vetor[i].movimentando = false;
                if (ITEM == ROCHA) {        
                    vetor[i].q_obj = 0;
                    vetor[i].morto = false;
                }
                else {      
                    vetor[i].q_obj = 1;
                    vetor[i].morto = false;
                    vetor[i].codificacao = mapa[j][l].codificacao;
                }
                i++;
            }
        }
    }                                                               //Fim do bloco
}
//Função que testa a colisão do jogador com os objetos e toma as ações necessárias dependendo de onde o jogador bateu
void colisao (t_objeto* diamantes, int *coord_p_x, int *coord_p_y, int *anterior_coord_p_x, int *anterior_coord_p_y, bool *flag_dimas) {

    //Se o jogador bateu na parede, permanece na mesma posição
    if (mapa[*coord_p_x][*coord_p_y].item == PAREDE) {
        *coord_p_y = *anterior_coord_p_y;
        *coord_p_x = *anterior_coord_p_x;
    }
    //Se o jogador bateu no tijolo, permanece na mesma posição
    if (mapa[*coord_p_x][*coord_p_y].item == TIJOLO) {
        *coord_p_y = *anterior_coord_p_y;
        *coord_p_x = *anterior_coord_p_x;
    }
    //Se o jogador bateu na rocha, permanece na mesma posição
    if (mapa[*coord_p_x][*coord_p_y].item == ROCHA) {               //Adicionar empurrar pedra
        *coord_p_y = *anterior_coord_p_y;
        *coord_p_x = *anterior_coord_p_x;
    }

    //Se o jogador bateu no diamante, "pega" o diamante, toca o sample do diamante e atualiza a flag_dimas para 1, para avisar que ele pegou o diamante
    if (mapa[*coord_p_x][*coord_p_y].item == DIAMANTE) {
        al_play_sample (sample_coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        *flag_dimas = true;
        //Aquele diamante que foi pego está morto
        for (int i = 0; i < N_DIAMANTES; i++) {
            if (diamantes[i].codificacao == mapa[*coord_p_x][*coord_p_y].codificacao)
                diamantes[i].morto = true;
        }
    }

    //Se o jogador chegou na porta, ele ganhou
    if (mapa[*coord_p_x][*coord_p_y].item == PORTA)
        estado_jogo = FIMDAPARTIDA;

}
//Função que interpreta as teclas que o jogador aperta
int controle (t_objeto* diamantes, ALLEGRO_EVENT *evento, int *coord_p_x, int *coord_p_y, int *direcao, bool* flag_dimas) {

    int ajuda = 0;
    int anterior_coord_p_x, anterior_coord_p_y;
    anterior_coord_p_x = *coord_p_x;
    anterior_coord_p_y = *coord_p_y;
    
    
    if (evento->keyboard.keycode == ALLEGRO_KEY_UP) {           //Se ele apertou para ir para cima, incrementa a variável que vai para cima
        *coord_p_x = *coord_p_x + CIMA;
        *direcao = 0;
    }
    else if (evento->keyboard.keycode == ALLEGRO_KEY_DOWN) {    //Se ele apertou para ir para baixo, incrementa a variável que vai para baixo
        *coord_p_x = *coord_p_x + BAIXO;
        *direcao = 0;
    }
    else if (evento->keyboard.keycode == ALLEGRO_KEY_LEFT) {    //Se ele apertou para ir para a esquerda, incrementa a variável que vai para a esquerda
        *coord_p_y = *coord_p_y + ESQUERDA;
        *direcao = ESQUERDA;
    }
    else if (evento->keyboard.keycode == ALLEGRO_KEY_RIGHT) {   //Se ele apertou para ir para direita, incrementa a variável que vai para a direita
        *coord_p_y = *coord_p_y + DIREITA;
        *direcao = DIREITA;
    }
    else if (evento->keyboard.keycode == ALLEGRO_KEY_H)         //Se ele apertou para ir para o menu ajuda, define a variável que vai para o menu ajuda como 1 
        ajuda = 1;
    else if (evento->keyboard.keycode == ALLEGRO_KEY_ESCAPE)    //Se ele apertou para ir para sair do jogo, define a variável que sai do jogo como 1
        estado_jogo = TERMINOU;

    //Depois tratar as teclas, vê aonde o player colidiu toma as ações necessárias
    colisao (diamantes,coord_p_x, coord_p_y, &anterior_coord_p_x, &anterior_coord_p_y, flag_dimas);
    mapa[anterior_coord_p_x][anterior_coord_p_y].item = NADA;
    mapa[*coord_p_x][*coord_p_y].item = PLAYER;

    //Retorna a variável ajuda
    return ajuda;
}
//Função que move as pedras e os diamante conforme as regras do jogo
//Os itens ou vão para baixo, ou se movem em L caso o item de baixo seja uma pedra
//É uma atualização por x ALLEGRO_EVENT_TIMER
void gravidade (int *coord_x_obj, int *coord_y_obj, bool *movimentando, int ITEM) {

    //Troca de acordo com o item tratado
    switch (ITEM)
    {
        //Se o item tratado for uma pedra
        case 0:

        //Se o item abaixo for uma terra, não acontece nada
        if ((mapa[*coord_x_obj + 1][*coord_y_obj].item == TERRA)) {
            *movimentando = false;
            return;
        }   

        //Se não tiver nada abaixo, move o objeto para baixo, e define a variável movimentando como verdadeira
        if (mapa[*coord_x_obj + 1][*coord_y_obj].item == NADA) {
            *movimentando = true;
            *coord_x_obj = *coord_x_obj + BAIXO;
            mapa[*coord_x_obj][*coord_y_obj].item = ROCHA;
            mapa[*coord_x_obj - 1][*coord_y_obj].item = NADA;
            return;
        }

        //Se abaixo dele estiver com uma pedra
        if (mapa[*coord_x_obj + 1][*coord_y_obj].item == ROCHA) {
            //se a coluna inferior esquerda estiver vaga e, se a coluna loga a esquerda estiver vaga, atualiza o objeto para a esquerda e define a variável movimentando como verdadeira
            //e a posição anterior do objeto fica como vazia  
            if ((mapa[*coord_x_obj + 1][*coord_y_obj - 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj - 1].item == NADA)) {
                *movimentando = true;
                *coord_y_obj = *coord_y_obj + ESQUERDA;
                mapa[*coord_x_obj][*coord_y_obj].item = ROCHA;
                mapa[*coord_x_obj][*coord_y_obj + 1].item = NADA;
            }
            //Caso não dê para ir a esquerda, testa para ver se foi para a direita
            //se a coluna inferior direita estiver vaga e, se a coluna loga a direita estiver vaga, atualiza o objeto para a direita e define a variável movimentando como verdadeira
            //e a posição anterior do objeto fica como vazia
            else if ((mapa[*coord_x_obj + 1][*coord_y_obj + 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj + 1].item == NADA)) {
                *movimentando = true;
                *coord_y_obj = *coord_y_obj + DIREITA;
                mapa[*coord_x_obj][*coord_y_obj].item = ROCHA;
                mapa[*coord_x_obj][*coord_y_obj - 1].item = NADA;
            }
            //Se o objeto estiver travado, então define a variável movimentando como falsa
            else
                *movimentando = false;
        }
        break;
        //Se o item tratado for um diamante
        case 1:

            //Se o item abaixo for uma terra, não acontece nada
            if ((mapa[*coord_x_obj + 1][*coord_y_obj].item == TERRA)) {
                *movimentando = false;
                return;
            }

            //Se não tiver nada abaixo, move o objeto para baixo, e define a variável movimentando como verdadeira
            if (mapa[*coord_x_obj + 1][*coord_y_obj].item == NADA) {
                *movimentando = true;
                *coord_x_obj = *coord_x_obj + BAIXO;
                mapa[*coord_x_obj][*coord_y_obj].item = DIAMANTE;
                mapa[*coord_x_obj][*coord_y_obj].codificacao = mapa[*coord_x_obj - 1][*coord_y_obj].codificacao;
                mapa[*coord_x_obj - 1][*coord_y_obj].item = NADA;
                return;    
            }

            //Se abaixo dele estiver com uma pedra
            if ((mapa[*coord_x_obj + 1][*coord_y_obj].item == ROCHA)) {
                //se a coluna inferior esquerda estiver vaga e, se a coluna loga a esquerda estiver vaga, atualiza o objeto para a esquerda e define a variável movimentando como verdadeira
                //e a posição anterior do objeto fica como vazia  
                if ((mapa[*coord_x_obj + 1][*coord_y_obj - 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj - 1].item == NADA)) {
                    *movimentando = true;
                    *coord_y_obj = *coord_y_obj + ESQUERDA;
                    mapa[*coord_x_obj][*coord_y_obj].item = DIAMANTE;
                    mapa[*coord_x_obj][*coord_y_obj].codificacao = mapa[*coord_x_obj][*coord_y_obj + 1].codificacao;
                    mapa[*coord_x_obj][*coord_y_obj + 1].item = NADA;
                }
                //Caso não dê para ir a esquerda, testa para ver se foi para a direita
                //se a coluna inferior direita estiver vaga e, se a coluna loga a direita estiver vaga, atualiza o objeto para a direita e define a variável movimentando como verdadeira
                //e a posição anterior do objeto fica como vazia
                else if ((mapa[*coord_x_obj + 1][*coord_y_obj + 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj + 1].item == NADA)) {
                    *movimentando = true;
                    *coord_y_obj = *coord_y_obj + DIREITA;
                    mapa[*coord_x_obj][*coord_y_obj].item = DIAMANTE;
                    mapa[*coord_x_obj][*coord_y_obj].codificacao = mapa[*coord_x_obj][*coord_y_obj - 1].codificacao;
                    mapa[*coord_x_obj][*coord_y_obj - 1].item = NADA;
                }
            }
            //Se o objeto estiver travado, então define a variável movimentando como falsa
            else
                *movimentando = false;
        break; 
    }
}
//Função que trata da atualização dos objetos
void deslizamento (t_objeto *objeto, int n_objetos, int frames_evento_morte) {

    for (int i = 0; i < n_objetos; i++) {
        
        //Se o objeto estiver na tela, aplica a gravidade nele
        if (objeto[i].morto == false)   
            gravidade (&objeto[i].coord_x, &objeto[i].coord_y, &objeto[i].movimentando, objeto[i].q_obj);
        
        //Apliquei um delay para a morte do jogador não ficar instantânea
        if (frames_evento_morte == DELAY_MORTE_JOGADOR)

            //Se o jogador morreu, toca o sample explosão e o jogo termina
            if ((mapa[objeto[i].coord_x + 1][objeto[i].coord_y].item == PLAYER) && (objeto[i].movimentando == true)) {
                al_play_sample (sample_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                estado_jogo = GAMEOVER;
            }
    }
}
//Desenha no buffer bitmap
void disp_pre_draw () {
    al_set_target_bitmap (buffer);
}

//Desenha no display
//Estica o buffer
void disp_post_draw () {
    al_set_target_backbuffer (display);

    //Estica o buffer para caber na tela
    al_draw_scaled_bitmap (buffer, 0, 0, LARGURA_BUFFER, ALTURA_BUFFER, 0, 0, LARGURA_DISPLAY, ALTURA_DISPLAY, 0);
    
    al_flip_display ();
}
void desenha_mapa (int largura, int altura, t_player * player, int sprite_diamante) {

    int aux_ani_dir = 14;
    int aux_ani_esq = 7;

    //Percorre a matriz e, de acordo com o item da matriz, printa o correto
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            switch (mapa[i][j].item) {

                case PAREDE:    //Se for o bloco parede                           
                    al_draw_bitmap (sheet.mapa[2], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case TIJOLO:    //Se for o bloco tijolo
                    al_draw_bitmap (sheet.mapa[2], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break; 
                case PLAYER:    //Se for o bloco player
                    if (player->direcao == DIREITA) {
                        al_draw_bitmap (sheet.player[player->tipo + aux_ani_dir], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    }
                    else if (player->direcao == ESQUERDA) {
                        al_draw_bitmap (sheet.player[player->tipo + aux_ani_esq], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    }
                    else
                        al_draw_bitmap (sheet.player[player->tipo], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case ROCHA:     //Se for o bloco rocha
                    al_draw_bitmap (sheet.mapa[5], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case DIAMANTE:  //Se for o bloco diamante
                    al_draw_bitmap (sheet.diamante[sprite_diamante], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case TERRA:     //Se for o bloco terra
                    al_draw_bitmap (sheet.mapa[3], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case PORTA:     //Se for o bloco 
                    al_draw_bitmap (sheet.mapa[1], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                default: break; //Se for casa vazia, não faz nada
            }
        }
    }
}
//Desenha a pontução e o progresso do jogador
void desenha_hud () {
    al_draw_textf (font, al_map_rgb(255, 255, 0), 10, 7, 0, "%d", diamante_minimos);
    al_draw_bitmap (sheet.diamante[0], 26, 3 ,0);
    al_draw_textf (font, al_map_rgb(255, 255, 255), 10 + 33, 7, 0, "%d", pontuacao_minima);
    al_draw_textf (font, al_map_rgb(255, 255, 0), LARGURA_BUFFER/2, 7, 0, "%d", contador_diamantes);
    al_draw_textf (font, al_map_rgb(255, 255, 255), LARGURA_BUFFER/2 + 40, 7, 0, "%d", pontos);
}

void inicializa_variaveis (int *largura_mapa, int *altura_mapa, t_player *player) {


    testa_inicializacao (al_init (), "a biblioteca allegro :(");        //Instala a biblioteca allegro e testa para ver se abriu direito
    testa_inicializacao (al_install_keyboard (), "o teclado :(");       //Instala a teclado e testa para ver se alocou
    testa_inicializacao (al_init_primitives_addon (), "as primitives"); //Instala a biblioteca resposável para as funções primitives 
    
    timer = al_create_timer (1.0/FPS);                                  //Cria o temporizador e testa para ver se alocou
    testa_inicializacao (timer, "o temporizador :(");

    fila = al_create_event_queue ();                                    //cria a fila de eventos e testa para ver se alocou
    testa_inicializacao (fila, "a fila de eventos :(");

    display = al_create_display (LARGURA_DISPLAY, ALTURA_DISPLAY);      //Cria o display e testa para ver se alocou
    testa_inicializacao (display, "o display :(");

    buffer = al_create_bitmap (LARGURA_BUFFER, ALTURA_BUFFER);          //Cria o bitmap buffer e testa para ver se alocou
    testa_inicializacao (buffer, "o bitmap buffer :(");

    font = al_create_builtin_font ();                                   //Cria a fonte de texto e testa para ver se alocou
    testa_inicializacao (font, "a fonte :(");

    testa_inicializacao (al_init_image_addon(), "imagem addon");        //Inicia o imagem addon e testa para ver se inicializou
    inicializa_sprites ();                                              //Carrega as sprites do jogo na memória

    //Inicializa as bibliotecas responsável pelo áudio do jogo
    testa_inicializacao (al_install_audio(), "o audio");
    testa_inicializacao (al_init_acodec_addon(), "o acodec");
    testa_inicializacao (al_reserve_samples (16), "o reserve samples");

    //Inicializa as samples que user no jogo
    sample_coin = al_load_sample ("./resources/coins.wav");
    testa_inicializacao (sample_coin, "o sample coin");

    sample_explosion = al_load_sample ("./resources/explosao.wav");
    testa_inicializacao (sample_explosion, "o sample explosao");

    sample_select = al_load_sample ("./resources/select.wav");
    testa_inicializacao (sample_select, "o sample select");

    carrega_mapa (largura_mapa, altura_mapa);                       //Carrega o mapa


    //Inicializa a estrutura t_player
    player->coord_x = PLAYER_COORD_X;
    player->coord_y = PLAYER_COORD_Y;
    player->direcao = 0;
    player->tipo = 0;

    //Inicializa as pontuações
    pontos = 0;
    contador_diamantes = 0;
    diamante_minimos = DIAMANTES_MINIMOS_F1;
    pontuacao_minima = diamante_minimos;

}
//Desenhas as mesagens do menu
void desenha_menu () {
    
    al_clear_to_color (al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle (0, 0, LARGURA_BUFFER, ALTURA_BUFFER/2, al_map_rgb (30, 144, 255));
    al_draw_textf (font, al_map_rgb (0, 0, 0), LARGURA_BUFFER/2, (ALTURA_BUFFER/2)/2, ALLEGRO_ALIGN_CENTER, "BOULDER");
    al_draw_textf (font, al_map_rgb (0, 0, 0), LARGURA_BUFFER/2, (ALTURA_BUFFER/2)/2 + 10, ALLEGRO_ALIGN_CENTER, "DASH");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 100, ALLEGRO_ALIGN_CENTER, "APERTE 'ENTER' PARA INICIAR");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 100 + 10, ALLEGRO_ALIGN_CENTER, "APERTE 'ESC' PARA SAIR");

}
void estado_menu (unsigned char *key, ALLEGRO_EVENT *evento) {

    //Enquanto estiver no menu
    while (estado_jogo == MENU) {
        al_wait_for_event (fila, evento);

        switch (evento->type) {
            
            case ALLEGRO_EVENT_TIMER:                           //Se o der o tempo para a atualização
                if (key[ALLEGRO_KEY_ENTER])                     //Teste para ver se o jogador quer jogar
                    estado_jogo = JOGANDO;
                else if (key[ALLEGRO_KEY_ESCAPE])               //Teste para ver se o jogador quer sair do jogo
                    estado_jogo = TERMINOU;
                //Bloco: Printa a mensagem do menu
                disp_pre_draw ();
                desenha_menu ();
                disp_post_draw ();                              //Fim do bloco

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                break;
        
            case ALLEGRO_EVENT_KEY_DOWN:
                key[evento->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[evento->keyboard.keycode] &= KEY_RELEASED;
                break;
        }
    }
}
//Printa as mensagens do menu ajuda e printa o easter egg caso o tempo necessário para isso passe
void desenha_ajuda (int easter_egg, int delay_animacao, int *sprite_animacao, bool* ida) {

    al_clear_to_color (al_map_rgb(0,0,0));
    //Se der o tempo necessário para mostrar o easter egg, mostra o easter egg
    if (easter_egg == DELAY_EASTER_EGG) {
        //A animção do jogador segue o mesmo modelo da função anima jogador
        if (*ida) {
            if (delay_animacao % (DELAY_ANIMACAO/7) == 0){
                *sprite_animacao = *sprite_animacao + 1;
                if (* sprite_animacao == 6)
                    *ida = false;
            }
            al_draw_ellipse (60, 240, 50, 28, al_map_rgb (255, 255, 255), 2);
            al_draw_textf (font, al_map_rgb (255, 255, 255), 50, 7+ 213, 0, "VAI");
            al_draw_textf (font, al_map_rgb (255, 255, 255), 32, 7+ 213 + 8, 0, "ESPERAR");
            al_draw_textf (font, al_map_rgb (255, 255, 255), 22, 7+ 213 + 16, 0, "ATÉ QUANDO,");
            al_draw_textf (font, al_map_rgb (255, 255, 255), 22, 7+ 213 + 24, 0, "MEU CHAPA?");
            al_draw_filled_circle (35, 280, 4, al_map_rgb(255, 255, 255));
            al_draw_bitmap (sheet.player[*sprite_animacao], 10, 300, 0);
        }
        else {
            if (delay_animacao % (DELAY_ANIMACAO/7) == 0){
                *sprite_animacao = *sprite_animacao - 1;
                if (*sprite_animacao == 0)
                    *ida = true;
            }
            al_draw_ellipse (60, 240, 50, 28, al_map_rgb (255, 255, 255), 2);
            al_draw_textf (font, al_map_rgb (255, 255, 255), 50, 7+ 213, 0, "VAI");
            al_draw_textf (font, al_map_rgb (255, 255, 255), 32, 7+ 213 + 8, 0, "ESPERAR");
            al_draw_textf (font, al_map_rgb (255, 255, 255), 22, 7+ 213 + 16, 0, "ATÉ QUANDO,");
            al_draw_textf (font, al_map_rgb (255, 255, 255), 22, 7+ 213 + 24, 0, "MEU CHAPA?");
            al_draw_filled_circle (30, 290, 2, al_map_rgb(255, 255, 255));
            al_draw_bitmap (sheet.player[*sprite_animacao], 10, 300, 0);
        }
    }
    //mostra as mensagens
    al_draw_filled_rectangle (78, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 10, LARGURA_BUFFER/2 + 240, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 107, al_map_rgb (40, 144, 255));
    al_draw_textf (font, al_map_rgb (255, 255, 255), 0, 0, 0, "Trabalho feito por Giordano Henrique Silveira");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 15, ALLEGRO_ALIGN_CENTER, "Olá! Vamos explicar algumas coisas.");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 25, ALLEGRO_ALIGN_CENTER, "Se as pedras caírem em cima de você, você morre");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 35, ALLEGRO_ALIGN_CENTER, "Se os diamantes caírem em cima de você, você morre.");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 45, ALLEGRO_ALIGN_CENTER, "Pegue os diamantes, abra a porta e caia fora");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 55, ALLEGRO_ALIGN_CENTER, "Para se mover pelo mapa, use as setas.");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 65, ALLEGRO_ALIGN_CENTER, "Você pode sair a qualquer momento do jogo.");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 75, ALLEGRO_ALIGN_CENTER, "É só precionar a tecla ESC (você também pode fechar a tela)."); 
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 85, ALLEGRO_ALIGN_CENTER, "Para sair do menu ajuda, aperte 'ENTER'");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, (ALTURA_BUFFER - 315 + CORRECAO_DISPLAY) + 95, ALLEGRO_ALIGN_CENTER, "'That's all folks'");

}
//Função que aparece quando o jogador pede ajuda
void estado_ajuda (unsigned char *key, ALLEGRO_EVENT* evento) {

    int close = 0;
    int easter_egg = 0;
    int delay_animacao = 0;
    int sprite_animacao = 0;
    bool ida = true;

    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));
    while (! close) {
        al_wait_for_event (fila, evento);
        switch (evento->type)
        {
            case ALLEGRO_EVENT_TIMER:
                if (key[ALLEGRO_KEY_ENTER])
                    close = 1;

                //Enquanto for menor que o DELAY_EASTER_EGG, aumenta a variável easter_egg
                //Necessário para a variável não crescer infinitamente
                if (easter_egg < DELAY_EASTER_EGG) 
                    easter_egg++;

                //Define a variável para 0
                //necessário para a variável não crescer infinitamente
                if (delay_animacao == DELAY_ANIMACAO) 
                    delay_animacao = 0;

                //Desenha o menu ajuda
                disp_pre_draw ();
                desenha_ajuda (easter_egg, delay_animacao, &sprite_animacao, &ida);
                disp_post_draw ();

                //Daqui para baixo é tudo necessário para arrumar o vetor key
                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                break;
        
            case ALLEGRO_EVENT_KEY_DOWN:
                key[evento->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[evento->keyboard.keycode] &= KEY_RELEASED;
                break;

        }
        delay_animacao ++;

    }

}
//Função que cuida da pontuação do player
void pontuacao (bool *flag_dimas) {

    //se o jogador pegou um diamante
    if (*flag_dimas) {

        //Aumenta a pontução e o número de diamantes do player
        pontos = pontos + pontuacao_minima;
        contador_diamantes ++;

        //Se o jogador os diamantes mínimos necessários para a troca do valor da pontuação mínima
        if (contador_diamantes == diamante_minimos) {
            diamante_minimos = diamante_minimos + 3;
            pontuacao_minima = diamante_minimos;
        }
    }
    *flag_dimas = false;

    //Se o jogador pegou o mínimo de diamantes necessários para passar de fase, abre a porta para ele
    if (contador_diamantes == DIAMANTES_MINIMOS_F1) {
        mapa[COORD_X_PORTA][COORD_Y_PORTA].item = PORTA;
        mapa[COORD_X_PORTA][COORD_Y_PORTA].na_tela = 1;
    }

}
//Função feita para animar o player
void anima_jogador (bool * flag_ida, int *delay_animacao, t_player *player) {

    //Se as animações do player estão crescendo
    if (*flag_ida) {
        //Apliquei um delay para a animação não ficar rápida demais
        //Se chegar no tempo do delay troca o valor da variável responsável pela animação
        if ((*delay_animacao % (DELAY_ANIMACAO/7)) == 0) {      //Div 7 porque são 7 sprites diferentes
            player->tipo = player->tipo + 1;                  
            if (player->tipo == 6)  //se a variável chegar no máximo de sprites, difine a flag_ida como false
                *flag_ida = false;
        }
    }
    //Se as animações do player estão voltando
    else {
        if ((*delay_animacao % (DELAY_ANIMACAO/7)) == 0) {
            player->tipo = player->tipo - 1;
            if (player->tipo == 0)  //se a variável chegar no mínimo de sprites, difine a flag_ida como true
                *flag_ida = true;
        }
    }
    //define o delay_animacao para 0
    //necessário para a variável não crescer infinitamente
    if (*delay_animacao == DELAY_ANIMACAO)
        *delay_animacao = 0;

}
//Função feita para trocar as sprites dos diamantes 
void anima_diamantes (int *delay_animacao, int *sprite_diamante) {
    
    //Apliquei um delay para a animção não ficar rápida demais
    //Se chegar no tempo do delay troca o valor da variável responsável pela animação
    if (*delay_animacao % (DELAY_ANIMACAO_DIAMANTE/8) == 0) {   //Div 8 porque são 8 sprites
        *sprite_diamante = *sprite_diamante + 1;
        if (*sprite_diamante == 7) //Se a variável chegar no máximo de sprites, define ela como 0 de novo
            *sprite_diamante = 0;
    }
    //define o delay_animação para 0
    //necessário para a variável não crescer infinitamente
    if (*delay_animacao == DELAY_ANIMACAO_DIAMANTE)
        *delay_animacao = 0;

}
//Função responsável para atualizar os objetos do mapa
void atualiza_objetos_mapa (int *atualizacao_objeto, t_objeto * rochas, t_objeto *diamantes, int *delay_morte) {

    //Apliquei um delay para a atualização dos objetos do mapa, senão fica muito rapido
    //Só atualiza a cada 10 ALLEGRO_EVENT_TIMER
    //Se chegar o tempo de atualizar aplica o deslizamento
    if (*atualizacao_objeto == ATUALIZACAO_OBJETO) {
        deslizamento (rochas, N_ROCHAS, *delay_morte);
        deslizamento (diamantes, N_DIAMANTES, *delay_morte);
        *atualizacao_objeto = 0;
    }

    //Define o delay morte para 0
    //Necessário para a variavél delay_morte não crescer infinitamente
    if (*delay_morte == DELAY_MORTE_JOGADOR)
        *delay_morte = 0;

}
//Estado que controla os acontecimentos do jogo
void estado_jogando (t_player player, t_objeto * rochas, t_objeto* diamantes, int largura_mapa, int altura_mapa, unsigned char * key, ALLEGRO_EVENT *evento) {

    int desenha = 0;
    int ajuda = 0;
    int sprite_diamante = 0;
    int aux_direcao ;
    bool ida = true;
    bool flag_keydown;
    bool flag_diamante = false;

    t_contaFrames delay;

    delay.atualizacao_objeto = 0;
    delay.evento_morte = 0;
    delay.animacao_player = 0;
    delay.animacao_diamante = 0;

    while (estado_jogo == JOGANDO) {
        al_wait_for_event (fila, evento);

        switch (evento->type) {

            case ALLEGRO_EVENT_TIMER:

                //Se o jogador apertou a tecla 'H', vai para o estado_ajuda 
                if (ajuda) {
                    estado_ajuda (key, evento);
                    ajuda = 0;
                }
 
                //Cuida da pontuação/progresso do jogador
                pontuacao (&flag_diamante);

                //Atualiza os objetos do mapa e testa para ver se o jogador morreu
                atualiza_objetos_mapa (&delay.atualizacao_objeto, rochas, diamantes, &delay.evento_morte);
                delay.atualizacao_objeto = delay.atualizacao_objeto + 1;                
                delay.evento_morte = delay.evento_morte + 1;

                //Troca as sprites do jogador para animá-lo 
                anima_jogador (&ida, &delay.animacao_player, &player);
                delay.animacao_player = delay.animacao_player + 1;
                
                //Troca as sprites dos diamantes para animá-las
                anima_diamantes (&delay.animacao_diamante, &sprite_diamante);
                delay.animacao_diamante++;
                
                desenha = 1;

                break;
            
            //Caso um tecla seja apertada, interpreta a ação dessa tecla
            //Também testa as colisões do jogador com o mapa, porém não ve se ele morreu
            case ALLEGRO_EVENT_KEY_CHAR :
                ajuda = controle (diamantes, evento, &player.coord_x, &player.coord_y, &player.direcao, &flag_diamante);
                break;

            //Caso o jogador feche o display, termina o jogo
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                estado_jogo = TERMINOU;
                break;

            //Caso o jogador não faça nada, a direção do player é definada como 0
            //Isso é preciso para a animação do player
            default:
                player.direcao = 0;
                break;

        }
        if ((desenha) && (al_is_event_queue_empty(fila))) {
            disp_pre_draw ();
            al_clear_to_color (al_map_rgb(0,0,0));
            desenha_hud ();
            desenha_mapa (largura_mapa, altura_mapa, &player, sprite_diamante);
            disp_post_draw ();
            desenha = 0;
        }
    }
}
//Desenha as mensagens do estado stand by
void desenha_standby () {

    al_clear_to_color (al_map_rgb(0, 0, 0));
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2, ALLEGRO_ALIGN_CENTER, "Parabens você venceu :)");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 10, ALLEGRO_ALIGN_CENTER, "Aperte 'ENTER' ou 'ESC' para sair" );

}
void estado_standby (ALLEGRO_EVENT* evento, unsigned char *key) {

    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));

    while (estado_jogo == FIMDAPARTIDA) {
        al_wait_for_event (fila, evento);

        switch (evento->type) {
            
            case ALLEGRO_EVENT_TIMER:                           //Se o der o tempo para a atualização
                if (key[ALLEGRO_KEY_ENTER])                     //Teste para ver se o jogador quer continuar
                {    
                    estado_jogo = TERMINOU;
                }
                else if (key[ALLEGRO_KEY_ESCAPE])               //Teste para ver se o jogador quer sair do jogo
                {    
                    estado_jogo = TERMINOU;
                }
                //Bloco: Printa a mensagem do fim de jogo
                disp_pre_draw ();
                desenha_standby ();
                disp_post_draw ();                              //Fim do bloco

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                break;
        
            case ALLEGRO_EVENT_KEY_DOWN:
                key[evento->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[evento->keyboard.keycode] &= KEY_RELEASED;
                break;
        }
    }

}
//Desenha as mensagens do estado game over
void desenha_gameover () {

    al_clear_to_color (al_map_rgb(0, 0, 0));
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2, ALLEGRO_ALIGN_CENTER, "Você perdeu! QUE PENA:'( ");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 10, ALLEGRO_ALIGN_CENTER, "Aperte 'ENTER' ou 'ESC' para sair" );

}
void estado_gameover (ALLEGRO_EVENT* evento, unsigned char *key) {
    
    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));
    while (estado_jogo == GAMEOVER) {
        
        al_wait_for_event (fila, evento);

        switch (evento->type) {
            
            case ALLEGRO_EVENT_TIMER:                           //Se o der o tempo para a atualização
                if (key[ALLEGRO_KEY_ENTER])                     //Teste para ver se o jogador quer continuar
                    estado_jogo = TERMINOU;
                else if (key[ALLEGRO_KEY_ESCAPE])               //Teste para ver se o jogador quer sair do jogo
                    estado_jogo = TERMINOU;

                //Bloco: Printa a mensagem do fim de jogo
                disp_pre_draw ();
                desenha_gameover ();
                disp_post_draw ();                              //Fim do bloco

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                break;
        
            case ALLEGRO_EVENT_KEY_DOWN:
                key[evento->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[evento->keyboard.keycode] &= KEY_RELEASED;
                break;
        }
    }
}
//Destroi algumas estruturas do jogo
void destroi_tudo () {

    //Desaloca a matriz mapa
    free (mapa[0]);
    free (mapa);

    //Destrói as sprites
    destruindo_sprites ();

    //Destrói as samples do jogo
    al_destroy_sample (sample_explosion);                       
    al_destroy_sample (sample_coin);
    al_destroy_sample (sample_select);

    al_destroy_font (font);                                     //Destrói a fonte
    al_destroy_display (display);                               //Destrói o display
    al_destroy_bitmap (buffer);                                 //Destrói o buffer
    al_destroy_timer (timer);                                   //Destrói o timer
    al_destroy_event_queue (fila);                              //Destrói a fila de eventos
}

int main () {

    t_objeto *diamantes;
    t_objeto *rochas;
    t_player player;
    ALLEGRO_EVENT evento;
    int largura_mapa, altura_mapa;
    unsigned char key[ALLEGRO_KEY_MAX];

    //Inicializa todas a varíaveis usadas no jogo
    inicializa_variaveis (&largura_mapa, &altura_mapa, &player);
    rochas = aloca_vetor_objeto (N_ROCHAS);                         //Aloca espaço na memória para um vetor t_objeto que cuidadará das rochas do mapa
    diamantes = aloca_vetor_objeto (N_DIAMANTES);                   //Aloca espaço na memória para um vetor t_objeto que cuidadará dos diamantes do mapa

    //Inicializa ambas estruturas com as coordenadas iniciais dos objetos
    inicializa_vetor_objetos (rochas, largura_mapa, altura_mapa, ROCHA);
    inicializa_vetor_objetos (diamantes, largura_mapa, altura_mapa, DIAMANTE);

    //Resgistra os eventos que serão necessário para o jogo
    al_register_event_source (fila, al_get_keyboard_event_source());
    al_register_event_source (fila, al_get_display_event_source(display));
    al_register_event_source (fila, al_get_timer_event_source(timer));

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
            case JOGANDO     : estado_jogando   (player, rochas, diamantes, largura_mapa, altura_mapa, key, &evento); break;
            case FIMDAPARTIDA: estado_standby   (&evento, key); break;
            case GAMEOVER    : estado_gameover  (&evento, key); break;
            default: break;
        }
    }
    //Desaloca os t_objeto* 
    free (diamantes);
    free (rochas);
    return 0;

}