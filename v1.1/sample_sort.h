void samplesort(int *array, int *final_array,int count, int thread);
void bucket_count(int *array,int total, int *num_count, int *spliter, int thread);
void distribute_bucket(int *array, int total, int *num_count, int *final_array, int *spliter, int thread);
void findspliter(int *array, int *spliter, int count, int thread);
void* bucket_sort( void* arg );// bucket sort that thread execute



typedef struct thread_paras{
    
    int id ;// represent the virrtual pid
    int upper_value;  // bucket1 upper bound value
    int lower_value;  // bucket1 lower bound value
    
    int * UNSORTED; //data1: unsorted
}th_paras;


