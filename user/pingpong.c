#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PING "ping"
#define PONG "pong"

void 
main(){

    int p[2];
    int c[2];
    
    pipe(p);
    pipe(c);

    int child_pid = fork();
    int self_pid = getpid();

    if(child_pid == 0){
        char buf[sizeof PING];

        close(p[1]);
        close(c[0]);

        read(p[0], buf, sizeof buf);
        fprintf(1, "%d: got %s\n", self_pid, buf);
        write(c[1], PONG, sizeof PONG);

        close(p[0]);
        close(c[1]);


    }else{
        char buf[sizeof PONG];

        close(p[0]);
        close(c[1]);

        write(p[1], PING, sizeof PING);
        wait( (int * ) 0);
        read(c[0], buf, sizeof buf);
        fprintf(1, "%d: got %s\n", self_pid, buf);

        close(p[0]);
        close(c[1]);

    }

    exit(0);

}