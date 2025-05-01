#include <gtk/gtk.h>
#include "common.h"

#ifndef CALCULATOR_H
#define CALCULATOR_H

void clear_calculator(GtkWidget *running_formula);

int update_running_formula(GtkWidget *widget, gpointer data);

void set_current_operation(GtkWidget *widget, gpointer data);

void calc_value(GtkWidget *entry, GtkWidget *sum_label);

void add_input(GtkWidget *container, const char *place_holder, GtkWidget *sum_label, GtkWidget *running_formula);

void add_clear_button(GtkWidget *container, GtkWidget *running_formula);

void add_operation_button(GtkWidget *container, CalcInput op, CalcInput *current_operation, GtkWidget *running_formula);

void add_num_button(GtkWidget *container, char *label);

#endif