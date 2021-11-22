/**
 * TITLE: Operative Systems
 * SUBTITLE: Lab Assignment 3
 * @author Mateo Díaz Allegue @login mateo.diaz
 * @author Álvaro Freire Ares @login alvaro.freirea
 * GROUP: 4.2
 * DATE: 18 / 11 / 2021
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "file.h"
#include "prints.h"

#define LEERCOMPLETO ((ssize_t)-1)
#define TAM 4096

/**
 * Function: LetraTF
 * ----------------------
 * Checks the mode of the file and
 * returns the corresponding letter.
 *
 * @return mode letter
 */
char LetraTF(mode_t m) {
    switch (m & S_IFMT) { /* and bit a bit con los bits de formato, 0170000 */
        case S_IFSOCK:
            return 's'; /* socket */
        case S_IFLNK:
            return 'l'; /* symbolic link */
        case S_IFREG:
            return '-'; /* fichero normal */
        case S_IFBLK:
            return 'b'; /* block device */
        case S_IFDIR:
            return 'd'; /* directorio */
        case S_IFCHR:
            return 'c'; /* char device */
        case S_IFIFO:
            return 'p'; /* pipe */
        default:
            return '?'; /* desconocido, no deberia aparecer */
    }
}

/**
 * Function: ConvierteModo
 * ----------------------
 * Checks permissions of the file and returns the
 * corresponding string in drwxrxwrxw format.
 *
 * @param m file mode
 * @param permisos initialized string
 * @return updated string with the
 *         corresponding permissions
 */
char *ConvierteModo(mode_t m, char *permisos) {
    strcpy(permisos, "---------- ");
    permisos[0] = LetraTF(m);
    if (m & S_IRUSR) permisos[1] = 'r'; /*propietario*/
    if (m & S_IWUSR) permisos[2] = 'w';
    if (m & S_IXUSR) permisos[3] = 'x';
    if (m & S_IRGRP) permisos[4] = 'r'; /*grupo*/
    if (m & S_IWGRP) permisos[5] = 'w';
    if (m & S_IXGRP) permisos[6] = 'x';
    if (m & S_IROTH) permisos[7] = 'r'; /*resto*/
    if (m & S_IWOTH) permisos[8] = 'w';
    if (m & S_IXOTH) permisos[9] = 'x';
    if (m & S_ISUID) permisos[3] = 's'; /*setuid, setgid y stickybit*/
    if (m & S_ISGID) permisos[6] = 's';
    if (m & S_ISVTX) permisos[9] = 't';

    return permisos;
}

/**
 * Function: find_size
 * ----------------------
 * Calculates the size of a file.
 *
 * @param path string of the file or
 *        directory to find its size
 * @return bytes of the file
 */
long int find_size(const char *path) {
    DIR *d;
    char firstpath[MAX_CHARS];
    struct dirent *dir;
    long int size = 0;
    struct stat filestat;
    stat(path, &filestat);
    getcwd(firstpath, sizeof(firstpath));

    /* se comprueba si es un directorio */
    if (LetraTF(filestat.st_mode) == 'd') {
        if (path == NULL) {
            return -1;
        }
        d = opendir(path); /* se abre el directorio */

        if (d) {
            /* se accede al directorio 'path' */
            if (chdir(path) != -1) {
                while ((dir = readdir(d)) != NULL) {
                    if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                        stat(dir->d_name, &filestat); /* se guarda información del archivo */
                        size += find_size(dir->d_name); /* se añade el tamaño del fichero */
                    }
                }
                if (strcmp(path, ".") != 0) {
                    chdir(".."); /* se vuelve al directorio anterior */
                }
                if (strcmp(path, "..") == 0) {
                    chdir(firstpath); /* se vuelve al directorio principal */
                }
            }
            closedir(d); /* se cierra el directorio */
        }

        return size;
    } else {
        FILE *fp = fopen(path, "r"); /* se abre el fichero en modo lectura */

        /* se comprueba la existencia del fichero: */
        if (fp == NULL) {
            print_error();
            return -1;
        }

        fseek(fp, 0L, SEEK_END);

        size = ftell(fp); /* se obtiene el tamaño del fichero: */

        fclose(fp); /* se cierra el fichero */

        return size;
    }

}

