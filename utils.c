#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>


void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void user_input(char* mod, size_t size) {
    printf("Введите Резьбу: \n");
    fgets(mod, (int)size, stdin);
    mod[strcspn(mod, "\n")] = '\0';
}

float number_user_input(const char* text) {
    float number;
    printf("Введите %s: ", text);
    scanf("%f", &number);
    clear_stdin(); // важно очищать stdin!
    return number;
}
