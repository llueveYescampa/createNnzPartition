#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "real.h"

int main(int argc, char *argv[]) 
{
    int worldSize;
    
    if (argc > 1 ) {
        worldSize=atoi(argv[1]);
    } else {
        printf("Use: %s  number_of_processes \n", argv[0]);       
        exit(0);
    } // endif //

    int global_n,myVoid,global_nnz;
    int *rows;
    int *nnzPP;
    
    myVoid = scanf("%d %d %d", &global_n, &global_n, &global_nnz);
    rows = (int *) malloc((global_n+1)*sizeof(int));    
    nnzPP = (int *) malloc(worldSize*sizeof(int));    
    
    real temp1;
    int temp2;
    
    printf("%d, %d\n", global_n, 1);
    int rowVal = 0;
    rows[0] = 0;
    
    for (int i=0; i<global_nnz; ++i) {
        scanf("%d %d %lf", &myVoid, &temp2, &temp1);
        if (myVoid != rowVal) {
            rows[myVoid]=i;
            rowVal=myVoid;            
        } // end if //
    } // end for //
    rows[global_n]=global_nnz;
    
        
    float nnzIncre = (float ) global_nnz/ (float) worldSize;
    float lookingFor=nnzIncre;
    int startRow=0, endRow;
    int partition=0;    

    for (int row=0; row<global_n; ++row) {    
        if ( (float) rows[row+1] >=  lookingFor ) { 
            // search for smallest difference
            if ( ( rows[row+1] - lookingFor)  <=  lookingFor - rows[row]   ) {
                endRow = row;
            } else {
                endRow = row-1;
            } // end if //
            nnzPP[partition] = rows[endRow+1] - rows[startRow];
            printf("for partition %4d, start row: %4d, end row: %4d, this partition: %4d rows and %4d non-zeros\n", partition, startRow, endRow, (endRow-startRow+1), nnzPP[partition] );
            startRow = endRow+1;
            ++partition;
            
            if (partition<worldSize-1) {
               lookingFor += nnzIncre;
            } else {
                lookingFor=global_nnz;
            } // end if //   
        } // end if // 
    } // end for //
    
    free(rows);
    free(nnzPP);

    return 0;    
} // end main() //
