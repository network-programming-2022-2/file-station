#include<gtk/gtk.h>
#include"menu.c"
GtkWidget *window;
GtkWidget *fixed;
GtkWidget *loginLabel;
GtkWidget *usernameLabel;
GtkWidget *passwordLabel;
GtkWidget *usernameEntry;
GtkWidget *passwordEntry;
GtkWidget *submitButton;
GtkWidget *backButton;
GtkBuilder *builder; 
void on_submitButton2_clicked(GtkButton *b){
        GtkWidget *menuFixed = createMenuView(0,NULL);
        gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
        gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(menuFixed));
        gtk_widget_show_all(window);

}
GtkWidget* createLoginView(int argc, char *argv[]){

        builder = gtk_builder_new_from_file("xml/login.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        loginLabel = GTK_WIDGET(gtk_builder_get_object(builder, "loginLabel"));
        usernameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "usernameLabel"));
        passwordLabel = GTK_WIDGET(gtk_builder_get_object(builder, "passwordLabel"));
        usernameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "usernameEntry"));
        passwordEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passwordEntry"));
        submitButton = GTK_WIDGET(gtk_builder_get_object(builder, "submitButton"));
        backButton = GTK_WIDGET(gtk_builder_get_object(builder, "backButton"));
        g_signal_connect(submitButton, "clicked", G_CALLBACK(on_submitButton2_clicked), NULL);

        return fixed;
}


