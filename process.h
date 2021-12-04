/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 22 / 11 / 2021
 */

#ifndef P3_SO_PROCESS_H
#define P3_SO_PROCESS_H

#include "env_list.h"

void cmdCambiarvar(char *trozos[], int n, char *arg3[], char *environ[], tListE *EnvironmentList);

void cmdEntorno(char *trozos[], int n, char *arg3[], char *environ[]);

void cmdMostrarvar(char *trozos[], int n, char *arg3[], char *environ[]);

void cmdPriority(char *trozos[], int n);

void cmdRederr(char *trozos[], int n, char **std_error);

void cmdUid(char *trozos[], int n);

#endif //P3_SO_PROCESS_H
