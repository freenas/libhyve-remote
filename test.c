#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <err.h>
#include <errno.h>
#include <string.h>

#define LIBHYVEREM  "/usr/local/lib/libhyverem.so"
int main(void);

int
main(void) {
    // DLOPEN
    char *loader;
    void *shlib;

    int (*check_sharedlibs)(char *lib_name);
    int (*load_functions_a)(void);

    loader = NULL;
    loader = strdup(LIBHYVEREM);
    shlib = dlopen(loader, RTLD_NOW);

    if (!shlib) {
        printf("Lib not loaded.\n");
        dlclose(shlib);
        free(loader);
        return (1);
    }

    check_sharedlibs = dlsym(shlib, "check_sharedlibs");
    load_functions_a = dlsym(shlib, "load_functions");

    int l = load_functions_a();
    printf("Load functions: %d\n", l);

    int check;
    check = check_sharedlibs("/usr/local/lib/libvncserver.so");
    printf("VNC: %d\n", check);

    return (0);
}
