#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define MAX_OPTIONS 10
#include "main.h"
#include "gears.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>
#include "solver.h"

struct option_sat{
	float module;
	int64_t z_sat;
	float a;
	float distance_sat;
	float d_rad_sat;
};


// Глобальные переменные (реализация)
const float standard_modules[] = {
    0.5, 0.6, 0.8, 1.0, 1.125, 1.25, 1.375, 1.5,
    1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.5, 4.0, 4.5, 5.0
};
const size_t n_modules = sizeof(standard_modules) / sizeof(standard_modules[0]);

float round_to_standard_module(float m){
        if(m <= 0.0f){ return 1.0f; }

        float best = standard_modules[0];
        float best_diff = fabs(m - best);

        for(size_t i = 1; i < n_modules; i++){
                float diff = fabs(m - standard_modules[i]);
                if(diff < best_diff){
                        best_diff = diff;
                        best = standard_modules[i];
                }

        }
        return best;

}

float module(){
				printf("Вам известен модуль шестерни?\n");
				printf("1. Расчитать модуль введя окружной шаг\n");
				printf("2. Ввести модуль\n");
				int8_t choise;
				scanf("%" SCNd8, &choise);
				clear_stdin();

				switch(choise){
					case 1:
						printf("Введите окружной шаг зубьев Pt: ");
						float pt;
						scanf("%f", &pt);
						clear_stdin();
						float mod = pt / M_PI;
						float mod_s = round_to_standard_module(mod);
						printf("| Вычисленный модуль: %0.3f, округленный модуль по ГОСТу: %0.3f |\n", mod, mod_s);
						return mod_s;
					case 2:
						printf("Введите модуль m: ");
						float m;
						scanf("%f", &m);
						clear_stdin();
						return m;
					default:
						printf("Нормальное число выбери\n");
						return module();
				}
}


// gears.c
#include "solver.h"
#include "utils.h"


void calculate_bevel_gear(){
	printf("\n--- Расчет Конической передачи ---\n");

	struct var context[MAX_VARS];
	size_t context_len = 0;

	const char* formula_file = "bevel_gear";
	
	struct maybe_double d1 = start_loop((char*)formula_file, (char*)"de1", context, &context_len);
	if(d1.valid){
		printf("Делительный диаметр полуосевой шестерни: %.3f мм\n", d1.value);
	}
	struct maybe_double d2 = start_loop((char*)formula_file, (char*)"de2", context, &context_len);
	if(d2.valid){
		printf("Делительный диаметр сателлита: %.3f мм\n", d2.value);
	}

	struct maybe_double ratio1 = start_loop((char*)formula_file, (char*)"ratio1", context, &context_len);
	struct maybe_double ratio2 = start_loop((char*)formula_file, (char*)"ratio2", context, &context_len);

	struct maybe_double delta1 = start_loop((char*)formula_file, (char*)"delta1", context, &context_len);
	if(delta1.valid){
		printf("Угол делительных конусов delta1: %.3f мм\n", delta1.value);
	}
	
	struct maybe_double delta2 = start_loop((char*)formula_file, (char*)"delta2", context, &context_len);
	if(delta2.valid){
		printf("Угол делительных конусов delta2: %.3f мм\n", delta2.value);
	}
	
	struct maybe_double Re = start_loop((char*)formula_file, (char*)"Re", context, &context_len);
	if(Re.valid){
		printf("Внешнее конусное расстояние: %.3f мм\n", Re.value);
	}
	
	struct maybe_double b = start_loop((char*)formula_file, (char*)"b", context, &context_len);
	if(b.valid){
		printf("Ширина зубчатого венца: %.3f мм\n", b.value);
	}

  printf("\n--- Использованные параметры ---\n");
  for (size_t i = 0; i < context_len; i++) {
      printf("%s = %.3f\n", context[i].name, context[i].value);
  }
	printf("Делительный диаметр полуосевой шестерни: %.3f мм\n", d1.value);
	printf("Делительный диаметр сателлита: %.3f мм\n", d2.value);
	printf("Угол делительных конусов delta1: %.3f мм\n", delta1.value);
	printf("Угол делительных конусов delta2: %.3f мм\n", delta2.value);
	printf("Внешнее конусное расстояние: %.3f мм\n", Re.value);
	printf("Ширина зубчатого венца: %.3f мм\n", b.value);
}

