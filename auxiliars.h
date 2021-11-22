/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 18 / 11 / 2021
 */

#ifndef P1_SO_AUXILIARS_H
#define P1_SO_AUXILIARS_H

#include "mem_list.h"
#include "hist_list.h"

void borrarComandosC(tListC *L);

void borrarFilenameM(tListM *L);

void checkoptions(char *trozos[], bool *opLong, bool *opLink, bool *opAcc,
                  bool *opReca, bool *opRecb, bool *opHid, int n);

void checkoptions_mem(char *trozos[], bool *opBlocks, bool *opVars, bool *opFuncs,
                      bool *opAll, bool *opPmap, int n);

char *ConvierteModo(mode_t m, char *permisos);

int copyfiles(char *param, char *trozos[]);

bool dealloc_mem(tPosM pos, tListM *L, bool exit);

long int find_size(const char *path);

void remove_file(char c, char *filename);

void fix_trozos(char *trozos[], char *file, bool opAcc, bool opLink,
                bool opLong, bool opHid, bool opReca, bool opRecb);

char LetraTF(mode_t m);

void liberarMemoria(tListM *L);

int trocearCadena(char *cadena, char *trozos[]);

void *Mmap(char *arg[], int *fd);

void printable_char(char c);

void printable_hex(char c);

void *SharedCreate(char *arg[], tListM L);

int SharedDelkey(char *args[]);

ssize_t LeerFichero(char *fich, void *p, ssize_t n);

void dopmap(void);

void llenar_mem(char *p, int nbytes, char c);

void doRecursiva(int n);

#endif
