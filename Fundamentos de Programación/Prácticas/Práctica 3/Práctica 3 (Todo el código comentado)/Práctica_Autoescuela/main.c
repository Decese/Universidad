#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion.h"

char* Param(int argc, char *argv[], char *etiqueta) {
    int i;
    for (i = 0; i < argc - 1; i++) {
        if (strcmp(argv[i], etiqueta) == 0) {
            return argv[i + 1];
        }
    }
    return NULL;
}
//Lo que hace es que permite que el usuario escriba los comandos en el orden que le dé la gana. Su misión es dar flexibilidad.
//argc - 1 por que no puede estar en el ultimo lo que buscamos, ej: --dia
//argv[i + 1] por que es el dato que va cono lo que queremos buscar
//Como es para los comentarios opcionales si no existe valen null

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: Faltan argumentos. Consulte el enunciado.\n");
        return 1;//Error se corta programa
    }

    char *opcion = argv[1];// La acción principal (ej: --add-alumno)

    if (strcmp(opcion, "--add-alumno") == 0) {
        if (argc >= 3) {
            anadirUsuario(argv[2], "alumno");//Ej: Perez;Juan;10/7  alumno se sustituye en tipo
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--edit-alumno") == 0) {
        if (argc >= 3) {
            editarUsuario(argv[2], "alumno");
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--delete-alumno") == 0) {
        if (argc >= 3) {
            borrarUsuario(argv[2], "alumno");
        } else {
            printf("Error. Faltan datos.\n");
        }
    }

    else if (strcmp(opcion, "--add-profesor") == 0) {
        if (argc >= 3) {
            anadirUsuario(argv[2], "profesor");
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--edit-profesor") == 0) {
        if (argc >= 3) {
            editarUsuario(argv[2], "profesor");
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--delete-profesor") == 0) {
        if (argc >= 3) {
            borrarUsuario(argv[2], "profesor");
        } else {
            printf("Error. Faltan datos.\n");
        }
    }

    else if (strcmp(opcion, "--add-practica") == 0) {
        if (argc < 3) {
            printf("Error. Faltan datos.\n");// Mínimo necesitamos el comando y el DNI del alumno
            return 1;
        }

        // El DNI del alumno es fijo (argv[2]), el resto se busca con Param
        char *dniAlumno = argv[2];
        char *dia = Param(argc, argv, "--dia");
        char *hora = Param(argc, argv, "--hora");
        char *dniProfesor = Param(argc, argv, "--profesor");

        if (dia != NULL && hora != NULL && dniProfesor != NULL) {
            anadirPractica(dniAlumno, dia, hora, dniProfesor);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--delete-practica") == 0) {
        if (argc < 3) {
            printf("Error. Faltan datos.\n");
            return 1;
        }

        char *dniAlumno = argv[2];
        char *dia = Param(argc, argv, "--dia");
        char *hora = Param(argc, argv, "--hora");
        char *dniProfesor = Param(argc, argv, "--profesor");

        if (dia != NULL && hora != NULL && dniProfesor != NULL) {
            borrarPractica(dniAlumno, dia, hora, dniProfesor);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }

    else if (strcmp(opcion, "--add-examen") == 0) {
        if (argc < 3) {
            printf("Error. Faltan datos.\n");
            return 1;
        }

        char *dniAlumno = argv[2];
        char *dia = Param(argc, argv, "--dia");
        char *hora = Param(argc, argv, "--hora");

        if (dia != NULL && hora != NULL) {
            anadirExamen(dniAlumno, dia, hora);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--delete-examen") == 0) {
        if (argc < 3) {
            printf("Error. Faltan datos.\n");
            return 1;
        }

        char *dniAlumno = argv[2];
        char *dia = Param(argc, argv, "--dia");
        char *hora = Param(argc, argv, "--hora");

        if (dia != NULL && hora != NULL) {
            borrarExamen(dniAlumno, dia, hora);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }

    else if (strcmp(opcion, "--listado-usuarios") == 0) {
        if (argc >= 3) {
            char *tipo = argv[2];
            //El tercer argumento es siempre el tipo. Es un argumento obligatorio y fijo.
            char *apellidos = Param(argc, argv, "--filter-apellidos");
            char *nombre = Param(argc, argv, "--filter-nombre");
            //Uso la función Param. Estos son argumentos opcionales.
            //Si el usuario escribió --filter-apellidos Garcia, la variable apellidos guardará "Garcia".
            //Si el usuario NO escribió nada sobre apellidos, la función Param devolverá NULL.    Le paso NULL a la función listarUsuarios para indicarle que no hay filtro y que debe mostrar a todos.

            listarUsuarios(tipo, apellidos, nombre);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--listado-practicas") == 0) {
        if (argc >= 2) {
        char *dia = Param(argc, argv, "--filter-dia");
        char *hora = Param(argc, argv, "--filter-hora");

        listarPracticas(dia, hora);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }
    else if (strcmp(opcion, "--listado-examenes") == 0) {
        if (argc >= 2) {
        char *dia = Param(argc, argv, "--filter-dia");
        char *hora = Param(argc, argv, "--filter-hora");

        listarExamenes(dia, hora);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }

    else {
        printf("Opcion no reconocida: %s\n", opcion);
    }

    return 0;
}