/**
 * Function: remove_file
 * ----------------------
 * Removes a file or a directory
 *
 * @param c type of the file
 * @param filename name of the file
 * @return void.
 */
void remove_file(char c, char *filename) {
    if (c == '-') {
        if (remove(filename) == 0) {
            printf("Removed file \"%s\"\n", filename);
        } else {
            print_error();
        }
    } else if (c == 'd') {
        if (rmdir(filename) == 0) {
            printf("Removed directory \"%s\"\n", filename);
        } else {
            print_error();
        }
    } else {
        printf("Error: \"%s\" could not be removed\n", filename);
    }
}

/**
 * Function: copyfiles
 * ----------------------
 * Copies in trozos all the files
 * found in the directory path.
 *
 * @param path string with the path
 * @param trozos array of strings
 * @return number of strings copied in trozos
 */
int copyfiles(char *path, char *trozos[]) {
    DIR *d;
    struct dirent *dir;
    int cnt = 1;

    d = opendir(path); /* se abre el directorio */
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            } else {
                /* se reserva memoria para un string */
                trozos[cnt] = (char *) malloc(strlen(dir->d_name) + 1);
                /* se copia en trozos el nombre de cada archivo
                 * encontrado y se incrementa el contador */
                strcpy(trozos[cnt++], dir->d_name);
            }
        }
        closedir(d); /* se cierra el directorio */
    }
    return cnt; /* se devuelve el número de strings copiados */
}

/**
 * Function: trocearCadena
 * ----------------------
 * Copies in trozos all the tokens found
 * in the string separated by spaces.
 *
 * @param cadena string with the command
 * @param trozos array of strings
 * @return number of strings copied in trozos
 */
int trocearCadena(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL) {
        return 0;
    }
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL) {
        i++;
    }

    return i;
}

/**
 * Function: checkoptions
 * ----------------------
 * Checks the options and changes its
 * value to the corresponding one.
 *
 * @param trozos array of strings
 * @param opLong, opLink, opAcc, opReca, opRecb, opHid booleans of the corresponding options
 * @param n number of arguments in trozos
 * @return void.
 */
void checkoptions(char *trozos[], bool *opLong, bool *opLink, bool *opAcc,
                  bool *opReca, bool *opRecb, bool *opHid, int n) {

    /* se identifican las opciones escogidas por el
     * usuario para los seis primeros parámetros: */
    for (int i = 1; i < n; i++) {
        if (trozos[i] == NULL) {
            break;
        } else if (strcmp(trozos[i], "-acc") == 0) {
            *opAcc = true;
        } else if (strcmp(trozos[i], "-link") == 0) {
            *opLink = true;
        } else if (strcmp(trozos[i], "-long") == 0) {
            *opLong = true;
        } else if (strcmp(trozos[i], "-reca") == 0) {
            *opReca = true;
        } else if (strcmp(trozos[i], "-recb") == 0) {
            *opRecb = true;
        } else if (strcmp(trozos[i], "-hid") == 0) {
            *opHid = true;
        }
    }
}

/**
 * Function: checkoptions_mem
 * ----------------------
 * Checks the options for memory and changes
 * its value to the corresponding one.
 *
 * @param trozos array of strings
 * @param opBlocks, opLink, opVars, opFuncs, opAll, opPmap booleans of the options
 * @param n number of arguments in trozos
 * @return void.
 */
void checkoptions_mem(char *trozos[], bool *opBlocks, bool *opVars, bool *opFuncs,
                      bool *opAll, bool *opPmap, int n) {

    for (int i = 1; i < n; i++) {
        if (trozos[i] == NULL) {
            break;
        } else if (strcmp(trozos[i], "-blocks") == 0) {
            *opBlocks = true;
        } else if (strcmp(trozos[i], "-vars") == 0) {
            *opVars = true;
        } else if (strcmp(trozos[i], "-funcs") == 0) {
            *opFuncs = true;
        } else if (strcmp(trozos[i], "-all") == 0) {
            *opAll = true;
        } else if (strcmp(trozos[i], "-pmap") == 0) {
            *opPmap = true;
        }
    }
}

