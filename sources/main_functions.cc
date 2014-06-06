#include <cstdio>
#include <iostream>
#include <string>
#include "main_functions.h"

using namespace std;

//default x = 0, y = 0, z = 0
typedef struct POS {
	float x = 0;
	float y = 0;
	float z = 0;
} POS;

typedef struct ISLA {
	POS pr;
	float x_superiror = 0;
	float x_inferiror = 0;
	float y_superiror = 0;
	float y_inferiror = 0;
} ISLA;

typedef float mm;
ISLA gISLA;
unsigned int gLINEA = 0;
int gALTURA_SEGURA = 0;
POS gPR; //posicion referencial

char* nueva_linea(unsigned int incremento = 10) {
	unsigned int nueva_linea_num = gLINEA + incremento;
	static char iso[10];
	snprintf(iso, 10, "N%u", nueva_linea_num);
	gLINEA = nueva_linea_num;
	/*	if(gLINEA == 150){
	 int i =0;
	 i++;
	 }*/
	return iso;
}

//Generar un comentario para codigo ISO - Para DEBUG
//return: codigo ISO en formato char[]
char* set_movimiento_rapido(char _debug[]) {
	int size_iso = 100;
	static char iso[100];
	//editar precision aqui:  entero - X%.0f Y%.0f  ,  2 digito float- X%.2f Y%.2f
	snprintf(iso, size_iso, "%s (%s)\n", nueva_linea(), _debug);
	return iso;
}

//Generar codigo ISO que mueve filo en horizontal (x, y)
//input: original - posicion actual del filo
//       final - posicion final del filo (solo tiene en cuenta x, y)
//return: codigo ISO en formato char[]
char* set_movimiento_rapido() {
	int size_iso = 100;
	static char iso[100];
	//editar precision aqui:  entero - X%.0f Y%.0f  ,  2 digito float- X%.2f Y%.2f
	snprintf(iso, size_iso, "%s G00\n", nueva_linea());
	return iso;
}

char* set_movimiento_lineal() {
	int size_iso = 100;
	static char iso[100];
	//editar precision aqui:  entero - X%.0f Y%.0f  ,  2 digito float- X%.2f Y%.2f
	snprintf(iso, size_iso, "%s G01\n", nueva_linea());
	return iso;
}

//Generar codigo ISO que mueve filo en horizontal (x, y)
//input: original - posicion actual del filo
//       final - posicion final del filo (solo tiene en cuenta x, y)
//return: codigo ISO en formato char[]
char* mover_filo_xy(mm x, mm y) {
	int size_iso = 100;
	static char iso[100];
	//editar precision aqui:  entero - X%.0f Y%.0f  ,  2 digito float- X%.2f Y%.2f
	snprintf(iso, size_iso, "%s X%.3f Y%.3f\n", nueva_linea(), x + gPR.x,
			y + gPR.y);
	return iso;
}

//Generar codigo ISO que mueve filo en eje X
//input: original - posicion actual del filo
//	     final - posicion final del filo (solo tiene en cuenta X)
//return:  codigo ISO en formato char[]
char* mover_filo_x(mm x) {
	int size_iso = 100;
	static char iso[100];
	//editar precision aqui:  entero - X%.0f Y%.0f  ,  2 digito float- X%.2f Y%.2f
	snprintf(iso, size_iso, "%s X%.3f\n", nueva_linea(), x + gPR.x);
	return iso;
}

//Generar codigo ISO que mueve filo en eje Y
//input: original - posicion actual del filo
//       final - posicion final del filo (solo tiene en cuenta Y)
//return:  codigo ISO en formato char[]
char* mover_filo_y(mm y) {
	int size_iso = 100;
	static char iso[100];
	//editar precision aqui:  entero - X%.0f Y%.0f  ,  2 digito float- X%.2f Y%.2f
	snprintf(iso, size_iso, "%s Y%.3f\n", nueva_linea(), y + gPR.y);
	return iso;
}

