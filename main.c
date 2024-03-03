#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <dlfcn.h>

extern char *optarg;
extern int optind, opterr, optopt;

const void *p[16];
int p_index;

void * variadic(void *(*func)(...), int arity) {
    // No good function takes more than 16 parameter.
    switch(arity) {
    default:
    case 0: return func();
    case 1: return func(p[0]);
    case 2: return func(p[0], p[1]);
    case 3: return func(p[0], p[1], p[2]);
    case 4: return func(p[0], p[1], p[2], p[3]);
    case 5: return func(p[0], p[1], p[2], p[3], p[4]);
    case 6: return func(p[0], p[1], p[2], p[3], p[4], p[5]);
    case 7: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
    case 8: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
    case 9: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
    case 10: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
    case 11: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10]);
    case 12: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11]);
    case 13: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12]);
    case 14: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13]);
    case 15: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14]);
    case 16: return func(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
    }
}

enum contex { TOP, FUNC, STRUCT };

int argparse(int argc, char *argv[], enum contex ctx) {
    const char *optstring = "hvl:f:p:";
    while(optind<argc) {
        switch(getopt(argc, argv, optstring)) {
        default:
        case 'h':
            fprintf(stderr, "help msg\n");
            return 2;
        case 'v': printf("cish, version %s (%s)\n", VERSION, TARGET); return 0;

        case 'l': {
            const char *libpath = optarg+1;
            size_t len = strlen(optarg);
            char lib[len+7];
            if(optarg[0]!=':') {
                lib[0] = 'l'; lib[1] = 'i'; lib[2] = 'b';
                lib[len+3] = '.'; lib[len+4] = 's'; lib[len+5] = 'o';
                lib[len+6] = 0;
                memcpy(lib+3, optarg, len);
                libpath = lib;
            }
            if(dlopen(libpath, RTLD_NOW)==NULL) {
                fprintf(stderr, "ERR: %s:%i: Cannot open library: %s\n",
                        optarg, optind, dlerror());
                return 1;
            }
        } break;

        case 'f': {
            const char *name = optarg;
            if(argparse(argc, argv, FUNC))
                return 1;
            void *(*func)(...);
            *(void **)&func = dlsym(NULL, name);
            variadic(func, p_index);
            return 0;
        }

        case 'p':
            if(ctx!=FUNC) {
                fprintf(stderr,
                        "ERR: %s:%i: Function parameter outside p-list scope\n",
                        optarg, optind);
                return 1;
            }
            p[p_index++] = strdup(optarg);
            break;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    return argparse(argc, argv, TOP);
}
