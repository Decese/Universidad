#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion.h"

// Funcion sencilla para imprimir cabeceras bonitas
void Titulo(char *titulo){
    printf("* %s\n\n", titulo);
}

// ============================================================================
// BLOQUE 1: GESTIÓN DE USUARIOS (Añadir, Editar, Borrar)
// ============================================================================

void anadirUsuario(char *datos, char *tipo){
    // datos viene como "DNI; Apellidos; Nombre"
    char copia[MAX_LINEA];
    strcpy(copia, datos);

    char *dni = strtok(copia, ";");
    char *apellidos = strtok(NULL, ";");
    char *nombre = strtok(NULL, ";");

    // Limpiamos el espacio inicial si lo hay
    if (apellidos != NULL && apellidos[0] == ' ') apellidos++;
    if (nombre != NULL && nombre[0] == ' ') nombre++;

    FILE *fichero = fopen("usuarios.csv", "a");
    if (fichero == NULL){
        printf("Error al abrir usuarios.csv\n");
        return;
    }

    fprintf(fichero, "%s; %s; %s; %s\n", dni, apellidos, nombre, tipo);
    fclose(fichero);

    Titulo("AÑADIR USUARIO");
    printf("DNI: %s\nAPELLIDOS: %s\nNOMBRE: %s\n", dni, apellidos, nombre);
    printf("Se añadió con éxito\n");
}

void editarUsuario(char *datos, char *tipo){
    // Para editar, primero borramos el viejo y luego añadimos el nuevo
    char copia[MAX_LINEA];
    strcpy(copia, datos);
    char *dni = strtok(copia, ";");

    // Borramos el antiguo
    borrarUsuario(dni, tipo);

    // Añadimos el nuevo
    anadirUsuario(datos, tipo);
}

void borrarUsuario(char *dni, char *tipo){
    FILE *f = fopen("usuarios.csv", "r");
    FILE *temp = fopen("temp.csv", "w");
    char linea[MAX_LINEA];
    int encontrado = 0;
    char d[MAX_DNI], a[MAX_TEXTO], n[MAX_TEXTO];

    if (f == NULL || temp == NULL) return;

    while (fgets(linea, MAX_LINEA, f) != NULL){
        char lineaCopia[MAX_LINEA];
        strcpy(lineaCopia, linea);

        // CAMBIO: Quitamos el salto de linea de forma manual usando strlen
        int largo = strlen(lineaCopia);
        if (largo > 0 && lineaCopia[largo-1] == '\n'){
            lineaCopia[largo-1] = '\0';
        }

        char *ptrDni = strtok(lineaCopia, ";");
        char *ptrApellido = strtok(NULL, ";");
        char *ptrNombre = strtok(NULL, ";");

        if (ptrDni != NULL && strcmp(ptrDni, dni) == 0){
            encontrado = 1;
            strcpy(d, ptrDni);
            if (ptrApellido) strcpy(a, ptrApellido);
            if (ptrNombre) strcpy(n, ptrNombre);
        }
        else{
            fprintf(temp, "%s", linea);
        }
    }

    fclose(f);
    fclose(temp);

    remove("usuarios.csv");
    rename("temp.csv", "usuarios.csv");

    if (encontrado == 1){
        Titulo("ELIMINAR USUARIO");
        printf("DNI: %s\nAPELLIDOS: %s\nNOMBRE: %s\n", d, a, n);
        printf("Se eliminó con éxito\n");
    }
    else{
        printf("No se encontro el usuario con ese DNI\n");
    }
}

// ============================================================================
// BLOQUE 2: GESTIÓN DE PRÁCTICAS Y EXÁMENES (Altas y Bajas)
// ============================================================================

void anadirPractica(char *dniAlumno, char *dia, char *hora, char *dniProfesor){
    FILE *f = fopen("practicas.csv", "a");
    if (f == NULL) return;

    fprintf(f, "%s;%s;%s;%s\n", dia, hora, dniAlumno, dniProfesor);
    fclose(f);

    Titulo("AÑADIR PRÁCTICA");
    printf("ALUMNO: %s\nDIA: %s\nHORA: %s\nPROFESOR: %s\n", dniAlumno, dia, hora, dniProfesor);
    printf("Se añadió con éxito\n");
}

void borrarPractica(char *dniAlumno, char *dia, char *hora, char *dniProfesor){
    FILE *f = fopen("practicas.csv", "r");
    FILE *temp = fopen("temp_practicas.csv", "w");
    char linea[MAX_LINEA];
    int encontrado = 0;

    if (f == NULL || temp == NULL) return;

    while (fgets(linea, MAX_LINEA, f) != NULL){
        char lineaOriginal[MAX_LINEA];
        strcpy(lineaOriginal, linea);

        char copia[MAX_LINEA];
        strcpy(copia, linea);

        // CAMBIO: Quitamos el salto de linea manual
        int largo = strlen(copia);
        if (largo > 0 && copia[largo-1] == '\n'){
            copia[largo-1] = '\0';
        }

        char *d = strtok(copia, ";");
        char *h = strtok(NULL, ";");
        char *a = strtok(NULL, ";");
        char *p = strtok(NULL, ";");

        if (d && h && a && p && strcmp(d, dia) == 0 && strcmp(h, hora) == 0 && strcmp(a, dniAlumno) == 0 && strcmp(p, dniProfesor) == 0) {
            encontrado = 1;
        }
        else{
            fprintf(temp, "%s", lineaOriginal);
        }
    }

    fclose(f);
    fclose(temp);

    remove("practicas.csv");
    rename("temp_practicas.csv", "practicas.csv");

    if (encontrado) {
        Titulo("ELIMINAR PRÁCTICA");
        printf("Práctica eliminada.\n");
    } else{
        printf("No se encontró la práctica.\n");
    }
}

