#include <stdio.h>
#include "stdlib.h"
#include "dlfcn.h"    //dlsym and dlopen need
#include "dlfunc.h"

int main(void) {
    void *handle;
    int (*fcn)(int x, int y);
    const char *errmsg;
    
    printf("\n#############################\n");
    printf("this is test ddl in dynamic load mode\n");
    printf("#############################\n");
    /* open the library */
    handle = dlopen("./libdlfunc.so", RTLD_NOW);
    if(handle == NULL) {
        fprintf(stderr, "Failed to load libdlfunc.so: %s\n", dlerror());
        return 1;
    }
    
    //*(void **)(&fcn) = dlsym(handle, "add");            //ok
    fcn = dlsym(handle, "dl_func");                       //ok
    if((errmsg = dlerror()) != NULL) {
        printf("%s\n", errmsg);
        return 1;
    }
    
    printf("dl_func[%p] return:[%d]\n", fcn, fcn(1,2));
    dlclose(handle);
    return 0;
}