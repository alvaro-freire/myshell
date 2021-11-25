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
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include "prints.h"

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
            printf("* Argument \"%s\" invalid *\n", trozos[1]);
            return;
        }
        if ((priority = getpriority(PRIO_PROCESS, pid)) == -1 && errno != 0) {
            print_error();
            return;
        }
        printf("Priority of process with pid %d: %d\n", pid, priority);
    } else if (n == 3) {
        if ((pid = (pid_t) atoi(trozos[1])) == 0) {
            printf("* Argument \"%s\" invalid *\n", trozos[1]);
            return;
        }
        value = atoi(trozos[2]);
        if (setpriority(PRIO_PROCESS, pid, value) == -1) {
            print_error();
            return;
        }
        printf("Priority of process with pid %d changed to %d\n", pid, value);
    }
}
