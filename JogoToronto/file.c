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

enum TECLAS { CIMA, BAIXO, DIREITA, ESQUERDA };
//Proporções do menu
const int altura_t = 653;
const int largura_t = 1162;

//Proporções da tela do jogo
const int altura_t2 = 800;
const int largura_t2 = 1010;

const int numF = 20;
int velocidadeF = 5; 
bool redraw = true;

bool* fim = false;
bool* fimmenu = false;
bool* fiminstrucao = false;
bool teclas[] = { false, false, false, false };
bool pulo = false;
bool gameOver = false;

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

void iniciarPersonagem(Personagem* sticker)
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

void iniciarPontuacao(Pontuacao* a)
{
    a->live = false;
    a->w = 20;
    a->h = 30;
}

void chamarPontuacao(Pontuacao* a)
{
    if (!a->live)
    {
        if (rand() % 10 == 0)
        {
            a->y = (rand() % 150) + 550;
            a->x = rand() % (largura_t2 - 30);
            a->live = true;
        }
    }
}

void colisaoPontuacao(Pontuacao* a, Personagem* sticker, ALLEGRO_SAMPLE* sompontuacao)
{
    if (a->live)
    {
        if (a->x - (a->w / 2) < sticker->x + (sticker->w / 2) &&
            a->x + (a->w / 2) > sticker->x &&
            a->y - a->h < sticker->y + (sticker->h / 2) &&
            a->y + a->h > sticker->y)
        {
            a->live = false;
            sticker->score++;
            al_play_sample(sompontuacao, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            redraw = true;
        }
    }
}

void velocidadeProjeteis(Projeteis f[], int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        f[i].speed = rand() % 3 + (1 + velocidadeF);
    }
}

