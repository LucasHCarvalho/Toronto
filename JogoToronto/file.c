// Arquivos para inicialização da biblioteca Allegro
#include <allegro5/allegro.h>;
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include "objetos.h";

enum TECLAS {CIMA, BAIXO, DIREITA, ESQUERDA};


int main()
{   
    // Variáveis do jogo
//____________________________________________________________________
    const int altura_t = 900;
    const int largura_t = 1010;

    int pos_x = 100;
    int pos_y = 100;
    int contador = 0;
    int FPS = 60;
    int flag = 0;
    int largura_sprite = 42;
    int altura_sprite = 96;
    int regiao_x_folha = 0;
    int regiao_y_folha = 0;
    int linha_atual = 1;
    int linhas_folha = 2;
    int coluna_atual = 1;
    int coluna_folha = 4;
    int frames_sprite = 3;
    int cont_frames = 0;
    int pos_x_sprite = 50;
    int pos_y_sprite = 50;
    int vel_x_sprite = 3;
    int vel_y_sprite = 5;



    bool fim = false;
    bool teclas[] = { false, false, false, false };
    bool desenhar = true;

    // Inicialização da Allegro e do Display
//____________________________________________________________________
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* imagem = NULL;
    ALLEGRO_BITMAP* imagemFull = NULL;
    ALLEGRO_SAMPLE* somteste = NULL;

    // Programa
//____________________________________________________________________
    if (!al_init())
    {
        al_show_native_message_box(NULL, "Aviso!", NULL, "FALHA AO CARREGAR ALLEGRO!", NULL, NULL);
        return -1;
    }

    display = al_create_display(largura_t, altura_t);

    if (!display)
    {
        al_show_native_message_box(NULL, "AVISO!", NULL, "FALHAO AO CRIAR O DISPLAY", NULL, NULL);
        return -1;
    }
    
    // Inicialização de Addons
//____________________________________________________________________
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon(); 
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(5);

    timer = al_create_timer(2.0 / FPS);


    // Criação da fila e demais dispositivos
//____________________________________________________________________
    fila_eventos = al_create_event_queue();
    //imagem = al_load_bitmap("sticker.bmp");
    imagemFull = al_load_bitmap("images/stickerfull.bmp");
    al_convert_mask_to_alpha(imagemFull, al_map_rgb(255, 0, 255));
    
    somteste = al_load_sample("aiao.ogg");

    // Registro de sources
//____________________________________________________________________
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    // Funcoes Iniciais

    // Loop Principal
//____________________________________________________________________
    al_start_timer(timer);

    while (!fim)
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(fila_eventos, &ev);       

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                fim = true;
            }

            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
                    teclas[CIMA] = true;  
                    al_play_sample(somteste, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[BAIXO] = true;        
                    al_play_sample(somteste, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[DIREITA] = true;    
                    al_play_sample(somteste, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[ESQUERDA] = true;
                    al_play_sample(somteste, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);               
                    break;
            }
        }

        if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                teclas[CIMA] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                teclas[BAIXO] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                teclas[DIREITA] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                teclas[ESQUERDA] = false;
                break;
            }

        }

        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            fim = true;
        }

        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (teclas[DIREITA])
            {
                pos_x_sprite += teclas[DIREITA] * 10;
                coluna_atual += 1;
                if (coluna_atual >= coluna_folha)
                {
                    coluna_atual = 0;
                }
                vel_x_sprite = 1;
            }
            if (teclas[ESQUERDA])
            {
                pos_x_sprite -= teclas[ESQUERDA] * 10;
                coluna_atual += 1;
                if (coluna_atual >= coluna_folha)
                {
                    coluna_atual = 0;
                }
                vel_x_sprite = -1;
            }
            
            pos_y_sprite += teclas[BAIXO] * 10;
            pos_y_sprite -= teclas[CIMA] * 10;
        }
        else if (pos_x_sprite >= largura_t-20)
        {
            pos_x_sprite = -pos_x_sprite;
        }
        else if (pos_x_sprite <= largura_t - 20)
        {
            pos_x_sprite = -pos_x_sprite;
        }
        else if (pos_y_sprite >= altura_t -20)
        {
            pos_y_sprite = -pos_y_sprite;
        }
        else if (pos_y_sprite <= altura_t - 20)
        {
            pos_y_sprite = -pos_y_sprite;
        }

        // DESENHO
//____________________________________________________________________
        if (vel_x_sprite > 0)
        {
            al_draw_bitmap_region(imagemFull, regiao_x_folha + (coluna_atual * largura_sprite), regiao_y_folha, largura_sprite, altura_sprite, pos_x_sprite, pos_y_sprite, flag);
        }
        else
        {
            al_draw_scaled_bitmap(imagemFull, regiao_x_folha + (coluna_atual * largura_sprite), regiao_y_folha, largura_sprite, altura_sprite, pos_x_sprite + largura_sprite, pos_y_sprite, -largura_sprite, altura_sprite, 0);
        }
        //al_draw_bitmap(imagemFull, pos_x, pos_y, 0);
        // al_draw_filled_rectangle(pos_x, pos_y, pos_x + 10, pos_y + 10, al_map_rgb(255, 255, 255));
        al_flip_display();
        al_clear_to_color(al_map_rgb(255, 255, 255));
    }
     
     
    // Finalização do programa
//____________________________________________________________________
    al_destroy_display(display);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(imagemFull);
    al_destroy_sample(somteste);

    return 0;
}
