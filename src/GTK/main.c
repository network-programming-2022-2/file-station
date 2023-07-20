#include "handler/homepage.c"

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);

    createHomePageView(argc, argv);
    gtk_widget_show(window);
    gtk_main();
    return 1;
}