#include<gtk/gtk.h>
#include"search.c"
GtkWidget *window;
GtkWidget *fixed;
GtkWidget *menuLabel;
GtkWidget *logoutButton;
GtkWidget *searchButton;
GtkWidget *downloadButton;
GtkBuilder *builder; 
void on_searchButton_clicked(GtkButton *b);

GtkWidget* createMenuView(int argc, char *argv[]){

        builder = gtk_builder_new_from_file("xml/menu.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        menuLabel = GTK_WIDGET(gtk_builder_get_object(builder, "menuLabel"));
        logoutButton = GTK_WIDGET(gtk_builder_get_object(builder, "logoutButton"));
        searchButton = GTK_WIDGET(gtk_builder_get_object(builder, "searchButton"));
        downloadButton = GTK_WIDGET(gtk_builder_get_object(builder, "downloadButton"));

        g_signal_connect(searchButton, "clicked", G_CALLBACK(on_searchButton_clicked), NULL);

        return fixed;
}

void on_searchButton_clicked(GtkButton *b){
        GtkWidget *searchFixed = createSearchView(0,NULL);
        gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
        gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(searchFixed));
        gtk_widget_show_all(window);


}