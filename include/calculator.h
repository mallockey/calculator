#include <gtk/gtk.h>
#include "common.h"

#ifndef CALCULATOR_H
#define CALCULATOR_H

void clear_calculator();

int update_running_formula(GtkWidget *widget, gpointer data);

void set_current_operation(GtkWidget *widget, gpointer data);

void calc_value(GtkWidget *entry);

void add_input(GtkWidget *container, const char *place_holder);

void add_clear_button(GtkWidget *container);

void add_operation_button(GtkWidget *container, Operation op, Operation *current_operation);

void add_num_button(GtkWidget *container, char *label);

#endif