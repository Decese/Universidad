//Funciones

#ifndef GESTION_H
#define GESTION_H

#include "estructuras.h"
//Copia el texto del archivo estructuras.h y lo pega. Se podria poner aqui directamente pero no lo hago por modulariddad

void anadirUsuario(char *datos, char *tipo);
//// anadirUsuario recibe 'datos' (una cadena ya formada con DNI;Apellido;Nombre)
// y el 'tipo' por separado para añadirlo al final.
void editarUsuario(char *datos, char *tipo);
// editarUsuario recibe los nuevos datos ya empaquetados para sustituir al viejo.
void borrarUsuario(char *dni, char *tipo);
// borrarUsuario solo necesita el DNI para buscar, no el resto de datos.

void anadirPractica(char *dniAlu, char *dia, char *hora, char *dniProf);
void borrarPractica(char *dniAlu, char *dia, char *hora, char *dniProf);
void anadirExamen(char *dniAlu, char *dia, char *hora);
void borrarExamen(char *dniAlu, char *dia, char *hora);
// Aquí recibimos los datos separados porque la función
// se encarga de ordenarlos y montar la línea del CSV.

void listarUsuarios(char *tipo, char *filtroApe, char *filtroNom);
void listarPracticas(char *filtroDia, char *filtroHora);
void listarExamenes(char *filtroDia, char *filtroHora);
// Funciones de visualización con punteros para filtros opcionales.

#endif
