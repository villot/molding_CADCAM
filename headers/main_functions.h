#ifndef __MAIN_FUNCTIONS_H_INCLUDED__
#define __MAIN_FUNCTIONS_H_INCLUDED__

struct POS;
typedef struct POS;

struct ISLA;
typedef struct ISLA;

typedef float mm;

char* nueva_linea(unsigned int incremento = 10);

char* set_movimiento_rapido(char _debug[]);

char* set_movimiento_rapido();

char* set_movimiento_lineal();

char* mover_filo_xy(mm x, mm y);

char* mover_filo_x(mm x);

char* mover_filo_y(mm y);

char* mover_filo_z(mm z);

string inicializar_filo(unsigned int vel_filo_mm, unsigned int rpm);

string parar_filo(unsigned int vel_filo_mm, unsigned int rpm);

string generar_zigzag(mm z, mm lado_x, mm lado_y, mm filo_radio);

bool choca_isla_x(mm y, mm filo_radio, ISLA isla);

bool choca_isla_y(mm x, mm filo_radio, ISLA isla);

string generar_zigzag_con_isla(mm z, mm lado_x, mm lado_y, mm filo_radio, ISLA isla);

string saltar(mm z, mm obj_x, mm obj_y);

string generar_limpieza_de_pared(mm z, mm lado_x, mm lado_y, mm filo_radio);

string posisionar_rapido(mm z, mm x, mm y);

string generar_limpieza_de_pared_con_isla(mm z, mm lado_x, mm lado_y, mm filo_radio, ISLA isla);

string generar_vaciado(POS pos, mm lado_x, mm lado_y, mm profundidad, mm filo_util, mm filo_radio);

string generar_limpieza_de_pared_isla(mm z, mm lado_x, mm lado_y, mm filo_radio, ISLA isla);

string generar_limpieza_de_pared_isla_old(mm z, mm lado_x, mm lado_y, mm filo_radio, ISLA isla);

string generar_vaciado_con_isla(POS pos, mm lado_x, mm lado_y, mm profundidad, mm filo_util,
		mm filo_radio, POS pos_isla = gPR, mm lado_x_isla = 0, mm lado_y_isla = 0);
