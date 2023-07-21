#include<gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *searchEntry;
GtkWidget *searchButton;
GtkWidget *treeView;
GtkBuilder *builder; 

GtkWidget* createSearchView(int argc, char *argv[]){

        builder = gtk_builder_new_from_file("xml/search.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));

        searchEntry = GTK_WIDGET(gtk_builder_get_object(builder, "searchEntry"));
        searchButton = GTK_WIDGET(gtk_builder_get_object(builder, "searchButton"));
        treeView = GTK_WIDGET(gtk_builder_get_object(builder, "treeView"));

        return fixed;
}


