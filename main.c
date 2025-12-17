#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "gears.h"
#include "threads.h"
#include "utils.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

FILE* output_file = NULL;




int main() {
		setup_log();
    main_menu(); // запуск главного меню
		
		if(output_file){
			fprintf(output_file, "\n==Конец Сессии ===\n");
			fclose(output_file); 
		}
    return 0;
}

void setup_log(){
	printf("Сохранить расчеты в файл(y/n)?\n");
	char choise;
	scanf(" %c", &choise);
	clear_stdin();
	
	if(choise == 'y' || choise == 'Y'){
		char filename[100];
		printf("Введите имя файла: ");
		fgets(filename, sizeof(filename), stdin);
		filename[strcspn(filename, "\n")] = '\0';

		output_file = fopen(filename, "a");
		if(output_file == NULL){
			printf("Не удалось открыть файл '%s' \n", filename);
			output_file = NULL;
		}
		else{
			fprintf(output_file, "\n=== Сессия расчётов от %s ===\n", __DATE__);
		}

	}
}

// Реализация главного меню
void main_menu() {
    printf("\n\tРасчетный калькулятор\t\n");
    printf("Что расчитать?\n");
    printf("-> 1. Шестерня\n");
    printf("-> 2. Резьба\n");
    printf("-> 3. Отмена\n");
    
    int8_t choise;
    scanf("%" SCNd8, &choise);
    clear_stdin();
    
    switch(choise) {
        case 1: gear_menu(); break;
        case 2: carving_menu(); break;
        case 3: printf("|Отмена|\n"); break;
    }
}
