#include<gtk/gtk.h>
#include"menu.c"

GtkWidget *loginWindow;
GtkWidget *loginFixed;
GtkWidget *loginLabel;
GtkWidget *usernameLoginLabel;
GtkWidget *passwordLoginLabel;
GtkWidget *usernameLoginEntry;
GtkWidget *passwordLoginEntry;
GtkWidget *submitLoginButton;
GtkWidget *backLoginButton;
GtkBuilder *builderLogin; 
void on_submitLoginButton_clicked(GtkButton *b){
        GtkWidget *menuFixed = createMenuView(0,NULL);
        gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
        gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(menuFixed));
        gtk_widget_show_all(window);

}
void on_backLoginButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(loginWindow);
        gtk_widget_show(window);
       
}
GtkWidget* createLoginView(int argc, char *argv[]){

        builderLogin = gtk_builder_new_from_file("xml/login.glade");
        loginWindow = GTK_WIDGET(gtk_builder_get_object(builderLogin, "window"));
        g_signal_connect(loginWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        loginFixed = GTK_WIDGET(gtk_builder_get_object(builderLogin, "fixed"));
        loginLabel = GTK_WIDGET(gtk_builder_get_object(builderLogin, "loginLabel"));
        usernameLoginLabel = GTK_WIDGET(gtk_builder_get_object(builderLogin, "usernameLabel"));
        passwordLoginLabel = GTK_WIDGET(gtk_builder_get_object(builderLogin, "passwordLabel"));
        usernameLoginEntry = GTK_WIDGET(gtk_builder_get_object(builderLogin, "usernameEntry"));
        passwordLoginEntry = GTK_WIDGET(gtk_builder_get_object(builderLogin, "passwordEntry"));
        submitLoginButton = GTK_WIDGET(gtk_builder_get_object(builderLogin, "submitButton"));
        backLoginButton = GTK_WIDGET(gtk_builder_get_object(builderLogin, "backButton"));
        g_signal_connect(submitLoginButton, "clicked", G_CALLBACK(on_submitLoginButton_clicked), NULL);
        g_signal_connect(backLoginButton, "clicked", G_CALLBACK(on_backLoginButton_clicked), NULL);


        return loginFixed;
}