//Generar codigo ISO que mueve filo en eje Z
//input: original - posicion actual del filo
//       final - posicion final del filo (solo tiene en cuenta Z)
//return: codigo ISO en formato char[]
char* mover_filo_z(mm z) {
	int size_iso = 100;
	static char iso[100];
	//editar precision aqui:  entero - X%.0f Y%.0f  ,  2 digito float- X%.2f Y%.2f
	snprintf(iso, size_iso, "%s Z%.3f\n", nueva_linea(), z + gPR.z);
	return iso;
}

//Generar codigo ISO para inicializar las parametros de filo
// - plano xy
// - coordenada absoluta
// - vel_filo_mm: velocidad de avance, mm/min
// - rpm: velocidad de rotacion del filo , rpm
// - unidad de dimension, milimetro
//input : - vel_filo_mm: velocidad de avance, mm/min
//        - rpm: velocidad de rotacion del filo , rpm
//return: codigo ISO generado para inicializar
string inicializar_filo(unsigned int vel_filo_mm, unsigned int rpm) {
	string iso;
	int size_iso_aux = 100;
	static char iso_aux[100];
	snprintf(iso_aux, size_iso_aux, "%s G90 G01 F%d S%d\n", nueva_linea(),
			vel_filo_mm, rpm);
	iso.append(iso_aux);
	//iso.append("%s G00 G90 F%d M3 S%d\n" , nueva_linea(), vel_filo_mm, rpm);
	//iso.append(set_movimiento_lineal());
	return iso;
}

string parar_filo(unsigned int vel_filo_mm, unsigned int rpm) {
	string iso;
	int size_iso_aux = 100;
	static char iso_aux[100];
	snprintf(iso_aux, size_iso_aux, "%s G90 G01 F%d S%d\n", nueva_linea(),
			vel_filo_mm, rpm);
	iso.append(iso_aux);
	//iso.append("%s G00 G90 F%d M3 S%d\n" , nueva_linea(), vel_filo_mm, rpm);
	//iso.append(set_movimiento_lineal());
	return iso;
}

string generar_zigzag(mm z, mm lado_x, mm lado_y, mm filo_radio) {
	string iso;
	bool volver = true;
	mm y;

	iso.append(set_movimiento_rapido());
	//iso.append(mover_filo_z(gALTURA_SEGURA));
	iso.append(mover_filo_xy(filo_radio, lado_y - filo_radio));
	iso.append(set_movimiento_lineal());
	iso.append(mover_filo_z(z));
	//Algoritmo Zigzag
	for (y = (lado_y - filo_radio); y >= filo_radio * 3; y -=
			(filo_radio * 2)) {
		iso.append(mover_filo_y(y));       //mover_filo_y(y);

		if (volver == true) {
			iso.append(mover_filo_x(lado_x - filo_radio)); //mover_filo_x(lado_y - filo_radio);
			volver = false;
		} else {
			iso.append(mover_filo_x(filo_radio));    //mover_filo_x(filo_radio);
			volver = true;
		}

	}
	if (y > (filo_radio)) {
		iso.append(mover_filo_y(filo_radio)); //mover_filo_y(lado_y - filo_radio);
		if (volver == true) {
			iso.append(mover_filo_x(lado_x - filo_radio)); //mover_filo_x(lado_y - filo_radio);
			volver = false;
		} else {
			iso.append(mover_filo_x(filo_radio));    //mover_filo_x(filo_radio);
			volver = true;
		}
	}
	iso.append(mover_filo_z(gALTURA_SEGURA));
	return iso;
}

bool choca_isla_x(mm y, mm filo_radio, ISLA isla) { //comprueba el avance  en el eje y choca con la isla
	int aux_y_superiror = y - filo_radio;
	int aux_y_inferiror = y + filo_radio;
	if ((aux_y_superiror < isla.y_superiror)
			&& (aux_y_inferiror > isla.y_inferiror)) {
		return true;
	} else {
		return false;
	}
}

bool choca_isla_y(mm x, mm filo_radio, ISLA isla) { //comprueba el avance en el eje y choca con la isla
	int aux_x_superiror = x - filo_radio;
	int aux_x_inferiror = x + filo_radio;
	if ((aux_x_superiror < isla.x_superiror)
			&& (aux_x_inferiror > isla.x_inferiror)) {
		return true;
	} else {
		return false;
	}
}

