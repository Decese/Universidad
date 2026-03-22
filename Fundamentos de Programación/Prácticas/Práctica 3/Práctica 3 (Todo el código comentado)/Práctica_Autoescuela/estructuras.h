//Estructuras

#ifndef ESTRUCTURAS_H   //Si NO está definido...
#define ESTRUCTURAS_H   //Defínelo ahora
//Sirven para evitar la doble inclusión de este archivo y que se procese una sola vez.

#define MAX_LINEA 200
#define MAX_DNI 20
#define MAX_TEXTO 100

typedef struct {
    char dni[MAX_DNI];
    char apellidos[MAX_TEXTO];
    char nombre[MAX_TEXTO];
    char tipo[20];
} Usuario;

typedef struct {
    char fecha[15];
    char hora[10];
    char dniAlumno[MAX_DNI];
    char dniProfesor[MAX_DNI];
} Practica;

typedef struct {
    char fecha[15];
    char hora[10];
    char dniAlumno[MAX_DNI];
} Examen;

#endif  //Fin del If

/*He incluido el archivo de estructuras principalmente por dos razones:
-Por las Constantes, para centralizar los tamaños fijos como MAX_LINEA y MAX_DNI en un solo sitio.
-Por Diseño y Escalabilidad: Aunque en las funciones de añadir o borrar trabajo directamente con texto (char*), he dejado las estructuras definidas para documentar
cómo es el modelo de datos.
*/
