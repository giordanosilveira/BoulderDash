#include "libDefines.h"
#include "lib_geral.h"
#include "lib_graficos.h"
#include "lib_move.h"

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

void localiza_explosao (t_objeto*rochas, t_objeto *diamantes, t_player player) {

    //Percorre todas as pedras e, se achar uma pedra que está logo entorno do jogador, destroi ela
    for (int i = 0; i < N_ROCHAS; i++) {
        //If's gigantescos que basicamente testam se achamram uma rocha entorno do jogador, um quadrado de 3x3
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
        //If's gigantescos que basicamente testam se acharam um diamante entorno do jogador; um quadrado de 3x3
        if (((diamantes[i].coord_x == player.coord_x) && (diamantes[i].coord_y == player.coord_y + DIREITA)) || ((diamantes[i].coord_x == player.coord_x) && (diamantes[i].coord_y == player.coord_y + ESQUERDA)))
            diamantes[i].morto = true;
        
        if (((diamantes[i].coord_x == player.coord_x + BAIXO) && (diamantes[i].coord_y == player.coord_y + DIREITA)) || ((diamantes[i].coord_x == player.coord_x + BAIXO) && (diamantes[i].coord_y == player.coord_y + ESQUERDA)) 
                                                                                                                        || ((diamantes[i].coord_x == player.coord_x + BAIXO) && (diamantes[i].coord_y == player.coord_y)))
            diamantes[i].morto = true;
        
        if (((diamantes[i].coord_x == player.coord_x + CIMA) && (diamantes[i].coord_y == player.coord_y + DIREITA)) || ((diamantes[i].coord_x == player.coord_x + CIMA) && (diamantes[i].coord_y == player.coord_y + ESQUERDA)) 
                                                                                                                        || ((diamantes[i].coord_x == player.coord_x + CIMA) && (diamantes[i].coord_y == player.coord_y)))
            diamantes[i].morto = true;
    }

    //Destroi o mapa no entorno do jogador; um quadrado de 3x3
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

void trata_tempo (int *delay_tempo, int ajuda) {
    /*Como o jogo atualiza a 60fps, ou seja, 60 frames por segundo, a cada 60 ALLEGRO_TIMER_EVENTE eu diminuo um da variável tempo.
    Não se passa um segundo perfeitamente, mas é uma aproximação muito boa*/

    //Testa para ver se a variável tempo chegou no seu máximo 
    if (*delay_tempo % TEMPORIZADOR == 0) {
        if (ajuda) {                                            //Caso o jogador esteja no estado ajuda, o tempo não diminui. Uma espécia de pause
            tempo = tempo;
        }
        else                                                    //Diminui um segundo do tempo total
            tempo = tempo - 1;
        *delay_tempo = 0;
    }
    //Caso o tempo chegue ao fim, o jogador morre
    if (tempo == 0) {
        al_play_sample (sample_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        morreu = 1;
    }

}

void pega_ultimo_score (FILE *arq) {

    fscanf (arq, "%d", &score);
    //Percorre o arquivo e vai pegando as pontuações até a última
    while (1) {
        if (feof (arq)){                            //Testa para ver se chegou ao final do arquivo
            break;
        }
        fscanf (arq, "%d", &score);
    }

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
    int easter_egg = 0;                                             //Variável que controla a aparição do easter egg
    int delay_animacao = 0;
    int sprite_animacao = 0;
    bool ida = true;

    //Reinicializa com 0 a variável key
    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));
    
    //Enquanto não fechou o menu ajuda
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

    int desenha = 0;                                        //Variável que controla se é possível desenhar
    int ajuda = 0;                                          //Variável que controla se a ajuda foi pedida    
    int sprite_diamante = 0;                                //Variável que auxilia na animação dos diamantes
    int sprite_explosao = 0;                                //Variável que auxilia na animação da explosão
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
        //Se pode desenhar e não tem nenhuma outra pendência na fila de eventos, desenha o jogo
        if ((desenha) && (al_is_event_queue_empty(fila))) {
            disp_pre_draw ();
            al_clear_to_color (al_map_rgb(0,0,0));                      //Fundo preto
            desenha_hud ();
            desenha_mapa (rochas, diamantes, largura_mapa, altura_mapa, &player, sprite_diamante, sprite_explosao);
            disp_post_draw ();
            desenha = 0;                                         
        }
    }
}

void estado_standby (ALLEGRO_EVENT* evento, unsigned char *key) {

    //Reinicializa com 0 a variável key
    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));

    //Enquanto está no estado standby
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
    
    //Reinicializa com 0 a variável key
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
