//ipc



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char*argv[])
{
    int pipefd[2];
    pid_t cpid;
    int i,fd1,fd2;
    char *fn1,*fn2,buf[2];
    pipe(pipefd);
    fn1=argv[1];
    fn2=argv[2];
    cpid=fork();
    if(cpid==0)
    {
        close(pipefd[1]);
        fd2=creat(fn2,O_RDONLY);
        while(read(pipefd[0],&buf,1)>0)
        {
            write(fd2,buf,1);
        }
        close(pipefd[0]);
        exit(0);
    }
    else
    {
        close(pipefd[0]);
        fd1=open(fn1,O_RDONLY);
        while(read(fd1,buf,1)>0)
        {
            write(pipefd[1],buf,1);
        }
        close(pipefd[1]);
        wait(NULL);
        exit(0);
    }
    return 0;
}


