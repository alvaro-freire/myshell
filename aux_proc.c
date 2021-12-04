/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 22 / 11 / 2021
 */

#include "aux_proc.h"
#include "errno.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

void liberarEnvironment(tListE *EnvironmentList) {
    tPosE pos;

    for (pos = firstE(*EnvironmentList); pos != LNULL; pos = nextE(pos, *EnvironmentList)) {
        free(getItemE(pos, *EnvironmentList).name);
    }
}

char *NombreUsuario(uid_t uid) {
    struct passwd *p;

    if ((p = getpwuid(uid)) == NULL)
        return ("??????");
    return p->pw_name;
}

uid_t UidUsuario(char *nombre) {
    struct passwd *p;

    if ((p = getpwnam(nombre)) == NULL)
        return (uid_t) -1;
    return p->pw_uid;
}

void MostrarUidsProceso(void) {
    uid_t real = getuid(), efec = geteuid();

    printf("Real credential: %d, (%s)\n", real, NombreUsuario(real));
    printf("Effective credential: %d, (%s)\n", efec, NombreUsuario(efec));
}

void CambiarUidLogin(char *login) {
    uid_t uid;

    if ((uid = UidUsuario(login)) == (uid_t) -1) {
        printf("Invalid login: %s\n", login);
        return;
    }
    if (setuid(uid) == -1)
        printf("Impossible to change credential: %s\n", strerror(errno));
}