string generar_zigzag_con_isla(mm z, mm lado_x, mm lado_y, mm filo_radio,
		ISLA isla) { // z = posicion actual del filo.
	string iso;
	bool volver = true;
	mm y;
	iso.append(set_movimiento_rapido());
	//iso.append(mover_filo_z(gALTURA_SEGURA));
	iso.append(mover_filo_xy(filo_radio, lado_y - filo_radio));
	iso.append(set_movimiento_lineal());
	iso.append(mover_filo_z(z));
	//Algoritmo Zigzag
	int choque = 0;
	for (y = (lado_y - filo_radio); y >= filo_radio * 3; y -=
			(filo_radio * 2)) {
		if (choca_isla_x(y, filo_radio, gISLA)) {
			//iso.append("DEBUG chocado"); //DEBUG
			if (choque == 0) {
				if ((y + filo_radio) > (gISLA.y_superiror + filo_radio))
					y = gISLA.y_superiror + filo_radio;
			}
			choque = 1; //primer choque con la isla
			if (volver == true) {
				if (isla.x_inferiror > (filo_radio * 2)) { //comprueba si cabe el filo o no
					iso.append(mover_filo_y(y));       //mover_filo_y(y);
					iso.append(mover_filo_x(isla.x_inferiror - filo_radio));
				}
				if ((isla.x_superiror + filo_radio) < (lado_x - filo_radio)) { //comprueba si cabe el filo o no
					iso.append(mover_filo_z(gALTURA_SEGURA)); //subir a la z con una distancia segura respecto al modelo;
					//iso.append(mover_filo_x( isla.x_superiror + filo_radio));
					iso.append(set_movimiento_rapido());
					iso.append(
							mover_filo_xy((isla.x_superiror + filo_radio), y));
					iso.append(set_movimiento_lineal());
					iso.append(mover_filo_z(z)); //bajar a la z para volver a filar;
					iso.append(mover_filo_x(lado_x - filo_radio));
				}
				volver = false;
			} else {
				if ((isla.x_superiror + filo_radio) < (lado_x - filo_radio)) { //comprueba si cabe el filo o no
					iso.append(mover_filo_y(y));       //mover_filo_y(y);
					iso.append(mover_filo_x(isla.x_superiror + filo_radio));
				}
				if (isla.x_inferiror > (filo_radio * 2)) { //comprueba si cabe el filo o no
					iso.append(mover_filo_z(gALTURA_SEGURA)); //subir a la z con una distancia segura respecto al modelo;
					iso.append(set_movimiento_rapido());
					iso.append(
							mover_filo_xy((isla.x_inferiror - filo_radio), y));
					iso.append(set_movimiento_lineal());
					iso.append(mover_filo_z(z)); //bajar a la z para volver a filar;
					iso.append(mover_filo_x(filo_radio));
				}
				volver = true;
			}
		} else {
			if (choque == 1) {    //primera linea despues de la isla
				y = gISLA.y_inferiror - filo_radio;
				choque = 2; //fin de choque con la isla
			}
			iso.append(mover_filo_y(y));       //mover_filo_y(y);

			if (volver == true) {
				iso.append(mover_filo_x(lado_x - filo_radio)); //mover_filo_x(lado_y - filo_radio);
				volver = false;
			} else {
				iso.append(mover_filo_x(filo_radio)); //mover_filo_x(filo_radio);
				volver = true;
			}
		}
	}
	if (y > filo_radio) {
		iso.append(mover_filo_y(filo_radio)); //mover_filo_y(lado_y - filo_radio);
		if (volver == true) {
			iso.append(mover_filo_x(lado_x - filo_radio)); //mover_filo_x(lado_y - filo_radio);
			volver = false;
		} else {
			iso.append(mover_filo_x(filo_radio));    //mover_filo_x(filo_radio);
			volver = true;
		}
	}
	iso.append(mover_filo_z(gALTURA_SEGURA));
	return iso;
}

