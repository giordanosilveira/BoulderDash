#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "libdefines.h"

#include "lib_estados_jogo.h"
#include "libgraficos.h"
#include "lib_inicializacao.h"
#include "libfisica.h"

int main () {

    t_objeto *diamantes;
    t_objeto *rochas;
    t_player player;
    ALLEGRO_EVENT evento;
    int largura_mapa, altura_mapa;
    unsigned char key[ALLEGRO_KEY_MAX];

    inicializa_variaveis (diamantes, rochas, &largura_mapa, &altura_mapa, &player);
    al_register_event_source (fila, al_get_keyboard_event_source());
    al_register_event_source (fila, al_get_display_event_source(display));
    al_register_event_source (fila, al_get_timer_event_source(timer));
    al_start_timer (timer);
    memset (key, 0, ALLEGRO_KEY_MAX * sizeof(unsigned char));

    estado_jogo = MENU;

    while (estado_jogo != TERMINOU) {
        switch (estado_jogo) {
            case MENU        : estado_menu      (key, &evento); break;
            case JOGANDO     : estado_jogando   (player, rochas, largura_mapa, altura_mapa, key, &evento); break;
            case FIMDAPARTIDA: estado_standby   (&evento, key); break;
            case GAMEOVER    : estado_gameover  (&evento, key); break;
            default: break;
        }
    }

    free (mapa[0]);
    free (mapa);
    free (diamantes);
    free (rochas);

    destruindo_sprites ();
    al_destroy_font (font);
    al_destroy_display (display);
    al_destroy_bitmap (buffer);
    al_destroy_timer (timer);
    al_destroy_event_queue (fila);

    return 0;
}