void calculate_hypoid(){
	printf("\n--- Расчет Дифференциала/Гипоидной передачи ---\n");
	calculate_bevel_gear();

	struct var context[MAX_VARS];
	size_t context_len = 0;
	const char* formula_file = "hypoid";

	struct maybe_double d1 = start_loop((char*)formula_file, (char*)"de1", context, &context_len);
	struct maybe_double d2 = start_loop((char*)formula_file, (char*)"de2", context, &context_len);
	struct maybe_double distance_a = start_loop((char*)formula_file, (char*)"A", context, &context_len);
	struct maybe_double theta = start_loop((char*)formula_file, (char*)"theta", context, &context_len);	
	struct maybe_double sin_theta = start_loop((char*)formula_file, (char*)"sin_theta", context, &context_len);
	struct maybe_double ratio_z = start_loop((char*)formula_file, (char*)"ratio_z", context, &context_len);	
	struct maybe_double delta2 = start_loop((char*)formula_file, (char*)"delta2", context, &context_len);
	struct maybe_double delta1 = start_loop((char*)formula_file, (char*)"delta1", context, &context_len);
	struct maybe_double Re1 = start_loop((char*)formula_file, (char*)"Re1", context, &context_len);
	struct maybe_double Re2 = start_loop((char*)formula_file, (char*)"Re2", context, &context_len);
	struct maybe_double beta2_deg = start_loop((char*)formula_file, (char*)"beta2_deg", context, &context_len);
	struct maybe_double beta2 = start_loop((char*)formula_file, (char*)"beta2", context, &context_len);
	struct maybe_double sin_beta2 = start_loop((char*)formula_file, (char*)"sin_beta2", context, &context_len);
	struct maybe_double beta1 = start_loop((char*)formula_file, (char*)"beta1", context, &context_len);
	struct maybe_double dae1 = start_loop((char*)formula_file, (char*)"dae1", context, &context_len);
	struct maybe_double dae2 = start_loop((char*)formula_file, (char*)"dae2", context, &context_len);

  // Вывод результатов
    printf("\n--- Результаты расчёта ---\n");
    if (d1.valid) printf("Внешний делительный диаметр ведущей шестерни (de1): %.3f мм\n", d1.value);
    if (d2.valid) printf("Внешний делительный диаметр ведомого колеса (de2): %.3f мм\n", d2.value);
    if (distance_a.valid)   printf("Межосевое расстояние (A): %.3f мм\n", distance_a.value);
    if (delta1.valid) printf("Угол начального конуса ведущей (delta1): %.3f рад (%.2f°)\n", delta1.value, delta1.value * 180.0 / M_PI);
    if (delta2.valid) printf("Угол начального конуса ведомого (delta2): %.3f рад (%.2f°)\n", delta2.value, delta2.value * 180.0 / M_PI);
    if (Re1.valid) printf("Внешнее конусное расстояние (Re): %.3f мм\n", Re1.value);
    if (beta1.valid) printf("Угол наклона зуба ведущей (beta1): %.3f рад (%.2f°)\n", beta1.value, beta1.value * 180.0 / M_PI);
    if (beta2.valid) printf("Угол наклона зуба ведомого (beta2): %.3f рад (%.2f°)\n", beta2.value, beta2.value * 180.0 / M_PI);
    if (dae1.valid) printf("Диаметр вершин ведущей (dae1): %.3f мм\n", dae1.value);
    if (dae2.valid) printf("Диаметр вершин ведомого (dae2): %.3f мм\n", dae2.value);


  printf("\n--- Использованные параметры ---\n");
  for (size_t i = 0; i < context_len; i++) {
      printf("%s = %.3f\n", context[i].name, context[i].value);
  }
}



