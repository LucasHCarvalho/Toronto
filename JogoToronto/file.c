
#include <allegro5/allegro.h>;
#include <allegro5/allegro_native_dialog.h>
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
    al_draw_bitmap(imagem, 900, 400,0);
    char tcaixa[50] = "Meu título";
    char titulo[100] = "Sou uma caixa de texto editada";
    char texto[200] = "A mensagem a ser exibida deve ficar aqui";
    int r = al_show_native_message_box(NULL, tcaixa, titulo, texto, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
    printf("%i", r);
    al_flip_display();
    al_rest(10.0);
    al_destroy_display(janela);

    return 0;

}