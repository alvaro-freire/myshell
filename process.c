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

extern char **environ;

void cmdEntorno(char *trozos[], int n, char *env[]) {

    if (n == 1) {
        print_env_var(env);
    } else if (n == 2 && strcmp(trozos[1], "-environ") == 0) {
        print_env_var(environ);
    } else if (n == 2 && strcmp(trozos[1], "-addr") == 0) {
        print_env_addr(env, environ);
    } else {
        cmd_not_found();
    }
}

void cmdPriority(char *trozos[], int n) {
    int priority, value;
    pid_t pid;
    errno = 0;

    if (n == 1) {
        if ((priority = getpriority(PRIO_PROCESS, 0)) == -1 && errno != 0) {
            print_error();
            return;
        }
        pid = getpid();
        printf("Priority of the current process (pid: %d): %d\n", pid, priority);
    } else if (n == 2) {
        if ((pid = (pid_t) atoi(trozos[1])) == 0) {
            invalid_arg();
            return;
        }
        if ((priority = getpriority(PRIO_PROCESS, pid)) == -1 && errno != 0) {
            print_error();
            return;
        }
        printf("Priority of process with pid %d: %d\n", pid, priority);
    } else if (n == 3) {
        if ((pid = (pid_t) atoi(trozos[1])) == 0) {
            invalid_arg();
            return;
        }
        value = atoi(trozos[2]);
        if (setpriority(PRIO_PROCESS, pid, value) == -1) {
            print_error();
            return;
        }
        printf("Priority of process with pid %d changed to %d\n", pid, value);
    } else {
        cmd_not_found();
    }
}

void cmdRederr(char *trozos[], int n) {
    int fd;

    if (n == 1) {
        printf("Standard error file descriptor: %d\n", STDERR_FILENO);
    } else if (n == 2) {
        if (strcmp(trozos[1], "-reset") == 0) {
            /* ========================= { DUDA } ========================= */
            if (dup2(1, STDERR_FILENO) == -1) {
                printf("It was not possible to reset standard error\n");
                return;
            }
            printf("Standard error was reset successfully\n");
        } else {
            if ((fd = open(trozos[1], O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
                print_error();
                return;
            }
            if (dup2(fd, STDERR_FILENO) == -1) {
                print_error();
                return;
            }
            printf("Standard error was changed to file \"%s\"\n", trozos[1]);
        }
    } else {
        invalid_nargs();
    }
}
