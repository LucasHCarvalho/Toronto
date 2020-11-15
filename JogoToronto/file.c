//  Arquivos para inicialização da biblioteca Allegro
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
#include <time.h>
#include <stdlib.h>

bool teclas[] = { false, false, false, false };
enum TECLAS {CIMA, BAIXO, DIREITA, ESQUERDA};

const int altura_t = 653;
const int largura_t = 1162;

const int altura_t2 = 800;
const int largura_t2 = 1010;

const int numF = 20;

typedef struct Personagem
{
    int x;
    int y;
    int vel_x;
    int vel_y;
    int w;
    int h;
    int vidas;
    int score;

    ALLEGRO_BITMAP* image;

}Personagem;

typedef struct Projeteis
{
    int x;
    int y;
    int w;
    int h;
    int speed;
    bool live;

    ALLEGRO_BITMAP* image;

}Projeteis;

typedef struct Pontuacao
{
    int x;
    int y;
    int w;
    int h;
    bool live;

    ALLEGRO_BITMAP* image;

}Pontuacao;

void iniciarPersonagem(Personagem *sticker)
{
    sticker->h = 96;
    sticker->w = 42;
    sticker->x = 50;
    sticker->y = 645;
    sticker->vel_x = 3;
    sticker->vel_y = 5;
    sticker->vidas = 4;
    sticker->score = 0;

}

void iniciarPontuacao(Pontuacao *a)
{
    a->live = false;
    a->w = 20;
    a->h = 30;

}

void chamarPontuacao(Pontuacao *a)
{
    if (!a->live)
    {
        if (rand() % 10 == 0)
        {
            a->live = true;
            a->y = (rand() % 150) + 550;
            a->x = rand() % (largura_t2 - 30);
        }
    }
}

void atualizarPontuacao(Pontuacao *a)
{
    if (a->live)
    {
        if (a->y > altura_t2 || a->y < 0 || a->x > largura_t2 || a->x < 0)
        {
            a->live = false;
        }
    }
}

void colisaoPontuacao(Pontuacao *a, Personagem* sticker)
{
    if (a->live)
    {
        if (a->x - (a->w/2) < sticker->x + (sticker->w/2) &&
            a->x + (a->w/2) > sticker->x &&
            a->y - a->h < sticker->y + (sticker->h/2) &&
            a->y + a->h > sticker->y)
        {
            sticker->score++;
            a->live = false;
        }
    }
}

void iniciarProjeteis(Projeteis f[], int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        f[i].live = false;
        f[i].speed = rand() % 10 + 1;
        f[i].w = 30;
        f[i].h = 20;
    }
}

void chamarProjeteis(Projeteis f[], int tamanho)
{
    for (int i = 0; i < tamanho; i++)
        if (!f[i].live)
            if (rand() % 10 == 0)
            {
                f[i].live = true;
                f[i].y = 0;
                f[i].x = rand() % (largura_t2 - 30);
                break;
            }
}

void atualizarProjeteis(Projeteis f[], int tamanho)
{
    for (int i = 0; i < tamanho; i++)
        if (f[i].live)
        {
            f[i].y += f[i].speed;
            if (f[i].y > altura_t2)
                f[i].live = false;
        }
}

void colisaoProjeteis(Projeteis f[], Personagem *sticker, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (f[i].live)
        {
            if (f[i].x - (f[i].w/2) < sticker->x + (sticker->w/2) &&
                f[i].x + (f[i].w/2) > sticker->x &&
                f[i].y - f[i].h < sticker->y + (sticker->h/2) &&
                f[i].y + f[i].h > sticker->y)
            {
                sticker->vidas--;
                f[i].live = false;
            }
            else if (f[i].y > altura_t2)
                f[i].live = false;
        }
    }
}

