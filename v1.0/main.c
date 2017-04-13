#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "sample_sort.h"

//for calculate time
struct timespec t_start, t_end;   
double elapsedTime;

//target array
int *Target;
int *Final;
//get source file data number
int Get_line_count(char *filename);
void Read_file(char *filename,int count);

int cmpf (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
int main(int argc, char *argv[])
{
    int opt;
    int i = 0;
    char *Source = NULL;
    char thread_num;
    int Thread = 0;
    int line = 0;
    //if p = 1 use sample sort
    int p = 0;
    while ((opt = getopt(argc, argv, "mht:f:")) != -1) {
        switch (opt) {
        case 'm':
            p = 1;
            break;
        case 'h':
            printf("Use -t to define thread number\n");
            printf("Use -f to set source file you want to sort\n");
            exit(EXIT_FAILURE);
        case 't':
            Thread = atoi(optarg);
            break;
        case 'f':
            Source = optarg; 
            break;
        default:
            fprintf(stderr, "type main -h to get information\n");
            exit(EXIT_FAILURE);
        }
    }
    if(argc == 1)
    {
        printf("Use -t to define thread number\n");
        printf("Use -f to set source file you want to sort\n");
        exit(EXIT_FAILURE);
    }    
    if(Thread > 16)
        Thread = 16;
    if(Thread <= 0)
        Thread = 1;
    printf("Thread : %d Source:%s\n", Thread, Source);
    line = Get_line_count(Source);
    printf("file have %d data\n",line);
    Read_file(Source, line);
/*print source file content
    for(i = 0 ; i< line ;i++)
    {
        printf("%d\n",Target[i]);
    } 
*/
/*
insert samplesort here
here will give thread number and array pointer
*/
    printf("start sorting\n");
    clock_gettime( CLOCK_REALTIME, &t_start);
    if(p == 1)
    {
        printf("using samplesort\n");
        samplesort(Target, Final, line, Thread);
    }
    else
    {
        printf("using qsort\n");
        qsort(Target,line,sizeof(int),cmpf);
    }
    
    clock_gettime( CLOCK_REALTIME, &t_end);
    elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;   
    elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;   

    printf("using time :%lf\n", elapsedTime);
//print source file content
//    for(i = 0 ; i< line ;i++)
//    {
//        printf("%d\n",Target[i]);
//    } 
//
    int * pointer;
    if(p == 1)
        pointer = Final;
    else
        pointer = Target;
    FILE *of ;
    of = fopen("mysorted.txt","w");
    for(i = 0 ; i < line; i++)
    {
        fprintf(of,"%d\n",pointer[i]);
    }
    printf("out put sorted data in sorted.txt\n");
    fclose(of);
    free(Target);
    free(Final);
    return 0;
}


int Get_line_count(char *filename)
{
    
    FILE * inf = NULL;
    int line_count = 0;
    int tmp = 0;
    inf = fopen(filename,"r");
    if(inf == NULL)
    {
        printf("file can not open, please chekc your path\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(inf))
    {
        fscanf(inf,"%d",&tmp);
        line_count++;
    }
    fclose(inf);
    return line_count-1;
}



void Read_file(char *filename,int count)
{
    
    FILE * inf = NULL;
    int tmp = 0;
    int i = 0 ;
    int line_count;
    Target = (int*) malloc(count * sizeof(int));
    Final = (int*) malloc(count * sizeof(int));
    inf = fopen(filename,"r");
    if(inf == NULL)
    {
        printf("file can not open, please chekc your path\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0 ; i < count ; i++)
    {
        fscanf(inf,"%d", &Target[i]);
    } 
    fclose(inf);
    return ;
}
