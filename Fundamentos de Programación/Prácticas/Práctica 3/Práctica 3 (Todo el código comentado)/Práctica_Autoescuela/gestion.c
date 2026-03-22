#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion.h"

void Titulo(char *titulo){
    printf("***********\n");
    printf("* %s\n", titulo);
    printf("***********\n\n");
}

//Usuarios
void anadirUsuario(char *datos, char *tipo){
    /*void anadirUsuario(char *datos, char *tipo)    Es la cabecera de la función.
    char *datos: Es un puntero a una cadena de texto. Lo que escribiste en el comando (ej: "11111111A;Perez;Juan").
    char *tipo: Es otra cadena de texto que nos dice el TIPO (ej: "alumno" o "profesor")
    */
    char copia[MAX_LINEA];
    /*
    copia: Es el nombre de este array.
    MAX_LINEA: Es una constante (un número fijo) que definiste en el archivo estructuras.h. Normalmente vale 200.
    */
    strcpy(copia, datos);
    /*Qué hace: Coge lo que hay en datos (el original) y lo duplica letra por letra dentro de tu caja nueva copia.
    "¿Qué es strcpy?"
    Respuesta: "Es una función de la librería <string.h> que copia el contenido de una cadena origen a una cadena destino."

    "¿Para qué copio los datos? ¿Por qué no uso datos directamente?"
    Más abajo se usa la función strtok.
    strtok es como una tijera: rompe la cadena original. Donde ve un ;, lo borra y pone un fin de cadena (\0).
    Al hacer una copia, dejas que strtok destroce la copia, mientras el original datos se queda intacto por si hiciera falta después.
    */

    char *dni = strtok(copia, ";");
    char *apellidos = strtok(NULL, ";");
    char *nombre = strtok(NULL, ";");
    /*Qué hace: La función strtok sirve para trocear una frase. Busca el separador que le digas (;) y "corta" la frase ahí.
    La primera línea: strtok(copia, ";") le dice: "Empieza a cortar la variable copia buscando ;". Devuelve un puntero al primer trozo (el DNI).
    Las líneas siguientes: strtok(NULL, ";"). ¡Ojo aquí! Le pasamos NULL para decirle: "No empieces de nuevo, sigue cortando la misma frase donde te quedaste antes".

    Pregunta 1 (Clásica): "En la segunda y tercera llamada a strtok, ¿por qué pones NULL en vez de copia?"
    Respuesta: "Porque strtok tiene memoria interna. Si pongo NULL, la función sabe que tiene que continuar troceando la cadena anterior desde donde se quedó.
    Si pusiera copia otra vez, volvería a empezar desde el principio y me devolvería el DNI tres veces."
    */

    if (dni == NULL) dni = "Sin DNI";
    if (apellidos == NULL) apellidos = "Sin Apellidos";
    if (nombre == NULL) nombre = "Sin Nombre";
    /*Qué hace: Protege tu programa de errores humanos.
    Si se olvida de poner una variable, no explota el programa, devuelve ese comentario.
    */

    if (apellidos[0] == ' ') apellidos++;
    if (nombre[0] == ' ') nombre++;
    /*El problema: Cuando escribes "1111A; Perez", después del punto y coma hay un espacio. strtok corta justo en el ;, por lo que el apellido se guardaría como " Perez" (con el espacio delante).
    apellidos[0] == ' ': Miramos si la letra a la que señala el dedo es un espacio.
    apellidos++: Si es así, movemos el dedo una posición a la derecha. Ahora el puntero señala a la "P" de Perez y nos hemos saltado el espacio.
    No se borra el espacio, sigue existiendo en la memoria. Lo que hago es avanzar el puntero.
    */

    FILE *fichero = fopen("usuarios.csv", "a");
    /*FILE *: Puntero a fichero. Es la variable que usará el programa para referirse a ese archivo abierto.
    "a" (Append / Añadir): Significa que abres el archivo para escribir al final.
        Si no existe: Lo crea nuevo.
    */
    if (fichero == NULL){
        printf("Error al abrir usuarios.csv\n");
        return;
    }
    /* fichero == NULL Significa que el archivo NO SE HA PODIDO ABRIR
    Por qué: A veces Windows no te deja abrir un archivo (por permisos, porque está abierto en Excel, disco lleno, etc.). Si fopen falla, devuelve NULL.
    */

    fprintf(fichero, "%s; %s; %s; %s\n", dni, apellidos, nombre, tipo);
    fclose(fichero);
    /*Qué es: Es igual que printf, pero la "f" del principio significa File. En vez de escribir en la pantalla negra, escribe dentro del archivo .csv.
    El formato: Fíjate que escribes los ; manualmente.

    fclose(fichero);
    Por qué es vital: Hasta que no haces fclose, los datos pueden estar en un búfer de memoria y no haberse guardado físicamente en el disco.
    Cerrar el fichero asegura que se guardan y liberan
    */

    Titulo("ANADIR USUARIO");
    printf("DNI: %s\nAPELLIDOS: %s\nNOMBRE: %s\n", dni, apellidos, nombre);
    printf("Se anadio con exito\n");
}

