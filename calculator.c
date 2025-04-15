#include <gtk/gtk.h>

static float running_sum = 0;
static GtkWidget *running_formula;
static GtkWidget *sum_label;

static void clear_calculator()
{
  gtk_entry_set_text(GTK_ENTRY(running_formula), "");
}

static void update_running_formula(GtkWidget *widget, gpointer data)
{
  const char *text_to_add = (const char *)data;

  // Get current entry text
  GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(running_formula));
  const char *current_text = gtk_entry_buffer_get_text(buffer);

  // Build new string
  char updated_text[128];
  snprintf(updated_text, sizeof(updated_text), "%s%s", current_text, text_to_add);

  // Set the new text
  gtk_entry_set_text(GTK_ENTRY(running_formula), updated_text);
}

static void calc_value(GtkWidget *entry)
{
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  float value = atof(text);
  running_sum += value;
  g_print("Running total: %f\n", running_sum);

  // Create formatted string
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.2f", running_sum);

  // Update label
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
  // g_print("%s", label);
  g_signal_connect(button, "clicked", G_CALLBACK(clear_calculator), NULL);

  // Add the button box to the container (GtkBox)
  gtk_container_add(GTK_CONTAINER(container), button_box);
}

static void add_num_button(GtkWidget *container, const char *label)
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
  char current_operation = 0;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  input_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(main_box), input_box, FALSE, FALSE, 0);

  add_input(input_box, "Enter stuff");

  box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(main_box), box1, FALSE, FALSE, 0);

  add_num_button(box1, "7");
  add_num_button(box1, "8");
  add_num_button(box1, "9");

  box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(main_box), box2, FALSE, FALSE, 0);

  add_num_button(box2, "4");
  add_num_button(box2, "5");
  add_num_button(box2, "6");

  box3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(main_box), box3, FALSE, FALSE, 0);

  add_num_button(box3, "1");
  add_num_button(box3, "2");
  add_num_button(box3, "3");

  clear_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(main_box), clear_box, FALSE, FALSE, 0);
  add_clear_button(clear_box);

  gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
