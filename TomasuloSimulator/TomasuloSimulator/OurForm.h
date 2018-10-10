#include </afs/cs.pitt.edu/usr0/jih127/private/TomasuloSimulator/Global/ADT/CircularQueue.h>
#include </afs/cs.pitt.edu/usr0/jih127/private/TomasuloSimulator/Global/DataTypes.h>
#include <limits.h>
#include <stdio.h>


typedef struct _ROBRow{
        int* Key;
        int busy;
        struct instruction* instruction;
        int state; //assue 0 - Not start anything, 1-issue, 2-execute, 3-write result, 4-commit
        int destination;
        struct INTReg* intRR;
        struct FPReg* fpRR;  //RR represent renaming register
//      intRR->intResult = INT_MIN; //?lack type judge???
//      fpRR->fpRestult = 1.0/0.0;              
 }robRow;
/*
typedef struct ReserSROW{
    int* Key;
    char name;
   // OpCode op;
    int busy;
    
    INTReg* intVjRR;
    INTReg* intVkRR;
    FPReg* fpVjRR;
    FPReg* fpVkRR;
    
    int Qj;
    int Qk;
    
    int dest;
    int A;

}RSRow2;

typedef struct RegSROW{
    int* Field;
    int reorderNum;
    int busy;
}regsRow2;

*/
