#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "threads.h"
#include "main.h"
#include "utils.h" // для user_input() и clear_stdin()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

void carving_profile(){
        printf("Введите шаг резьбы:\n");
        float step;
        scanf("%f", &step);
        clear_stdin();
        float res = 0.86 * step;
        printf("Высота профиля резьбы: %0.3f\n", res);
}


void find_word(){
        FILE *file = fopen("carving.txt", "r");
        if(file == NULL){
                printf("Не найден файл базы резьб.");
                return;
        }

        char user[20];
        user_input(user, sizeof(user));

        char buffer[100];
        while(fgets(buffer, sizeof(buffer), file)){
                if (buffer[0] == '\0' || buffer[0] == '#') continue;
                //printf("Проверяю строку: '%s'\n", buffer);

                char name[20];
                double step = 0.0;

                if(sscanf(buffer, "%s %lf", name, &step) == 2){
                        //printf("Извлечено: name='%s', step=%.2f\n", name, step);

                        if(strcmp(name, user) == 0){
                                printf("Резьба %s: шаг = %.2lfмм\n", name, step);
                                fclose(file);
                                return;
                        }
                }
        }
        printf("Не найдено\n");
        fclose(file);
        return;
}