void editarUsuario(char *datos, char *tipo){
    char copia[MAX_LINEA];
    strcpy(copia, datos);
    /*Hace copia de seguridad
    La variable datos contiene la información completa y nueva (ej: "1111A;NuevoApellido;NuevoNombre").
    Necesito esa variable datos INTACTA para pasársela luego a la función anadirUsuario al final.
    Pero también necesito sacar el DNI ahora mismo para saber a quién borrar. Como sacar el DNI requiere strtok (que rompe la cadena), lo hago sobre una copia para no romper la original datos.

    "¿Por qué haces strcpy al principio? ¿Qué pasaría si haces strtok(datos, ";") directamente?"
    "Si hago strtok directamente sobre datos, la cadena se rompería. El puntero datos pasaría a valer solo el DNI (porque strtok pone un \0 después). Entonces, cuando llegue a la última línea anadirUsuario(datos, tipo), solo estaría añadiendo el DNI, y se perderían el apellido y el nombre nuevos. Hago la copia para usar una para borrar (sacando el DNI) y la original para añadir (con todo el contenido)."
    */
    char *dni = strtok(copia, ";");
    /*Qué hace: Corta la copia para quedarse solo con el primer trozo (el DNI).
    Para qué: Este es la identificacion para buscar a la víctima que vamos a eliminar.
    */

    borrarUsuario(dni, tipo);
    //Llama a tu función de borrado. Busca en el archivo y elimina la línea vieja que coincida con ese DNI.

    anadirUsuario(datos, tipo);
}

