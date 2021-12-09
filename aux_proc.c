/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 22 / 11 / 2021
 */

#include "aux_proc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "prints.h"

#define MAXVAR 255

void liberarEnvironment(tListE EnvironmentList) {
    tPosE pos;

    for (pos = firstE(EnvironmentList); pos != LNULL; pos = nextE(pos, EnvironmentList)) {
        free(getItemE(pos, EnvironmentList).name);
    }
}

int BuscarVariable(char *var, char *e[]) {
    int pos = 0;
    char aux[MAXVAR];

    strcpy(aux, var);
    strcat(aux, "=");

    while (e[pos] != NULL) {
        if (!strncmp(e[pos], aux, strlen(aux))) {
            return (pos);
        } else {
            pos++;
        }
    }

    errno = ENOENT; /* no hay tal variable */
    return (-1);
}

char *NombreUsuario(uid_t uid) {
    struct passwd *p;

    if ((p = getpwuid(uid)) == NULL) {
        return ("??????");
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

    printf("Real credential: %d (%s)\n", real, NombreUsuario(real));
    printf("Effective credential: %d (%s)\n", efec, NombreUsuario(efec));
}

int CambiarUidLogin(char *login) {
    uid_t uid;

    if ((uid = UidUsuario(login)) == (uid_t) -1) {
        printf("Invalid login: %s\n", login);
        return -1;
    }

    if (setuid(uid) == -1) {
        print_error();
        return -1;
    }
    return 0;
}

int set_priority(char *file, pid_t pid) {
    int value;

    /* se intenta convertir el argumento
     * del valor de prioridad a número */
    value = atoi(file);
    /* se cambia la prioridad del proceso con el pid introducido
     * por comando al valor correspondiente */
    if (setpriority(PRIO_PROCESS, pid, value) == -1) {
        return -1;
    }

    return value;
}

void liberarProcessCommand(tListP ProcessList) {
    tPosP pos;

    for (pos = firstP(ProcessList); pos != LNULL; pos = nextP(pos, ProcessList)) {
        free(getItemP(pos, ProcessList).command);
    }
}

char *check_status(int status) {

    switch (status) {
        case EXITED:
            return "EXITED";
        case RUNNING:
            return "RUNNING";
        case STOPPED:
            return "STOPPED";
        case KILLED:
            return "KILLED";
        default:
            return "UNKNOWN";
    }

}

tItemP update_status(tItemP item) {
    int status;

    if (waitpid(item.pid, &status, WNOHANG | WUNTRACED | WCONTINUED) == item.pid) {
        /* el estado del proceso ha cambiado desde la última revisión */
        if (WIFEXITED(status)) {
            item.state = EXITED;
            item.end = WEXITSTATUS(status);
        } else if (WIFCONTINUED(status)) {
            item.state = RUNNING;
        } else if (WIFSTOPPED(status)) {
            item.state = STOPPED;
            item.end = WSTOPSIG(status);
        } else if (WIFSIGNALED(status)) {
            item.state = KILLED;
            item.end = WTERMSIG(status);
        } else {
            item.state = UNKNOWN;
        }
    }

    return item;
}