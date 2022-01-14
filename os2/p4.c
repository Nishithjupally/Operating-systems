#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>

int a,b,c,d;
int **A,**B;
int **C;
void init_matrix(int **X,int a,int b)
{
	int i,j;
    for(i=0;i<a;i++)
    {
        for(j=0;j<b;j++)
        {
            X[i][j]=rand()%20;
            printf("%d ",X[i][j]);
        }   
        printf("\n");
    }

    
}

float single_thread_mm()
{
	A=(int **)malloc(a * sizeof(int*));

    for(int i=0;i<a;i++)
    {
        A[i]=(int *)malloc(b * sizeof(int));
    }

    B=(int **)malloc(c * sizeof(int*));

    for(int i=0;i<c;i++)
    {
        B[i]=(int *)malloc(d * sizeof(int));
    }

	C=(int **)malloc(a * sizeof(int*));

	for (int i=0;i<a;i++)
    {
        C[i]=(int*) malloc(d * sizeof(int));
    }

	init_matrix(A,a,b);
	init_matrix(B,c,d);
	int i,j,k;
	clock_t s,e;

	
    s=clock();
	for(i=0;i<a;i++)
	{
		for(j=0;j<d;j++)
		{
			C[i][j]=0;
			for(k=0;k<b;k++)
			{
				C[i][j]+=A[i][k]*B[k][j];
			}
			printf("%d ",C[i][j]);
		}
		printf("\n");
	}
	e=clock();
	
	free(A);
	for (int i = 0; i < b; ++i)
	{
		free(A[i]);
	}
	free(A);
	for (int i = 0; i < d; ++i)
	{
		free(B[i]);
	}
	free(B);
	for (int i = 0; i < d; ++i)
	{
		free(C[i]);
	}
	free(C);
	
	return(e-s);	

}

struct element{
	int i;
	int j;
};

void *matmul(void *arg)
{
	struct element *num=arg;
	int n,sum=0;
	
	for(int k=0;k<b;k++)
	{
	
		sum+=A[num->i][k]*B[k][num->j];
	}
	
	C[num->i][num->j]=sum;
	
	pthread_exit(NULL);
}

float multi_thread_mm()
{
	int i,j;
	A=(int **)malloc(a * sizeof(int*));

    for(int i=0;i<a;i++)
    {
        A[i]=(int *)malloc(b * sizeof(int));
    }

    B=(int **)malloc(c * sizeof(int*));

    for(int i=0;i<c;i++)
    {
        B[i]=(int *)malloc(d * sizeof(int));
    }

	C=(int **)malloc(a * sizeof(int*));

	for (int i=0;i<a;i++)
    {
        C[i]=(int*) malloc(d * sizeof(int));
    }
    init_matrix(A,a,b);
    init_matrix(B,c,d);
    clock_t s,e;
    for(i=0;i<a;i++)
    {
    	for(j=0;j<d;j++)
    	{
    		
    		struct element *num=(struct element*)malloc(sizeof(struct element));
    		num->i=i;
    		num->j=j;
    		pthread_t tid;
    		s=clock();
    		pthread_create(&tid,NULL,matmul,num);
    		pthread_join(tid,NULL);
    		e=clock();
    	}
    }

    for(i=0;i<a;i++)
    {
    	for(j=0;j<d;j++)
    	{
    		printf("%d ",C[i][j]);
    	}	
    	printf("\n");
    }
    
    free(A);
	for (int i = 0; i < b; ++i)
	{
		free(A[i]);
	}
	free(A);
	for (int i = 0; i < d; ++i)
	{
		free(B[i]);
	}
	free(B);
	for (int i = 0; i < d; ++i)
	{
		free(C[i]);
	}
	free(C);

    return (e-s);	
}

int multi_process_mm()
{

	int aid=shmget(IPC_PRIVATE,(a*b*sizeof(int)),IPC_CREAT|0666);
	int bid=shmget(IPC_PRIVATE,(c*d*sizeof(int)),IPC_CREAT|0666);
	int cid=shmget(IPC_PRIVATE,(a*d*sizeof(int)),IPC_CREAT|0666);

	A=(int**)shmat(aid,NULL,0);
	B=(int**)shmat(bid,NULL,0);
	C=(int**)shmat(cid,NULL,0);
   
   	int i,j,k;

   	printf("sdfwsdf\n");

	init_matrix(A,a,b);
	init_matrix(B,c,d);
	printf("wef\n");
	for(i=0;i<a;i++)
	{
		printf("dfgb\n");
		int pid=fork();
		if(pid==0)
		{
			A=(int**)shmat(aid,NULL,0);
	   		B=(int**)shmat(bid,NULL,0);
	    	C=(int**)shmat(cid,NULL,0);

			for(j=0;j<d;j++)
			{
				int temp=0;
				for(k=0;k<b;k++)
				{
					temp+=A[i][k]*B[k][j];
				}
				C[i][j]=temp;
			}
			exit(0);
		}
	}

	for(i=0;i<a;i++)
    {
    	for(j=0;j<d;j++)
    	{
    		printf("%d ",C[i][j]);
    	}	
    	printf("\n");
    }
    return 1;
}
int main(int argc,char **argv)
{
	//int w,x,y,z;
	

	a=atoi(argv[1]);
	b=atoi(argv[2]);
	c=atoi(argv[3]);
	d=atoi(argv[4]);

	
	float k=single_thread_mm();

	printf("time taken : %0.6f\n",((k)*(1000000)/CLOCKS_PER_SEC));
	float m=multi_thread_mm();
	printf("time taken : %0.6f\n",((m)*(1000000)/CLOCKS_PER_SEC));
	float speed=k/m;
	printf("%f\n",speed);
	printf("wedf\n");
	multi_process_mm();
	printf("here\n");


}