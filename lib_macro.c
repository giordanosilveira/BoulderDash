#include "libDefines.h"
#include "lib_macro.h"
#include "lib_micro.h"

#include <stdio.h>
#include <stdlib.h>

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

