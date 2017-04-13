#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "sample_sort.h"
#include <omp.h>
#define DEBUG
/*
typedef struct thread_paras{
    
    int id ;// represent the virrtual pid
    
    // used to sort intput file1
    int upper;// data1 : upper bound
    int lower;// data1 : lower bound
    int count;// data1 : element count
    int upper_value_;  // bucket1 upper bound value
    int lower_value_;  // bucket1 lower bound value
    
    int * UNSORTED_; //data1: unsorted
    int UNSORTED_LENGTH_; // data1 :length
    int * SORTED_; // data1: sorted array address
}th_paras;
*/



int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}


void* bucket_sort( void* arg )
{
    th_paras para = *(th_paras*) arg;
    #ifdef DEBUG
    printf("Now, thread[%d] is sorting %d to %d \n",para.id,para.lower_value,para.upper_value);
    #endif
    qsort(para.UNSORTED, para.upper_value-para.lower_value, sizeof(int), cmpfunc);
    #ifdef DEBUG
    printf("Done, thread[%d] finish %d to %d  sort\n",para.id,para.lower_value,para.upper_value);
    #endif
}


void samplesort(int *array, int *final_array, int count, int thread)
{
#ifdef DEBUG
    //for calculate time
    struct timespec start_t, end_t;   
    double dotime;
#endif

    int *spliter = NULL;
    int *num_count = NULL;
    spliter = (int *) malloc(thread * sizeof(int));
    num_count = (int *) malloc(thread * sizeof(int));
    //get spliter 

#ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &start_t);
#endif
/////////////////////////////
//      findspliter        //
/////////////////////////////
    findspliter(array, spliter, count, thread);
#ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &end_t);
    dotime = (end_t.tv_sec - start_t.tv_sec) * 1000.0;   
    dotime += (end_t.tv_nsec - start_t.tv_nsec) / 1000000.0;   

    printf("-----step[findspliter] using time :%lf-----\n", dotime);
#endif
    int i = 0;
#ifdef DEBUG
    for(i = 0 ; i < thread;i++)
    {
        printf("%d,",spliter[i]);
    }
    printf("\n");
#endif
#ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &start_t);
#endif
    bucket_count(array, count, num_count, spliter, thread);
#ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &end_t);
    dotime = (end_t.tv_sec - start_t.tv_sec) * 1000.0;   
    dotime += (end_t.tv_nsec - start_t.tv_nsec) / 1000000.0;   

    printf("-----step[bucket_count] using time :%lf-----\n", dotime);
#endif
#ifdef DEBUG
    for(i = 0 ; i < thread;i++)
    {
        printf("%d,",num_count[i]);
    }
    printf("\n");
#endif
#ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &start_t);
#endif
    distribute_bucket(array, count, num_count, final_array, spliter, thread);

#ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &end_t);
    dotime = (end_t.tv_sec - start_t.tv_sec) * 1000.0;   
    dotime += (end_t.tv_nsec - start_t.tv_nsec) / 1000000.0;   

    printf("-----step[distribute_bucket] using time :%lf-----\n", dotime);
#endif

#ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &start_t);
#endif 
    #pragma omp parallel for shared(num_count,final_array) private(i)
    for(int i = 0; i < thread; i++)
    {
        if(i == 0)
            qsort(&final_array[num_count[i-1]], num_count[i] - 0, sizeof(int), cmpfunc);
        else
            qsort(&final_array[num_count[i-1]], num_count[i] - num_count[i-1], sizeof(int), cmpfunc);
    }
   

 #ifdef DEBUG
    clock_gettime( CLOCK_REALTIME, &end_t);
    dotime = (end_t.tv_sec - start_t.tv_sec) * 1000.0;   
    dotime += (end_t.tv_nsec - start_t.tv_nsec) / 1000000.0;   

    printf("-----step[openmp do qsort] using time :%lf-----\n", dotime);
#endif   
    free(spliter);
    free(num_count);
    return;
}


void distribute_bucket(int *array, int total, int *num_count, int *final_array, int *spliter, int thread)
{
    int i, j, k = 0;
    
    for(i = 0; i < total; i++)
    {
        j = thread - 1;
        while(j >= 0)
        {
            if(array[i] > spliter[j])
            {
                final_array[num_count[j]++] = array[i];
                break;
            }
            j--;
        }
    }
    return;
}



void bucket_count(int *array,int total, int *num_count, int *spliter, int thread)
{
    int i, j, k = 0;

    for(i = 0; i < total; i++)
    {
        j = thread - 1;
        while(j >= 0)
        {
            if(array[i] > spliter[j])
            {
                num_count[j]++;
                break;
            }
            j--;
        }
    }
    #ifdef DEBUG
        printf("num\n");
        #endif
    for(i = 1 ;i < thread; i++)
    {
        #ifdef DEBUG
        printf("%d,",num_count[i]);
        #endif
        num_count[i] += num_count[i-1];
    }
    #ifdef DEBUG
        printf("\n");
        #endif
    for(i = thread-1 ;i > 0 ; i--)
    {
        num_count[i] = num_count[i-1];
    }
    num_count[0] = 0;
    return;

}


//using oversampling, but only do in one thread 
void findspliter(int *array, int *spliter,int count, int thread)
{
    int bucket_size = count/thread;
    int i,j,k = 0;
    int *index = NULL;
    int *tmp_spliter = NULL;
    index = (int *)malloc(sizeof(int) * (thread));
    tmp_spliter = (int *)malloc(sizeof(int) * (thread - 1) * thread);
    j = 0;
    for(i = 0; i < count ; i += bucket_size)
    {
        if((i + 2*bucket_size) > count)
            bucket_size = count - i;
        // move qsort to different thread????
        //qsort(&array[i], bucket_size, sizeof(int), cmpfunc);
        index[j] = bucket_size;
        j++;
    }
    int tmp_index = 0;
    k = 0;

    for(i = 0 ; i < thread ; i++)
    {
        int bump_size = index[i]/thread;
        int re = tmp_index;
        j = thread - 1 ;
        while(j > 0)
        {
            tmp_spliter[k] = array[re + bump_size];
            re += bump_size;
            k++;
            j--;
        }
        tmp_index += index[i];
    }
    qsort(tmp_spliter, thread *(thread-1), sizeof(int),cmpfunc);
    k = thread-1 ;
    for(i = 1 ; i < thread ; i++)
    {
        //fill spliter
        spliter[i] = tmp_spliter[k];
        k+=thread;
    }
    spliter[0] = -2147483648;
    free(tmp_spliter);
    free(index);
    return;
}
