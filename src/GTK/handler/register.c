#include<gtk/gtk.h>
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
void on_submitRegisterButton_clicked(GtkButton *b){
        gchar *username = gtk_entry_get_text(GTK_ENTRY(usernameRegisterEntry));
        gchar *password = gtk_entry_get_text(GTK_ENTRY(passwordRegisterEntry));
        printf("%s, %s", username, password);

}
void on_backRegisterButton_clicked(GtkButton *b,int argc, char *argv[]){

        GtkWidget *window = createHomePageView(0,NULL);
        gtk_widget_hide(registerWindow);
        gtk_widget_show(window);
       
}
GtkWidget* createRegisterView(int argc, char *argv[]){

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

        return registerFixed;
}

