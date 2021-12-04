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

#include "env_list.h"
#include <unistd.h>
#include <pwd.h>

int find_index(char *param, char *trozos[]);

void liberarEnvironment(tListE *EnvironmentList);

char *NombreUsuario (uid_t uid);

uid_t UidUsuario (char *nombre);

void MostrarUidsProceso (void);

void CambiarUidLogin (char *login);

#endif //P3_SO_AUX_PROC_H
