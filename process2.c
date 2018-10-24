#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

sem_t mutex,full,empty;  //semaphore variable
int top=-1;

struct arg
{
    int *a;
    int k,m;
};

void push(int a[],int k,int x)
{
    if(top<k-1)
    {
        top++;
        a[top]=x;
    }
}

int pop(int a[],int k)
{
    if(top!=-1)
        top--;
}
void *producer_fn(void *arg)
{
    struct arg *S=(struct arg*)arg;
    int *a=S->a;
    int k=S->k;
    int m=S->m;

    int i,r;
    srand(time(0));
    for(i=0;i<m;i++)
    {
        r=rand()%101;
        //entry section
        sem_wait(&empty);
        sem_wait(&mutex);  //wait
        printf("%d ",r);
        push(a,k,r);
        usleep(2000);
        sem_post(&mutex);  //signal
        sem_post(&full);
        //exit section
    }
    return NULL;
}

void *consumer_fn(void *arg)
{
    struct arg *S=(struct arg*)arg;
    int *a=S->a;
    int k=S->k;
    int m=S->m;
      int i;

    int* buff=(int*)malloc(sizeof(int)*m);
    for(i=0;i<m;i++)
    {
        //entry section
        sem_wait(&full);
        sem_wait(&mutex);  //wait

        int x=a[top];
        pop(a,k);
        buff[i]=x;
        usleep(1000);

        sem_post(&mutex);  //signal
        sem_post(&empty);
        //exit section
    }

    return (void*)buff;
}

void sort(int a[],int n)
{
    int i,j,temp;
    for(i=0;i<n;i++)
    {
        for(j=i+1;j<n;j++)
        {
            if(a[i]>a[j])
            {
                temp=a[i];
                a[i]=a[j];
                a[j]=temp;
            }
        }
    }
}

int main()
{
    int k=1,n,m,seed=0;
    FILE *fp;
    
	    fp=fopen("op.txt","w+");
    printf("Enter value of n ie. no. of producer or consumer processes\n");
            scanf("%d",&n);
            printf("Enter value of m ie. no. of items to be produced or consumed\n");
            scanf("%d",&m);
    for(int g=0;g<10;g++)
    {
            srand(time(0)+seed);
            seed++;
            k+=2;
            printf("\n\n%d\n\n" ,k);
	    clock_t start,end;
	    int i,j,c,d;
	    double exe_time,thput,thput_sum,avg_thput;
	    struct arg S;
	    

        int a[k];
        thput_sum=0;

        
            start=clock();
            
            printf("Items produced by producer process:\n");

            sem_init(&mutex,0,1); //initialisation of semaphore
            sem_init(&full,0,0);
            sem_init(&empty,0,k);

            S.a=a;
            S.k=k;
            S.m=m;

            pthread_t pid[n];
            pthread_t cid[n];
            for(i=0;i<n;i++)
            {
                pthread_create(&pid[i],NULL,producer_fn,(void*)(&S));
                pthread_create(&cid[i],NULL,consumer_fn,(void*)(&S));
            }

            int *buff;
            void *pointer;
            int output[n*m];
            int index=0;
            for(i=0;i<n;i++)
            {
                pthread_join(pid[i],NULL);
                pthread_join(cid[i],&pointer);
                buff=(int*)pointer;
                   for(j=0;j<m;j++)
                {
                    output[index++]=buff[j];
                }
                free(buff);
            }
            sem_destroy(&mutex);  //destroying semaphore
            sem_destroy(&full);
            sem_destroy(&empty);

            sort(output,index);

            printf("\nItems received by parent process:\n");
            for(i=0;i<index;i++)
                printf("%d ",output[i]);
            printf("\nFinal Value of Stack top pointer = %d\n",top);

            end=clock();
            exe_time=((double)(end-start))/CLOCKS_PER_SEC;
            thput=(2*n+1)/exe_time;
            printf("Execution time = %lf\nThroughput = %lf\n",exe_time,thput);
            thput_sum+=thput;
          
          avg_thput=thput_sum/5;
        
        fprintf(fp,"%lf    %d\n",avg_thput,k);  //table entry
    }

    fclose(fp);
    char * commandsForGnuplot1[] = {"plot 'op.txt' with lines"};
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    for (int i=0; i < 1; i++)
    {
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot1[i]); //Send commands to gnuplot one by one.
    }
    fclose(gnuplotPipe);
    return 0;
}
