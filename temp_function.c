#include "temp_function.h"  
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 

// Функция для чтения данных о температуре из файла
int read_temperature_data(const char *filename, TemperatureData **data_arr, int *num_str) {
    FILE *file = fopen(filename, "r"); 
    if (file == NULL) { // Проверка успешности открытия файла
        fprintf(stderr, "Error opening file: %s\n", filename); // Вывод сообщения об ошибке
        return -1; // Возвращение значения -1 в случае ошибки
    }

    *data_arr = malloc(sizeof(TemperatureData)); // Выделение памяти под массив структур
    if (*data_arr == NULL) { // Проверка успешности выделения памяти
        fprintf(stderr, "Error: unable to allocate memory for temperature data array\n"); // Вывод сообщения об ошибке
        fclose(file); // Закрытие файла
        return -1; // Возвращение значения -1 в случае ошибки
    }

    *num_str = 0; // Инициализация переменной для подсчета строк
    int line_number = 0; // Переменная для хранения номера строки
    char buffer[256]; // Буфер для чтения строки из файла
    while (fgets(buffer, sizeof(buffer), file) != NULL) { // Чтение строк из файла
        if (strlen(buffer) <= 1) { // Проверка на пустую строку
            continue; // Пропуск пустых строк
        }

        int result = sscanf(buffer, "%d;%d;%d;%d;%d;%d",
                            &((*data_arr)[*num_str].year),
                            &((*data_arr)[*num_str].month),
                            &((*data_arr)[*num_str].day),
                            &((*data_arr)[*num_str].hour),
                            &((*data_arr)[*num_str].minute),
                            &((*data_arr)[*num_str].temperature)); // Считывание данных из строки

        if (result == 6) { // Проверка успешности считывания всех значений
            (*num_str)++; // Увеличение счетчика строк
            TemperatureData *tmp = realloc(*data_arr, (*num_str + 1) * sizeof(TemperatureData)); // Перераспределение памяти для массива
            if (tmp == NULL) { // Проверка успешности перераспределения памяти
                fprintf(stderr, "Error: unable to allocate memory for temperature data array\n"); // Вывод сообщения об ошибке
                fclose(file); // Закрытие файла
                free(*data_arr); // Освобождение памяти
                return -1; // Возвращение значения -1 в случае ошибки
            }
            *data_arr = tmp; 
        } else {
            fprintf(stderr, "Incorrect line format in file %s on line %d\n", filename, line_number + 1); // Вывод сообщения об ошибке неверного формата строки
        }
        line_number++; // Увеличение номера строки
    }
    fclose(file); // Закрытие файла
    return line_number; // Возвращение количества успешно прочитанных строк
}

// Функция для отображения статистики по месяцам
void display_monthly_stats(const TemperatureData *data_arr, int num_str, int target_month) {
    int month_count[MAX_MONTHS] = {0}; // Массив для подсчета количества значений по месяцам
    int month_sum[MAX_MONTHS] = {0}; // Массив для подсчета суммы значений по месяцам
    int month_min[MAX_MONTHS]; // Массив для хранения минимальных значений по месяцам
    int month_max[MAX_MONTHS]; // Массив для хранения максимальных значений по месяцам

    for (int i = 0; i < num_str; i++) { // Цикл по всем данным
        int month_index = data_arr[i].month - 1; // Индекс месяца в массиве
        month_count[month_index]++; 
        month_sum[month_index] += data_arr[i].temperature;
        if (month_count[month_index] == 1) { 
            month_min[month_index] = data_arr[i].temperature; 
            month_max[month_index] = data_arr[i].temperature; 
        } else { // Если уже есть значения для месяца
            if (data_arr[i].temperature < month_min[month_index]) { // Проверка на минимальную температуру
                month_min[month_index] = data_arr[i].temperature; // Обновление минимальной температуры
            }
            if (data_arr[i].temperature > month_max[month_index]) { // Проверка на максимальную температуру
                month_max[month_index] = data_arr[i].temperature; // Обновление максимальной температуры
            }
        }
    }

    for (int i = 0; i < MAX_MONTHS; i++) { // Цикл по всем месяцам
        if (month_count[i] > 0) { // Проверка наличия данных для месяца
            if (target_month == 0 || (i + 1) == target_month) { // Проверка соответствия месяца целевому месяцу
                if (target_month != 0) 
                    printf("Statistics for month %d:\n", target_month); 
                else
                    printf("\nMonth %d:\n", i + 1); // Вывод заголовка месяца
                printf("Average temperature: %.2f\n", (float)month_sum[i] / month_count[i]); // Вывод средней температуры
                if (month_min[i] == month_max[i]) { // Проверка на равенство минимальной и максимальной температуры
                    printf("The minimum and maximum temperatures are equal: %d\n", month_min[i]); // Вывод сообщения о равенстве
                } else { // Если минимальная и максимальная температуры различны
                    printf("Minimum temperature: %d\n", month_min[i]); // Вывод минимальной температуры
                    printf("Maximum temperature: %d\n", month_max[i]); // Вывод максимальной температуры
                }
            }
        }
    }
}

// Функция для отображения годовой статистики
void display_yearly_stats(const TemperatureData *data_arr, int num_str) {
    int year_sum = 0;
    int year_min = 99; 
    int year_max = -99; 

    for (int i = 0; i < num_str; i++) { // Цикл по всем данным
        year_sum += data_arr[i].temperature;
        if (data_arr[i].temperature < year_min) { // Проверка минимальной температуру за год
            year_min = data_arr[i].temperature; // Обновление минимальной температуры за год
        }
        if (data_arr[i].temperature > year_max) { // Проверка максимальной температуру за год
            year_max = data_arr[i].temperature; // Обновление максимальной температуры за год
        }
    }

    printf("\nYearly Statistics:\n"); // Вывод заголовка годовой статистики
    printf("Average annual temperature: %.2f\n", (float)year_sum / num_str); // Вывод среднегодовой температуры
    printf("Minimum temperature: %d\n", year_min); // Вывод минимальной температуры за год
    printf("Maximum temperature: %d\n", year_max); // Вывод максимальной температуры за год
}

