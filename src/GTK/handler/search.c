#include<gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *searchEntry;
GtkWidget *searchButton;
GtkWidget *treeView;
GtkBuilder *builder; 
void on_searchButton2_clicked(GtkWidget *button) {

    // Get the main window and tree view from the Glade file

    gtk_container_remove(GTK_CONTAINER(fixed), GTK_WIDGET(treeView));


    // Create the new tree view and add it to the main window
    GtkTreeView *new_tree_view = GTK_TREE_VIEW(gtk_tree_view_new());
    gtk_container_add(GTK_CONTAINER(fixed), GTK_WIDGET(new_tree_view));

    // Create the ListStore (you can replace this with your own model)
    GtkListStore *list_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    GtkTreeIter iter;
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "John", 1, "Doe", 2, "30", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "Jane", 1, "Smith", 2, "25", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "Bob", 1, "Johnson", 2, "40", -1);

    // Set the model for the new tree view
    gtk_tree_view_set_model(new_tree_view, GTK_TREE_MODEL(list_store));

    // Create the TreeViewColumns
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column_first_name = gtk_tree_view_column_new_with_attributes("First Name", renderer, "text", 0, NULL);
    GtkTreeViewColumn *column_last_name = gtk_tree_view_column_new_with_attributes("Last Name", renderer, "text", 1, NULL);
    GtkTreeViewColumn *column_age = gtk_tree_view_column_new_with_attributes("Age", renderer, "text", 2, NULL);

    // Add the columns to the new tree view
    gtk_tree_view_append_column(new_tree_view, column_first_name);
    gtk_tree_view_append_column(new_tree_view, column_last_name);
    gtk_tree_view_append_column(new_tree_view, column_age);

    gtk_widget_show_all(GTK_WIDGET(window));
}

GtkWidget* createSearchView(int argc, char *argv[]){

        builder = gtk_builder_new_from_file("xml/search.glade");
        window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));

        searchEntry = GTK_WIDGET(gtk_builder_get_object(builder, "searchEntry"));
        searchButton = GTK_WIDGET(gtk_builder_get_object(builder, "searchButton"));
        treeView = GTK_WIDGET(gtk_builder_get_object(builder, "treeView"));
        g_signal_connect(searchButton, "clicked", G_CALLBACK(on_searchButton2_clicked), NULL);

        return fixed;
}
