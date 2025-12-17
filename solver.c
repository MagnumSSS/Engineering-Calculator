#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "parser.h"
#include "file_io.h"
#include "utils.h"

struct maybe_double compute_and_print(const char* formula, struct var context[], size_t* context_len) {
    if (!formula || formula[0] == '\0') {
        printf("Пустая формула\n");
        return none_double;
    }

    char input[1024];
    strncpy(input, formula, sizeof(input) - 1);
    input[sizeof(input) - 1] = '\0';

    char* tokens[MAX_TOKENS];
    size_t count = 0;
    tokenize(input, tokens, &count);

    char* rpn_out[MAX_TOKENS];
    size_t rpn_n = infix_to_rpn(tokens, count, rpn_out);

    char number_strs[MAX_TOKENS][64];
    char* final_tokens[MAX_TOKENS];

    //Обрабатываем каждый токен в RPN
    for (size_t i = 0; i < rpn_n; i++) {
        char* tok = rpn_out[i];

        if (is_operator(tok)) {
            final_tokens[i] = tok;
        }
        else if (is_function(tok)) {
            final_tokens[i] = tok;
        }
        else {
            // Проверяем, является ли токен числом
            char* end;
            strtod(tok, &end);
            if (*end == '\0' && end != tok) {
                final_tokens[i] = tok;
            }
            else {
                // Это переменная — ищем в контексте
                double known_value = 0.0;
                bool found = false;
                for (size_t j = 0; j < *context_len; j++) {
                    if (strcmp(context[j].name, tok) == 0) {
                        known_value = context[j].value;
                        found = true;
                        break;
                    }
                }

                if (found) {
                    snprintf(number_strs[i], sizeof(number_strs[i]), "%g", known_value);
                    final_tokens[i] = number_strs[i];
                } else {
                    double val;
                    printf("Введите значение переменной %s: ", tok);
                    scanf("%lf", &val);
                    clear_stdin();

                    // Сохраняем в контекст
                    if (*context_len < MAX_VARS) {
                        strncpy(context[*context_len].name, tok, sizeof(context[0].name) - 1);
                        context[*context_len].name[sizeof(context[0].name) - 1] = '\0';
                        context[*context_len].value = val;
                        (*context_len)++;
                    }

                    snprintf(number_strs[i], sizeof(number_strs[i]), "%g", val);
                    final_tokens[i] = number_strs[i];
                }
            }
        }
    }

    struct maybe_double result = evaluate_rpn(final_tokens, rpn_n);
    if (!result.valid) {
        printf("None\n");
    }
    return result;
}

struct maybe_double start_loop(char* name_file, char* name_formula, struct var context[], size_t* context_len){
		if(name_formula){
			size_t index = 0;
			if(find_formula(name_file, name_formula, &index)){
				char formula[1024];
				print_formula_by_index(name_file, index, formula, sizeof(formula));
				if (formula[0] == '\0') {
					printf("Формула не найдена\n");
					return none_double;
				}
				return compute_and_print(formula, context, context_len);
			}	
		}
		return none_double;
}