//saltar desde posicion acutal a posicion obj x y
string saltar(mm z, mm obj_x, mm obj_y) {
	string iso;
	iso.append(mover_filo_z(gALTURA_SEGURA));
	iso.append(set_movimiento_rapido());
	iso.append(mover_filo_xy(obj_x, obj_y));
	iso.append(set_movimiento_lineal());
	iso.append(mover_filo_z(z));
	return iso;
}

string generar_limpieza_de_pared(mm z, mm lado_x, mm lado_y, mm filo_radio) { //z = profundidad que esta afilando
	string iso;
	//limpieza de pared
	//iso.append("DEBUG: Limpieza de pared\n");

	//inicializar la posicion
	iso.append(set_movimiento_rapido());
	//iso.append(mover_filo_z(gALTURA_SEGURA));
	iso.append(mover_filo_xy(filo_radio, lado_y - filo_radio));
	iso.append(set_movimiento_lineal());
	iso.append(mover_filo_z(z));

	//empieza a vaciar
	iso.append(mover_filo_x(lado_x - filo_radio));
	iso.append(mover_filo_y(filo_radio));
	iso.append(mover_filo_x(filo_radio));
	iso.append(mover_filo_y(lado_y - filo_radio));
	iso.append(mover_filo_z(gALTURA_SEGURA)); //volver a la altura segura.
	return iso;
}

string posisionar_rapido(mm z, mm x, mm y) {
	string iso;
	iso.append(set_movimiento_rapido());
	iso.append(mover_filo_xy(x, y));
	iso.append(set_movimiento_lineal());
	iso.append(mover_filo_z(z));
	return iso;
}

string generar_limpieza_de_pared_con_isla(mm z, mm lado_x, mm lado_y,
		mm filo_radio, ISLA isla) { //z = profundidad que esta filando
	string iso;
	//Limpieza de pared
	//iso.append("DEBUG: Limpieza de pared\n");

	//inicializar la posicion
	//iso.append(mover_filo_z(gALTURA_SEGURA));

	bool fin_previouse = false;
	//bool fin_y = false;
	//empieza a vaciar pared
	//  1  ----->  2
	//  |          |
	//  |          |
	//  4  <-----  3

	//de 1 a 2
	if (choca_isla_x(lado_y - filo_radio, filo_radio, isla)) { //hay una isla entre 1 y 2
		if (isla.x_inferiror > (filo_radio * 2)) { //cabe entre isla y punto 1
			//cabe en el punto 1
			iso.append(posisionar_rapido(z, filo_radio, lado_y - filo_radio));
			iso.append(mover_filo_x(isla.x_inferiror - filo_radio));
		}
		if (isla.x_superiror < (lado_x - filo_radio * 2)) { //cabe entre isla y posicion 2
			iso.append(
					posisionar_rapido(z, isla.x_superiror + filo_radio,
							lado_y - filo_radio));
			iso.append(mover_filo_x(lado_x - filo_radio));
			fin_previouse = true;
		}
	} else {
		//No hay isla entre 1 y 2
		iso.append(mover_filo_x(lado_x - filo_radio));
		fin_previouse = true;
	}
	//de 2 a 3
	if (choca_isla_y(lado_x - filo_radio, filo_radio, isla)) { //hay una isla entre 2 y 3
		if (isla.y_superiror < (lado_y - filo_radio * 2)) { //cabe entre isla y punto 2
			iso.append(mover_filo_y(isla.y_superiror + filo_radio));
		}
		if (isla.y_inferiror > (filo_radio * 2)) {
			iso.append(
					posisionar_rapido(z, lado_x - filo_radio,
							isla.y_inferiror - filo_radio));
			iso.append(mover_filo_y(filo_radio));
		}
	} else {
		//No hay isla entre 2 y 3
		iso.append(mover_filo_y(filo_radio));
	}
	//de 3 a 4 en eje x
	if (choca_isla_x(filo_radio, filo_radio, isla)) { //hay una isla entre 3 y 4
		if (isla.x_superiror < (lado_x - filo_radio * 2)) {
			iso.append(mover_filo_x(isla.x_superiror + filo_radio));
		}
		if (isla.x_inferiror > (filo_radio * 2)) {
			iso.append(
					posisionar_rapido(z, isla.x_inferiror - filo_radio,
							filo_radio));
			iso.append(mover_filo_x(filo_radio));
		}
	} else {
		//No hay isla entre 3 y 4
		iso.append(mover_filo_x(filo_radio));
	}
	//de 4 a 1 en eje y
	if (choca_isla_y(filo_radio, filo_radio, isla)) { //hay una isla entre 4 y 1
		if (isla.y_inferiror > (filo_radio * 2)) { //cabe entre la isla y en punto 4
			iso.append(mover_filo_y(isla.y_inferiror - filo_radio));
		}
		if (isla.y_superiror < (lado_y - filo_radio * 2)) {
			iso.append(
					posisionar_rapido(z, filo_radio,
							isla.y_superiror + filo_radio));
			iso.append(mover_filo_y(lado_y - filo_radio));
		}
	} else {
		//No hay isla entre 4 y 1
		iso.append(mover_filo_y(lado_y - filo_radio));
	}

	return iso;
}

