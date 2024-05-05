#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_function.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || strcmp(argv[1], "-h") == 0) {
        printf("Welcome to TempAnalyzer!\n");
        printf("Usage:\n");
        printf("  -f <filename.csv> : specify input file\n");
        printf("  -m <month number> : display statistics for a specific month\n");
        printf("  -h: display this help message\n");
        return 0;
    }

    char *filename = NULL;
    int month = 0;
    TemperatureData *data_arr = NULL;
    int num_str = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                filename = argv[i + 1];
                read_temperature_data(filename, &data_arr, &num_str);
                if (i + 2 < argc && strcmp(argv[i + 2], "-m") == 0) {
                    if (i + 3 < argc) {
                        month = atoi(argv[i + 3]);
                        display_monthly_stats(data_arr, num_str, month);
                    } else {
                        printf("Error: month number is not specified\n");
                        return -1;
                    }
                } else {
                    display_monthly_stats(data_arr, num_str, 0);
                    display_yearly_stats(data_arr, num_str);
                }
                break;
            } else {
                printf("Error: file name is not specified\n");
                return -1;
            }
        }
    }

    if (filename == NULL) {
        printf("Error: file name is not specified\n");
        return -1;
    }
    free(data_arr);
    return 0;
}
