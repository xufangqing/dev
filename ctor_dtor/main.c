#include <stdio.h>
#include <stdlib.h>
/* #include <unistd.h> */ /* sleep used */

/*  test function utilizing attribute 'section' ".ctors"/".dtors"
    to create constuctors/destructors without assigned priority.
    (provided by Michael Ambrus in earlier answer)
*/

#define SECTION( S ) __attribute__ ((section ( S )))

void test (void) {
printf("\n\ttest() utilizing -- (.section .ctors/.dtors) w/o priority\n");
}

void (*funcptr1)(void) SECTION(".ctors") =test;
void (*funcptr2)(void) SECTION(".ctors") =test;
void (*funcptr3)(void) SECTION(".dtors") =test;

/*  functions constructX, destructX use attributes 'constructor' and
    'destructor' to create prioritized entries in the .ctors, .dtors
    ELF sections, respectively.

    NOTE: priorities 0-100 are reserved
*/
void construct1 () __attribute__ ((constructor (101)));
void construct2 () __attribute__ ((constructor (102)));
void destruct1 () __attribute__ ((destructor (101)));
void destruct2 () __attribute__ ((destructor (102)));

/*  init_some_function() - called by elf_init()
*/
int init_some_function () {
    printf ("\n  init_some_function() called by elf_init()\n");
    return 1;
}

/*  elf_init uses inline-assembly to place itself in the ELF .init section.
*/
int elf_init (void)
{
    __asm__ (".section .init \n call elf_init \n .section .text\n");

    if(!init_some_function ())
    {
        exit (1);
    }

    printf ("\n    elf_init() -- (.section .init)\n");

    return 1;
}

/*
    function definitions for constructX and destructX
*/
void construct1 () {
    printf ("\n      construct1() constructor -- (.section .ctors) priority 101\n");
}

void construct2 () {
    printf ("\n      construct2() constructor -- (.section .ctors) priority 102\n");
}

void destruct1 () {
    printf ("\n      destruct1() destructor -- (.section .dtors) priority 101\n\n");
}

void destruct2 () {
    printf ("\n      destruct2() destructor -- (.section .dtors) priority 102\n");
}

/* main makes no function call to any of the functions declared above
*/
int
main (int argc, char *argv[]) {

    printf ("\n\t  [ main body of program ]\n");
#if 0
    while(1) {
        printf("\n sleep 1 second...");
        sleep(1);
    }
#endif
    return 0;
}

