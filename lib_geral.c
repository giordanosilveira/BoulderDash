#include "libDefines.h"
#include "lib_geral.h"
#include "lib_graficos.h"
#include "lib_move.h"

//Função que aloca um matriz do tipo mapa
//Retorno o ponteiro da matriz
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

//Função que aloca um vetor do tipo objeto
//Retorno o ponteiro da vetor
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

//Função que testa se uma variável ALLEGRO foi alocada
void testa_inicializacao (bool test, const char *descricao) {

    //Teste para ver se inicializou corretamente
    if (test)
        return;

    //Caso não tenha inicializado, printa a mensagem e sai do programa
    fprintf (stderr, "Não foi possível inicializar %s\n", descricao);
    exit (1);

}

//Função que carrega o mapa de uma arquivo para a memória
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

//Função que pega uma sub-bitmap de uma bitmap maior, é passada as coordenadas da sub-bitmap e as dimensões dela
//É retornada a sub-bitmap 
ALLEGRO_BITMAP* sprite_grab(int coord_x_inicial, int coord_y_inicial, int largura_sprite, int altura_sprite) {
    
    //Seleciona uma sub-sprite da imagem grande
    //Todos os valores foram previamente calculados usando o software 'gimp'
    //al_create_sub_bitmap é facilmente explicável
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap (sheet.bitmap_sprites, coord_x_inicial, coord_y_inicial, largura_sprite, altura_sprite);
    testa_inicializacao(sprite, " o sub-sprite");
    return sprite;
    
}

//Carrega as sprites de uma arquivo para a memória
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

//Destrói todas as sprites do jogo
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

//Função que destroi todo o entorno do jogador
void localiza_explosao (t_objeto*rochas, t_objeto *diamantes, t_player player) {

    //Percorre todas as pedras e, se achar uma pedra que está logo entorno do jogador, destroi ela
    for (int i = 0; i < N_ROCHAS; i++) {
        //If's gigantescos que basicamente testam se acham uma rocha entorno do jogador, um quadrado de 3x3
        if (((rochas[i].coord_x == player.coord_x) && (rochas[i].coord_y == player.coord_y + DIREITA)) || ((rochas[i].coord_x == player.coord_x) && (rochas[i].coord_y == player.coord_y + ESQUERDA)))
            rochas[i].morto = true;
        
        if (((rochas[i].coord_x == player.coord_x + BAIXO) && (rochas[i].coord_y == player.coord_y + DIREITA)) || ((rochas[i].coord_x == player.coord_x + BAIXO) && (rochas[i].coord_y == player.coord_y + ESQUERDA)) 
                                                                                                                        || ((rochas[i].coord_x == player.coord_x + BAIXO) && (rochas[i].coord_y == player.coord_y)))
            rochas[i].morto = true;
        
        if (((rochas[i].coord_x == player.coord_x + CIMA) && (rochas[i].coord_y == player.coord_y + DIREITA)) || ((rochas[i].coord_x == player.coord_x + CIMA) && (rochas[i].coord_y == player.coord_y + ESQUERDA)) 
                                                                                                                        || ((rochas[i].coord_x == player.coord_x + CIMA) && (rochas[i].coord_y == player.coord_y)))
            rochas[i].morto = true;
        
    }

    //Percorre todos os diamantes e, se achar um diamante que está logo entorno do jogador, destroi ele
    for (int i = 0; i < N_DIAMANTES; i++) {
        //If's gigantescos que basicamente testam se acham um diamante entorno do jogador, um quadrado de 3x3
        if (((diamantes[i].coord_x == player.coord_x) && (diamantes[i].coord_y == player.coord_y + DIREITA)) || ((diamantes[i].coord_x == player.coord_x) && (diamantes[i].coord_y == player.coord_y + ESQUERDA)))
            diamantes[i].morto = true;
        
        if (((diamantes[i].coord_x == player.coord_x + BAIXO) && (diamantes[i].coord_y == player.coord_y + DIREITA)) || ((diamantes[i].coord_x == player.coord_x + BAIXO) && (diamantes[i].coord_y == player.coord_y + ESQUERDA)) 
                                                                                                                        || ((diamantes[i].coord_x == player.coord_x + BAIXO) && (diamantes[i].coord_y == player.coord_y)))
            diamantes[i].morto = true;
        
        if (((diamantes[i].coord_x == player.coord_x + CIMA) && (diamantes[i].coord_y == player.coord_y + DIREITA)) || ((diamantes[i].coord_x == player.coord_x + CIMA) && (diamantes[i].coord_y == player.coord_y + ESQUERDA)) 
                                                                                                                        || ((diamantes[i].coord_x == player.coord_x + CIMA) && (diamantes[i].coord_y == player.coord_y)))
            diamantes[i].morto = true;
    }

    //Destroi o mapa logo ao entorno do jogador, um quadrado de 3x3
    mapa[player.coord_x][player.coord_y].item = EXPLODIDO;
    mapa[player.coord_x][player.coord_y + DIREITA].item = EXPLODIDO;
    mapa[player.coord_x][player.coord_y + ESQUERDA].item = EXPLODIDO;
    mapa[player.coord_x + BAIXO][player.coord_y].item = EXPLODIDO;
    mapa[player.coord_x + BAIXO][player.coord_y + DIREITA].item = EXPLODIDO;
    mapa[player.coord_x + BAIXO][player.coord_y + ESQUERDA].item = EXPLODIDO;
    mapa[player.coord_x + CIMA][player.coord_y].item = EXPLODIDO;
    mapa[player.coord_x + CIMA][player.coord_y + DIREITA].item = EXPLODIDO;
    mapa[player.coord_x + CIMA][player.coord_y + ESQUERDA].item = EXPLODIDO;

}

