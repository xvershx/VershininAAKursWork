#ifndef TEMP_function_H
#define TEMP_function_H
#define MAX_MONTHS 12  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int temperature;
} TemperatureData;

int read_temperature_data(const char *filename, TemperatureData **data_arr, int *num_str);
void display_monthly_stats(const TemperatureData *data_arr, int num_str, int target_month);
void display_yearly_stats(const TemperatureData *data_arr, int num_str);
void print_temperature_data(const TemperatureData *data_arr, int num_str);

#endif