void calculate_radial_distances(){
			struct option_sat sat[MAX_OPTIONS];
      float module_n = module();
			if(module_n <= 0) {
				printf("Модуль должен быть больше\n");
        return;
			}
			int64_t z_sat;
			size_t n, z_cent;
			printf("Введите количество зубьев солнца: ");
			scanf("%zu", &z_cent);
			clear_stdin();

			printf("Введите количество сатилитов: ");
			scanf("%zu", &n);
			clear_stdin();

			float a, distance_sat, d_rad_sat;
			//printf("Введите количество зубьев сатилита: ");
			//scanf("%zu", &z_sat);
			//clear_stdin();
			
			size_t option_count = 0;
			for(z_sat = (int64_t)z_cent; z_sat >= 1 && option_count < MAX_OPTIONS; z_sat -= 1){
				a = (module_n * (z_sat + z_cent) / 2.0);
				float corner_to_rad = (180.0f / (float)n) * (M_PI/180.0f);
				distance_sat = (2.0*a) * sinf(corner_to_rad);
				d_rad_sat = module_n * (z_sat + 2.0);

				if((z_cent + z_sat) % n == 0){
					if(distance_sat > d_rad_sat){
					sat[option_count].z_sat = z_sat;
					sat[option_count].a = a;
					sat[option_count].distance_sat = distance_sat;
					sat[option_count].d_rad_sat = d_rad_sat;
					option_count++;
					}
				}	
				if(z_sat < 17){
					printf("Внимание z_sat = " "%" PRId64 "< 17 - возможна подрезания зубьев\n", z_sat);
				}
			}

			if(option_count == 0){
				printf("Вариантов нет\n");
				return;
			}

			printf("\n\t= = = Варианты Сатилитов и эпициклов = = =\t\n");
			for(size_t i = 0; i < option_count; i++){
					printf("\n\t+= = = Результаты расчетов параметров Сатилитов #%zu = = =+\t\n", i+1);
					printf("\t| Количество зубьев сатилитов:                 " "%" PRId64 " |\n", sat[i].z_sat);
					printf("\t| Межосевое расстояние и радиус расположения сатилитов: %0.2f |\n", sat[i].a);
					printf("\t| Расстояния между осями сатилитов:                     %0.2f |\n", sat[i].distance_sat);
					printf("\t| Наружный диаметр сатилитов:                           %0.2f |\n", sat[i].d_rad_sat);
					

					printf("\n\t+= = = Результаты расчетов параметров Эпицикла #%zu = = =+\t\n", i);
					//число зубьев эпицикла
					int64_t z_epi = z_cent + (2 * sat[i].z_sat);
					printf("\t| Число зубьев Эпицикла:            " "%" PRId64 " |\n", z_epi);
					
					//делительный диаметр
					float d_epi = module_n * z_epi;
					printf("\t| Делительный диаметр Эпицикла:   %0.1f |\n", d_epi);

					//диаметр вершин
					float d_epi_z = module_n * (z_epi - 2);
					printf("\t| Диаметр вершин Эпицикла:        %0.1f |\n", d_epi_z);

					//диаметр впадин
					float d_epi_f = module_n * (z_epi + 2.5);
					printf("\t| Диаметр впадин Эпицикла:        %0.1f |\n", d_epi_f);

					printf("\n\t##################################################\n");
			}
			
}

