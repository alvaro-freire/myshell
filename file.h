/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 18 / 11 / 2021
 */

#ifndef P1_SO_COMMANDS_H
#define P1_SO_COMMANDS_H

#define MAX_CHARS 1000
#define MAX_ARGS 100

#include "hist_list.h"
#include "mem_list.h"
#include "env_list.h"

void cmdAutores(char *param, int n);

void cmdAyuda(char *param, int n);

void cmdBorrar(char *trozos[], int n);

void cmdBorrarrec(char *trozos[], int n);

void cmdCarpeta(char *param, int n);

void cmdComando(char *param, int n, tListC *CommandList, int *commandNumber,
                tListM *MemoryList, tListE *EnvironmentList, char *arg3[], char *environ[]);

void cmdCrear(char *trozos[], int n);

void cmdExit(int n, bool *exit, tListC *CommandList, tListM *MemoryList, tListE *EnvironmentList);

void cmdFecha(char *param, int n);

void cmdHist(char *param, int n, tListC *CommandList, int *commandNumber);

void cmdInfosis(int n);

void cmdListdir(char *trozos[], int n);

void cmdListfich(char *trozos[], int n, bool aux_function);

void cmdPid(char *param, int n);

void cmdSwitcher(char *trozos[], int n, bool *exit, tListC *CommandList, int *commandNumber,
                 tListM *MemoryList, tListE *EnvironmentList, char *arg3[], char *environ[]);

void procesarEntrada(char *command, bool *exit, tListC *CommandList, int *commandNumber,
                     tListM *MemoryList, tListE *EnvironmentList, char *arg3[], char *environ[]);

#endif
