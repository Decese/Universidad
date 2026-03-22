#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion.h"

// Función auxiliar para buscar los parámetros (como --dia o --hora) en la lista de argumentos
char* Param(int argc, char *argv[], char *etiqueta) {
    int i;
    // Recorremos hasta el penúltimo para no salirnos del array
    for (i = 0; i < argc - 1; i++) {
        if (strcmp(argv[i], etiqueta) == 0) {
            // Si encontramos la etiqueta, devolvemos el valor que va después
            return argv[i + 1];
        }
    }
    return NULL; // Si no lo encuentra, devuelve nulo
}

int main(int argc, char *argv[]) {
    //Minimo tiene que haber dos palabras escritas al ejecutar Ej: bloque3.exe --add-alumno "datos"
    if (argc < 2) {
        printf("Error: Faltan argumentos. Consulte el enunciado.\n");
        return 1;
    }

    char *opcion = argv[1];

    // ---------------------------------------------------------
    // GESTIÓN DE ALUMNOS (Alta, Edición, Baja)
    // ---------------------------------------------------------
    if (strcmp(opcion, "--add-alumno") == 0) {
        if (argc >= 3) {
            anadirUsuario(argv[2], "alumno");
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

    // ---------------------------------------------------------
    // GESTIÓN DE PROFESORES (Alta, Edición, Baja)
    // ---------------------------------------------------------
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

    // ---------------------------------------------------------
    // GESTIÓN DE PRÁCTICAS (Alta y Baja)
    // Requiere: --dia, --hora y --profesor
    // ---------------------------------------------------------
    else if (strcmp(opcion, "--add-practica") == 0) {
        if (argc < 3) {
            printf("Error. Faltan datos.\n");
            return 1;
        }

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

        // Nota: Para borrar práctica también necesitamos saber cuál es (día/hora/profesor)
        if (dia != NULL && hora != NULL && dniProfesor != NULL) {
            borrarPractica(dniAlumno, dia, hora, dniProfesor);
        } else {
            printf("Error. Faltan datos.\n");
        }
    }

    // ---------------------------------------------------------
    // GESTIÓN DE EXÁMENES (Alta y Baja)
    // Requiere: --dia y --hora
    // ---------------------------------------------------------
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

    // ---------------------------------------------------------
    // LISTADOS (Usuarios, Prácticas, Exámenes)
    // ---------------------------------------------------------
    else if (strcmp(opcion, "--listado-usuarios") == 0) {
        if (argc >= 3) {
            char *tipo = argv[2]; // Puede ser "alumno" o "profesor"
            char *apellidos = Param(argc, argv, "--filter-apellidos");
            char *nombre = Param(argc, argv, "--filter-nombre");

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

    // Si la opción no coincide con ninguna de las anteriores
    else {
        printf("Opcion no reconocida: %s\n", opcion);
    }

    return 0;
}
