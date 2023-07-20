#include<gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *registerLabel;
GtkWidget *usernameLabel;
GtkWidget *passwordLabel;
GtkWidget *usernameEntry;
GtkWidget *passwordEntry;
GtkWidget *submitButton;
GtkBuilder *builder; 

GtkWidget* createRegisterView(int argc, char *argv[]){

        builder = gtk_builder_new_from_file("xml/register.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        registerLabel = GTK_WIDGET(gtk_builder_get_object(builder, "registerLabel"));
        usernameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "usernameLabel"));
        passwordLabel = GTK_WIDGET(gtk_builder_get_object(builder, "passwordLabel"));
        usernameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "usernameEntry"));
        passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passwordEntry"));
        submitButton = GTK_WIDGET(gtk_builder_get_object(builder, "submitButton"));

        return window;
}