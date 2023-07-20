#include<gtk/gtk.h>
#include "register.c"
#include "login.c"

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *registerButton;
GtkWidget *loginButton;
GtkWidget *appNameLabel;
GtkBuilder *builder; 
void on_registerButton_clicked(GtkButton *b,int argc, char *argv[]);
void on_loginButton_clicked(GtkButton *b,int argc, char *argv[]);
int createHomePageView(int argc, char *argv[]){
        gtk_init(&argc, &argv);

        builder = gtk_builder_new_from_file("xml/homepage.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        registerButton = GTK_WIDGET(gtk_builder_get_object(builder, "registerButton"));
        loginButton = GTK_WIDGET(gtk_builder_get_object(builder, "loginButton"));
        appNameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "appNameLabel"));
        g_signal_connect(registerButton, "clicked", G_CALLBACK(on_registerButton_clicked), NULL);
        g_signal_connect(loginButton, "clicked", G_CALLBACK(on_loginButton_clicked), NULL);

        gtk_widget_show_all(window);
        gtk_main();
        return EXIT_SUCCESS;
}

void on_registerButton_clicked(GtkButton *b,int argc, char *argv[]){
        GtkWidget *registerFixed = createRegisterView(&argc, &argv);
        gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
        gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(registerFixed));
        gtk_widget_show_all(window);
       
}
void on_loginButton_clicked(GtkButton *b,int argc, char *argv[]){
        GtkWidget *loginFixed = createLoginView(&argc, &argv);
        gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
        gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(loginFixed));
        gtk_widget_show_all(window);
       
}