#include </afs/cs.pitt.edu/usr0/jih127/private/TomasuloSimulator/OurForm.h>
//#include </afs/cs.pitt.edu/usr0/jih127/private/TomasuloSimulator/Global/ADT/DictionaryRSRow.h>
//#include </afs/cs.pitt.edu/usr0/jih127/private/TomasuloSimulator/Global/ADT/DictionarySaveCorrect.h>
//#include </afs/cs.pitt.edu/usr0/jih127/private/TomasuloSimulator/Global/ADT/DictionaryRegsRow.h>
#include <stdio.h>

int getHashCode(int* key){

	return *key;
}

int compareValues(void* value1, void* value2){
	if(value1 == value2){
		return 0;
	}
	else{
		return 1;
	}
}

/*
int* createRSRow(int* Key){
	RSRow* res = (RSRow*)malloc(sizeof(RSRow));
	res->Key = Key;
	res->busy = 0;
	res->Qj = -1;
	res->Qk = -1;
	res->dest = -1;
	res->A = -1;
	return res;	
}
*/

RegsRow* createRegsRow(int* Key){
	RegsRow* res = (RegsRow*)malloc(sizeof(RegsRow));
	res->Key = Key;
	res->reorderNum = -1;
	res->busy = 0;
	return res;
}


RSRow* createRSRow(int* Key){
        RSRow* res = (RSRow*)malloc(sizeof(RSRow));
        res->Key = Key;
        res->busy = 0;
        res->Qj = -1;
        res->Qk = -1;
        res->dest = -1;
        res->A = -1;
        return res;     
}



