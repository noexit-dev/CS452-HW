// #include "utils.h"
// #include "bm.h"
// #include "bbm.h"
// #include "freelist.h"
#include "balloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    Balloc pool = bcreate(32, 1, 1);
    bprint(pool);

    return 0;
}