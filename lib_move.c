#include "libDefines.h"
#include "lib_move.h"

void colisao (FILE*arq, int delay_pedra, int direcao, int *coord_p_x, int *coord_p_y, int *anterior_coord_p_x, int *anterior_coord_p_y, bool *flag_dimas) {

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
    //Se o jogador bateu numa rocha, se deu delay necessário para que a pedra se mova foi cumprido, move a pedra
    for (int i = 0; i < N_ROCHAS; i++){
        
        //Se achou uma pedra
        if ((*coord_p_x == rochas[i].coord_x) && (*coord_p_y == rochas[i].coord_y)) {               
            //Se o delay necessário foi cumprido
            if ((mapa[*coord_p_x][rochas[i].coord_y + direcao].item == NADA) && (delay_pedra == ATUALIZACAO_OBJETO)) {
                    
                rochas[i].coord_y = rochas[i].coord_y + direcao;    //Move a pedra
                mapa[*coord_p_x][*coord_p_y].item = PLAYER;
                mapa[*coord_p_x][*coord_p_y + direcao].item = VAGO;
                    
            }
            //Caso contrário, o player permanece no mesmo lugar
            else {
                *coord_p_y = *anterior_coord_p_y;
                *coord_p_x = *anterior_coord_p_x;
            }
        }
    }
    
    //Se o jogador bateu no diamante, "pega" o diamante, toca o sample do diamante e atualiza a flag_dimas para 1, para avisar que ele pegou o diamante
    for (int i = 0; i < N_DIAMANTES; i++) {

        if (((*coord_p_x == diamantes[i].coord_x) && (*coord_p_y == diamantes[i].coord_y)) && diamantes[i].morto == false) {
            al_play_sample (sample_coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            *flag_dimas = true;
            //Aquele diamante que foi pego está morto
            diamantes[i].morto = true;
        }
    }

    //Se o jogador chegou na porta, ele ganhou
    if (mapa[*coord_p_x][*coord_p_y].item == PORTA) {
        estado_jogo = FIMDAPARTIDA;
    }

}

int controle (FILE *arq, ALLEGRO_EVENT *evento, int delay_pedra, int *coord_p_x, int *coord_p_y, int *direcao, bool* flag_dimas) {


    int ajuda = 0;                                                  //Variável que controla se o jogador pediu ajuda
    
    //Variável para guardar o posição anterior do jogador; necessária caso o jogador fique na mesma posição
    int anterior_coord_p_x, anterior_coord_p_y;
    anterior_coord_p_x = *coord_p_x;
    anterior_coord_p_y = *coord_p_y;
    
    //Cláusula para o jogador não mexer o player quando ele morre 
    if (! morreu) {
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
        colisao (arq, delay_pedra, *direcao, coord_p_x, coord_p_y, &anterior_coord_p_x, &anterior_coord_p_y, flag_dimas);
        mapa[anterior_coord_p_x][anterior_coord_p_y].item = NADA;
        mapa[*coord_p_x][*coord_p_y].item = PLAYER;

        //Retorna a variável ajuda
        return ajuda;
    }
    else {
        return 0;
    }

}

void gravidade (t_objeto *secundario, t_objeto *primario, int tam_sec, int tam_pri, int *coord_x_obj, int *coord_y_obj, bool *movimentando) {

    //Se o item abaixo for uma terra, não acontece nada
    if ((mapa[*coord_x_obj + 1][*coord_y_obj].item == TERRA)) {
        *movimentando = false;
        return;
    }   

    //Se não tiver nada abaixo, move o objeto para baixo, e define a variável movimentando como verdadeira
    if (mapa[*coord_x_obj + 1][*coord_y_obj].item == NADA) {
        *movimentando = true;
        *coord_x_obj = *coord_x_obj + BAIXO;
        mapa[*coord_x_obj][*coord_y_obj].item = VAGO;
        mapa[*coord_x_obj - 1][*coord_y_obj].item = NADA;
        return;
    }

        //Se abaixo dele estiver com uma pedra
    for (int i = 0; i < tam_pri; i++) {
        if ((*coord_x_obj + 1 == primario[i].coord_x) && (*coord_y_obj == primario[i].coord_y)) {
                
            //se a coluna inferior esquerda estiver vaga e, se a coluna loga a esquerda estiver vaga, atualiza o objeto para a esquerda e define a variável movimentando como verdadeira
            //e a posição anterior do objeto fica como vazia  
            if ((mapa[*coord_x_obj + 1][*coord_y_obj - 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj - 1].item == NADA)) {
                *movimentando = true;
                *coord_y_obj = *coord_y_obj + ESQUERDA;
                mapa[*coord_x_obj][*coord_y_obj].item = VAGO;
                mapa[*coord_x_obj][*coord_y_obj + 1].item = NADA;
            }
            //Caso não dê para ir a esquerda, testa para ver se foi para a direita
            //se a coluna inferior direita estiver vaga e, se a coluna loga a direita estiver vaga, atualiza o objeto para a direita e define a variável movimentando como verdadeira
            //e a posição anterior do objeto fica como vazia
            else if ((mapa[*coord_x_obj + 1][*coord_y_obj + 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj + 1].item == NADA)) {
                *movimentando = true;
                *coord_y_obj = *coord_y_obj + DIREITA;
                mapa[*coord_x_obj][*coord_y_obj].item = VAGO;
                mapa[*coord_x_obj][*coord_y_obj - 1].item = NADA;
            }
            //Se o objeto estiver travado, então define a variável movimentando como falsa
            else
                *movimentando = false;
        }
    }
    for (int i = 0; i < tam_sec; i++) {
        if ((*coord_x_obj + 1 == secundario[i].coord_x) && (*coord_y_obj == secundario[i].coord_y)) {
                
            //se a coluna inferior esquerda estiver vaga e, se a coluna loga a esquerda estiver vaga, atualiza o objeto para a esquerda e define a variável movimentando como verdadeira
            //e a posição anterior do objeto fica como vazia  
            if ((mapa[*coord_x_obj + 1][*coord_y_obj - 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj - 1].item == NADA)) {
                *movimentando = true;
                *coord_y_obj = *coord_y_obj + ESQUERDA;
                mapa[*coord_x_obj][*coord_y_obj].item = VAGO;
                mapa[*coord_x_obj][*coord_y_obj + 1].item = NADA;
            }
            //Caso não dê para ir a esquerda, testa para ver se foi para a direita
            //se a coluna inferior direita estiver vaga e, se a coluna loga a direita estiver vaga, atualiza o objeto para a direita e define a variável movimentando como verdadeira
            //e a posição anterior do objeto fica como vazia
            else if ((mapa[*coord_x_obj + 1][*coord_y_obj + 1].item == NADA) && (mapa[*coord_x_obj][*coord_y_obj + 1].item == NADA)) {
                *movimentando = true;
                *coord_y_obj = *coord_y_obj + DIREITA;
                mapa[*coord_x_obj][*coord_y_obj].item = VAGO;
                mapa[*coord_x_obj][*coord_y_obj - 1].item = NADA;
            }
            //Se o objeto estiver travado, então define a variável movimentando como falsa
            else
                *movimentando = false;
        }
    }
}

void deslizamento (FILE*arq, int frames_evento_morte) {

    for (int i = 0; i < N_ROCHAS; i++) {
        
        //Se a rocha não estiver morta, aplica a gravidade nela
        if (rochas[i].morto == false)   
            gravidade (diamantes, rochas, N_DIAMANTES, N_ROCHAS, &rochas[i].coord_x, &rochas[i].coord_y, &rochas[i].movimentando);
        
        //Apliquei um delay para a morte do jogador não ficar instantânea
        if (frames_evento_morte == DELAY_MORTE_JOGADOR)

            //Se o jogador morreu, toca o sample explosão, escreve o score no arquivo e o jogo termina
            if ((mapa[rochas[i].coord_x + 1][rochas[i].coord_y].item == PLAYER) && (rochas[i].movimentando == true)) {
                al_play_sample (sample_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                morreu = 1;
            }
    }
    for (int i = 0; i < N_DIAMANTES; i++) {
        
        //Se o diamante não estiver morto, aplica a gravidade nele
        if (diamantes[i].morto == false)   
            gravidade (rochas, diamantes, N_ROCHAS, N_DIAMANTES, &diamantes[i].coord_x, &diamantes[i].coord_y, &diamantes[i].movimentando);
        
        //Apliquei um delay para a morte do jogador não ficar instantânea
        if (frames_evento_morte == DELAY_MORTE_JOGADOR)

            //Se o jogador morreu, toca o sample explosão, escreve o score no arquivo e o jogo termina 
            if ((mapa[diamantes[i].coord_x + 1][diamantes[i].coord_y].item == PLAYER) && (diamantes[i].movimentando == true)) {
                al_play_sample (sample_explosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);  
                morreu = 1;
            }
    } 
}

void atualiza_objetos_mapa (FILE *arq, int *atualizacao_objeto, int *delay_morte) {

    //Apliquei um delay para a atualização dos objetos do mapa, senão fica muito rapido
    //Só atualiza a cada 10 ALLEGRO_EVENT_TIMER
    //Se chegar o tempo de atualizar, aplica o deslizamento
    if (*atualizacao_objeto == ATUALIZACAO_OBJETO) {
        deslizamento (arq, *delay_morte);
        *atualizacao_objeto = 0;
    }

    //Define o delay morte para 0
    //Necessário para a variavél delay_morte não crescer infinitamente
    if (*delay_morte == DELAY_MORTE_JOGADOR)
        *delay_morte = 0;

}

