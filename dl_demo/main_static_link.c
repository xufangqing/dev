#include <stdio.h>
#include "dlfunc.h"

void main() {
    printf("\n#############################\n");
    printf("this is test ddl in static link\n");
    printf("#############################\n");
    printf("dl_func[%p] return:[%d]\n", dl_func, dl_func(1,2));
    return;
}
