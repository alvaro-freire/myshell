/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 22 / 11 / 2021
 */

#ifndef P3_SO_BG_LIST_H
#define P3_SO_BG_LIST_H

#include <stdbool.h>

#define LNULL NULL

typedef struct tItemP {
    pid_t pid;
} tItemP;

typedef struct tNodeP *tPosP;
typedef struct tNodeP {
    tItemP item;
    tPosP next;
} tNodeP;
typedef tNodeP *tListP;


void createEmptyListP(tListP *L);

bool isEmptyListP(tListP L);

tPosP firstP(tListP L);

tPosP lastP(tListP L);

tPosP previousP(tPosP p, tListP L);

tPosP nextP(tPosP p, tListP L);

tItemP getItemP(tPosP p, tListP L);

tPosP findPosP(int n, tListP L);

void deleteItemP(tPosP p, tListP *L);

void deleteListP(tListP *L);

bool insertItemP(tItemP i, tListP *L);

#endif //P3_SO_BG_LIST_H