//Trata do tempo do jogo, não é uma função perfeita, mas funciona razoavelmente bem
void trata_tempo (int *delay_tempo, int ajuda) {

    if (*delay_tempo % TEMPORIZADOR == 0) {
        if (ajuda) {
            tempo = tempo;
        }
        else
            tempo = tempo - 1;
        *delay_tempo = 0;
    }
    if (tempo == 0) {
        al_play_sample (sample_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        morreu = 1;
    }

}

//Pega o último score do arquivo
void pega_ultimo_score (FILE *arq) {

    fscanf (arq, "%d", &score);
    while (1) {
        if (feof (arq)){
            break;
        }
        fscanf (arq, "%d", &score);
    }

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

    sample_select = al_load_sample ("./resources/select.wav");
    testa_inicializacao (sample_select, "o sample select");

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

void estado_jogando (FILE *arq, t_player player, t_objeto * rochas, t_objeto* diamantes, int largura_mapa, int altura_mapa, unsigned char * key, ALLEGRO_EVENT *evento) {

    int desenha = 0;
    int ajuda = 0;
    int sprite_diamante = 0;
    int sprite_explosao = 0;
    bool ida = true;
    bool flag_diamante = false;

    t_contaFrames delay;

    delay.atualizacao_objeto = 0;
    delay.evento_morte = 0;
    delay.animacao_player = 0;
    delay.animacao_diamante = 0;
    delay.tempo = 0;

    while (estado_jogo == JOGANDO) {
        al_wait_for_event (fila, evento);
    
        switch (evento->type) {

            case ALLEGRO_EVENT_TIMER:

                trata_tempo (&delay.tempo, ajuda);
                delay.tempo++;

                //Se o jogador apertou a tecla 'H', vai para o estado_ajuda 
                if (ajuda) {
                    estado_ajuda (key, evento);
                    ajuda = 0;
                }
 
                //Se o jogador morreu, trata desse evento
                if (morreu) {
                    localiza_explosao (rochas, diamantes, player);
                    anima_explosao (&sprite_explosao, &delay.morte);
                }
                delay.morte++;

                //Cuida da pontuação/progresso do jogador
                pontuacao (&flag_diamante);

                //Atualiza os objetos do mapa e testa para ver se o jogador morreu
                atualiza_objetos_mapa (arq, &delay.atualizacao_objeto, rochas, diamantes, &delay.evento_morte);
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
                ajuda = controle (arq, diamantes, rochas, evento, delay.atualizacao_objeto, &player.coord_x, &player.coord_y, &player.direcao, &flag_diamante);
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
            desenha_mapa (rochas, diamantes, largura_mapa, altura_mapa, &player, sprite_diamante, sprite_explosao);
            disp_post_draw ();
            desenha = 0;
        }
    }
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
