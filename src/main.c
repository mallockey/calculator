#include <gtk/gtk.h>
#include <stdbool.h>

#include <common.h>
#include <calculator.h>
#include <stack.h>

static void activate(GtkApplication *app, gpointer user_data)
{
    OperationData *operation_data = (OperationData *)user_data;

    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *box3;
    GtkWidget *box4;
    GtkWidget *clear_box;
    GtkWidget *input_box;
    GtkWidget *parent_box;
    GtkWidget *left_side_child_box;
    GtkWidget *right_side_child_box;

    GtkWidget *sum_label;
    GtkWidget *running_formula;
    GtkWidget *operations_box;

    // Create UI widgets
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

    // Set up UI fields and bind them to operation_data
    running_formula = gtk_entry_new();
    sum_label = gtk_label_new("0");
    operation_data->running_formula = running_formula;
    operation_data->sum = sum_label;

    add_input(input_box, "Enter stuff", sum_label, running_formula);

    parent_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), parent_box, FALSE, FALSE, 0);

    left_side_child_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    right_side_child_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(parent_box), left_side_child_box, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(parent_box), right_side_child_box, FALSE, TRUE, 0);

    // Row 1
    box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box1, FALSE, FALSE, 0);
    add_operation_button(box1, NUM_7, operation_data);
    add_operation_button(box1, NUM_8, operation_data);
    add_operation_button(box1, NUM_9, operation_data);

    // Row 2
    box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box2, FALSE, FALSE, 0);
    add_operation_button(box2, NUM_4, operation_data);
    add_operation_button(box2, NUM_5, operation_data);
    add_operation_button(box2, NUM_6, operation_data);

    // Row 3
    box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box3, FALSE, FALSE, 0);
    add_operation_button(box3, NUM_1, operation_data);
    add_operation_button(box3, NUM_2, operation_data);
    add_operation_button(box3, NUM_3, operation_data);

    // Row 4
    box4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(left_side_child_box), box4, FALSE, FALSE, 0);
    add_operation_button(box4, NUM_0, operation_data);

    // Clear and operations column
    clear_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(right_side_child_box), clear_box, FALSE, FALSE, 0);
    add_clear_button(clear_box, operation_data);

    operations_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(right_side_child_box), operations_box, FALSE, FALSE, 0);
    add_operation_button(operations_box, OP_BACKSPACE, operation_data);
    add_operation_button(operations_box, OP_DIVIDE, operation_data);
    add_operation_button(operations_box, OP_MULTIPLY, operation_data);
    add_operation_button(operations_box, OP_SUBTRACT, operation_data);
    add_operation_button(operations_box, OP_ADD, operation_data);
    add_operation_button(operations_box, OP_EQUALS, operation_data);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    OperationData *operation_data = malloc(sizeof(OperationData));
    operation_data->eval_stack = create_eval_stack(2);
    operation_data->output_stack = create_output_stack(2);
    operation_data->token_stack = create_output_stack(2);
    operation_data->temp_num_stack = create_stack(2);
    operation_data->operation_stack = create_stack(2);

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), operation_data);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    free_eval_stack(operation_data->eval_stack);
    free_output_stack(operation_data->output_stack);
    free_output_stack(operation_data->token_stack);
    free_stack(operation_data->temp_num_stack);
    free_stack(operation_data->operation_stack);
    free(operation_data);

    g_object_unref(app);
    return status;
}
