/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 4 / 12 / 2021
 */

#include "signals.h"

char *NombreSenal(int sen) {    /*devuelve el nombre senal a partir de la senal*/
    /* para sitios donde no hay sig2str*/
    int i;

    for (i = 0; sigstrnum[i].nombre != NULL; i++) {
        if (sen == sigstrnum[i].senal) {
            return sigstrnum[i].nombre;
        }
    }

    return ("SIGUNKNOWN");
}