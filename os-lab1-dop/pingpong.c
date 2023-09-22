#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define PING "ping"
#define PONG "pong"

int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("pass pipe name as arg\n");
        return 1;
    }

    int pfd, cfd, rd, wr;
    char *fifo = argv[1];

    pfd = open(fifo, O_RDWR);
    cfd = open(fifo, O_RDWR);

    if (pfd == -1 || cfd == -1) {
        printf("failed to open pipe\n");
        return 1;
    }

    int cpid = fork();
    int ppid = getpid();

    if (cpid == 0){
        close(pfd);
        char buf[sizeof PING];

        sleep(1);

        rd = read(cfd, buf, sizeof(PING)-1);

        if (rd == -1) printf("%d : read failed\n", ppid);
        else printf("%d got: %s\n", ppid,buf);

        wr = write(cfd, PONG, sizeof PONG-1);

        if (wr == -1) printf("%d write failed\n", ppid);

        close(cfd);
    }else{
        close(cfd);
        char buf[sizeof PONG];

        wr = write(pfd, PING, sizeof PING-1);
        
        if (wr == -1) printf("%d write failed\n", ppid);

        sleep(2);

        rd = read(pfd, buf, sizeof PONG-1);
        
        if (rd == -1) printf("%d : read failed\n", ppid);
        else printf("%d got: %s\n",ppid, buf);
        
        close(pfd);
    }

    return 0;

}