#include <gtk/gtk.h>
#include <stdbool.h>

#include <common.h>
#include <calculator.h>

static GtkWidget *sum_label;
static GtkWidget *running_formula;

static float running_sum = 0;

static CalcInput current_operation = OP_NONE;

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

    OperationData operation_data;
    running_formula = gtk_entry_new();

    operation_data.running_formula = running_formula;
    operation_data.operation_stack = create_stack(2);
    operation_data.output_stack = create_output_stack(2);
    operation_data.token_stack = create_stack(2);
    operation_data.temp_num_stack = create_stack(2);

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

    add_input(input_box, "Enter stuff", sum_label, running_formula);

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

    add_operation_button(box1, NUM_7, &operation_data);
    add_operation_button(box1, NUM_8, &operation_data);
    add_operation_button(box1, NUM_9, &operation_data);

    box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box2, FALSE, FALSE, 0);

    add_operation_button(box2, NUM_4, &operation_data);
    add_operation_button(box2, NUM_5, &operation_data);
    add_operation_button(box2, NUM_6, &operation_data);

    box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box3, FALSE, FALSE, 0);

    add_operation_button(box3, NUM_1, &operation_data);
    add_operation_button(box3, NUM_2, &operation_data);
    add_operation_button(box3, NUM_3, &operation_data);

    clear_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(right_side_child_box), clear_box, FALSE, FALSE, 0);
    add_clear_button(clear_box, running_formula);

    operations_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(right_side_child_box), operations_box, FALSE, FALSE, 0);

    add_operation_button(operations_box, OP_BACKSPACE, &operation_data);
    add_operation_button(operations_box, OP_DIVIDE, &operation_data);
    add_operation_button(operations_box, OP_MULTIPLY, &operation_data);
    add_operation_button(operations_box, OP_SUBTRACT, &operation_data);
    add_operation_button(operations_box, OP_ADD, &operation_data);
    add_operation_button(operations_box, OP_EQUALS, &operation_data);

    gtk_widget_show_all(window);

    // free(operation_data.operation_stack);
    // free(operation_data.output_stack);
    // free(operation_data.temp_num_stack);
    // free(operation_data.token_stack);
    // free(output_data.stack);
    // free(operation_data.stack);
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