void bevel_planetar_gear(){
	struct option_sat sat[MAX_OPTIONS];
  float module_n = module();
  if(module_n <= 0) {
		printf("Модуль должен быть больше\n");
    return;
  }
  int64_t z_sat;
  size_t n, z_cent; 
	float beta;
  printf("Введите количество зубьев солнца: ");
  scanf("%zu", &z_cent);
  clear_stdin();

  printf("Введите количество сатилитов: ");
  scanf("%zu", &n);
  clear_stdin();
		
	printf("Введите угол наклона Бета: ");
	printf("Тихоходные 8° – 15°\n");
	printf("Общего назначения 15° – 25°\n");
	printf("Высокоскоростные 25° – 45°\n");
	scanf("%f", &beta);
	beta = beta * (M_PI / 180.0f);
	
	size_t option_count = 0;
  float a, distance_sat, d_rad_sat;	
	for(z_sat = (int64_t)z_cent; z_sat >= 1 && option_count < MAX_OPTIONS; z_sat -= 1){
			a = module_n * (z_cent + z_sat) / (2.0f * cosf(beta));
			float corner_to_rad = (180.0f / (float)n) * (M_PI/180.0f);
			distance_sat = (2.0*a) * sinf(corner_to_rad);
			d_rad_sat = module_n * (z_sat + 2.0) / cosf(beta);

			if((z_cent + z_sat) % n == 0){
				if(distance_sat > d_rad_sat){
				sat[option_count].z_sat = z_sat;
				sat[option_count].a = a;
				sat[option_count].distance_sat = distance_sat;
				sat[option_count].d_rad_sat = d_rad_sat;
				option_count++;
				}
			}	
			if(z_sat < 17){
				printf("Внимание z_sat = " "%" PRId64 "< 17 - возможна подрезания зубьев\n", z_sat);
			}
	}		


			printf("\n\t= = = Варианты Сатилитов и эпициклов = = =\t\n");
			LOG("\n\t= = = Варианты Сатилитов и эпициклов = = =\t\n");
			for(size_t i = 0; i < option_count; i++){
					printf("\n\t+= = = Результаты расчетов параметров Сатилитов #%zu = = =+\t\n", i+1);
					//LOG("\n\t+= = = Результаты расчетов параметров Сатилитов #%zu = = =+\t\n", i+1);
					printf("\t| Количество зубьев сатилитов:                 " "%" PRId64 " |\n", sat[i].z_sat);
					//LOG("\t| Количество зубьев сатилитов:                 " "%" PRId64 " |\n", sat[i].z_sat);
					printf("\t| Межосевое расстояние и радиус расположения сатилитов: %0.2f |\n", sat[i].a);
					//LOG("\t| Межосевое расстояние и радиус расположения сатилитов: %0.2f |\n", sat[i].a);
					printf("\t| Расстояния между осями сатилитов:                     %0.2f |\n", sat[i].distance_sat);
					//LOG("\t| Расстояния между осями сатилитов:                     %0.2f |\n", sat[i].distance_sat);
					printf("\t| Наружный диаметр сатилитов:                           %0.2f |\n", sat[i].d_rad_sat);
					//LOG("\t| Наружный диаметр сатилитов:                           %0.2f |\n", sat[i].d_rad_sat);

					printf("\n\t+= = = Результаты расчетов параметров Эпицикла #%zu = = =+\t\n", i+1);
					//LOG("\n\t+= = = Результаты расчетов параметров Эпицикла #%zu = = =+\t\n", i+1);
					//число зубьев эпицикла
					int64_t z_epi = z_cent + (2 * sat[i].z_sat);
					printf("\t| Число зубьев Эпицикла:            " "%" PRId64 " |\n", z_epi);
					//LOG("\t| Число зубьев Эпицикла:            " "%" PRId64 " |\n", z_epi);
					
					//делительный диаметр
					float d_epi = module_n * z_epi / cosf(beta);
					printf("\t| Делительный диаметр Эпицикла:   %0.1f |\n", d_epi);
					//LOG("\t| Делительный диаметр Эпицикла:   %0.1f |\n", d_epi);

					//диаметр вершин
					float d_epi_z = module_n * (z_epi - 2) / cosf(beta);
					printf("\t| Диаметр вершин Эпицикла:        %0.1f |\n", d_epi_z);
					//LOG("\t| Диаметр вершин Эпицикла:        %0.1f |\n", d_epi_z);

					//диаметр впадин
					float d_epi_f = module_n * (z_epi + 2.5) / cosf(beta);
					printf("\t| Диаметр впадин Эпицикла:        %0.1f |\n", d_epi_f);
					//LOG("\t| Диаметр вершин Эпицикла:        %0.1f |\n", d_epi_z);

					printf("\n\t##################################################\n");
					//LOG("\n\t##################################################\n");

					printf("Перерасчитать?(y/n) \n");

			}
}

void calculate_center_distances(){
			float m = module();
			if(m <= 0) {
				printf("Модуль должен быть больше\n");
				return;
			}

      size_t n;
      printf("Введите количество ступеней(пар шестерен): ");
      scanf("%zu", &n);
			clear_stdin();

      float* a_list = malloc(n * sizeof(float));
			if(a_list == NULL) return;
      float total_distance = 0.0f;

      for(size_t i = 0; i < n; i++){
              size_t z_driving, z_driven;
              printf("Ступень %zu:\n", i + 1);
              printf("Число зубьев ведущей шестерни: ");
              scanf("%zu", &z_driving);
							clear_stdin();
              printf("Число зубьев ведомой шестерни: ");
							scanf("%zu", &z_driven);
							clear_stdin();

              if(z_driving <= 0 || z_driven <= 0){
                      printf("Количество зубьев должно быть больше\n");
                      free(a_list);
                      return;
                }

              a_list[i] = (m * (z_driving + z_driven)) / 2.0;
              total_distance += a_list[i];

        }
			printf("\n      \t+= = = Результаты = = =+\t\n");
			//LOG("\n      \t+= = = Результаты Расчета межосевого расстояния= = =+\t\n");
      for(size_t i = 0; i < n; i++){
							printf("\t| Ступень %zu: a =              %.2f мм |\n", i+1, a_list[i]);
							//LOG("\t| Ступень %zu: a =              %.2f мм |\n", i+1, a_list[i]);
      }
      printf("\t| Общая длина линейной ступени: %0.2f мм |\n", total_distance);
			//LOG("\t| Общая длина линейной ступени: %0.2f мм |\n", total_distance);
      free(a_list);
}

