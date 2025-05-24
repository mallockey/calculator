#include <gtk/gtk.h>

#include <common.h>

#ifndef CALCULATOR_H
#define CALCULATOR_H

void clear_calculator(GtkWidget *widget, gpointer user_data);

int handle_button_click(GtkWidget *widget, gpointer user_data);

void handle_button_click_equals(GtkWidget *widget, gpointer user_data);

void add_input(GtkWidget *container, const char *place_holder, GtkWidget *sum_label, GtkWidget *running_formula);

void add_clear_button(GtkWidget *container, OperationData *data);

void add_operation_button(GtkWidget *container, CalcInput op, OperationData *data);

#endif