/**
 * Function: fix_trozos
 * ----------------------
 * Copies the command to an auxiliar array of strings
 * so that it only prints information about one file.
 *
 * @param trozos array of strings
 * @param file name of the file to print information about
 * @param opAcc, opLink, opLong, opHid, opReca, opRecb booleans of the corresponding options
 * @return void.
 */
void fix_trozos(char *trozos[], char *file,
                bool opAcc, bool opLink, bool opLong, bool opHid, bool opReca, bool opRecb) {
    int i = 1;
    char *aux_trozos[MAX_ARGS];

    /* se guarda en i el número de opciones a true: */
    i += opAcc + opLong + opLink + opHid + opReca + opRecb;

    /* se reserva en aux_trozos el espacio necesario para
     * copiar todas las opciones a true correspondientes */
    for (int j = 0; j < i; ++j) {
        aux_trozos[j] = (char *) malloc(strlen(trozos[j]) + 1);
        strcpy(aux_trozos[j], trozos[j]);
    }

    /* se copia en aux_trozos el archivo del
     * cual se quiere imprimir información */
    aux_trozos[i] = (char *) malloc(strlen(file) + 1);
    strcpy(aux_trozos[i], file);
    i++;

    /* se envía el nuevo array de strings a listfich: */
    cmdListfich(aux_trozos, i, true);

    /* se liberan todos los espacios de memoria
     * que se habían reservado anteriormente */
    for (int j = 0; j < i; ++j) {
        free(aux_trozos[j]);
    }
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam) { /* Obtienen un puntero a una zona de memoria compartida */
    /* si tam > 0 intenta crearla y si tam == 0 asume que existe */
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    /* si tam no es 0 la crea en modo exclusivo esta
     * funcion vale para shared y shared -create */
    if (tam) {
        flags = flags | IPC_CREAT | IPC_EXCL;
    }

    /* si tam es 0 intenta acceder a una ya creada */
    if (clave == IPC_PRIVATE) { /* no nos vale */
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1) {
        return (NULL);
    }
    if ((p = shmat(id, NULL, 0)) == (void *) -1) {
        aux = errno; /* si se ha creado y no se puede mapear */
        if (tam) {
            /* se borra */
            shmctl(id, IPC_RMID, NULL);
        }
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s);

    /* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....); */
    return (p);
}

void *SharedCreate(char *arg[], tListM L) { /* arg[0] is the key and arg[1] is the size */
    key_t k;
    size_t tam = 0;
    void *p;
    if (arg[0] == NULL || arg[1] == NULL) {
        print_shared(L);
        return 0;
    }
    k = (key_t) atoi(arg[0]);
    if (arg[1] != NULL) {
        tam = (size_t) atoll(arg[1]);
    }
    if ((p = ObtenerMemoriaShmget(k, tam)) == NULL) {
        return NULL;
    } else {
        printf("Shared memory of key %d at %p\n", k, p);
    }
    return p;
}

void *MmapFichero(char *fichero, int protection, int *fd) {
    int map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat filestat;
    void *p;
    if (protection & PROT_WRITE) modo = O_RDWR;

    if (stat(fichero, &filestat) == -1 || (*fd = open(fichero, modo)) == -1) {
        return NULL;
    }
    if ((p = mmap(NULL, filestat.st_size, protection, map, *fd, 0)) == MAP_FAILED) {
        return NULL;
    }

    /* Guardar Direccion de Mmap (p, filestat.st_size,fichero,fd......) */
    return p;
}

