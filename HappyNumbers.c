#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t myLock;

int happyNum =0;
void* calculation(void* i){
    int j= (intptr_t) i;//the number we want to decide is happy or unhappy
    int sum=0;//used to keep track of the sum of the number's digits
    while(sum !=1 && sum!=4){//if sum is 1 or 4 we have found if it is happy or unhappy
        sum=0;//reset
        while (j!=0){
            int partial= j%10;//get the last digit
            sum = sum +(partial*partial);//square the digit and add to sum
            j=j/10;//update j
        }
        j=sum;//the new number we want to calcuate for 
    } 
    if(sum==1){
        pthread_mutex_lock(&myLock);
        happyNum++;//increment happy number count 
        pthread_mutex_unlock(&myLock);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    //if user doesn't have 5 arguments they don't have proper arguments
    if(argc != 5){
        printf("Error, invalid command line arguments.\n");
        exit(1);
    }
    int endNum= atoi(argv[4]);
    int threads= atoi(argv[2]);
    pthread_mutex_init(&myLock, NULL);
    pthread_t thd[4];
    int i=0;
    int k;

    //required for project 
    if((strcmp(argv[1],"-t"))!=0){
        printf("Error, the argv at index 1 needs to be -t.\n");
        exit(1);
    }
    //the only allowed values for threads are 1,2,3,and 4
    if(threads != 1 && threads != 2 && threads != 3 && threads != 4){
        printf("Error, invalid thread count.\n");
        exit(1);
    }
    //project doesn't state we can start form any other number
    if((strcmp(argv[3],"-1"))!=0){
        printf("Error, the argv at index 3 needs to be -1.\n");
        exit(1);
    }
    //user cannot input negative numbers
    if(endNum <0){
        printf("Error, invalid limit number.\n");
        exit(1);
    }
    //the minimum input user can have is 100
    if(endNum <100){
        printf("Error, limit must be > 100.\n");
        exit(1);
    }

    printf("Count of Happy and Sad numbers from 1 to %d\n", endNum);
    printf("Please wait. Running...\n\n");

    //loop until we find happy numbers for every numbers
    while(i<=endNum){
        //create the desired amount of threads 
        for(k=1; k<=threads; k++){
            i++;
            pthread_create(&thd[k], NULL, calculation, (void*)(intptr_t)i);

            if(i>endNum)//at any point if limit is reached, stop creating threads 
                break;
        }
        for(int x=1; x<k; x++){
            pthread_join(thd[x], NULL);//join all the threads that have been created
        }
    }
        printf("Count of Happy Numbers: %d\n", happyNum);
        printf("Count of Sad Numbrs: %d\n", endNum-happyNum);

    exit(0);
}
