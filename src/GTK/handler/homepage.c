#include<gtk/gtk.h>
#include "login.c"

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *registerButton;
GtkWidget *loginButton;
GtkWidget *appNameLabel;
GtkBuilder *builder; 

GtkWidget *registerWindow;
GtkWidget *registerFixed;
GtkWidget *registerLabel;
GtkWidget *usernameRegisterLabel;
GtkWidget *passwordRegisterLabel;
GtkWidget *usernameRegisterEntry;
GtkWidget *passwordRegisterEntry;
GtkWidget *submitRegisterButton;
GtkWidget *backRegisterButton;
GtkBuilder *builderRegister; 

void on_registerButton_clicked(GtkButton *b,int argc, char *argv[]){
        // gtk_container_remove(GTK_CONTAINER(window), GTK_WIDGET(fixed));
        // gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(registerFixed));
        gtk_widget_hide(window);
        gtk_widget_show(registerWindow);
       
}
void on_loginButton_clicked(GtkButton *b,int argc, char *argv[]){
        gtk_widget_hide(window);
        gtk_widget_show(loginWindow);
       
}
/// REGISTER

void on_submitRegisterButton_clicked(GtkButton *b){
        gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameRegisterEntry));
        gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordRegisterEntry));
        printf("%s, %s", username, password);

}
void on_backRegisterButton_clicked(GtkButton *b,int argc, char *argv[]){

        gtk_widget_hide(registerWindow);
        gtk_widget_show(window);
       
}

GtkWidget* createHomePageView(int argc, char *argv[]){

        builder = gtk_builder_new_from_file("xml/homepage.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
        registerButton = GTK_WIDGET(gtk_builder_get_object(builder, "registerButton"));
        loginButton = GTK_WIDGET(gtk_builder_get_object(builder, "loginButton"));
        appNameLabel = GTK_WIDGET(gtk_builder_get_object(builder, "appNameLabel"));
        g_signal_connect(registerButton, "clicked", G_CALLBACK(on_registerButton_clicked), NULL);
        g_signal_connect(loginButton, "clicked", G_CALLBACK(on_loginButton_clicked), NULL);

        builderRegister = gtk_builder_new_from_file("xml/register.glade");
        registerWindow = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerWindow"));
        g_signal_connect(registerWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        registerFixed = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerFixed"));
        registerLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerLabel"));
        usernameRegisterLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "usernameLabel"));
        passwordRegisterLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "passwordLabel"));
        usernameRegisterEntry = GTK_WIDGET(gtk_builder_get_object(builderRegister, "usernameEntry"));
        passwordRegisterEntry = GTK_WIDGET(gtk_builder_get_object(builderRegister, "passwordEntry"));
        submitRegisterButton = GTK_WIDGET(gtk_builder_get_object(builderRegister, "submitButton"));
        backRegisterButton = GTK_WIDGET(gtk_builder_get_object(builderRegister, "backButton"));
        g_signal_connect(submitRegisterButton, "clicked", G_CALLBACK(on_submitRegisterButton_clicked), NULL);
        g_signal_connect(backRegisterButton, "clicked", G_CALLBACK(on_backRegisterButton_clicked), NULL);


        return window;
}


// GtkWidget* createRegisterView(int argc, char *argv[]){

//         builderRegister = gtk_builder_new_from_file("xml/register.glade");
//         registerWindow = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerWindow"));
//         g_signal_connect(registerWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//         registerFixed = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerFixed"));
//         registerLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "registerLabel"));
//         usernameRegisterLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "usernameLabel"));
//         passwordRegisterLabel = GTK_WIDGET(gtk_builder_get_object(builderRegister, "passwordLabel"));
//         usernameRegisterEntry = GTK_WIDGET(gtk_builder_get_object(builderRegister, "usernameEntry"));
//         passwordRegisterEntry = GTK_WIDGET(gtk_builder_get_object(builderRegister, "passwordEntry"));
//         submitRegisterButton = GTK_WIDGET(gtk_builder_get_object(builderRegister, "submitButton"));
//         backRegisterButton = GTK_WIDGET(gtk_builder_get_object(builderRegister, "backButton"));
//         g_signal_connect(submitRegisterButton, "clicked", G_CALLBACK(on_submitRegisterButton_clicked), NULL);
//         g_signal_connect(backRegisterButton, "clicked", G_CALLBACK(on_backRegisterButton_clicked), NULL);

//         return registerFixed;
// }