void *Mmap(char *arg[], int *fd) { /* arg[0] is the file name and arg[1] is the permissions */
    char *perm;
    void *p;
    int protection = 0;

    if (arg[0] == NULL) {
        return false; /* Listar Direcciones de Memoria mmap; */
    }
    if ((perm = arg[1]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
    }

    p = MmapFichero(arg[0], protection, fd);

    return p;
}

ssize_t LeerFichero(char *fich, void *p, ssize_t n) { /* lee n bytes del fichero fich en p */
    ssize_t nleidos, tam = n; /* si n==-1 lee el fichero completo */
    int df, aux;
    struct stat s;

    if (stat(fich, &s) == -1 || (df = open(fich, O_RDONLY)) == -1) {
        return ((ssize_t) -1);
    }
    if (n == LEERCOMPLETO) {
        tam = (ssize_t) s.st_size;
    }
    if ((nleidos = read(df, p, tam)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return ((ssize_t) -1);
    }
    close(df);

    return (nleidos);
}

int SharedDelkey(char *args[]) { /* arg[0] points to a str containing the key */
    key_t clave;
    int id;
    char *key = args[0];

    if (key == NULL || (clave = (key_t) strtoul(key, NULL, 10)) == IPC_PRIVATE) {
        printf("shared -delkey valid_key\n");
        return -1;
    }
    if ((id = shmget(clave, 0, 0666)) == -1) {
        perror("Could not get shared memory");
        return -1;
    }
    if (shmctl(id, IPC_RMID, NULL) == -1) {
        perror("Could not delete shared memory\n");
        return -1;
    }
    return 0;
}

void dopmap(void) { /* no arguments necessary */
    pid_t pid;      /* ejecuta el comando externo pmap para */
    char elpid[32]; /* pasandole el pid del proceso actual */
    char *argv[3] = {"pmap", elpid, NULL};
    sprintf(elpid, "%d", (int) getpid());

    if ((pid = fork()) == -1) {
        perror("Could not create proccess");
        return;
    }
    if (pid == 0) {
        if (execvp(argv[0], argv) == -1) {
            perror("cannot execute pmap");
        }
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

void doRecursiva(int n) {
    char automatico[TAM];
    static char estatico[TAM];

    printf("parameter: %d (%p) ", n, &n);
    printf("array %p, ", estatico);
    printf("static array %p\n", automatico);

    if (n > 1) doRecursiva(--n);
}

void llenar_mem(char *p, int nbytes, char c) {
    int i;

    printf("Filling %d bytes of memory with the char '%c' from the address %p\n", nbytes, c, p);

    for (i = 0; i < nbytes; i++) {
        p[i] = c;
    }
}

void printable_char(char c) {
    bool printable = (c > 32 && c < 126);


    if (printable) {
        printf("%2c ", c);
    } else {
        printf("%2c ", ' ');
    }
}

void printable_hex(char c) {
    bool printable = (c > 32 && c < 126);


    if (printable) {
        printf("%2x ", c);
    } else {
        printf("%2c ", '0');
    }
}

bool dealloc_mem(tPosM pos, tListM *L, bool exit) {
    tItemM item = getItemM(pos, *L);

    switch (item.cmdType) {
        case MALLOC:
            if (!exit) {
                printf("deallocated %zu at %p\n", item.size, item.dir);
            }
            free(item.dir);
            deleteItemM(pos, L);
            break;

        case MMAP:
            if (munmap(item.dir, item.size) == -1) {
                print_error();
                return false;
            } else {
                close(item.Union.fich.fd);
                if (!exit) {
                    printf("unmapped file %s\n", item.Union.fich.filename);
                    free(item.Union.fich.filename);
                }
                deleteItemM(pos, L);
            }
            break;

        case SHARED:
            /* checks if the adress was already deatached */
            if ((shmget(item.Union.key, 0, 0)) != -1) {
                /* tries to deatach the adress: */
                if (shmdt(item.dir) == -1) {
                    print_error();
                    return false;
                }
            }
            if (!exit) {
                printf("Shared memory block at %p (key %d) has been dealocated\n", item.dir, item.Union.key);
            }
            deleteItemM(pos, L);

            break;
    }
    return true;
}


/**
 * Function: borrarComandos
 * ----------------------
 * The memory reserved for each command in the
 * list is freed.
 *
 * @param L command list
 * @return void.
 */
void borrarComandosC(tListC *L) {
    tItemC item;

    for (tPosC pos = firstC(*L); pos != NULL; pos = nextC(pos, *L)) {
        item = getItemC(pos, *L);
        free(item.CommandName);
    }
}

void borrarFilenameM(tListM *L) {
    tItemM item;

    for (tPosM pos = firstM(*L); pos != NULL; pos = nextM(pos, *L)) {
        item = getItemM(pos, *L);
        if (item.cmdType == MMAP) {
            free(item.Union.fich.filename);
        }
    }
}

void liberarMemoria(tListM *L) {
    tPosM pos, next;

    for (pos = firstM(*L); pos != LNULL; pos = next) {
        next = nextM(pos, *L);
        dealloc_mem(pos, L, true);
    }
}