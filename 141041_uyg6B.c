//152120141041 - Emre KOÇ
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

//these variables points to specific points
extern char *__executable_start;
extern char *etext;
extern char *end;

//global variable for test
int global;

//functions for test
void dummyFunc() {}
void testAddress() {}

//start of the main
void main()
{
    //local variables, page size and heap size for test
    int local;
    int *local2;
    local2 = (int *)malloc(sizeof(int));
    int size = getpagesize();
    long unsigned int *heap = sbrk(0);

    //char c for choice, char x for memory operations, cnt for finding second variable
    char c;
    char x;
    int cnt = 0;

    srand(time(NULL));

    //random values for variables
    global = rand() % 10;
    local = rand() % 10;
    *local2 = rand() % 10;

    //function call
    void dummyFunc();
    void testAddress();

    //printing page size and addresses of variables
    printf("pageSize   = %d\n", size);
    printf(" globlVar  = %d\n", global);
    printf("&globlVar  = 0x%lx\n", (long unsigned int)&global);
    printf(" localVar  = %d\n", local);
    printf("&localVar  = 0x%lx\n", (long unsigned int)&local);
    printf("*localVar2 = localVar2 = %d\n", *local2);
    printf(" localVar2 = 0x%lx\n", (long unsigned int)local2);
    printf("&localVar2 = 0x%lx\n\n", (long unsigned int)&local2);

    printf("Prototypes:\n");
    printf("void dummyFunc();             //does nothing\n");
    printf("void testAddress();           //does nothing\n\n");

    //printing addresses of specific points and functions
    printf("Addresses:\n");
    printf("&dummyFunc()       = 0x%lx\n", (long unsigned int)dummyFunc);
    printf("&testAddress()     = 0x%lx\n", (long unsigned int)testAddress);
    printf("&main()            = 0x%lx\n", (long unsigned int)main);
    printf(" text-seg 1st page = 0x%lx\n", (long unsigned int)&__executable_start);
    printf(" text-seg end      = 0x%lx\n", (long unsigned int)&etext);
    printf(" data-seg end      = 0x%lx\n", (long unsigned int)&end);
    printf(" heap-seg end      = 0x%lx\n\n", (long unsigned int)&heap);

    //beginning of the loop for memory reading and writing
    while (1)
    {

        printf("Enter (w: rw ok / r: only read ok / n : rw not ok /e:exit): ");
        scanf(" %c", &c);

        //if the choice is writing and reading
        if (c == 'w')
        {
            //finding not zero variable for second try
            if (cnt == 1)
            {
                char *temp = (char *)&global;
                printf("Reading             0x%lx\t: ", (long unsigned int)temp);
                x = *temp;
                printf("%d\n", x);
                printf("Writing %-4dback to 0x%lx\t: ", x, (long unsigned int)temp);
                *temp = x;
                printf("Ok!\n\n");
            }
            //reading and writing
            else
            {
                //finding readable and writable part and generating random address between these values
                long signed int a1 = 0x000008049fff - 0x0000080498cc;
                long unsigned int rnd = ((rand() % (((long unsigned int)&end + a1) - ((long unsigned int)&global))) + ((long unsigned int)&global));
                char *temp = (char *)rnd;
                //reading
                printf("Reading             0x%lx\t: ", (long unsigned int)temp);
                x = *temp;
                printf("%d\n", x);
                //writing
                printf("Writing %-4dback to 0x%lx\t: ", x, (long unsigned int)temp);
                *temp = x;
                printf("Ok!\n\n");
            }
        }

        //if the choice is reading
        else if (c == 'r')
        {
            //finding just readable part and generating random address between these values
            long unsigned int rnd = ((rand() % ((long unsigned int)&etext - (long unsigned int)&__executable_start + 1)) + (long unsigned int)&__executable_start);
            char *temp = (char *)rnd;
            //reading
            printf("Reading             0x%lx\t: ", (long unsigned int)temp);
            x = *temp;
            printf("%d\n", x);
            //writing
            printf("Writing %-4dback to 0x%lx\t: ", x, (long unsigned int)temp);
            printf("\n");
            //catching segmentation fault and return to main
            if (SIGSEGV)
            {
                printf("Segmentation fault (core dumped) ------> Signal Handled!\n\n\n");
                printf("---THIS PROGRAM WILL BE STARTED ONE MORE TIME---\n\n\n");
                return main();
            }
            *temp = x;
            printf("Ok!\n\n");
        }

        //if the choice is not reading nor writing
        else if (c == 'n')
        {
            //finding not readable nor writing part and generating random address between these values
            long unsigned int rnd = ((rand() % ((long unsigned int)&__executable_start + 1)));
            char *temp = (char *)rnd;
            //reading
            printf("Reading             0x%lx\t: ", (long unsigned int)temp);
            printf("\n");
            //catching segmentation fault and return to main
            if (SIGSEGV)
            {
                printf("Segmentation fault (core dumped) ------> Signal Handled!\n\n\n");
                printf("---THIS PROGRAM WILL BE STARTED ONE MORE TIME---\n\n\n");
                return main();
            }
            x = *temp;
            printf("%d\n", x);
            //writing
            printf("Writing %-4dback to 0x%lx\t: ", x, (long unsigned int)temp);
            *temp = x;
            printf("Ok!\n\n");
        }

        //if the choice is exit
        else if (c == 'e')
        {
            exit(1);
        }

        //choice error
        else
        {
            printf("Error! Invalid choice!\n");
        }
        printf("Let's try again...\n");
        cnt++;
    }//end of the loop
}//end of the main