void anadirExamen(char *dniAlumno, char *dia, char *hora){
    FILE *f = fopen("examenes.csv", "a");
    if (f == NULL) return;

    fprintf(f, "%s;%s;%s\n", dia, hora, dniAlumno);
    fclose(f);
    printf("Examen añadido.\n");
}

void borrarExamen(char *dniAlumno, char *dia, char *hora){
    FILE *f = fopen("examenes.csv", "r");
    FILE *temp = fopen("temp_examenes.csv", "w");
    char linea[MAX_LINEA];
    int encontrado = 0;

    if (f == NULL || temp == NULL) return;

    while (fgets(linea, MAX_LINEA, f) != NULL){
        char lineaOriginal[MAX_LINEA];
        strcpy(lineaOriginal, linea);

        char copia[MAX_LINEA];
        strcpy(copia, linea);

        // CAMBIO: Quitamos el salto de linea manual
        int largo = strlen(copia);
        if (largo > 0 && copia[largo-1] == '\n'){
            copia[largo-1] = '\0';
        }

        char *d = strtok(copia, ";");
        char *h = strtok(NULL, ";");
        char *a = strtok(NULL, ";");

        if (d && h && a && strcmp(d, dia) == 0 && strcmp(h, hora) == 0 && strcmp(a, dniAlumno) == 0){
            encontrado = 1;
        }
        else{
            fprintf(temp, "%s", lineaOriginal);
        }
    }

    fclose(f);
    fclose(temp);

    remove("examenes.csv");
    rename("temp_examenes.csv", "examenes.csv");

    if (encontrado){
        Titulo("ELIMINAR EXAMEN");
        printf("Examen eliminado.\n");
    }
    else{
        printf("No se encontró el examen.\n");
    }
}

// ============================================================================
// BLOQUE 3: LISTADOS (Usuarios, Prácticas, Exámenes)
// ============================================================================

void listarUsuarios(char *tipo, char *filtrarApellido, char *filtrarNombre) {
    FILE *f = fopen("usuarios.csv", "r");
    char linea[MAX_LINEA];

    if (f == NULL){
        printf("No hay fichero de usuarios.\n");
        return;
    }

    Titulo("LISTADO USUARIOS");

    while (fgets(linea, MAX_LINEA, f) != NULL){
        // CAMBIO: Quitamos el salto de linea manual
        int largo = strlen(linea);
        if (largo > 0 && linea[largo-1] == '\n'){
            linea[largo-1] = '\0';
        }

        char copia[MAX_LINEA];
        strcpy(copia, linea);

        char *dni = strtok(copia, ";");
        char *ape = strtok(NULL, ";");
        char *nom = strtok(NULL, ";");
        char *tip = strtok(NULL, ";");

        if (dni && ape && nom && tip){
            if (ape[0] == ' ') ape++;
            if (nom[0] == ' ') nom++;
            if (tip[0] == ' ') tip++;

            if (strcmp(tip, tipo) == 0){
                int mostrar = 1;
                if (filtrarApellido != NULL && strstr(ape, filtrarApellido) == NULL) mostrar = 0;
                if (filtrarNombre != NULL && strstr(nom, filtrarNombre) == NULL) mostrar = 0;

                if (mostrar == 1){
                    printf("%s\t%s\t%s\n", dni, ape, nom);
                }
            }
        }
    }
    fclose(f);
}

void listarPracticas(char *filtrarDia, char *filtrarHora){
    FILE *f = fopen("practicas.csv", "r");
    char linea[MAX_LINEA];

    if (f == NULL) return;

    Titulo("LISTADO PRÁCTICAS");

    while (fgets(linea, MAX_LINEA, f) != NULL){
        // CAMBIO: Quitamos el salto de linea manual
        int largo = strlen(linea);
        if (largo > 0 && linea[largo-1] == '\n'){
            linea[largo-1] = '\0';
        }

        char copia[MAX_LINEA];
        strcpy(copia, linea);

        char *d = strtok(copia, ";");
        char *h = strtok(NULL, ";");
        char *alu = strtok(NULL, ";");
        char *prof = strtok(NULL, ";");

        if (d && h && alu){
            int mostrar = 1;
            if (filtrarDia != NULL && strcmp(d, filtrarDia) != 0) mostrar = 0;
            if (filtrarHora != NULL && strcmp(h, filtrarHora) != 0) mostrar = 0;

            if (mostrar == 1){
                printf("%s %s\n%s\nP: %s\n\n", d, h, alu, prof ? prof : "N/A");
            }
        }
    }
    fclose(f);
}

void listarExamenes(char *filtrarDia, char *filtrarHora){
    FILE *f = fopen("examenes.csv", "r");
    char linea[MAX_LINEA];

    if (f == NULL){
        printf("No hay fichero de examenes.\n");
        return;
    }

    Titulo("LISTADO EXÁMENES");

    while (fgets(linea, MAX_LINEA, f) != NULL){
        // CAMBIO: Quitamos el salto de linea manual
        int largo = strlen(linea);
        if (largo > 0 && linea[largo-1] == '\n'){
            linea[largo-1] = '\0';
        }

        char copia[MAX_LINEA];
        strcpy(copia, linea);

        char *d = strtok(copia, ";");
        char *h = strtok(NULL, ";");
        char *alu = strtok(NULL, ";");

        if (d && h && alu){
            int mostrar = 1;
            if (filtrarDia != NULL && strcmp(d, filtrarDia) != 0) mostrar = 0;
            if (filtrarHora != NULL && strcmp(h, filtrarHora) != 0) mostrar = 0;

            if (mostrar == 1){
                printf("%s\n%s\n%s\n\n", d, h, alu);
            }
        }
    }
    fclose(f);
}
