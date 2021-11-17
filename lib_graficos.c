
#include "libDefines.h" 
#include "lib_graficos.h"

void disp_pre_draw () {
    al_set_target_bitmap (buffer);
}

void disp_post_draw () {
    al_set_target_backbuffer (display);

    //Estica o buffer para caber na tela
    al_draw_scaled_bitmap (buffer, 0, 0, LARGURA_BUFFER, ALTURA_BUFFER, 0, 0, LARGURA_DISPLAY, ALTURA_DISPLAY, 0);
    
    al_flip_display ();
}

void desenha_mapa (t_objeto *rochas, t_objeto * diamante, int largura, int altura, t_player * player, int sprite_diamante, int sprite_explosao) {

    //Variável que auxiliam para printar o sprite correto do player
    int aux_ani_dir = 14;
    int aux_ani_esq = 7;

    //Percorre a matriz e, de acordo com o item da matriz, printa o correto
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            
            switch (mapa[i][j].item) {

                case PAREDE:    //Se for o bloco parede                           
                    al_draw_bitmap (sheet.mapa[0], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case TIJOLO:    //Se for o bloco tijolo
                    al_draw_bitmap (sheet.mapa[2], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break; 
                case PLAYER:    //Se for o bloco player, vê qual direção ele está indo para printar a sprite certa
                    if (player->direcao == DIREITA) {
                        al_draw_bitmap (sheet.player[player->tipo + aux_ani_dir], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    }
                    else if (player->direcao == ESQUERDA) {
                        al_draw_bitmap (sheet.player[player->tipo + aux_ani_esq], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    }
                    else
                        al_draw_bitmap (sheet.player[player->tipo], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case TERRA:     //Se for o bloco terra
                    al_draw_bitmap (sheet.mapa[3], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case PORTA:     //Se for o bloco 
                    al_draw_bitmap (sheet.mapa[1], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;
                case EXPLODIDO:
                    al_draw_bitmap (sheet.explosao[sprite_explosao], j*LARGURA_BITMAP_BLOCO, i*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
                    break;

                default: break; //Se for casa vazia/vaga, não faz nada
            }
            
        }
    }
    //Percorre o vetor de rochas e printa elas caso não estejam mortas
    for (int i = 0; i < N_ROCHAS; i++){
        if (rochas[i].morto == false)
            al_draw_bitmap (sheet.mapa[5], rochas[i].coord_y*LARGURA_BITMAP_BLOCO, rochas[i].coord_x*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
    }
    //Percorre o vetor de diamante e printa eles caso não estejam mortos
    for (int i = 0; i < N_DIAMANTES; i++){
        if (diamante[i].morto == false)
            al_draw_bitmap (sheet.diamante[sprite_diamante], diamante[i].coord_y*LARGURA_BITMAP_BLOCO, diamante[i].coord_x*ALTURA_BITMAP_BLOCO + CORRECAO_DISPLAY, 0);
    }
}

void desenha_hud () {
    al_draw_textf (font, al_map_rgb(255, 255, 0), 10, 7, 0, "%d", diamante_minimos);
    al_draw_bitmap (sheet.diamante[0], 26, 3 ,0);
    al_draw_textf (font, al_map_rgb(255, 255, 255), 10 + 33, 7, 0, "%d", pontuacao_minima);
    al_draw_textf (font, al_map_rgb(255, 255, 0), LARGURA_BUFFER/2, 7, 0, "%d", contador_diamantes);
    al_draw_textf (font, al_map_rgb(255, 255, 255), LARGURA_BUFFER/2 + 40, 7, 0, "%d", pontos);
    al_draw_textf (font, al_map_rgb(255, 255, 255), LARGURA_BUFFER/2 + 80, 7, 0, "%d", tempo);
}

void desenha_menu () {
    
    al_clear_to_color (al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle (0, 0, LARGURA_BUFFER, ALTURA_BUFFER/2, al_map_rgb (30, 144, 255));
    al_draw_textf (font, al_map_rgb (0, 0, 0), LARGURA_BUFFER/2, (ALTURA_BUFFER/2)/2, ALLEGRO_ALIGN_CENTER, "BOULDER");
    al_draw_textf (font, al_map_rgb (0, 0, 0), LARGURA_BUFFER/2, (ALTURA_BUFFER/2)/2 + 10, ALLEGRO_ALIGN_CENTER, "DASH");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 100, ALLEGRO_ALIGN_CENTER, "APERTE 'ENTER' PARA INICIAR");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 100 + 10, ALLEGRO_ALIGN_CENTER, "APERTE 'ESC' PARA SAIR");

}

void desenha_ajuda (int easter_egg, int delay_animacao, int *sprite_animacao, bool* ida) {

    al_clear_to_color (al_map_rgb(0,0,0));
    //Se der o tempo necessário para mostrar o easter egg, mostra o easter egg
    if (easter_egg == DELAY_EASTER_EGG) {
        //A animção do jogador segue o mesmo modelo da função anima_jogador
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

void desenha_standby () {

    al_clear_to_color (al_map_rgb(0, 0, 0));
    
    //Escreve as mensagens
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2, ALLEGRO_ALIGN_CENTER, "Parabens você venceu :)");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 10, ALLEGRO_ALIGN_CENTER, "Aperte 'ENTER' ou 'ESC' para sair" );

}

void desenha_gameover () {

    al_clear_to_color (al_map_rgb(0, 0, 0));
    //Escreve as mensagens
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2, ALLEGRO_ALIGN_CENTER, "Você perdeu! QUE PENA:'( ");
    al_draw_textf (font, al_map_rgb (255, 255, 255), LARGURA_BUFFER/2, ALTURA_BUFFER/2 + 10, ALLEGRO_ALIGN_CENTER, "Aperte 'ENTER' ou 'ESC' para sair" );

}

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

void anima_explosao (int *sprite_explosao, int *delay_morte) {

    //se der o tempo necessário para mudar a animação da explosão, troca o sprite da explosão
    if (*delay_morte % (DELAY_EXPLOSAO/8) == 0) {
        *sprite_explosao = *sprite_explosao + 1;
        //Cláusula necessário para as variáveis não crescerem infinitamente
        if (*sprite_explosao == 7) {
            *delay_morte = 0;
            *sprite_explosao = 0;
            estado_jogo = GAMEOVER;                                 //Muda o estado do jogo para game over
        }
    }
    
}
