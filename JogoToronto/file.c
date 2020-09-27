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

//void initPersonagem(struct PersonagemPrincipal *s);
//void desenhaPersonagem(struct PersonagemPrincipal *s);

int main()
{   
    // Variáveis do jogo
    const int largura_t = 900;
    const int altura_t = 900;

    int pos_x = 100;
    int pos_y = 100;

    bool fim = false;
    ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;

    bool teclas[] = { false, false, false, false };
    bool desenhar = true;

    int contador = 0;
    int FPS = 60;


    ALLEGRO_TIMER* timer = NULL;
    // Criar a fonte
    // ALLEGRO_FONT *font20 = NULL;
    // font20 = al_load_font("arial.ttf", 20, NULL);

    // Inicialização da Allegro e do Display
    ALLEGRO_DISPLAY *display = NULL;
    
    if (!al_init())
    {
        al_show_native_message_box(NULL, "Aviso!", NULL, "FALHA AO CARREGAR ALLEGRO!", NULL, NULL);
        return -1;
    }

    display = al_create_display(altura_t, largura_t);

    if (!display)
    {
        al_show_native_message_box(NULL, "AVISO!", NULL, "FALHAO AO CRIAR O DISPLAY", NULL, NULL);
        return -1;
    }
    
    // Inicialização de AddOns
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    timer = al_create_timer(1.0 / FPS);

    // Criação da fila e demais dispositivos
    fila_eventos = al_create_event_queue();
    al_init_primitives_addon();

    // Registro de sources
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    // Funcoes Iniciais

    //initPersonagem(&sticker);

    // Loop Principal
    
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
        /*else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            pos_x = ev.mouse.x;
            pos_y = ev.mouse.y;
        }
        
         else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (ev.mouse.button & 2) {
                fim = true;
            }
            else if (ev.mouse.button & 1) {
                desenhar = !desenhar;
            }
        }*/


        // Lógica do jogo

        // PARTE COMENTADA EXCLUI POSSIBILIDADE DE MOVIMENTO EM DIAGONAL
        //if (teclas[CIMA] 
        //{
            
        //}
        //else if (teclas[BAIXO])
        //{
            
        //}
        // else if (teclas[ESQUERDA])
        //{
            
        //}
        //else if (teclas[DIREITA])
        //{
            
        //}
        
          
        // DESENHO

        al_draw_filled_rectangle(pos_x, pos_y, pos_x + 10, pos_y + 10, al_map_rgb(255, 255, 255));
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
       



    // Funções de Desenho
    //int pixels_x = al_get_display_width(display);
    //int pixels_y = al_get_display_height(display);

    


    // Funções de Display
    //desenhaPersonagem(&sticker);
    al_flip_display();
    // al_rest(5);

    // Finalização do programa
    // al_destroy_font(font20);
    al_destroy_display(display);
    al_destroy_event_queue(fila_eventos);
  

    return 0;
}


// Personagem

/*void initPersonagem(struct PersonagemPrincipal *s);
{
    s->sticker.x = 20;
    s->sticker.y = altura_t / 2;
    s->sticker.ID = JOGADOR;
    s->sticker.vidas = 3;
    s->sticker.velocidade = 5;
    s->sticker.borda_x = 6;
    s->sticker.borda_y = 6;
    s->sticker.pontos = 0;
}

void desenhaPersonagem(struct PersonagemPrincipal *s)
{
    s->al_draw_filled_rectangle(sticker.x, sticker.y -9, sticker.x + 10, sticker.y -7, al_map_rgb(255, 255, 255));
}*/