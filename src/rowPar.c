#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "real.h"

#define ROWLOW(id,p,n)  ((id)*(n)/(p))
#define ROWHIGH(id,p,n) (ROWLOW((id)+1,p,n)-1)
#define ROWSIZE(id,p,n) (ROWLOW((id)+1,p,n)-ROWLOW(id,p,n)) // eblack


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


    for (int par=0; par<worldSize; ++par ) {
        int startRow = ROWLOW(par,worldSize, global_n);
        int endRow   = ROWHIGH(par,worldSize, global_n);
        int size     = ROWSIZE(par,worldSize, global_n);
        int nnzPerPar=rows[endRow+1] - rows[startRow];
        
        //int nnzPerPar=rows[ROWHIGH(par,worldSize, global_n) + 1] - rows[ROWLOW(par,worldSize, global_n)];
        printf("for partition %4d, start row: %8d, end row: %8d, this partition: %8d rows and %8d non-zeros\n", par, startRow, endRow, size, nnzPerPar );
    } // end for //
    free(rows);
    return 0;    
} // end main() //
