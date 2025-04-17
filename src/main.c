#include <gtk/gtk.h>
#include <stdbool.h>

#include <common.h>
#include <calculator.h>

static GtkWidget *sum_label;
static GtkWidget *running_formula;

static float running_sum = 0;

static Operation current_operation = OP_NONE;

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *box3;
    GtkWidget *clear_box;
    GtkWidget *input_box;
    GtkWidget *parent_box;
    GtkWidget *left_side_child_box;
    GtkWidget *right_side_child_box;

    GtkWidget *operations_box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_widget_set_margin_top(main_box, 10);
    gtk_widget_set_margin_bottom(main_box, 10);
    gtk_widget_set_margin_start(main_box, 10);
    gtk_widget_set_margin_end(main_box, 10);

    input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), input_box, FALSE, FALSE, 0);

    add_input(input_box, "Enter stuff");

    input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    parent_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    gtk_box_pack_start(GTK_BOX(main_box), parent_box, FALSE, FALSE, 0);

    // Create child boxes (or widgets to go in those positions)
    left_side_child_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    right_side_child_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Pack them into the parent box
    gtk_box_pack_start(GTK_BOX(parent_box), left_side_child_box, TRUE, TRUE, 0);   // Left: expand
    gtk_box_pack_start(GTK_BOX(parent_box), right_side_child_box, FALSE, TRUE, 0); // Right: no expand

    box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box1, FALSE, FALSE, 0);

    add_num_button(box1, "7");
    add_num_button(box1, "8");
    add_num_button(box1, "9");

    box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box2, FALSE, FALSE, 0);

    add_num_button(box2, "4");
    add_num_button(box2, "5");
    add_num_button(box2, "6");

    box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box3, FALSE, FALSE, 0);

    add_num_button(box3, "1");
    add_num_button(box3, "2");
    add_num_button(box3, "3");

    clear_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(right_side_child_box), clear_box, FALSE, FALSE, 0);
    add_clear_button(clear_box);

    operations_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(right_side_child_box), operations_box, FALSE, FALSE, 0);

    add_operation_button(operations_box, OP_DIVIDE, &current_operation);
    add_operation_button(operations_box, OP_MULTIPLY, &current_operation);
    add_operation_button(operations_box, OP_SUBTRACT, &current_operation);
    add_operation_button(operations_box, OP_ADD, &current_operation);
    add_operation_button(operations_box, OP_EQUALS, &current_operation);
    add_operation_button(operations_box, OP_BACKSPACE, &current_operation);

    OperatorStack my_test = create_operator_stack(1);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