void borrarUsuario(char *dni, char *tipo){
    /*La Lógica: "Mudanza Selectiva"
    1. Abres la casa vieja (usuarios.csv) para mirar.
    2. Construyes una casa nueva vacía (temp.csv).
    3. Mueves a todos los vecinos de la vieja a la nueva, MENOS al que te cae mal (el que quieres borrar).
    4. Al final, demueles la casa vieja y le pones su nombre a la nueva.
    */
    FILE *f = fopen("usuarios.csv", "r");
    //Abre el archivo original ("casa vieja") en modo lectura ("r"). Solo vamos a mirar, no a tocar.
    FILE *temp = fopen("temp.csv", "w");
    /*Crea un archivo auxiliar ("casa nueva") en modo escritura ("w").
    Al usar "w", si temp.csv ya existiera por algún error anterior, lo borra y lo deja en blanco.
    */
    char linea[MAX_LINEA];
    //Es la "carretilla". Es el espacio de memoria donde cargaremos cada línea del archivo viejo para llevarla al nuevo.
    int encontrado = 0;
    //Al principio vale 0 (Falso). Si encontramos al usuario y lo borramos, lo pondremos a 1 (Verdadero).
    //Nos sirve para saber al final qué mensaje imprimir ("Se eliminó con éxito" o "No se encontró").
    char d[MAX_DNI], a[MAX_TEXTO], n[MAX_TEXTO];
    /*Son variables auxiliares.
    Como vamos a borrar al usuario, sus datos desaparecerán. Antes de "matarlo", copiaremos aquí su nombre y apellidos para poder decir al final: "Se ha eliminado a Pepito Pérez". Si no guardamos esto, solo podríamos decir "Se ha eliminado a alguien".
    */

    if (f == NULL || temp == NULL) return;
    /*Comprobamos si ambos archivos se han abierto bien.
        El return; (sin nada más) te saca de la función void inmediatamente.
    */

    while (fgets(linea, MAX_LINEA, f) != NULL){
        char lineaCopia[MAX_LINEA];
        strcpy(lineaCopia, linea);
        /*fgets: Lee una línea entera del archivo original hasta que se acaban (devuelve NULL).
        lineaCopia y strcpy (CRUCIAL):
            Variable linea que contiene la frase original (ej: "1111A; Perez; Juan\n").
            Copia en lineaCopia.
            ¿Por qué? Porque vas a usar strtok sobre la copia. strtok destroza la cadena (pone \0 donde había ;).
            Si destrozaras la original linea, luego no podrías escribirla en el archivo temporal si resulta que no era el usuario que querías borrar.
        */

        int largo = strlen(lineaCopia);
        if (largo > 0 && lineaCopia[largo-1] == '\n'){
            lineaCopia[largo-1] = '\0';
        }
        //Si hay un \n se sobrescribe por un \0 (fin de cadena)
        /*Cuando usas fgets para leer una línea de un archivo, la función lee todo hasta que encuentra el salto de línea (cuando pulsaste Enter al guardar). Lo malo es que fgets dice: "Ah, ¿hay un Enter? Pues me lo guardo también".
        Si en el archivo tienes: 1111A;Perez;Juan En tu variable lineaCopia se guarda: 1111A;Perez;Juan\n. Ese \n es el salto de línea.
            Si intentas comparar "Juan\n" con "Juan", NO son iguales.
            Si imprimes "Juan\n", te hace un salto de línea feo que te rompe el formato de la tabla.

        int largo = strlen(lineaCopia);
            Mide la longitud de la frase, cuantos caracteres tiene. Ej: Supongamos que lineaCopia es "Hola\n".      'H', 'o', 'l', 'a', '\n'.       La longitud es 5.

        largo > 0: Si por algún error raro la cadena estuviera vacía (longitud 0), intentar acceder a la posición -1 haría que el programa explotara (crash).

        lineaCopia[largo-1] Es la ultima posicion de lineaCopia porque se empieza en 0
        */

        char *ptrDni = strtok(lineaCopia, ";");
        char *ptrApellido = strtok(NULL, ";");
        char *ptrNombre = strtok(NULL, ";");

        if (ptrDni != NULL && strcmp(ptrDni, dni) == 0){
            encontrado = 1;
            //Compara el dni que quieres borrar (dni) con el de la lista (ptrDni) uno por uno, si no es el que quieres borrar lo pasa al fichero temp con else
            strcpy(d, ptrDni);
            if (ptrApellido) strcpy(a, ptrApellido);
            if (ptrNombre) strcpy(n, ptrNombre);
            //Se guardan datos en variables d, a, n
        }
        else{
            fprintf(temp, "%s", linea);
        }
        //Escribimos en temp la variable linea original. Salvamos al usuario inocente copiándolo a la casa nueva
    }

    fclose(f);
    fclose(temp);

    remove("usuarios.csv");
    rename("temp.csv", "usuarios.csv");

    if (encontrado == 1){
        Titulo("ELIMINAR USUARIO");
        printf("DNI: %s\nAPELLIDOS: %s\nNOMBRE: %s\n", d, a, n);
        printf("Se elimino con exito\n");
    }
    else{
        printf("No se encontro- el usuario con ese DNI\n");
    }
}

//Prácticas
void anadirPractica(char *dniAlumno, char *dia, char *hora, char *dniProfesor){
    FILE *f = fopen("practicas.csv", "a");
    /*
    "r" = Lectura (Read). Falla si no existe.
    "a" = Añadir (Append). Crea si no existe y escribe al final.
    "w" = Escribir (Write). Crea nuevo, pero BORRA todo lo anterior.
    */
    if (f == NULL) return;

    fprintf(f, "%s;%s;%s;%s\n", dia, hora, dniAlumno, dniProfesor);
    //Escritura en fichero
    fclose(f);

    Titulo("ANADIR PRACTICA");
    printf("ALUMNO: %s\nDIA: %s\nHORA: %s\nPROFESOR: %s\n", dniAlumno, dia, hora, dniProfesor);
    printf("Se anadio con exito\n");
}

