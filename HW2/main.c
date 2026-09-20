// #include "utils.h"
// #include "bm.h"
// #include "bbm.h"
// #include "freelist.h"
#include "balloc.h"
#include "freelist.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    Balloc pool = bcreate(32,5,5);
    bprint(pool);

    void * ptr = balloc(pool, 32);

    return 0;
}