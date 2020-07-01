#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "real.h"

#define LOW(id,p,n)  ((id)*(n)/(p))
#define HIGH(id,p,n) (LOW((id)+1,p,n)-1)
#define SIZE(id,p,n) (LOW((id)+1,p,n)-LOW(id,p,n)) // eblack


int main(int argc, char *argv[]) 
{
    int worldSize;
    
    if (argc > 1 ) {
        worldSize=atoi(argv[1]);
    } else {
        printf("Use: %s  number_of_processes \n", argv[0]);       
        exit(0);
    } // endif //

    int global_n,global_nnz;
    int *rows;
    scanf("%d %d %d", &global_n, &global_n, &global_nnz);
    rows = (int *) malloc((global_n+1)*sizeof(int));    
    real temp1;
    int temp2;

    int currentRow;
    printf("%d, %d\n", global_n, 1);

    // creating the row pointer vector     
    int previousRow = 0;
    rows[0] = 0;
    scanf("%d %d %lf", &currentRow, &temp2, &temp1);
    previousRow=currentRow;
    int nnzPerRow=1;
    for (int i=1; i<global_nnz; ++i) {
        scanf("%d %d %lf", &currentRow, &temp2, &temp1);
        if (currentRow == previousRow) {
            ++nnzPerRow;    
        } else {
            rows[previousRow+1]=nnzPerRow;
            nnzPerRow=1;
            previousRow=currentRow;
        } // end if //
    } // end for //
    rows[global_n]=nnzPerRow;
    for (int row=0; row<=global_n; ++row) {
        rows[row+1] += rows[row];
    } // end for //
    
    ////////////////   begining of actual rutine ///////////////////////////
    int *rowsPerSet=(int *) malloc(worldSize*sizeof(int));
    int lowRow=0, upRow;
    int reducedWorldSize= worldSize;
    int reducedNnz=global_nnz;
    int nnzLimit = rows[lowRow] + SIZE(0,reducedWorldSize, reducedNnz);
    int partition=0;    


    for (int row = 0; row<global_n; ++row) {
        if ( rows[row+1] >=  nnzLimit ) { 
            if ( ( rows[row+1] - nnzLimit)  <=  nnzLimit - rows[row]   ) {
                upRow = row;
            } else {
                upRow = row-1;
            } // end if //
            rowsPerSet[partition] = upRow-lowRow+1;
            
            printf("for partition %4d, start row: %8d, end row: %8d, this partition: %8d rows and %8d non-zeros\n", 
                                                     partition, 
                                                     lowRow, 
                                                     upRow, 
                                                     rowsPerSet[partition], 
                                                     rows[upRow+1]-rows[lowRow] );
            
            reducedNnz -=  (rows[upRow+1]-rows[lowRow]);
            --reducedWorldSize;
            lowRow=upRow+1;
            if (partition < worldSize-1 ) nnzLimit= rows[lowRow] + SIZE(0,reducedWorldSize, reducedNnz);
            ++partition;
        } // end if // 
        
    } // end for //
    free(rowsPerSet);
    free(rows);
    ////////////////   end of actual rutine ///////////////////////////
    return 0;    
} // end main() //
