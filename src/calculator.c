#include <gtk/gtk.h>
#include <stdbool.h>
#include <common.h>

static float running_sum = 0;

void clear_calculator(GtkWidget *widget, gpointer user_data)
{
  GtkWidget *running_formula = GTK_WIDGET(user_data);
  gtk_entry_set_text(GTK_ENTRY(running_formula), "");
}

int handle_button_click(GtkWidget *widget, gpointer user_data)
{
  ButtonClickHandlerParams *data = (ButtonClickHandlerParams *)user_data;

  const char *text_to_add = &data->op;

  bool should_remove = false;

  // Get current entry text
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data->running_formula));
  const char *current_text = gtk_entry_buffer_get_text(buffer);

  CalcInput list_of_operations[5] = {OP_ADD, OP_DIVIDE, OP_EQUALS, OP_SUBTRACT, OP_MULTIPLY};

  int is_added_operator = 0;
  int is_last_char_operator = 0;

  if (strlen(current_text) > 0)
  {
    char last_of_current_text = current_text[strlen(current_text) - 1];

    // Turn last character into a proper null-terminated string
    char last_char_str[2] = {last_of_current_text, '\0'};

    for (int i = 0; i < sizeof(list_of_operations) / sizeof(list_of_operations[0]); i++)
    {
      char *op_string = operation_to_string(list_of_operations[i]);

      if (strncmp(op_string, text_to_add, 1) == 0)
        is_added_operator = 1;

      if (strncmp(op_string, last_char_str, 1) == 0)
        is_last_char_operator = 1;
    }
  }

  if (is_added_operator && is_last_char_operator)
  {
    return 1; // Don't add the new operator
  }

  // If backspace and there is nothing in the current text, nothing to do
  if (strlen(current_text) == 0 && strncmp(text_to_add, operation_to_string(OP_BACKSPACE), 2) == 0)
  {
    return 1;
  }

  if (strncmp(text_to_add, operation_to_string(OP_BACKSPACE), 2) == 0 && strlen(current_text) > 0)
  {
    should_remove = true;
  }

  char updated_text[128];

  if (should_remove)
  {
    snprintf(updated_text, strlen(current_text), "%s", current_text);
    pop(data->temp_num_stack);
  }
  else
  {

    if (!is_added_operator)
    {
      push(data->temp_num_stack, *text_to_add);
    }

    snprintf(updated_text, sizeof(updated_text), "%s%s", current_text, text_to_add);
  }

  if (is_added_operator)
  {

    char dest[100];
    char num_buf[100];

    for (int i = 0; i < data->temp_num_stack->top; i++)
    {
      num_buf[i] = data->temp_num_stack->stack[i];
    }

    strcpy(dest, num_buf);

    StackItem top = data->operation_stack->stack[data->operation_stack->top - 1];

    if (data->operation_stack->top == 0 || get_precedence((CalcInput)top, *text_to_add) == LOWER)
    {
      push(data->operation_stack, *text_to_add);
    }
    else
    {
      while (data->operation_stack->top > 0)
      {
        StackItem top = data->operation_stack->stack[data->operation_stack->top - 1];
        if (get_precedence((CalcInput)top, *text_to_add) == LOWER)
          break;

        StackItem popped = pop(data->operation_stack);

        const char *str = operation_to_string((CalcInput)popped);
        printf("My STR: %c\n", popped);
        push_output_stack(data->output_stack, str);
      }

      printf("%s text to add,", text_to_add);
      push(data->operation_stack, *text_to_add);
    }

    while (data->temp_num_stack->top > 0)
    {
      pop(data->temp_num_stack);
    }
  }

  gtk_entry_set_text(GTK_ENTRY(data->running_formula), updated_text);

  return 0;
}

void calc_value(GtkWidget *entry, GtkWidget *sum_label)
{
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  float value = atof(text);
  running_sum += value;
  g_print("Running total: %f\n", running_sum);

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.2f", running_sum);

  gtk_label_set_text(GTK_LABEL(sum_label), buffer);
}

void add_input(GtkWidget *container, const char *place_holder, GtkWidget *sum_label, GtkWidget *running_formula)
{
  gtk_entry_set_placeholder_text(GTK_ENTRY(running_formula), "Type here...");
  g_signal_connect(running_formula, "activate", G_CALLBACK(calc_value), sum_label);
  sum_label = gtk_label_new("0.00");
  gtk_container_add(GTK_CONTAINER(container), sum_label);
  gtk_container_add(GTK_CONTAINER(container), running_formula);
}

void add_clear_button(GtkWidget *container, GtkWidget *running_formula)
{
  GtkWidget *button;
  GtkWidget *button_box;
  // Create a button and a button box
  button = gtk_button_new_with_label("Clear");
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

  // Add the button to the button box
  gtk_container_add(GTK_CONTAINER(button_box), button);

  // Connect signals to the button
  g_signal_connect(button, "clicked", G_CALLBACK(clear_calculator), running_formula);

  // Add the button box to the container (GtkBox)
  gtk_container_add(GTK_CONTAINER(container), button_box);
}

static void handles_equals()
{
}

void add_operation_button(GtkWidget *container, CalcInput op, OperationData *data)
{
  GtkWidget *button;
  GtkWidget *button_box;

  const char *operation_label = operation_to_string(op);
  button = gtk_button_new_with_label(operation_label);
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(button_box), button);
  gtk_container_add(GTK_CONTAINER(container), button_box); // don't forget to pack the button box!

  ButtonClickHandlerParams *button_handle_click = malloc(sizeof(ButtonClickHandlerParams));
  if (!button_handle_click)
  {
    fprintf(stderr, "Failed to allocate ButtonClickHandlerParams\n");
    return;
  }

  button_handle_click->op = operation_label[0];

  button_handle_click->operation_stack = data->operation_stack;
  button_handle_click->output_stack = data->output_stack;
  button_handle_click->running_formula = data->running_formula;
  button_handle_click->temp_num_stack = data->temp_num_stack;
  button_handle_click->token_stack = data->token_stack;

  g_signal_connect(button, "clicked", G_CALLBACK(handle_button_click), button_handle_click);
}
