// Arquivos para inicialização da biblioteca Allegro
#include <allegro5/allegro.h>;
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include "objetos.h";

enum TECLAS {CIMA, BAIXO, DIREITA, ESQUERDA};


int main()
{   
    // Variáveis do jogo
//____________________________________________________________________
    const int altura_t = 900;
    const int largura_t = 910;

    int pos_x = 100;
    int pos_y = 100;
    int contador = 0;
    int FPS = 60;
    int flag = 0;

    bool fim = false;
    bool teclas[] = { false, false, false, false };
    bool desenhar = true;

    // Inicialização da Allegro e do Display
//____________________________________________________________________
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* imagem = NULL;

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
    timer = al_create_timer(1.0 / FPS);

    // Criação da fila e demais dispositivos
//____________________________________________________________________
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("sticker.bmp");
    

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
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[BAIXO] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[DIREITA] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[ESQUERDA] = true;
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
            pos_x += teclas[DIREITA] * 10;
            pos_x -= teclas[ESQUERDA] * 10;
            pos_y += teclas[BAIXO] * 10;
            pos_y -= teclas[CIMA] * 10;
        }
        else if (pos_x >= largura_t-20)
        {
            pos_x = -pos_x;
        }
        else if (pos_x <= largura_t - 20)
        {
            pos_x = -pos_x;
        }
        else if (pos_y >= altura_t -20)
        {
            pos_y = -pos_y;
        }
        else if (pos_y <= altura_t - 20)
        {
            pos_y = -pos_y;
        }
          
        // DESENHO
//____________________________________________________________________
   
        al_draw_bitmap(imagem, pos_x, pos_y, flag);
        // al_draw_filled_rectangle(pos_x, pos_y, pos_x + 10, pos_y + 10, al_map_rgb(255, 255, 255));
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
     

    // Finalização do programa
//____________________________________________________________________
    al_destroy_display(display);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(imagem);
  

    return 0;
}