void criarMenu(ALLEGRO_BITMAP *menu, ALLEGRO_BITMAP *menuinstrucao, ALLEGRO_BITMAP* instrucao, ALLEGRO_BITMAP* instrucao2, ALLEGRO_BITMAP *menusair, ALLEGRO_BITMAP *menuavancar, ALLEGRO_DISPLAY *display,
    ALLEGRO_DISPLAY *display2, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_EVENT ev, bool *fim, bool *fimmenu, bool *fiminstrucao)
{

    al_draw_bitmap(menu, 0, 0, 0);

    // fecha o programa quando a tecla ESC for acionada
    if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) 
    {
        *fim = true;
    }

    if (*fiminstrucao == false)
    {
        // detecta posicao do mouse nos logos do menu para iniciar e destroir o menu ou encerrar o programa
        if (ev.mouse.x > 15 && ev.mouse.x < 253 && ev.mouse.y > 294 && ev.mouse.y < 355)
        {
            al_draw_bitmap(menuavancar, 0, 0, 0);
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                al_clear_to_color(al_map_rgb(255, 255, 255));
                al_destroy_display(display);
                display2 = al_create_display(largura_t2, altura_t2);
                al_register_event_source(fila_eventos, al_get_display_event_source(display2));
                *fimmenu = true;
            }
        }
        else if (ev.mouse.x > 15 && ev.mouse.x < 253 && ev.mouse.y > 407 && ev.mouse.y < 467)
        {
            al_draw_bitmap(menuinstrucao, 0, 0, 0);
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                *fiminstrucao = true;
            }
        }
        else if (ev.mouse.x > 15 && ev.mouse.x < 253 && ev.mouse.y > 520 && ev.mouse.y < 576)
        {
            al_draw_bitmap(menusair, 0, 0, 0);
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                *fim = true;
        }
    }
    else
    {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_bitmap(instrucao, 0, 0, 0);
        if (ev.mouse.x > 407 && ev.mouse.x < 612 && ev.mouse.y > 583 && ev.mouse.y < 632)
        {
            al_draw_bitmap(instrucao2, 0, 0, 0);
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                al_clear_to_color(al_map_rgb(255, 255, 255));
                al_destroy_display(display);
                display2 = al_create_display(largura_t2, altura_t2);
                al_register_event_source(fila_eventos, al_get_display_event_source(display2));
                *fimmenu = true;
            }
        }
    }
}

void gameover(ALLEGRO_BITMAP* vidas[], Personagem* sticker, bool *gameOver)
{
    for (int i = 4; i > 0; i--)
    {
        if (sticker->vidas == i)
        {
            al_draw_bitmap(vidas[--i], 0, 50, 0);
        }
    }

    if (sticker->vidas == 0)
    {
        *gameOver = true;
    }

    if (sticker->score >= 15)
    {
        *gameOver = true;
    }
}

