#include "libDefines.h"
#include "lib_init_dest.h"

void reseta_jogo (t_player *player) {
    
    int largura_mapa, altura_mapa;

    tempo = TEMPO_TOTAL/2;
    morreu = 0;                                                     //Reinicializa a variável que diz se o jogador morreu

    //Reinicializa a estrutura t_player
    player->coord_x = PLAYER_COORD_X;
    player->coord_y = PLAYER_COORD_Y;
    player->direcao = 0;
    player->tipo = 0;

    //Reinicializa as pontuações
    pontos = pontos + tempo;
    contador_diamantes = 0;
    diamante_minimos = DIAMANTES_MINIMOS_F1;
    pontuacao_minima = diamante_minimos;
    
    carrega_mapa (&largura_mapa,&altura_mapa);                      //Reinicializa o mapa

    //Reinicializa o t_objeto
    inicializa_vetor_objetos (rochas, largura_mapa, altura_mapa, ROCHA);
    inicializa_vetor_objetos (diamantes, largura_mapa, altura_mapa, DIAMANTE);

}

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

    //Bloco: inicializa as estruturas da matriz
    for (int i = 0; i < *altura; i++) {
        for (int j = 0; j < *largura; j++) {
            fscanf (arquivo, "%d ", &item);
            mapa[i][j].item = item;
            mapa[i][j].na_tela = 1;
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

    //Carrega os sprites de explosao
    sheet.explosao[0] = sprite_grab (128, 112,LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    sheet.explosao[1] = sprite_grab (128, 96,LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    sheet.explosao[2] = sprite_grab (128, 80,LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    sheet.explosao[3] = sprite_grab (128, 64,LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    sheet.explosao[4] = sprite_grab (112, 96,LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    sheet.explosao[5] = sprite_grab (112, 80,LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    sheet.explosao[6] = sprite_grab (112, 64, LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);
    sheet.explosao[7] = sprite_grab (112, 112,LARGURA_BITMAP_BLOCO, ALTURA_BITMAP_BLOCO);

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

    //Percorre o vetor de bitmap e desaloca cada sprite
    for (int i = 0; i < N_VARIACOES_EXPLOSAO; i++) {
        al_destroy_bitmap (sheet.explosao[i]);
    }

}

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
                    mapa[j][l].item = VAGO;
                }
                else {      
                    vetor[i].q_obj = 1;
                    vetor[i].morto = false;
                    mapa[j][l].item = VAGO2;
                }
                i++;
            }
        }
    }                                                               //Fim do bloco
}

void inicializa_variaveis ( int *largura_mapa, int *altura_mapa, t_player *player) {


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

    sample_pause = al_load_sample ("./resources/Pause.wav");
    testa_inicializacao (sample_pause, "o sample Pause");

    carrega_mapa (largura_mapa, altura_mapa);                       //Carrega o mapa


    tempo = TEMPO_TOTAL;
    morreu = 0;                                                     //Inicializa a variável que diz se o jogador morreu

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
    
    //Inicializa o t_objeto
    rochas = aloca_vetor_objeto (N_ROCHAS);
    diamantes = aloca_vetor_objeto (N_DIAMANTES);

}

void destroi_tudo () {

    free (rochas);
    free (diamantes);

    //Desaloca a matriz mapa
    free (mapa[0]);
    free (mapa);

    //Destrói as sprites
    destruindo_sprites ();

    //Destrói as samples do jogo
    al_destroy_sample (sample_explosion);                       
    al_destroy_sample (sample_coin);
    al_destroy_sample (sample_pause);

    al_destroy_font (font);                                     //Destrói a fonte
    al_destroy_display (display);                               //Destrói o display
    al_destroy_bitmap (buffer);                                 //Destrói o buffer
    al_destroy_timer (timer);                                   //Destrói o timer
    al_destroy_event_queue (fila);                              //Destrói a fila de eventos
}
