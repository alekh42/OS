//cat


#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main()
{
    int fd1,i;
    char fn1[100],b1[100];
    scanf("%s",fn1);
    fd1=open(fn1,O_RDONLY,0777);
    while(i=read(fd1,b1,1)>0)
    {
        printf("%c",b1[0]);
    }
    printf("\n");


}




//mov



    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <string.h>
    #include <fcntl.h>
int main()
{
    int i,fd1,fd2;
    char fn1[100],fn2[100],buf[100];
    scanf("%s",fn1);
    scanf("%s",fn2);
    fd1=open(fn1,O_RDONLY,0777);
    fd2=creat(fn2,0777);
    while((i=read(fd1,buf,1)>0))
    {
        write(fd2,buf,1);    
    }
    remove(fn1);
}



//filesize



#include  <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    int fd;
    char fn[100],buf[100];
    scanf("%s",fn);
    fd=open(fn,O_RDONLY,0777);
    int size=lseek(fd,0,SEEK_END);
    printf("File size in bytes %d",size);
    int block;
    if(size%512==0)
        block=size/512;
    else
        block=(size/512) + 1;
    printf("Block  size %d",block);
    return 0;
}


//delete



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main(int argc,char*argv[])
{
    char*del;
    char a[10]="rm -r";
    del=argv[1];
    strcat(a,del);
    system(a);
    return 0;
}

//truncate5thbyte


#include <stdio.h>
#include <stdlib.h>
int main(int argc,char*argv[])
{
    FILE *fpr,*fpw;
    int counter=1,i=0,j=0;
    char ch;
    fpr=fopen(argv[1],"r");
    fpw=fopen(argv[1],"r+");
    fseek(fpr,0,SEEK_END);
    int size=ftell(fpr);
    printf("FILE SIZE ::%d",size);
    fseek(fpr,0,SEEK_SET);
    while(counter<size)
    {
        ch=fgetc(fpr);
        if(counter%5!=0)
        {
            fputc(ch,fpw);
            j++;
        }
        else{
            i++;
        }
        counter++;
    }
    fclose(fpr);
    fclose(fpw);
    truncate(argv[1],j);
    return 0;
}
