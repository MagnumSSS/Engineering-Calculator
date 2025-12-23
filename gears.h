#ifndef GEARS_H
#define GEARS_H
#include <stddef.h> // для size_t

// Глобальные переменные (объявляем extern)
extern const float standard_modules[];
extern const size_t n_modules;


// Прототипы функций
void calculate_radial_distances();
void calculate_center_distances();
void gear_chain_ratio(void);
//void top_gear(void);
float module(void);
void gear_calc(void);
float round_to_standard_module(float m);
void bevel_planetar_gear(void);
void bevel_gear_calc(void);
//void bevel_gear(void);

#endif // GEARS_H