void movimentacao(ALLEGRO_EVENT ev)
{
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
    {
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

}

void fase01(ALLEGRO_EVENT ev)
{
    ALLEGRO_BITMAP* saladeaula;

    saladeaula = al_load_bitmap("images/saladeaula.bmp");
    al_convert_mask_to_alpha(saladeaula, al_map_rgb(255, 0, 255));

    al_draw_bitmap(saladeaula, 0, 0, 0);


}

int main()
{   
    // Variáveis do jogo
//___________________________________________________________________
    Personagem sticker;    
    Projeteis f[20];
    Pontuacao a;

    int FPS = 60;
    int regiao_x_folha = 0;
    int regiao_y_folha = 0;
    int linha_atual = 1;
    int linhas_folha = 2;
    int coluna_atual = 1;
    int coluna_folha = 4;
    int pulos = 1;
    float velpulo = -17;
    
    const gravidade = 1;

    bool *fim = false;
    bool *fimmenu = false;
    bool *fiminstrucao = false;
    bool pulo = false;
    bool gameOver = false;

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
    ALLEGRO_BITMAP* menu = NULL;
    ALLEGRO_BITMAP* menuavancar = NULL;
    ALLEGRO_BITMAP* menusair = NULL;
    ALLEGRO_BITMAP* menuinstrucao = NULL;
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_BITMAP* vidas[4];
    ALLEGRO_BITMAP* instrucao;
    ALLEGRO_BITMAP* instrucao2;

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

    font = al_load_font("arial.ttf", 18, 0);
    timer = al_create_timer(2.0 / FPS);
    
    srand(time(0));

    iniciarPersonagem(&sticker);
    iniciarProjeteis(f, numF);
    iniciarPontuacao(&a);

    // Criação da fila e demais dispositivos
//____________________________________________________________________
    fila_eventos = al_create_event_queue();
    //imagem = al_load_bitmap("sticker.bmp");
    imagemFull = al_load_bitmap("images/stickerfull.bmp");
    al_convert_mask_to_alpha(imagemFull, al_map_rgb(255, 0, 255));
    borracha = al_load_bitmap("images/borracha.bmp");
    al_convert_mask_to_alpha(borracha, al_map_rgb(255, 0, 255));
    menu = al_load_bitmap("images/stickeredia_menu.bmp");
    menuavancar = al_load_bitmap("images/stickeredia_jogar.bmp");
    menusair = al_load_bitmap("images/stickeredia_sair.bmp");
    menuinstrucao = al_load_bitmap("images/stickeredia_instrucoes.bmp");
    somdefundo = al_load_sample("trilha_sonora.ogg");
    somcorrendo = al_load_sample("Punch_04.wav");
    instrucao = al_load_bitmap("images/instrucoes_stick.bmp");
    instrucao2 = al_load_bitmap("images/instrucoes_stick2.bmp");

    sticker.image = al_load_bitmap("images/stickerfull.bmp"); 
    vidas[3] = al_load_bitmap("images/barravida4.bmp");
    al_convert_mask_to_alpha(vidas[3], al_map_rgb(255, 0, 255));
    vidas[2] = al_load_bitmap("images/barravida3.bmp");
    al_convert_mask_to_alpha(vidas[2], al_map_rgb(255, 0, 255));
    vidas[1] = al_load_bitmap("images/barravida2.bmp");
    al_convert_mask_to_alpha(vidas[1], al_map_rgb(255, 0, 255));
    vidas[0] = al_load_bitmap("images/barravida1.bmp");
    al_convert_mask_to_alpha(vidas[0], al_map_rgb(255, 0, 255));

    for (int i = 0; i < numF; i++)
    {
        f[i].image = al_load_bitmap("images/f.bmp");
        al_convert_mask_to_alpha(f[i].image, al_map_rgb(255, 0, 255));
    }

    a.image = al_load_bitmap("images/A.bmp");
    al_convert_mask_to_alpha(a.image, al_map_rgb(255, 0, 255));

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

    al_play_sample(somdefundo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    

    while (!fim)
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(fila_eventos, &ev);

        if (!fimmenu)
        {
            criarMenu(menu, menuinstrucao, instrucao, instrucao2, menusair, menuavancar, display, display2, fila_eventos, ev, &fim, &fimmenu, &fiminstrucao);
        }
        else
        {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                fim = true;
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                fim = true;
            }
            else if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                if (sticker.y > altura_t2 - 147)
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

                if (!gameOver)
                {
                    chamarProjeteis(f, numF);
                    atualizarProjeteis(f, numF);
                    colisaoProjeteis(f, &sticker, numF);
                    chamarPontuacao(&a);
                    atualizarPontuacao(&a);
                    colisaoPontuacao(&a, &sticker);                    

                    al_draw_textf(font, al_map_rgb(0, 0, 0), 70, 100, ALLEGRO_ALIGN_CENTER, "Pontuacao = %d", sticker.score);

                    gameover(vidas, &sticker, &gameOver);
                }
                else
                {
                    fim = true;                   
                }
            }

            fase01(ev);

            // DESENHO
    //____________________________________________________________________
            for (int i = 0; i < numF; i++)
            {
                al_draw_bitmap(f[i].image, f[i].x, f[i].y, 0);
            }

            al_draw_bitmap(a.image, a.x, a.y, 0);

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

            al_draw_bitmap(borracha, 1010, 752, 0);

        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(255, 255, 255));
    }

     
    // Finalização do programa
//____________________________________________________________________
    al_destroy_display(display2);
    al_destroy_event_queue(fila_eventos);
    al_destroy_bitmap(sticker.image);
    al_destroy_bitmap(f->image);
    al_destroy_bitmap(a.image);
    for (int i = 0; i < 4; i++)
    {
        al_destroy_bitmap(vidas[i]);
    }
    al_destroy_bitmap(borracha);
    al_destroy_sample(somdefundo);
    al_destroy_sample(somcorrendo);
    al_destroy_font(font);

    return 0;
}
