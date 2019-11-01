#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//prototypes
void merge_sort(int *lr);   // l should be first index, r should be last index of sub array.     // so to sort call (0,n-1)
void merge(int,int); //l,r,
int read_input();//returns number of elements read
// global variables

int a[(int)1e7]={0};  //10 million elements

int main()
{


    int numOfitems=read_input();

    int last_idx=numOfitems-1; // last index will be passed to the function


    int lr[2]={0,last_idx}; //initializing the arguments for the sort function, to pass it to pthread_create
    pthread_t topthread;
    pthread_create(&topthread,NULL,merge_sort,lr); //passing null so the thread has default attributes
    pthread_join(topthread,0); // wait for the the sort to end then continue the program or else it will be wrong
    for(int i=0;i<numOfitems;i++){
        printf("%d ",a[i]);
    }

    return 0;
}
void merge_sort(int *lr){ // pass array containing indices of leftmost, and rightmost items respectively at 0 and 1
    int l=lr[0];//left at 0
    int r=lr[1];//right at 1
    if(l<r) //the base case is l==r : exactly one element so dont divide
    {
        int mid=(l+r)/2;  //divide

        int llr[2]={l,mid}; //argument for left half

        int lrr[2]={mid+1,r};//argument for right half
        //merge_sort(llr);//left half

        pthread_t lthread;
        pthread_t rthread;
        //two threads for left and right halves
        pthread_create(&lthread,NULL,merge_sort,llr);
        //merge_sort(lrr);//right half
        pthread_create(&rthread,NULL,merge_sort,lrr);

        pthread_join(lthread,0);  //stop this thread until the left thread completes if it didnot end already
        pthread_join(rthread,0);  // wait until right finishes
        merge(l,r);
    }
}
void merge(int l,int r){
    int mid=(l+r)/2;
    //int n = r-l+1;  don't need it
    int lsize=mid-l+1;
    int rsize=r-(mid+1)+1;
    //lsize+rsize= n
    int leftar[lsize];     //left subarray from left till mid
    int rightar[rsize]; //right subarray from mid+1  till r
    //copy left half and right halves to their corresponding sub arrays\

    int r_idx,l_idx;
    r_idx=l_idx=0;
    for(int i = l;i<=r;i++){//from l to r inclusive
        if(i<=mid){  //copy left sub array
            leftar[l_idx++]=a[i];
        }else{//copy right sub array from mid+1
            rightar[r_idx++]=a[i];
        }
    }

    r_idx=l_idx=0; //reset counters
    int c=l;//copy back starting from index L in the original array

    for(;l_idx<lsize && r_idx<rsize;){  //could be a while loop but i wanted to use multiple indices seperated by commas but I failed
        if(leftar[l_idx] <rightar[r_idx]){
            a[c++]=leftar[l_idx++];
        }else{
            a[c++]=rightar[r_idx++];
        }
    }
    for(;l_idx<lsize;l_idx++){ //append the remaining elements in the left sub array to the original array
        a[c++]=leftar[l_idx];
    }
    for(;r_idx<rsize;r_idx++){ //append the remaining elements in the left subarray  to the original array
        a[c++]=rightar[r_idx];
    }


}
int read_input(){ // I don't really need a function

    int n;
    FILE *fptr = fopen("input.txt","r");
    fscanf(fptr,"%d\n",&n); // read number of items first
    for(int idx= 0;idx<n;idx++){ //read exactly n items
        fscanf(fptr,"%d ",&a[idx]);
    }
    fclose(fptr);  //close the file
    free(fptr);//free the pointer
    return n; // return number of elements

}
