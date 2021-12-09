/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 22 / 11 / 2021
 */

#ifndef P3_SO_AUX_PROC_H
#define P3_SO_AUX_PROC_H

#include "bg_list.h"
#include "env_list.h"
#include <unistd.h>
#include <pwd.h>

void liberarEnvironment(tListE EnvironmentList);

void liberarProcessCommand(tListP ProcessList);

int set_priority(char *file, pid_t pid);

char *NombreUsuario (uid_t uid);

uid_t UidUsuario (char *nombre);

void MostrarUidsProceso (void);

int CambiarUidLogin (char *login);

int BuscarVariable(char *var, char *e[]);

char *check_status(int status);

tItemP update_status(tItemP item);

#endif //P3_SO_AUX_PROC_H