string generar_vaciado(POS pos, mm lado_x, mm lado_y, mm profundidad,
		mm filo_util, mm filo_radio) {
	static string iso;
	mm z;

	iso.append(inicializar_filo(100, 2500));
	//Avance con profundidad filo_util
	//iso.append("DEBUG: Vaciado\n");
	for (z = -filo_util; z > -profundidad; z -= filo_util) {
		iso.append(generar_limpieza_de_pared(z, lado_x, lado_y, filo_radio));
		iso.append(generar_zigzag(z, lado_x, lado_y, filo_radio));
	}
	if (z != profundidad) {
		z = profundidad;
		iso.append(generar_limpieza_de_pared(z, lado_x, lado_y, filo_radio));
		iso.append(generar_zigzag(z, lado_x, lado_y, filo_radio));
	}
	return iso;
}
string generar_limpieza_de_pared_isla(mm z, mm lado_x, mm lado_y, mm filo_radio,
		ISLA isla) { //z = profundidad que esta filando
	string iso;
	//Limpieza de pared
	//iso.append("DEBUG: Limpieza de pared\n");

	//inicializar la posicion
	//iso.append(mover_filo_z(gALTURA_SEGURA));

	bool fin_previouse = false;
	//bool fin_y = false;
	//empieza a vaciar pared
	//  1  ----->  2
	//  |   isla   |
	//  |          |
	//  4  <-----  3

	//pared 1 - 2
	if (isla.y_superiror < (lado_y - filo_radio * 2)) { //cabe en el punto 1
		//iso.append(mover_filo_y(isla.y_superiror + filo_radio));
		if (isla.x_inferiror > filo_radio) {    //from punto 1
			iso.append(
					posisionar_rapido(z, isla.x_inferiror,
							isla.y_superiror + filo_radio));
		} else {
			iso.append(
					posisionar_rapido(z, filo_radio,
							isla.y_superiror + filo_radio));
		}
		if (isla.x_superiror < filo_radio) {    //to punto 2
			iso.append(mover_filo_x(isla.x_superiror));
		} else {
			iso.append(mover_filo_x(lado_x - filo_radio));
		}
	}

	//pared 2 - 3
	if (isla.x_superiror < (lado_x - filo_radio * 2)) { //cabe en el punto 2
		if (isla.y_superiror < (lado_y - filo_radio)) { //from punto 2
			posisionar_rapido(z, isla.x_superiror + filo_radio,
					isla.y_superiror);
		} else {
			posisionar_rapido(z, lado_x - filo_radio, isla.y_superiror);
		}
		if (isla.y_inferiror > filo_radio) {
			iso.append(mover_filo_y(isla.y_inferiror));
		} else {
			iso.append(mover_filo_y(filo_radio));
		}
	}

	//de 1 a 2
	if (choca_isla_x(lado_y - filo_radio, filo_radio, isla)) { //hay una isla entre 1 y 2
		if (isla.x_inferiror > (filo_radio * 2)) { //cabe entre isla y punto 1
			//cabe en el punto 1
			iso.append(posisionar_rapido(z, filo_radio, lado_y - filo_radio));
			iso.append(mover_filo_x(isla.x_inferiror - filo_radio));
		}
		if (isla.x_superiror < (lado_x - filo_radio * 2)) { //cabe entre isla y posicion 2
			iso.append(
					posisionar_rapido(z, isla.x_superiror + filo_radio,
							lado_y - filo_radio));
			iso.append(mover_filo_x(lado_x - filo_radio));
			fin_previouse = true;
		}
	} else {
		//No hay isla entre 1 y 2
		iso.append(mover_filo_x(lado_x - filo_radio));
		fin_previouse = true;
	}
	//de 2 a 3
	if (choca_isla_y(lado_x - filo_radio, filo_radio, isla)) { //hay una isla entre 2 y 3
		if (isla.y_superiror < (lado_y - filo_radio * 2)) { //cabe entre isla y punto 2
			iso.append(mover_filo_y(isla.y_superiror + filo_radio));
		}
		if (isla.y_inferiror > (filo_radio * 2)) {
			iso.append(
					posisionar_rapido(z, lado_x - filo_radio,
							isla.y_inferiror - filo_radio));
			iso.append(mover_filo_y(filo_radio));
		}
	} else {
		//No hay isla entre 2 y 3
		iso.append(mover_filo_y(filo_radio));
	}

	//de 3 a 4 en eje x
	if (choca_isla_x(filo_radio, filo_radio, isla)) { //hay una isla entre 3 y 4
		if (isla.x_superiror < (lado_x - filo_radio * 2)) {
			iso.append(mover_filo_x(isla.x_superiror + filo_radio));
		}
		if (isla.x_inferiror > (filo_radio * 2)) {
			iso.append(
					posisionar_rapido(z, isla.x_inferiror - filo_radio,
							filo_radio));
			iso.append(mover_filo_x(filo_radio));
		}
	} else {
		//No hay isla entre 3 y 4
		iso.append(mover_filo_x(filo_radio));
	}
	//de 4 a 1 en eje y
	if (choca_isla_y(filo_radio, filo_radio, isla)) { //hay una isla entre 4 y 1
		if (isla.y_inferiror > (filo_radio * 2)) { //cabe entre la isla y en punto 4
			iso.append(mover_filo_y(isla.y_inferiror - filo_radio));
		}
		if (isla.y_superiror < (lado_y - filo_radio * 2)) {
			iso.append(
					posisionar_rapido(z, filo_radio,
							isla.y_superiror + filo_radio));
			iso.append(mover_filo_y(lado_y - filo_radio));
		}
	} else {
		//No hay isla entre 4 y 1
		iso.append(mover_filo_y(lado_y - filo_radio));
	}

	return iso;
}

