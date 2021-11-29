/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 22 / 11 / 2021
 */

#include "aux_proc.h"
#include <string.h>
#include <stdlib.h>

int find_index(char *param, char *trozos[]) {
    int i;
    char *aux, *cpy;

    for (i = 0; trozos[i] != NULL; i++) {
        cpy = (char *) malloc(strlen(trozos[i]) + 1);
        strcpy(cpy, trozos[i]);
        aux = strtok(cpy, "=");
        if (strcmp(aux, param) == 0) {
            free(cpy);
            return i;
        }
        free(cpy);
    }
    return -1;
}

void liberarEnvironment(tListE *EnvironmentList) {
    tPosE pos;

    for (pos = firstE(*EnvironmentList); pos != LNULL; pos = nextE(pos, *EnvironmentList)) {
        free(getItemE(pos, *EnvironmentList).name);
    }
}