void gear_chain_ratio(){
	printf("| Ведущая шестерня — та, что приводится в движение (ближе к двигателю) |\n");
	printf("|           Ведомая шестерня — та, что вращается от ведущей            |\n");

	size_t n;
	printf("Введите количество ступеней(пар шестерен): ");
	scanf("%zu", &n);

	float* ratios = malloc(n * sizeof(float));
	if(ratios == NULL) return;

	float total_ratios = 1.0f;

	for(size_t i = 0; i < n; i++){
		size_t z_driving, z_driven;
		printf("\nСтупень %zu:\n", i + 1);
		printf("Число зубьев ведущей шестерни: ");
		scanf("%zu", &z_driving);
		clear_stdin();
		printf("Число зубьев ведомой шестерни: ");
		scanf("%zu", &z_driven);
		clear_stdin();
		
		if(z_driving <= 0 || z_driven <= 0){
			printf("Количество зубьев должно быть больше\n");
			free(ratios);
			return;
		}

		ratios[i] = (float)z_driven / z_driving;
		total_ratios *= ratios[i];

	}

	printf("\n  \t+= = = Результаты Расчета Передач = = =+\t\n");
	//LOG("\n  \t+= = = Результаты Расчета Передач = = =+\t\n");
	LOG("\t|Ступеней:                    %zu |\n", n);
	for(size_t i = 0; i < n; i++){
		printf("\t| Ступень %zu: ПЧ =        %.3f |\n", i+1, ratios[i]);
		//LOG("\t| Ступень %zu: ПЧ =        %.3f |\n", i+1, ratios[i]);
	}
	printf("\t| Общее передаточное число: %0.3f |\n", total_ratios);
	//LOG("\t| Общее передаточное число: %0.3f |\n", total_ratios);
	free(ratios);
}

void gear_calc(){
        float z;
        printf("Введите кол-во зубьев вашей шестерни: ");
        scanf("%f", &z);
        clear_stdin();
        float m = module();
				//LOG("=== Расчет стандартных параметров шестерни ===\n");
				printf("\n\t+= = = = = = =   Результаты   = = = = = = =+\t\n");
				

        float d = z*m;
        printf("\t| Диаметр Делительный d:              %0.1f |\n", d);
				//LOG("| Диаметр Делительный d:              %0.1f |\n", d);

        float h = 2.25*m;
        printf("\t| Высота зуба h:                       %0.1f |\n", h);
				//LOG("| Высота зуба h:                      %0.1f |\n", h);

        float ha = m;
        printf("\t| Высота головки ha:                   %0.1f |\n", ha);
				//LOG("| Высота головки ha:                  %0.1f |\n", ha);

        float hf = 1.25*m;
        printf("\t| Высота ножки hf:                     %0.1f |\n", hf);
				//LOG("| Высота ножки hf:                    %0.1f |\n", hf);			

        float da = d + (2*ha);
        printf("\t| Диаметр вершин зубьев da:           %0.1f |\n", da);
				//LOG("| Диаметр вершин зубьев da:           %0.1f |\n", da);

        float df = d - (2*hf);
        printf("\t| Диаметр впадин зубчатого колеса df: %0.1f |\n", df);
				//LOG("| Диаметр впадин зубчатого колеса df: %0.1f |\n", df);
				//LOG("\n");
}





void bevel_gear_calc(){
        float z = number_user_input("Число Зубьв Z");
        float de = number_user_input("Диаметр Вершин De");
        float beta = number_user_input("Угол наклона на вершинах Be");

        float beta_rad = beta * (M_PI / 180.0);
        float target = z * tanf(beta_rad);

        float beta_deg = 0;
        //float beta_deg_rad = beta_deg * (M_PI / 180.0);

        for(size_t i = 0; i < 50; i++){
                beta_deg = beta - 1.0;
                float beta_deg_rad = beta_deg * (M_PI / 180.0);

                float beta_sin = sinf(beta_deg_rad);
                float beta_math = target / (z / cosf(beta_deg_rad) + 2);

                if(fabsf(beta_sin - beta_math) < 1e-5) break;

                if(beta_sin < beta_math) beta_deg += 0.1f;
                else beta_deg -= 0.1f;

        }
				printf("\n              \t+= = = Результаты = = =+\t\n");
        printf("\t| Угол наклона на делительной окружности:                      %0.1f |\n", beta_deg);
        float module_normal = de / (z / cosf(beta_deg * (M_PI/180)) + 2.0);

        module_normal = round_to_standard_module(module_normal);
        printf("\t| Нормальный Модуль данной шестерни по полученным результатам: %0.1f |\n", module_normal);
}


