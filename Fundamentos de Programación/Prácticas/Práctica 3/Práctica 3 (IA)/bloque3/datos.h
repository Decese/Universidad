//Estructuras

#ifndef DATOS_H
#define DATOS_H

#define MAX_LINEA 200
#define MAX_DNI 20
#define MAX_TEXTO 100

typedef struct {
    char dni[MAX_DNI];
    char apellidos[MAX_TEXTO];
    char nombre[MAX_TEXTO];
    char tipo[20]; // guardara "alumno" o "profesor"
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

#endif
