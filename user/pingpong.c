#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void){
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);

    int pid = fork();

    if(pid==0){
        // child
        close(p2c[1]);  // p->c parent write close
        close(c2p[0]);  // c->p child read close

        // read parent
        char buf;
        read(p2c[0], &buf, sizeof(buf));
        printf("%d: received ping\n", getpid());

        // write to parent
        write(c2p[1], ".", 1);

        // close pipe
        close(p2c[0]);  // p->c parent read close
        close(c2p[1]);  // c->p child write close
        exit(0);
    }
    else{
        // parent
        close(p2c[0]);  // parent read close
        close(c2p[1]);  // child write close

        // p->c write
        write(p2c[1], ".", 1);

        // read child
        char buf;
        read(c2p[0], &buf, sizeof(buf));
        printf("%d: received pong\n", getpid());

        // close pipe
        close(p2c[1]);  // p->c write close
        close(c2p[0]);  // c->p read close
        wait(0);  // wait child
        exit(0);
    }
}