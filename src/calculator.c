#include <gtk/gtk.h>
#include <stdbool.h>
#include <common.h>
#include <ctype.h>

void clear_calculator(GtkWidget *widget, gpointer user_data)
{
  GtkWidget *running_formula = GTK_WIDGET(user_data);
  gtk_entry_set_text(GTK_ENTRY(running_formula), "");
}

bool is_number(const char *str)
{
  if (str == NULL || *str == '\0')
  {
    return false;
  }

  for (int i = 0; str[i] != '\0'; i++)
  {
    if (!isdigit(str[i]))
    {
      return false;
    }
  }
  return true;
}

CalcInput convert_stack_item_to_calc_input(StackItem stack_item)
{
  char temp[2] = {stack_item, '\0'};
  if (strncmp(temp, "+", 1) == 0)
  {
    return OP_ADD;
  }
  else if (strncmp(temp, "-", 1) == 0)
  {
    return OP_SUBTRACT;
  }
  else if (strncmp(temp, "/", 1) == 0)
  {
    return OP_DIVIDE;
  }
  else
  {
    return OP_MULTIPLY;
  }
}

CalcInput convert_output_stack_item_to_calc_input(OutputStackItem stack_item)
{

  if (strncmp(stack_item, "+", 1) == 0)
  {
    return OP_ADD;
  }
  else if (strncmp(stack_item, "-", 1) == 0)
  {
    return OP_SUBTRACT;
  }
  else if (strncmp(stack_item, "/", 1) == 0)
  {
    return OP_DIVIDE;
  }
  else
  {
    return OP_MULTIPLY;
  }
}

void handle_button_click_equals(GtkWidget *widget, gpointer user_data)
{
  ButtonClickHandlerParams *data = (ButtonClickHandlerParams *)user_data;

  if (data->temp_num_stack->top > 0)
  {
    char dest[100];

    memcpy(dest, data->temp_num_stack->stack, data->temp_num_stack->top);
    dest[data->temp_num_stack->top] = '\0';

    data->temp_num_stack->top = 0;

    push_output_stack(data->token_stack, strdup(dest));
  }

  for (int i = 0; i < data->token_stack->top; i++)
  {
    OutputStackItem current_token = data->token_stack->stack[i];

    if (is_number(current_token))
    {
      push_output_stack(data->output_stack, current_token);
    }
    else
    {

      StackItem top = data->operation_stack->stack[data->operation_stack->top - 1];
      while (data->operation_stack->top > 0 && get_precedence(convert_stack_item_to_calc_input(top), convert_output_stack_item_to_calc_input(current_token)) == HIGHER)
      {
        StackItem popped = pop(data->operation_stack);
        char *op_copy = malloc(2);
        op_copy[0] = popped;
        op_copy[1] = '\0';
        push_output_stack(data->output_stack, op_copy);
      }

      push(data->operation_stack, current_token[0]);
    }
  }

  while (data->operation_stack->top > 0)
  {
    StackItem popped = pop(data->operation_stack);
    char *op_str = malloc(2);
    op_str[0] = popped;
    op_str[1] = '\0';
    push_output_stack(data->output_stack, op_str);
  }

  for (int i = 0; i < data->output_stack->top; i++)
  {
    OutputStackItem current_item = data->output_stack->stack[i];

    if (is_number(current_item))
    {
      push_eval_stack(data->eval_stack, atoi(current_item));
    }
    else
    {
      EvalStackItem popped1 = pop_eval_stack(data->eval_stack);
      EvalStackItem popped2 = pop_eval_stack(data->eval_stack);

      double result = 0;

      if (strncmp(current_item, "+", 1) == 0)
      {
        result = popped2 + popped1;
      }
      else if (strncmp(current_item, "-", 1) == 0)
      {
        result = popped2 - popped1;
      }
      else if (strncmp(current_item, "/", 1) == 0)
      {
        result = popped2 / popped1;
      }
      else if (strncmp(current_item, "*", 1) == 0)
      {
        result = popped2 * popped1;
      }

      push_eval_stack(data->eval_stack, result);
    }
  }

  char updated_text[128] = "SEGMENTATION FAULT";

  char str[20];
  sprintf(str, "%d", (int)data->eval_stack->stack[0]); // cast to int if needed
  gtk_label_set_text(GTK_LABEL(data->sum), str);

  // for (int i = 0; i < data->eval_stack->top; i++)
  // {
  //   printf("The Sum is: %d\n", data->eval_stack->stack[i]);
  // }
  // gtk_entry_set_text(GTK_ENTRY(data->running_formula), updated_text);
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

  // if backspace and there is nothing in the current text, nothing to do
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
    strcpy(updated_text, current_text);
    pop(data->temp_num_stack);
  }

  snprintf(updated_text, sizeof(updated_text), "%s%s", current_text, text_to_add);

  if (is_added_operator)
  {

    if (data->temp_num_stack->top > 0)
    {
      char dest[100];

      memcpy(dest, data->temp_num_stack->stack, data->temp_num_stack->top);
      dest[data->temp_num_stack->top] = '\0';

      data->temp_num_stack->top = 0;

      push_output_stack(data->token_stack, strdup(dest));

      push_output_stack(data->token_stack, strdup(text_to_add));
    }
    else
    {
      push_output_stack(data->token_stack, strdup(text_to_add));
    }
  }
  else
  {
    push(data->temp_num_stack, *text_to_add);
  }

  gtk_entry_set_text(GTK_ENTRY(data->running_formula), updated_text);

  return 0;
}

void add_input(GtkWidget *container, const char *place_holder, GtkWidget *sum_label, GtkWidget *running_formula)
{
  gtk_entry_set_placeholder_text(GTK_ENTRY(running_formula), "Type here...");
  // g_signal_connect(running_formula, "activate", G_CALLBACK(calc_value), sum_label);
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
  button_handle_click->eval_stack = data->eval_stack;
  button_handle_click->sum = data->sum;

  if (strncmp(&operation_label[0], operation_to_string(OP_EQUALS), 2) == 0)
  {
    g_signal_connect(button, "clicked", G_CALLBACK(handle_button_click_equals), button_handle_click);
  }
  else
  {
    g_signal_connect(button, "clicked", G_CALLBACK(handle_button_click), button_handle_click);
  }
}