string generar_limpieza_de_pared_isla_old(mm z, mm lado_x, mm lado_y,
		mm filo_radio, ISLA isla) {
	string iso;
	//Limpieza de pared
	//iso.append("DEBUG: Limpieza de pared\n");

	iso.append(set_movimiento_rapido());
	iso.append(mover_filo_z(gALTURA_SEGURA));
	if (isla.x_inferiror > (filo_radio * 2)
			&& isla.x_superiror < (lado_x - filo_radio)) {
		int i = 0;
	}
	iso.append(mover_filo_xy(filo_radio, lado_y - filo_radio));
	iso.append(set_movimiento_lineal());
	iso.append(mover_filo_z(z));

	//iso.append(mover_filo_xy(filo_radio, lado_y - filo_radio));
	iso.append(set_movimiento_lineal());
	iso.append(mover_filo_x(lado_x - filo_radio));
	iso.append(mover_filo_y(filo_radio));
	iso.append(mover_filo_x(filo_radio));
	iso.append(mover_filo_y(lado_y - filo_radio));

	iso.append(mover_filo_z(gALTURA_SEGURA)); //volver a la altura segura.
	return iso;
}

string generar_vaciado_con_isla(POS pos, mm lado_x, mm lado_y, mm profundidad,
		mm filo_util, mm filo_radio, POS pos_isla = gPR, mm lado_x_isla = 0,
		mm lado_y_isla = 0) {
	int size_iso = 10000;
	//static char iso[10000];
	static string iso;
	//for()
	//mm x;

	gPR.x = pos.x;
	gPR.y = pos.y;

	mm z;

	gISLA.pr = pos;
	gISLA.x_superiror = pos_isla.x + lado_x_isla - gPR.x;
	gISLA.x_inferiror = pos_isla.x - gPR.x;
	gISLA.y_superiror = pos_isla.y + lado_y_isla - gPR.x;
	gISLA.y_inferiror = pos_isla.y - gPR.x;

	gALTURA_SEGURA = filo_util * 2;

	iso.append(inicializar_filo(100, 2500));

	iso.append(mover_filo_z(gALTURA_SEGURA));
	iso.append(set_movimiento_rapido());
	iso.append(mover_filo_xy(filo_radio, lado_y - filo_radio));
	//Avance con profundidad filo_util
	//iso.append("DEBUG: Vaciado\n");
	for (z = -filo_util; z >= -profundidad; z -= filo_util) {
		iso.append(
				generar_limpieza_de_pared_con_isla(z, lado_x, lado_y,
						filo_radio, gISLA));
		iso.append(
				generar_zigzag_con_isla(z, lado_x, lado_y, filo_radio, gISLA));
		//iso.append(generar_limpieza_de_pared_isla(z, lado_x, lado_y, filo_radio, gISLA));
		//iso.append(mover_filo_z(filo_util*2));       //mover_filo_y(z);
		//int i = 0;
	}
	if (z > -profundidad) {
		z = profundidad;
		iso.append(
				generar_limpieza_de_pared_con_isla(z, lado_x, lado_y,
						filo_radio, gISLA));
		iso.append(
				generar_zigzag_con_isla(z, lado_x, lado_y, filo_radio, gISLA));
		//iso.append(generar_limpieza_de_pared_isla(z, lado_x, lado_y, filo_radio, gISLA));
	}
	return iso;
}
int main(int argc, const char * argv[]) //for mover_filo_xy debug
		{
	POS a, b;
	a.x = 0;
	a.y = 0;
	a.z = 0;
	b.x = 0;
	b.y = 100;
	b.z = 1;

/*	 std::cout << " a  " << a.x << "  "<< a.y <<  "  "<<a.z << std::endl;
	 std::cout << " b  " << b.x << "  "<< b.y <<  "  "<<b.z << std::endl;
	 // insert code here...
	 std::cout << mover_filo_xy(a) << std::endl;
	 std::cout << mover_filo_xy(b) << std::endl;
	 std::cout << mover_filo_x(b) << std::endl;
	 std::cout << mover_filo_y(b) << std::endl;
	 std::cout << mover_filo_z(b) << std::endl;
	 std::cout << mover_filo_x(100) << std::endl;
	 std::cout << mover_filo_y(102) << std::endl;
	 std::cout << mover_filo_z(104) << std::endl;*/

	int size_iso = 10000;
	static string iso;
	//iso.append(mover_filo_y(100)).append(mover_filo_x(1000));
	//snprintf(iso, size_iso, "%s\n", mover_filo_x(50)); //mover_filo_x(filo_radio);

	//snprintf(iso, size_iso, "%s\n", mover_filo_z(100)); //mover_filo_x(filo_radio);
	//iso.append(set_movimiento_rapido("Inicializa filo"));
	//iso.append(generar_vaciado(b, 200, 200, 10, 3, 2));mm pos_isla, mm lado_x_isla, mm lado_y_isla
	//sin isla
	//iso.append(generar_vaciado_con_isla(a, 200, 200, 3, 3, 2)); //, b, 0, 0));

	//con isla
	iso.append(generar_vaciado_con_isla(a, 200, 200, 3, 3, 2, b, 100, 100));
	iso.append("\n");
	std::cout << iso << std::endl;
	return 0;
}