void borrarPractica(char *dniAlumno, char *dia, char *hora, char *dniProfesor){
    FILE *f = fopen("practicas.csv", "r");
    FILE *temp = fopen("temp_practicas.csv", "w");
    char linea[MAX_LINEA];
    int encontrado = 0;

    char d[20], h[10], a[MAX_DNI], p[MAX_DNI];

    if (f == NULL || temp == NULL) return;
    // Seguridad: Si falla algún archivo, abortamos.

    while (fgets(linea, MAX_LINEA, f) != NULL){
        char lineaOriginal[MAX_LINEA];
        strcpy(lineaOriginal, linea);
        // TRUCO IMPORTANTE: Usamos 'lineaOriginal' para escribir en el archivo temp
        // porque 'copia' va a ser destrozada por strtok.

        char copia[MAX_LINEA];
        strcpy(copia, linea);

        int largo = strlen(copia);
        if (largo > 0 && copia[largo-1] == '\n'){
            copia[largo-1] = '\0';
        }

        char *ptrD = strtok(copia, ";");
        char *ptrH = strtok(NULL, ";");
        char *ptrA = strtok(NULL, ";");
        char *ptrP = strtok(NULL, ";");
        // Extraemos los 4 campos en orden.

        if (ptrD == NULL) ptrD = "?";
        if (ptrH == NULL) ptrH = "?";
        if (ptrA == NULL) ptrA = "?";
        if (ptrP == NULL) ptrP = "?";
        // SEGURIDAD EXTRA:
        // Si hay una línea vacía o rota en el archivo, los punteros serán NULL.
        // Asignamos "?" para evitar que el strcmp de abajo explote (crash).

        if (ptrD && ptrH && ptrA && ptrP && strcmp(ptrD, dia) == 0 && strcmp(ptrH, hora) == 0 && strcmp(ptrA, dniAlumno) == 0 && strcmp(ptrP, dniProfesor) == 0){
        // Para borrar, TIENEN que coincidir los 4 datos a la vez.
        // Usamos '&&' (Y). Si falla uno solo, no es la práctica que buscamos.
            encontrado = 1;
            strcpy(d, ptrD);
            strcpy(h, ptrH);
            strcpy(a, ptrA);
            strcpy(p, ptrP);
            // Guardamos los datos en las variables seguras para el printf final.
        }
        else{
            fprintf(temp, "%s", lineaOriginal);
            // Si no coincide, salvamos la línea copiándola al archivo temporal.
        }
    }

    fclose(f);
    fclose(temp);

    remove("practicas.csv");
    rename("temp_practicas.csv", "practicas.csv");

    if (encontrado) {
        Titulo("ELIMINAR PRACTICA");
        printf("ALUMNO: %s\nDIA: %s\nHORA: %s\nPROFESOR: %s\n", a, d, h, p);
        printf("Se elimino con exito\n");
    } else{
        printf("No se encontro la practica.\n");
    }
    // if (encontrado) es lo mismo que  if (encontrado) == 1
}

//Examenes
void anadirExamen(char *dniAlumno, char *dia, char *hora){
    FILE *f = fopen("examenes.csv", "a");
    if (f == NULL) return;
    // Si Windows no nos deja abrir el archivo, nos vamos.

    fprintf(f, "%s;%s;%s\n", dia, hora, dniAlumno);
    fclose(f);
    // Es vital cerrar para volcar el búfer al disco duro.

    Titulo("ANADIR EXAMEN");
    printf("ALUMNO: %s\nDIA: %s\nHORA: %s\n", dniAlumno, dia, hora);
    printf("Se anadio con exito\n");
}

