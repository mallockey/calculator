#include <gtk/gtk.h>
#include <stdbool.h>
#include <common.h>

static GtkWidget *sum_label;
static GtkWidget *running_formula;

static float running_sum = 0;

void clear_calculator()
{
  gtk_entry_set_text(GTK_ENTRY(running_formula), "");
}

int update_running_formula(GtkWidget *widget, gpointer data)
{
  const char *text_to_add = (const char *)data;

  bool should_remove = false;

  // Get current entry text
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(running_formula));
  const char *current_text = gtk_entry_buffer_get_text(buffer);

  Operation list_of_operations[5] = {OP_ADD, OP_DIVIDE, OP_EQUALS, OP_SUBTRACT, OP_MULTIPLY};

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
  if (strlen(current_text) == 0 && strncmp(text_to_add, "<-", 2) == 0)
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
  }
  else
  {
    snprintf(updated_text, sizeof(updated_text), "%s%s", current_text, text_to_add);
  }

  gtk_entry_set_text(GTK_ENTRY(running_formula), updated_text);

  return 0;
}

void set_current_operation(GtkWidget *widget, gpointer user_data)
{
  OperationData *data = (OperationData *)user_data;
  *(data->current_op_ptr) = data->op;

  const char *text_to_add = operation_to_string(data->op);
  update_running_formula(widget, (gpointer)text_to_add);
}

void calc_value(GtkWidget *entry)
{
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  float value = atof(text);
  running_sum += value;
  g_print("Running total: %f\n", running_sum);

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.2f", running_sum);

  gtk_label_set_text(GTK_LABEL(sum_label), buffer);
}

void add_input(GtkWidget *container, const char *place_holder)
{
  running_formula = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(running_formula), "Type here...");
  g_signal_connect(running_formula, "activate", G_CALLBACK(calc_value), NULL);
  sum_label = gtk_label_new("0.00");
  gtk_container_add(GTK_CONTAINER(container), sum_label);
  gtk_container_add(GTK_CONTAINER(container), running_formula);
}

void add_clear_button(GtkWidget *container)
{
  GtkWidget *button;
  GtkWidget *button_box;
  // Create a button and a button box
  button = gtk_button_new_with_label("Clear");
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

  // Add the button to the button box
  gtk_container_add(GTK_CONTAINER(button_box), button);

  // Connect signals to the button
  g_signal_connect(button, "clicked", G_CALLBACK(clear_calculator), NULL);

  // Add the button box to the container (GtkBox)
  gtk_container_add(GTK_CONTAINER(container), button_box);
}

void add_operation_button(GtkWidget *container, Operation op, Operation *current_operation)
{
  GtkWidget *button;
  GtkWidget *button_box;

  // Create a button and a button box
  const char *operation_label = operation_to_string(op);
  button = gtk_button_new_with_label(operation_label);
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

  // Add the button to the button box
  gtk_container_add(GTK_CONTAINER(button_box), button);

  // Allocate and fill data
  OperationData *data = malloc(sizeof(OperationData));
  data->op = op;
  data->current_op_ptr = current_operation;

  // Connect signal with packed data
  g_signal_connect(button, "clicked", G_CALLBACK(set_current_operation), data);

  // Add the button box to the container (GtkBox)
  gtk_container_add(GTK_CONTAINER(container), button_box);
}

void add_num_button(GtkWidget *container, char *label)
{
  GtkWidget *button;
  GtkWidget *button_box;

  // Create a button and a button box
  button = gtk_button_new_with_label(label);
  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

  // Add the button to the button box
  gtk_container_add(GTK_CONTAINER(button_box), button);

  // Connect signals to the button
  // g_print("%s", label);
  g_signal_connect(button, "clicked", G_CALLBACK(update_running_formula), label);

  // Add the button box to the container (GtkBox)
  gtk_container_add(GTK_CONTAINER(container), button_box);
}