void iniciarProjeteis(Projeteis f[], int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        f[i].live = false;
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

void colisaoProjeteis(Projeteis f[], Personagem* sticker, int tamanho, ALLEGRO_SAMPLE* somdano)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (f[i].live)
        {
            if (f[i].x - (f[i].w / 2) < sticker->x + (sticker->w / 2) &&
                f[i].x + (f[i].w / 2) > sticker->x &&
                f[i].y - f[i].h < sticker->y + (sticker->h / 2) &&
                f[i].y + f[i].h > sticker->y)
            {
                sticker->vidas--;
                f[i].live = false;
                al_play_sample(somdano, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            else if (f[i].y > altura_t2)
                f[i].live = false;
        }
    }
}

void criarMenu(ALLEGRO_BITMAP* menu, ALLEGRO_BITMAP* menuinstrucao, ALLEGRO_BITMAP* instrucao, ALLEGRO_BITMAP* instrucao2, ALLEGRO_BITMAP* menusair, ALLEGRO_BITMAP* menuavancar, ALLEGRO_DISPLAY* display,
    ALLEGRO_DISPLAY* display2, ALLEGRO_EVENT_QUEUE* fila_eventos, ALLEGRO_EVENT ev, bool* fim, bool* fimmenu, bool* fiminstrucao)
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
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                *fiminstrucao = true;
            }
        }
        else if (ev.mouse.x > 15 && ev.mouse.x < 253 && ev.mouse.y > 520 && ev.mouse.y < 576)
        {
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

int main()
{

    // Variáveis do jogo
    //___________________________________________________________________
    Personagem sticker;

    //siglas dos elementos químicos
    //___________________________________________________________________
    Projeteis f[20];

    //perguntas
    //___________________________________________________________________
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

    bool fimfase1 = false;
    bool fimfase2 = false;
    bool fimfase3 = false;
    bool fimfase4 = false;
    bool ganhou = false; 
   

    // Inicialização da Allegro e do Display
    //____________________________________________________________________
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_DISPLAY* display2 = NULL;
    ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_BITMAP* imagem = NULL;
    ALLEGRO_BITMAP* imagemFull = NULL;
    ALLEGRO_BITMAP* borracha = NULL;
    ALLEGRO_SAMPLE* somdano = NULL;
    ALLEGRO_SAMPLE* somfundo = NULL;
    ALLEGRO_BITMAP* menu = NULL;
    ALLEGRO_BITMAP* menuavancar = NULL;
    ALLEGRO_BITMAP* menusair = NULL;
    ALLEGRO_BITMAP* menuinstrucao = NULL;
    ALLEGRO_FONT* font = NULL;
    ALLEGRO_BITMAP* vidas[4];
    ALLEGRO_BITMAP* tabela_inc;
    ALLEGRO_BITMAP* instrucao;
    ALLEGRO_BITMAP* instrucao2;
    ALLEGRO_BITMAP* vitoria;
    ALLEGRO_BITMAP* derrota;
    ALLEGRO_SAMPLE* sompontuacao;
    ALLEGRO_SAMPLE* somderrota;

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

    font = al_load_font("arial.ttf", 22, 0);
    timer = al_create_timer(1.0 / FPS);

    srand(time(0));

    iniciarPersonagem(&sticker);
    iniciarProjeteis(f, numF);
    iniciarPontuacao(&a);

    // Criação da fila e demais dispositivos
//____________________________________________________________________
    fila_eventos = al_create_event_queue();
    imagemFull = al_load_bitmap("images/stickerfull.bmp");
    al_convert_mask_to_alpha(imagemFull, al_map_rgb(255, 0, 255));
    borracha = al_load_bitmap("images/borracha.bmp");
    al_convert_mask_to_alpha(borracha, al_map_rgb(255, 0, 255));
    menu = al_load_bitmap("images/stickeredia_menu.bmp");
    menuavancar = al_load_bitmap("images/stickeredia_jogar.bmp");
    menusair = al_load_bitmap("images/stickeredia_sair.bmp");
    menuinstrucao = al_load_bitmap("images/stickeredia_instrucoes.bmp");
    somdano = al_load_sample("images/oof.mp3");
    somderrota = al_load_sample("images/somderrota.mp3");
    sompontuacao = al_load_sample("images/sompontuacao.mp3");
    somfundo = al_load_sample("images/introclaro.mp3");
    instrucao = al_load_bitmap("images/instrucoes1.bmp");
    instrucao2 = al_load_bitmap("images/instrucoes2.bmp");    
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
        f[i].image = al_load_bitmap("images/fluor.bmp");
        al_convert_mask_to_alpha(f[i].image, al_map_rgb(255, 0, 255));
    }
    a.image = al_load_bitmap("images/Q.bmp");
    al_convert_mask_to_alpha(a.image, al_map_rgb(255, 0, 255));
    tabela_inc = al_load_bitmap("images/abenza.bmp");
    al_convert_mask_to_alpha(tabela_inc, al_map_rgb(255, 0, 255));
    vitoria = al_load_bitmap("images/vitoria.bmp");
    al_convert_mask_to_alpha(vitoria, al_map_rgb(255, 0, 255));
    derrota = al_load_bitmap("images/derrota.bmp");
    al_convert_mask_to_alpha(derrota, al_map_rgb(255, 0, 255));


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

    al_play_sample(somfundo, 0.05, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    while (!fim)
    {
        
        velocidadeProjeteis(f, numF);
        ALLEGRO_EVENT ev;

        al_wait_for_event(fila_eventos, &ev);

        if (fimmenu == false)
        {
            criarMenu(menu, menuinstrucao, instrucao, instrucao2, menusair, menuavancar, display, display2, fila_eventos, ev, &fim, &fimmenu, &fiminstrucao);
        }

        else
        {   
            al_unregister_event_source(fila_eventos, al_get_mouse_event_source());         
            al_draw_bitmap(tabela_inc, 0, 0, 0);

            if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    fim = true;
                }

                if (gameOver == false)
                {
                    chamarProjeteis(f, numF);
                    atualizarProjeteis(f, numF);
                    colisaoProjeteis(f, &sticker, numF, somdano);
                    chamarPontuacao(&a);
                    colisaoPontuacao(&a, &sticker, sompontuacao);

                    if (sticker.score == 5 && !fimfase1)
                    {
                        tabela_inc = al_load_bitmap("images/samuel.bmp");
                        sticker.score = 0;
                        velocidadeF += 1;
                        fimfase1 = true;
                        fimfase2 = false;
                    }
                    else if (sticker.score == 5 && !fimfase2)
                    {  
                        tabela_inc = al_load_bitmap("images/claro.bmp");
                        sticker.score = 0;
                        velocidadeF += 2;
                        fimfase2 = true;
                        fimfase3 = false;
                    }
                    else if (sticker.score == 5 && !fimfase3)
                    {
                        tabela_inc = al_load_bitmap("images/teaga.bmp");
                        sticker.score = 0;
                        velocidadeF += 2;
                        fimfase3 = true;
                        fimfase4 = false;
                    }
                    else if (sticker.score == 5 && !fimfase4)
                    {
                        tabela_inc = al_load_bitmap("images/konrad.bmp");
                        sticker.score = 0;
                        velocidadeF += 3;
                        fimfase4 = true;     
                    }

                    if (sticker.score >= 6)
                    {
                        gameOver = true;
                        ganhou = true;
                    }

                    if (sticker.vidas == 0)
                    {
                        gameOver = true;
                    }
                }
            }

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
                        sticker.x += teclas[DIREITA] * 5;
                    }
                    
                    coluna_atual += 1;
                    if (coluna_atual >= coluna_folha)
                    {
                        coluna_atual = 0;
                    }                    
                    else

                    sticker.vel_x = 1;

                }
                if (teclas[ESQUERDA])
                {
                    if (sticker.x >= 0)
                        sticker.x -= teclas[ESQUERDA] * 5;

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
                    sticker.vel_y += 1;
                }
                else
                {
                    sticker.vel_y = 0;
                }

                sticker.y += sticker.vel_y;
               
            }

            // DESENHO
    //____________________________________________________________________
                for (int i = 0; i < numF; i++)
                {
                    al_draw_bitmap(f[i].image, f[i].x, f[i].y, 0);
                }

                if (sticker.vidas == 4)
                {
                    al_draw_bitmap(vidas[3], 0, 10, 0);

                }
                else if (sticker.vidas == 3)
                {
                    al_draw_bitmap(vidas[2], 0, 10, 0);
                    
                }
                else if (sticker.vidas == 2)
                {
                    al_draw_bitmap(vidas[1], 0, 10, 0);
                }
                else 
                {
                    al_draw_bitmap(vidas[0], 0, 10, 0);
                }

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

                if (redraw)
                {     
                    if (sticker.score == 1)
                        a.image = al_load_bitmap("images/U.bmp");
                    if (sticker.score == 2)
                        a.image = al_load_bitmap("images/E.bmp");
                    if (sticker.score == 3)
                        a.image = al_load_bitmap("images/R.bmp");
                    if (sticker.score == 4)
                        a.image = al_load_bitmap("images/Y.bmp");

                    if (fimfase1 == true)
                    {
                        if (sticker.score == 0)
                            a.image = al_load_bitmap("images/Q.bmp");
                        if (sticker.score == 1)
                            a.image = al_load_bitmap("images/U.bmp");
                        if (sticker.score == 2)
                            a.image = al_load_bitmap("images/I.bmp");
                        if (sticker.score == 3)
                            a.image = al_load_bitmap("images/C.bmp");
                        if (sticker.score == 4)
                            a.image = al_load_bitmap("images/K.bmp");
                    }
                    if (fimfase2 == true)
                    {
                        if (sticker.score == 0)
                            a.image = al_load_bitmap("images/O.bmp");
                        if (sticker.score == 1)
                            a.image = al_load_bitmap("images/R.bmp");
                        if (sticker.score == 2)
                            a.image = al_load_bitmap("images/D.bmp");
                        if (sticker.score == 3)
                            a.image = al_load_bitmap("images/E.bmp");
                        if (sticker.score == 4)
                            a.image = al_load_bitmap("images/R.bmp");
                    }
                    if (fimfase3 == true)
                    {
                        if (sticker.score == 0)
                            a.image = al_load_bitmap("images/I.bmp");
                        if (sticker.score == 1)
                            a.image = al_load_bitmap("images/N.bmp");
                        if (sticker.score == 2)
                            a.image = al_load_bitmap("images/T.bmp");
                        if (sticker.score == 3)
                            a.image = al_load_bitmap("images/2.bmp");
                        if (sticker.score == 4)
                            a.image = al_load_bitmap("images/1.bmp");
                    }
                    if (fimfase4 == true)
                    {
                        if (sticker.score == 0)
                            a.image = al_load_bitmap("images/L.bmp");
                        if (sticker.score == 1)
                            a.image = al_load_bitmap("images/I.bmp");
                        if (sticker.score == 2)
                            a.image = al_load_bitmap("images/M.bmp");
                        if (sticker.score == 3)
                            a.image = al_load_bitmap("images/I.bmp");
                        if (sticker.score == 4)
                            a.image = al_load_bitmap("images/T.bmp");
                        if (sticker.score == 5)
                            a.image = al_load_bitmap("images/E.bmp");
                    }
                    redraw = false;
                }

                al_draw_bitmap(a.image, a.x, a.y, 0);
            }

            if (gameOver == true)
            {
                if (ev.type == ALLEGRO_EVENT_TIMER)
                {
                    al_unregister_event_source(fila_eventos, al_get_keyboard_event_source());
                    al_clear_to_color(al_map_rgb(255, 255, 255));   
                    if (ganhou)
                    {
                        al_draw_bitmap(vitoria, 0, 0, 0);
                    }
                    else
                    {
                        al_draw_bitmap(derrota, 0, 0, 0);
                    }
                }
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
        al_destroy_sample(somdano);
        al_destroy_sample(sompontuacao);
        al_destroy_sample(somfundo);
        al_destroy_font(font);
        al_destroy_bitmap(derrota);
        al_destroy_bitmap(vitoria);

    return 0;
}