void main(){
	CircularQueue *ROB;
	int UpperSizeOfROB = 4;
	int OperNumth = 2;
	ROB = createCircularQueue(UpperSizeOfROB); //create size = UpperSizeROB CircularQueue
	robRow *rRow2;
	robRow *rRow0 = malloc (sizeof(robRow));
	robRow *rRow1 = malloc (sizeof(robRow)); 
	rRow2 = malloc (sizeof(robRow)); 
	robRow *rRow3 = malloc (sizeof(robRow));
	robRow *rRowOper = malloc (sizeof(robRow));
	printf("The entry number of the %d is %d\n ", OperNumth, rRow2->Key);
	enqueueCircular(ROB, rRow0);
	enqueueCircular(ROB, rRow1);
	enqueueCircular(ROB, rRow2);
	enqueueCircular(ROB, rRow3);
	
	rRowOper = getNumthCircularQueue(ROB, OperNumth); //return a pointer which is the OperNumth Row in ROB
	rRowOper->Key = OperNumth;
	printf("After revision, the entry number of the %d is %d\n ", OperNumth, rRow2->Key);
 	
	printf("Test result is:\n");
	printf("%d\n", getCountCircularQueue(ROB));
//}



//void main(){
	int a0 = 0;
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	RSRow* rsRow0 = createRSRow(&a0);
	RSRow* rsRow1 = createRSRow(&a1);
	RSRow* rsRow2 = createRSRow(&a2);
	RSRow* rsRow3 = createRSRow(&a3);
	printf("value = %d,KeyAddress = %d\n", *(rsRow3->Key), rsRow3->Key);
	printf("rsRow0 value = %d\n", *(rsRow0->Key));
	RSRow* rsRow4 = createRSRow(&a4);
	DictionaryRSRow* RS = createDictionaryRSRow(getHashCode, compareValues);
	DictionaryRSRowEntry* Oper_rsRow = createDictionaryRSRowEntry(RS, rsRow4->Key, rsRow4);

	//DictionaryRSRowEntry* rsEntry0 = createDictionaryRSRowEntry(RS, rsRow0->Key, rsRow0);
	//printf(" hashkey is %d\n", rsEntry0->hashKey);
	//printf("busy is %d\n", rsEntry0->value->busy);
	//printf("Qj is %d\n", rsEntry0->value->Qj);
	//DictionaryRSRowEntry* rsEntry1 = createDictionaryRSRowEntry(RS, rsRow1->Key, rsRow1);
	//DictionaryRSRowEntry* rsEntry2 = createDictionaryRSRowEntry(RS, rsRow2->Key, rsRow2);
	//DictionaryRSRowEntry* rsEntry3 = createDictionaryRSRowEntry(RS, rsRow3->Key, rsRow3);
	//DictionaryRSRowEntry* rsEntry4 = createDictionaryRSRowEntry(RS, rsRow4->Key, rsRow4);

	printf("rsRow0->Key value = %d, rsRow0->Key = %d\n", *(rsRow0->Key), rsRow0->Key);
	printf("rsRow3->Key value = %d, rsRow3->Key = %d\n", *(rsRow3->Key), rsRow3->Key);
	printf("RS address = %d\n", RS);
	addDictionaryRSRowEntry(RS, rsRow0->Key, rsRow0);
	addDictionaryRSRowEntry(RS, rsRow1->Key, rsRow1);
	addDictionaryRSRowEntry(RS, rsRow2->Key, rsRow2);
	addDictionaryRSRowEntry(RS, rsRow3->Key, rsRow3);
	addDictionaryRSRowEntry(RS, rsRow4->Key, rsRow4);

	printf("81\n");
	//return the rsRow with its responding key
	Oper_rsRow = getValueChainByDictionaryRSRowKey(RS, (rsRow0->Key));
	printf("Before get, Oper_rsEntry->value is %d, and rsRow0 busy is %d, and rsRow0 Qj is %d\n", *(Oper_rsRow->value->Key), rsRow0->busy, rsRow0->Qj);
	Oper_rsRow->value->busy = 1;
	Oper_rsRow->value->Qj = 93;
	printf("After get, Oper_rsEntry->value is %d, and rsRow0 busy is %d, and rsRow0 Qj is %d\n", *(Oper_rsRow->value->Key), rsRow0->busy, rsRow0->Qj);
	
	//test remove an entry from dictionary
	Oper_rsRow = getValueChainByDictionaryRSRowKey(RS, (rsRow3->Key));
	printf("%d\n", *(Oper_rsRow->value->Key));
	removeDictionaryRSRowEntriesByKey(RS, rsRow3->Key);
	Oper_rsRow = getValueChainByDictionaryRSRowKey(RS, (rsRow4->Key));
        printf("%d\n", *(Oper_rsRow->value->Key));

//	Oper_rsRow = getValueChainByDictionaryRSRowKey(RS, (rsRow3->Key));
//	printf("%d\n", *(Oper_rsRow->value->Key));
//}


//void main(){
        int b0 = 0;
        int b1 = 1;
        int b2 = 2;
        int b3 = 3;
        int b4 = 4;
        RegsRow* regsRow0 = createRegsRow(&b0);
        printf("value = %d,KeyAddress = %d\n", *(regsRow0->Key), regsRow0->Key);
        RegsRow* regsRow1 = createRegsRow(&b1);
        RegsRow* regsRow2 = createRegsRow(&b2);
        RegsRow* regsRow3 = createRegsRow(&b3);
        printf("value = %d,KeyAddress = %d\n", *(regsRow3->Key), regsRow3->Key);
        printf("regsRow0 value = %d\n", *(regsRow0->Key));
        RegsRow* regsRow4 = createRegsRow(&b4);
        DictionaryRegsRow* RegStatus = createDictionaryRegsRow(getHashCode, compareValues);
        DictionaryRegsRowEntry* Oper_regsRow = createDictionaryRegsRowEntry(RegStatus, regsRow4->Key, regsRow4);

        //DictionaryRegsRowEntry* rsEntry0 = createDictionaryRegsRowEntry(RS, regsRow0->Key, regsRow0);
        //printf(" hashkey is %d\n", rsEntry0->hashKey);
        //printf("busy is %d\n", rsEntry0->value->busy);
        //printf("Qj is %d\n", rsEntry0->value->Qj);
        //DictionaryRegsRowEntry* rsEntry1 = createDictionaryRegsRowEntry(RS, regsRow1->Key, regsRow1);
        //DictionaryRegsRowEntry* rsEntry2 = createDictionaryRegsRowEntry(RS, regsRow2->Key, regsRow2);
        //DictionaryRegsRowEntry* rsEntry3 = createDictionaryRegsRowEntry(RS, regsRow3->Key, regsRow3);
        //DictionaryRegsRowEntry* rsEntry4 = createDictionaryRegsRowEntry(RS, regsRow4->Key, regsRow4);                                                           

        printf("regsRow0->Key value = %d, regsRow0->Key = %d\n", *(regsRow0->Key), regsRow0->Key);
        printf("regsRow3->Key value = %d, regsRow3->Key = %d\n", *(regsRow3->Key), regsRow3->Key);
        printf("RS address = %d\n", RegStatus);
        addDictionaryRegsRowEntry(RegStatus, regsRow0->Key, regsRow0);
        addDictionaryRegsRowEntry(RegStatus, regsRow1->Key, regsRow1);
        addDictionaryRegsRowEntry(RegStatus, regsRow2->Key, regsRow2);
        addDictionaryRegsRowEntry(RegStatus, regsRow3->Key, regsRow3);
        addDictionaryRegsRowEntry(RegStatus, regsRow4->Key, regsRow4);
         
        printf("81\n");
        //return the regsRow with its responding key
        Oper_regsRow = getValueChainByDictionaryRegsRowKey(RegStatus, (regsRow0->Key));
        printf("Before get, Oper_rsEntry->value is %d, and regsRow0 busy is %d, and regsRow0 Qj is %d\n", *(Oper_regsRow->value->Key), regsRow0->busy, regsRow0->reorderNum);
        Oper_regsRow->value->busy = 1;
        Oper_regsRow->value->reorderNum = 102;
        printf("After get, Oper_rsEntry->value is %d, and regsRow0 busy is %d, and regsRow0 Qj is %d\n", *(Oper_regsRow->value->Key), regsRow0->busy, regsRow0->reorderNum);
                                                        
        //test remove an entry from dictionary
	Oper_regsRow = getValueChainByDictionaryRegsRowKey(RegStatus, (regsRow3->Key));
	printf("%d\n", *(Oper_regsRow->value->Key));
	removeDictionaryRegsRowEntriesByKey(RegStatus, regsRow3->Key);
	Oper_regsRow = getValueChainByDictionaryRegsRowKey(RegStatus, (regsRow4->Key));
	printf("%d\n", *(Oper_regsRow->value->Key));
	Oper_regsRow = getValueChainByDictionaryRegsRowKey(RegStatus, (regsRow3->Key));
	printf("%d\n", *(Oper_regsRow->value->Key));
}

