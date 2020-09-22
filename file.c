
#include <allegro5/allegro.h>;
#include "allegro5/allegro_native_dialog.h";
#include <allegro5/allegro_image.h>
#include <stdio.h>;

int main()
{
    ALLEGRO_DISPLAY* janela = NULL;
    ALLEGRO_BITMAP* imagem = NULL;
    al_init();
    al_init_image_addon();
    janela = al_create_display(1800, 800);
    imagem = al_load_bitmap("Bitmap1.bmp");
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_draw_bitmap(imagem, 900, 400, 0);
    ALLEGRO_BITMAP* load_bitmap_at_size(const char* imagem, int w, int h)
    {
        ALLEGRO_BITMAP* resized_bmp, * loaded_bmp, * prev_target;

        // 1. create a temporary bitmap of size we want
        resized_bmp = al_create_bitmap(900, 400);
        if (!resized_bmp) return NULL;

        // 2. load the bitmap at the original size
        loaded_bmp = al_load_bitmap(imagem);
        if (!loaded_bmp)
        {
            al_destroy_bitmap(resized_bmp);
            return NULL;
        }

        // 3. set the target bitmap to the resized bmp
        prev_target = al_get_target_bitmap();
        al_set_target_bitmap(resized_bmp);

        // 4. copy the loaded bitmap to the resized bmp
        al_draw_scaled_bitmap(loaded_bmp,0, 0,   // source origin
            al_get_bitmap_width(loaded_bmp),     // source width
            al_get_bitmap_height(loaded_bmp),    // source height
            0, 0,                                // target origin
            900, 400,                                // target dimensions
            0                                    // flags
        );

        // 5. restore the previous target and clean up
        al_set_target_bitmap(prev_target);
        al_destroy_loaded_bmp(loaded_bmp);

        return resized_bmp;
    }
    //int al_get_bitmap_width(ALLEGRO_BITMAP *imagem) = 900;
    //int al_get_bitmap_heigth(ALLEGRO_BITMAP *imagem) = 400;
    char tcaixa[50] = "Meu título";
    char titulo[100] = "Sou uma caixa de texto";
    char texto[200] = "A mensagem a ser exibida deve ficar aqui";
    int r = al_show_native_message_box(NULL, tcaixa, titulo, texto, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
    printf("%i", r);
    al_flip_display();
    al_rest(10.0);
    al_destroy_display(janela);

    return 0;
}