void borrarExamen(char *dniAlumno, char *dia, char *hora){
    FILE *f = fopen("examenes.csv", "r");
    FILE *temp = fopen("temp_examenes.csv", "w");
    char linea[MAX_LINEA];
    int encontrado = 0;

    char d[20], h[10], a[MAX_DNI];

    if (f == NULL || temp == NULL) return;

    while (fgets(linea, MAX_LINEA, f) != NULL){ //Lee del archivo f (examenes.csv) y lo guarda en la variable "linea", si es NULL significa que se acabó el archivo y para
        char lineaOriginal[MAX_LINEA];
        strcpy(lineaOriginal, linea);

        char copia[MAX_LINEA];
        strcpy(copia, linea);

        int largo = strlen(copia);
        if (largo > 0 && copia[largo-1] == '\n'){
            copia[largo-1] = '\0';
        }

        char *ptrD = strtok(copia, ";");
        char *ptrH = strtok(NULL, ";");
        char *ptrA = strtok(NULL, ";");

        if (ptrD == NULL) ptrD = "?";
        if (ptrH == NULL) ptrH = "?";
        if (ptrA == NULL) ptrA = "?";

        if (ptrD && ptrH && ptrA && strcmp(ptrD, dia) == 0 && strcmp(ptrH, hora) == 0 && strcmp(ptrA, dniAlumno) == 0){
            encontrado = 1;
            strcpy(d, ptrD);
            strcpy(h, ptrH);
            strcpy(a, ptrA);
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
        printf("ALUMNO: %s\nDIA: %s\nHORA: %s\n", a, d, h);
        printf("Se elimino con exito\n");
    }
    else{
        printf("No se encontro el examen.\n");
    }
    // if (encontrado) es lo mismo que  if (encontrado) == 1
}

//Listados
void listarUsuarios(char *tipo, char *filtrarApellido, char *filtrarNombre) {
    FILE *f = fopen("usuarios.csv", "r");
    char linea[MAX_LINEA];

    if (f == NULL){
        printf("No hay fichero de usuarios.\n");
        return;
    }

    Titulo("LISTADO USUARIOS");

    while (fgets(linea, MAX_LINEA, f) != NULL){
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
        // Recuperamos los 4 campos. 'tip' es ALUMNO o PROFESOR.

        if (dni && ape && nom && tip){
        //dni, ape, nom, tip NO es nulo
            if (ape[0] == ' ') ape++;
            if (nom[0] == ' ') nom++;
            if (tip[0] == ' ') tip++;
            //Con el dni no se hace porque va el primero y no va a llevar espacios delannte

            if (strcmp(tip, tipo) == 0){
                int mostrar = 1;
                if (filtrarApellido != NULL && strstr(ape, filtrarApellido) == NULL) mostrar = 0;
                if (filtrarNombre != NULL && strstr(nom, filtrarNombre) == NULL) mostrar = 0;
                //Si filtrar... esta vacío no pasa nada. Pero si filtrar... tiene algo se mira si esta en ape, si no esta devuelve NULL por lo que mostrar = 0, si esta
                /*"¿Qué hace la función strstr?"
                Tu respuesta: "Es una función de la librería <string.h>. Busca si una cadena pequeña (filtrarApellido) está contenida dentro de una grande (ape).
                Devuelve un puntero a la primera coincidencia o NULL si no la encuentra."
                */

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

    if (f == NULL){
        printf("No hay fichero de practicas.\n");
        return;
    }

    Titulo("LISTADO PRACTICAS");

    while (fgets(linea, MAX_LINEA, f) != NULL){
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
            /*En listarUsuarios usabas strstr (parcial) porque querías buscar "Perez" y que saliera "Perez Garcia".
            Aquí usas strcmp (exacto).
            ¿Por qué? Porque las fechas y horas son rígidas. Si filtras la hora "12:00", no quieres que te salga la "12:00:01" ni la "12:30". Quieres exactitud.
            */

            if (mostrar == 1){
                if (prof != NULL){
                    printf("%s %s\t%s\tP: %s\n", d, h, alu, prof);
                }
                //Si prof si esta disponible lo imprime
                else{
                    printf("%s %s\t%s\tP: No disponible\n", d, h, alu);
                }
                // Si prof es NULL (no había dato)
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

    Titulo("LISTADO EXAMENES");

    while (fgets(linea, MAX_LINEA, f) != NULL){
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
            /*En caso de que por ejemplo quieras filtrar un dia y si este en la lista, como el if de imprimir esta dentro del bucle solo se mostraría ese
            y pasaría a probar uno a uno todos los dias y solo se mostraría por pantalla el que quieres filtrar
            */

            if (mostrar == 1){
                printf("%s %s\t%s\n", d, h, alu);
            }
        }
    }
    fclose(f);
}
