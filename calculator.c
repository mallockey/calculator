#include <gtk/gtk.h>
#include <stdbool.h>

static GtkWidget *sum_label;
static GtkWidget *running_formula;

static float running_sum = 0;
static char current_operation = 0;

static void clear_calculator()
{
  gtk_entry_set_text(GTK_ENTRY(running_formula), "");
}

static int update_running_formula(GtkWidget *widget, gpointer data)
{
  const char *text_to_add = (const char *)data;

  bool should_remove = false;

  // Get current entry text
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(running_formula));
  const char *current_text = gtk_entry_buffer_get_text(buffer);

  printf("Size: %d\n", strlen(current_text));

  // If backspace and there is nothing in the current text, nothing to do
  if (strlen(current_text) == 0 && strncmp(text_to_add, "<-", 2) == 0)
  {
    return 1;
  }

  if (strncmp(text_to_add, "<-", 2) == 0 && strlen(current_text) > 0)
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

static void set_current_operation(GtkWidget *widget, gpointer data)
{
  const char *text_to_add = (const char *)data;
  current_operation = *text_to_add;
  update_running_formula(widget, data);
}

static void calc_value(GtkWidget *entry)
{
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  float value = atof(text);
  running_sum += value;
  g_print("Running total: %f\n", running_sum);

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.2f", running_sum);

  gtk_label_set_text(GTK_LABEL(sum_label), buffer);
}

static void add_input(GtkWidget *container, const char *place_holder)
{
  running_formula = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(running_formula), "Type here...");
  g_signal_connect(running_formula, "activate", G_CALLBACK(calc_value), NULL);
  sum_label = gtk_label_new("0.00");
  gtk_container_add(GTK_CONTAINER(container), sum_label);
  gtk_container_add(GTK_CONTAINER(container), running_formula);
}

static void add_clear_button(GtkWidget *container)
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

static void add_operation_button(GtkWidget *container, char *label)
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
  g_signal_connect(button, "clicked", G_CALLBACK(set_current_operation), label);

  // Add the button box to the container (GtkBox)
  gtk_container_add(GTK_CONTAINER(container), button_box);
}

static void add_num_button(GtkWidget *container, char *label)
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

static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *main_box;
  GtkWidget *box1;      // Horizontal box for buttons
  GtkWidget *box2;      // Horizontal box for buttons
  GtkWidget *box3;      // Horizontal box for buttons
  GtkWidget *clear_box; // Horizontal box for buttons
  GtkWidget *input_box; // Vertical box for input + label
  GtkWidget *parent_box;
  GtkWidget *left_side_child_box;
  GtkWidget *right_side_child_box;

  GtkWidget *operations_box; // Vertical box for input + label

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
  gtk_container_add(GTK_CONTAINER(window), main_box);
  gtk_widget_set_margin_top(main_box, 10);
  gtk_widget_set_margin_bottom(main_box, 10);
  gtk_widget_set_margin_start(main_box, 10); // left
  gtk_widget_set_margin_end(main_box, 10);   // right

  input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(main_box), input_box, FALSE, FALSE, 0);

  add_input(input_box, "Enter stuff");

  input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  // Create the parent horizontal box
  parent_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // 10 is spacing between children

  gtk_box_pack_start(GTK_BOX(main_box), parent_box, FALSE, FALSE, 0);

  // Create child boxes (or widgets to go in those positions)
  left_side_child_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // or any other widget
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
  // TODO: Add typedef
  add_operation_button(operations_box, "/");
  add_operation_button(operations_box, "X");
  add_operation_button(operations_box, "-");
  add_operation_button(operations_box, "+");
  add_operation_button(operations_box, "=");
  add_operation_button(operations_box, "<-");

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
