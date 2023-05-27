#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main(void)
{
    // create 2 pipes; one for parent->child, one child->parent
    int childToParent[2];
    int parentToChild[2];
    int ret = pipe(childToParent);
    int ret2 = pipe(parentToChild);

    int flag = fork();
    if (flag)
    {
        // parent
        char ping;
        
        // close the "wrong" end of each pipe for the parent
        close(childToParent[1]); // write end = 1
        close(parentToChild[0]); // read end = 0

        clock_t start, end;
    
        start = clock();
        for (int i=0; i<100000; i++) {
            write(parentToChild[1], &ping, 1);
            read(childToParent[0], &ping, 1);
        }
        end = clock();
        
        wait(0);
        close(childToParent[0]);
        close(parentToChild[1]);

        printf("Exchanges per second: %ld\n", (unsigned long)(1 / ((((double)(end - start)) / CLOCKS_PER_SEC) / 100000)));
    }
    else
    {
        // child
        char pong;

        // close the "wrong" end of each pipe for the child
        close(parentToChild[1]);
        close(childToParent[0]);
        

        for (int j=0; j<100000; j++) {
            read(parentToChild[0], &pong, 1);
            write(childToParent[1], &pong, 1);
        }

        close(parentToChild[0]);
        close(childToParent[1]);
    }
}