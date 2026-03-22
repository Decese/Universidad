#ifndef GESTION_H
#define GESTION_H

#include "datos.h"

// Funciones para Usuarios
void anadirUsuario(char *datos, char *tipo);
void editarUsuario(char *datos, char *tipo);
void borrarUsuario(char *dni, char *tipo);

// Funciones para Practicas y Examenes
void anadirPractica(char *dniAlu, char *dia, char *hora, char *dniProf);
void borrarPractica(char *dniAlu, char *dia, char *hora, char *dniProf);
void anadirExamen(char *dniAlu, char *dia, char *hora);
void borrarExamen(char *dniAlu, char *dia, char *hora);

// Funciones para Listados
void listarUsuarios(char *tipo, char *filtroApe, char *filtroNom);
void listarPracticas(char *filtroDia, char *filtroHora);
void listarExamenes(char *filtroDia, char *filtroHora);

#endif
