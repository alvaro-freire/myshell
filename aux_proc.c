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
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>

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

void MostrarEntorno(char **entorno, char *nombre_entorno) {
    int i = 0;
    while (entorno[i] != NULL) {
        printf("%p->%s[%d]=(%p) %s\n", &entorno[i],
               nombre_entorno, i, entorno[i], entorno[i]);
        i++;
    }
}

int BuscarVariable(char *var, char *e[]) {
    int pos = 0;
    char aux[MAXVAR];
    strcpy(aux, var);
    strcat(aux, "=");
    while (e[pos] != NULL)
        if (!strncmp(e[pos], aux, strlen(aux)))
            return (pos);
        else
            pos++;
    errno = ENOENT; /*no hay tal variable*/
    return (-1);
}

int CambiarVariable(char *var, char *valor, char *e[]) {
    int pos;
    char *aux;

    if ((pos = BuscarVariable(var, e)) == -1) {
        return (-1);
    }

    if ((aux = (char *) malloc(strlen(var) + strlen(valor) + 2)) == NULL) {
        return -1;
    }

    strcpy(aux, var);
    strcat(aux, "=");
    strcat(aux, valor);
    e[pos] = aux;

    return (pos);
}

char *NombreUsuario(uid_t uid) {
    struct passwd *p;

    if ((p = getpwuid(uid)) == NULL) {
        return (" ??????");
    }

    return p->pw_name;
}

uid_t UidUsuario(char *nombre) {
    struct passwd *p;

    if ((p = getpwnam(nombre)) == NULL) {
        return (uid_t) -1;
    }

    return p->pw_uid;
}

void MostrarUidsProceso(void) {
    uid_t real = getuid(), efec = geteuid();

    printf("Credencial real: %d, (%s)\n", real, NombreUsuario(real));
    printf("Credencial efectiva: %d, (%s)\n", efec, NombreUsuario(efec));
}

void CambiarUidLogin(char *login) {
    uid_t uid;

    if ((uid = UidUsuario(login)) == (uid_t) -1) {
        printf("loin no valido: %s\n", login);
        return;
    }

    if (setuid(uid) == .1) {
        printf("Imposible cambiar credencial: %s\n", strerror(errno));
    }
}