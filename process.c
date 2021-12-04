/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 22 / 11 / 2021
 */

#include "process.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "prints.h"
#include "aux_proc.h"

void cmdCambiarvar(char *trozos[], int n, char *arg3[], char *environ[], tListE *EnvironmentList) {
    int i;
    char *var;
    tItemE item;

    if (n == 4) {
        /* se reserva memoria para encadenar los strings
         * correspondientes con el formato "name=value" */
        var = malloc(strlen(trozos[2]) + strlen(trozos[3]) + 4);
        strcpy(var, trozos[2]);
        strcat(var, "=");
        strcat(var, trozos[3]);
        /* se comprueba el modo en el que se va a acceder a la variable de entorno */
        if (strcmp(trozos[1], "-a") == 0) {
            /* se busca la variable en el array arg3 del main */
            if ((i = find_index(trozos[2], arg3)) == -1) {
                printf("Environment variable \"%s\" does not exist\n", trozos[2]);
                free(var);
                return;
            }
            /* se hace que el puntero de la variable correspondiente
             * apunte al string con el nuevo valor */
            arg3[i] = var;
        } else if (strcmp(trozos[1], "-e") == 0) {
            /* se busca la variable en el array environ del main */
            if ((i = find_index(trozos[2], environ)) == -1) {
                printf("Environment variable \"%s\" does not exist\n", trozos[2]);
                free(var);
                return;
            }
            /* se hace que el puntero de la variable correspondiente
             * apunte al string con el nuevo valor */
            environ[i] = var;
        } else if (strcmp(trozos[1], "-p") == 0) {
            /* si la variable existe se cambia su valor, si no existe se crea */
            if (putenv(var) != 0) {
                print_error();
                free(var);
                return;
            }
        } else {
            cmd_not_found();
            free(var);
            return;
        }
        printf("Environment variable \"%s\" changed its value to \"%s\"\n", trozos[2], trozos[3]);
        /* se añade la dirección del string a una lista para luego liberar su memoria reservada */
        item.name = var;
        insertItemE(item, EnvironmentList);
    } else {
        invalid_nargs();
    }
}

void cmdEntorno(char *trozos[], int n, char *arg3[], char *environ[]) {

    if (n == 1) {
        /* se imprime por pantalla el array del
         * tercer argumento del main (env) */
        print_var(arg3, "main arg3");
    } else if (n == 2 && strcmp(trozos[1], "-environ") == 0) {
        /* se imprime por pantalla el array de la
         * variable externa del main (environ) */
        print_var(environ, "environ");
    } else if (n == 2 && strcmp(trozos[1], "-addr") == 0) {
        /* se imprimen las direcciones de los dos
         * arrays mencionados anteriormente */
        print_env_addr(arg3, environ);
    } else {
        cmd_not_found();
    }
}

void cmdMostrarvar(char *trozos[], int n, char *arg3[], char *environ[]) {
    int i, j;
    char *value;

    if (n == 1) {
        /* se imprime por pantalla el array del
         * tercer argumento del main (arg3) */
        print_var(arg3, "main arg3");
    } else if (n == 2) {
        /* se obtiene el valor de la variable de entorno "trozos[1]" */
        value = getenv(trozos[1]);
        if (value == NULL) {
            printf("Variable \"%s\" does not exists\n", trozos[1]);
            return;
        }
        /* se busca en los arrays (3er argumento del main y variable
         * externa) la variable de entorno mencionada en el comando */
        i = find_index(trozos[1], arg3);
        j = find_index(trozos[1], environ);

        /* se imprimen valores y direcciones correspondientes de cada array */
        printf("With arg3 main:\t%s (%p) %p\n", arg3[i], arg3[i], &arg3[i]);
        printf("With environ:\t%s (%p) %p\n", environ[j], environ[j], &environ[j]);
        printf("With getenv:\t%s (%p)\n", value, &value);
    } else {
        cmd_not_found();
    }
}

void cmdPriority(char *trozos[], int n) {
    int priority, value;
    pid_t pid;
    /* se inicializa el errno a 0 para detectar
     * posibles errores más adelante */
    errno = 0;

    if (n == 1) {
        /* se intenta obtener la prioridad del proceso actual */
        if ((priority = getpriority(PRIO_PROCESS, 0)) == -1 && errno != 0) {
            print_error();
            return;
        }
        pid = getpid();
        printf("Priority of the current process (pid: %d): %d\n", pid, priority);
    } else if (n == 2) {
        /* se intenta convertir el argumento del pid a número */
        if ((pid = (pid_t) atoi(trozos[1])) == 0) {
            invalid_arg();
            return;
        }
        /* se intenta obtener la prioridad del proceso actual teniendo en
         * cuenta que puede devolver -1 pero no haberse producido un error */
        if ((priority = getpriority(PRIO_PROCESS, pid)) == -1 && errno != 0) {
            print_error();
            return;
        }
        printf("Priority of process with pid %d: %d\n", pid, priority);
    } else if (n == 3) {
        /* se intenta convertir el argumento del pid a número */
        if ((pid = (pid_t) atoi(trozos[1])) == 0) {
            invalid_arg();
            return;
        }
        /* se intenta convertir el argumento
         * del valor de prioridad a número */
        value = atoi(trozos[2]);
        /* se cambia la prioridad del proceso con el pid introducido
         * por comando al valor correspondiente */
        if (setpriority(PRIO_PROCESS, pid, value) == -1) {
            print_error();
            return;
        }
        printf("Priority of process with pid %d changed to %d\n", pid, value);
    } else {
        cmd_not_found();
    }
}

void cmdRederr(char *trozos[], int n, char **std_error) {
    int fd;

    if (n == 1) {
        /* se imprime el file descriptor en el que se encuentra
         * direccionada la salida estándar de errores */
        printf("Standard error in file \"%s\"\n", *std_error);
    } else if (n == 2) {
        if (strcmp(trozos[1], "-reset") == 0) {
            /* se restablece el fd de la salida estándar de errores al
             * valor predeterminado (salida por pantalla) */
            if (dup2(STDOUT_FILENO, STDERR_FILENO) == -1) {
                printf("It was not possible to reset standard error\n");
                return;
            }
            *std_error = "Initial setup";
            printf("Standard error was reset successfully\n");
        } else {
            /* se abre el archivo indicado en el comando y se guarda su fd */
            if ((fd = open(trozos[1], O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
                print_error();
                return;
            }
            /* con el fd del archivo se restablece el fd de la salida estándar de errores */
            if (dup2(fd, STDERR_FILENO) == -1) {
                print_error();
                return;
            }
            *std_error = "hola";
            printf("Standard error was changed to file \"%s\"\n", trozos[1]);
        }
    } else {
        invalid_nargs();
    }
}

void cmdUid(char *trozos[], int n) {

    if (n == 1) {
        MostrarUidsProceso();
    } else if (n == 2) {
        if (strcmp(trozos[1], "-get") == 0 || strcmp(trozos[1], "-set") == 0) {
            MostrarUidsProceso();
        } else {
            invalid_arg();
        }
    } else if (n == 3) {
        if (strcmp(trozos[1], "-set") == 0) {
            CambiarUidLogin(NombreUsuario(atoi(trozos[2])));
        } else {
            invalid_arg();
        }
    } else if (n == 4) {
        if (strcmp(trozos[1], "-set") == 0 && strcmp(trozos[2], "-l") == 0) {
            CambiarUidLogin(trozos[3]);
        } else {
            invalid_arg();
        }
    } else {
        cmd_not_found();
    }
}
