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
#include <Windows.h>

enum TECLAS {CIMA, BAIXO, DIREITA, ESQUERDA};

bool colisaoPulo (int px, int py, int pw, int ph, int ox, int oy, int oh, int ow)
{
    if (px + (pw/2) > ox && px < ox + ow && py + ph > oy && py < oy + oh)        
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool colisaoInferior(int px, int py, int pw, int ph, int ox, int oy, int oh, int ow)
{
    if (px + (pw / 2) > ox && px < ox + ow && py + ph > oy + oh)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool colisaoLateral(int px, int py, int pw, int ph, int ox, int oy, int oh, int ow)
{
    if (px + (pw / 2) > ox && px < ox + ow && (py > oy || py + ph < oy + oh))
    {
        return true;
    }
    else
    {
        return false;
    }
}


typedef struct
{
    int x;
    int y;
    int vel_x;
    int vel_y;
    int w;
    int h;

    ALLEGRO_BITMAP* image;

} Personagem;

typedef struct 
{
    int x;
    int y;
    int w;
    int h;

    ALLEGRO_BITMAP* image;
} Tiles ;

typedef struct obj
{
    int wx, wy, x, y, w, h;
    ALLEGRO_BITMAP* image;

} fcaindo;

fcaindo projeteis[50];

int gravidadeprojeteis = 1;
int caindo = 1;

void fall()
{

    for (int i = 0; i < 50; i++)
    {
        projeteis[i].y += gravidadeprojeteis;

    }
   
}

int main()
{   
    // Variáveis do jogo
//____________________________________________________________________
    const int altura_t = 600;
    const int largura_t = 800;

    const int altura_t2 = 800;
    const int largura_t2 = 1010;

    Tiles lapis[10];
    Personagem sticker;

    lapis[1].x = 120;
    lapis[1].y = 630;
    
    lapis[2].x = 400;
    lapis[2].y = 630;    
        

    sticker.h = 96;
    sticker.w = 42;
    sticker.x = 50;
    sticker.y = 645;
    sticker.vel_x = 3;
    sticker.vel_y = 5;

    int pos_x = 100;
    int pos_y = 100;
    int contador = 0;
    int FPS = 60;
    int regiao_x_folha = 0;
    int regiao_y_folha = 0;
    int linha_atual = 1;
    int linhas_folha = 2;
    int coluna_atual = 1;
    int coluna_folha = 4;
    int frames_sprite = 3;
    int cont_frames = 0;
    int pulos = 1;
    float velpulo = -17;
    int larguralapis = 155;
    int alturalapis = 20;

    const gravidade = 1;

    bool fim = false;
    bool fimmenu = false;
    bool teclas[] = { false, false, false, false };
    bool desenhar = true;
    bool pulo = false;

    // Inicialização da Allegro e do Display
//____________________________________________________________________
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_DISPLAY* display2 = NULL;
    ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* imagem = NULL;
    ALLEGRO_BITMAP* imagemFull = NULL;
    ALLEGRO_BITMAP* borracha = NULL;
    ALLEGRO_SAMPLE* somdefundo = NULL;
    ALLEGRO_SAMPLE* somcorrendo = NULL;
   // ALLEGRO_BITMAP* pointer = NULL; 
    ALLEGRO_BITMAP* menu = NULL;
    ALLEGRO_BITMAP* menuavancar = NULL;
    ALLEGRO_BITMAP* menusair = NULL;
    ALLEGRO_BITMAP* lapistile = NULL;


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
    borracha = al_load_bitmap("images/borracha.bmp");
    al_convert_mask_to_alpha(borracha, al_map_rgb(255, 0, 255));
    menu = al_load_bitmap("images/menu1.bmp");
    menuavancar = al_load_bitmap("images/menu2.bmp");
    menusair = al_load_bitmap("images/menu3.bmp");
    somdefundo = al_load_sample("trilha_sonora.ogg");
    somcorrendo = al_load_sample("Punch_04.wav");

    lapistile = al_load_bitmap("images/lapistile.bmp");
    al_convert_mask_to_alpha(lapistile, al_map_rgb(255, 0, 255));

    lapis[1].image = al_load_bitmap("images/lapistile.bmp");
    lapis[2].image = al_load_bitmap("images/lapistile.bmp");
    al_convert_mask_to_alpha(lapis[1].image, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(lapis[2].image, al_map_rgb(255, 0, 255));

    for (int i = 0; i < 10; i++)
    {
        lapis[i].h = 20;
        lapis[i].w = 150;
    }

    sticker.image = al_load_bitmap("images/stickerfull.bmp");
    
    // Registro de sources
//____________________________________________________________________
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(display));    
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    // Funcoes Iniciais

    for (int i = 0; i < 50; i++)
    {
        projeteis[i].image = al_load_bitmap("F.bmp");
        projeteis[i].y = 0;
        projeteis[i].x = i * 30;
        al_convert_mask_to_alpha(projeteis[i].image, al_map_rgb(255, 0, 255));
    }

    // Loop Principal
//____________________________________________________________________
    al_start_timer(timer);

    al_play_sample(somdefundo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    while (!fim)
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(fila_eventos, &ev);


        if (fimmenu != true) {

            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) // fecha o programa quando a tecla ESC for acionada
            {
                fimmenu = true;
                fim = true;
            }
            if (ev.mouse.x > 60 && ev.mouse.x < 340 &&
                ev.mouse.y > 150 && ev.mouse.y < 220) { // detecta posicao do mouse nos logos do menu para iniciar e destroir o menu ou encerrar o programa

                al_draw_bitmap(menuavancar, 0, 0, 0);
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {

                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    al_destroy_display(display);
                    display2 = al_create_display(largura_t2, altura_t2);
                    al_register_event_source(fila_eventos, al_get_display_event_source(display2));


                    fimmenu = true;
                }
            }
            else if (ev.mouse.x > 60 && ev.mouse.x < 340 &&
                ev.mouse.y > 365 && ev.mouse.y < 435) {

                al_draw_bitmap(menusair, 0, 0, 0);
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    fim = true;

                    return 0;
                }
            }
            else {

                al_draw_bitmap(menu, 0, 0, 0);

            }

        }
        if (fimmenu == true)
        {
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
                    al_play_sample(somcorrendo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[BAIXO] = true;
                    al_play_sample(somcorrendo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[DIREITA] = true;
                    al_play_sample(somcorrendo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[ESQUERDA] = true;
                    al_play_sample(somcorrendo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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

                if (colisaoInferior(sticker.x, sticker.y, sticker.w, sticker.h, lapis[1].x, lapis[1].y, lapis[1].h, lapis[1].w))
                {
                    sticker.y -= sticker.vel_y;
                }

                /*if (colisaoLateral(sticker.x, sticker.y, sticker.w, sticker.h, lapis[1].x, lapis[1].y, lapis[1].h, lapis[1].w))
                {
                    sticker.x -= teclas[DIREITA] * 10;
                }*/

                if (colisaoPulo(sticker.x, sticker.y, sticker.w, sticker.h, lapis[1].x, lapis[1].y, lapis[1].h, lapis[1].w))
                {
                    pulo = true;
                }

                /*if (sticker.x + sticker.w > lapis[1].x + 10 && sticker.x < lapis[1].w + lapis[1].x && sticker.y + sticker.h > lapis[1].y)
                {
                    pulo = true;
                }
                else if (sticker.x + sticker.w > lapis[2].x + 10 && sticker.x < lapis[2].w + lapis[2].x && sticker.y + sticker.h > lapis[2].y)
                {
                    pulo = true;
                }*/
                else if (sticker.y > altura_t2 - 147)
                {
                    pulo = true;
                }
                else
                {
                    pulo = false;
                }

                if (teclas[DIREITA])
                {
                    if (sticker.x <= largura_t2 - 50)
                    {
                        sticker.x += teclas[DIREITA] * 10;
                    }

                    coluna_atual += 1;
                    if (coluna_atual >= coluna_folha)
                    {
                        coluna_atual = 0;
                    }
                    sticker.vel_x = 1;

                }
                if (teclas[ESQUERDA])
                {
                    if (sticker.x >= 0)
                        sticker.x -= teclas[ESQUERDA] * 10;

                    coluna_atual += 1;
                    if (coluna_atual >= coluna_folha)
                    {
                        coluna_atual = 0;
                    }
                    sticker.vel_x = -1;
                }        
                if (teclas[CIMA] && pulo)
                {
                    sticker.vel_y = velpulo;
                    pulo = false;
                    teclas[CIMA] = false;
                }

                if (!pulo)
                {
                    sticker.vel_y += gravidade;
                }
                else
                {
                    sticker.vel_y = 0;
                }

                sticker.y += sticker.vel_y;
            }

            // DESENHO
    //____________________________________________________________________
            if (sticker.vel_x > 0)
            {
                al_draw_bitmap_region(imagemFull, regiao_x_folha + (coluna_atual * sticker.w), regiao_y_folha, sticker.w, sticker.h, sticker.x, sticker.y, 0);
            }
            else
            {
                al_draw_scaled_bitmap(imagemFull, regiao_x_folha + (coluna_atual * sticker.w), regiao_y_folha, sticker.w, sticker.h, sticker.x + sticker.w, sticker.y, -sticker.w, sticker.h, 0);
            }
            al_draw_bitmap(borracha, -1, 752, 0);
            for (int i = 101; i < 1000; i += 100)
            {
                al_draw_bitmap(borracha, i, 752, 0);
            }                        

            for (int i = 0; i < 50; i++)
            {   
                
                al_draw_bitmap(projeteis[i].image, projeteis[i].x, projeteis[i].y, 0);
                if (!pulo)
                {
                    fall();
                }
                else
                {
                    projeteis[i].y = 0;
                }
            }        
        }

            al_draw_bitmap(lapis[1].image, lapis[1].x, lapis[1].y, 0);
            al_draw_bitmap(lapis[2].image, lapis[2].x, lapis[2].y, 0);

            al_draw_bitmap(borracha, 1010, 752, 0);
            al_flip_display();
            al_clear_to_color(al_map_rgb(255, 255, 255));
        }

     
    // Finalização do programa
//____________________________________________________________________
    al_destroy_display(display2);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(sticker.image);
    al_destroy_bitmap(menu);
    al_destroy_bitmap(menuavancar);
    al_destroy_bitmap(menusair);
    al_destroy_bitmap(lapis[1].image);
    al_destroy_bitmap(lapis[2].image);
    al_destroy_bitmap(borracha);
    //al_destroy_bitmap(pointer);
    al_destroy_sample(somdefundo);
    al_destroy_sample(somcorrendo);
    return 0;
}


