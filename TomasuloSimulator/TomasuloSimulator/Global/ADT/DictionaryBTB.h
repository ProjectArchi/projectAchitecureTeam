/*
 * DictionaryBTBRow.h
 *
 *  Created on: Oct 2, 2015
 *      Author: DebashisGanguly
 */

//#ifndef GLOBAL_ADT_DICTIONARY_H_
//#define GLOBAL_ADT_DICTIONARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include </afs/cs.pitt.edu/usr0/jih127/private/TomasuloSimulator/OurForm.h>

/*
typedef struct _ReserSROW{
    int* Key;
    char name;
    struct OpCode* op;
    int busy;

    struct INTReg* intVjRR;
    struct INTReg* intVkRR;
    struct FPReg* fpVjRR;
    struct FPReg* fpVkRR;

    int Qj;
    int Qk;

    int dest;
    int A;
}RSRow;
*/

/*
typedef struct _RegSROW{
    int* Key;
    int reorderNum;
    int busy;
}RegsRow;
*/

typedef struct _BTBRow{
	int* Key;
	int FetchedPC;
	int PredictPC;
	int BitOfExtra;
}BTBRow;

//typedef int (*_getHashCode)(int *key);
//typedef int (*_compareValues)(void *value1, void *value2);

typedef struct _dictionaryBTBRowValue {
	void *value;
	struct _dictionaryValue *next;
} DictionaryBTBRowValue;

typedef struct _dictionaryBTBRowEntry {
	int hashKey;
	int *key;
	BTBRow *value;
	void *next;
} DictionaryBTBRowEntry;

typedef struct _dictionaryBTBRow {
	DictionaryBTBRowEntry *head;
	_getHashCode getHashCode;
	_compareValues compareValues;
} DictionaryBTBRow;

static inline DictionaryBTBRow *createDictionaryBTBRow (_getHashCode getHashCode, _compareValues compareValues) {
	DictionaryBTBRow *table = (DictionaryBTBRow *)malloc (sizeof(DictionaryBTBRow));
	table -> head = NULL;
	table -> getHashCode = getHashCode;
	table -> compareValues = compareValues;
	return table;
}


static inline DictionaryBTBRowValue *createDictionaryBTBRowValue (void *value)  {
	DictionaryBTBRowValue *newValue = (DictionaryBTBRowValue *)malloc (sizeof(DictionaryBTBRowValue));
	newValue -> value = value;
	newValue -> next = NULL;
	return newValue;
}

static inline DictionaryBTBRowEntry *createDictionaryBTBRowEntry (DictionaryBTBRow *table, int *key, BTBRow *value) {
	DictionaryBTBRowEntry *newEntry = (DictionaryBTBRowEntry *)malloc (sizeof(DictionaryBTBRowEntry));
	newEntry -> hashKey = table -> getHashCode (key);
	newEntry -> key = key;
	newEntry -> value = value;
	newEntry -> next = NULL;
	printf("Entry Address is %d\n", newEntry);
	return newEntry;
}

static inline DictionaryBTBRowEntry *getValueChainByDictionaryBTBRowKey (DictionaryBTBRow *table, int *key) {
	DictionaryBTBRowEntry *match = NULL;
	DictionaryBTBRowEntry *current;

	int hashKey = table -> getHashCode (key);
	if (table != NULL) {
		for (current = table -> head; current != NULL; current = current -> next)
			if (current -> hashKey == hashKey) {
				match = current;
				break;
			}
	}
	return match;
}

static inline void addValueToDictionaryBTBRowChain (DictionaryBTBRowEntry *existing, void *value, _compareValues compareValues) {
	DictionaryBTBRowValue *previous;
	DictionaryBTBRowValue *current;
	DictionaryBTBRowValue *match = NULL;

	for (current = existing -> value; current != NULL; previous = current, current = current -> next)
		if (compareValues (value, current -> value) == 0) {
			match = current;
		}

	if (match == NULL)
		previous -> next = createDictionaryBTBRowValue (value);
}

static inline void addDictionaryBTBRowEntry (DictionaryBTBRow *table, int *key, BTBRow *value) {
	DictionaryBTBRowEntry *existing;

	if (table != NULL) {
		if (table -> head == NULL) {
			table -> head = createDictionaryBTBRowEntry(table, key, value);
		} else {
			if ((existing = getValueChainByDictionaryBTBRowKey(table, key)) != NULL) {
				addValueToDictionaryBTBRowChain (existing, value, table -> compareValues);
			} else {
				DictionaryBTBRowEntry *previous;
				DictionaryBTBRowEntry *current;

				for (current = table -> head; current != NULL; previous = current, current = current -> next);

				previous -> next = createDictionaryBTBRowEntry (table, key, value);
			}
		}
	}
}

static inline void removeDictionaryBTBRowEntriesByKey (DictionaryBTBRow *table, int *key) {
	DictionaryBTBRowEntry *previous;
	DictionaryBTBRowEntry *current;
	DictionaryBTBRowEntry *match = NULL;

	int hashKey = table -> getHashCode (key);

	if (table != NULL) {
		for (current = table -> head; current != NULL; previous = current, current = current -> next) {
			if (current -> hashKey == hashKey) {
				match = current;
				break;
			}
		}

		if (match != NULL) {
			if (match == table -> head) {
				if (match -> next == NULL) {
					table -> head = NULL;
					free (match);
				} else {
					table -> head = table -> head -> next;
					free (match);
				}
			} else {
				previous -> next = match -> next;
				free (match);
			}
		}
	}
}
/*
static inline void removeDictionaryBTBRowEntryByKeyValue (DictionaryBTBRow *table, int *key, void *value) {
	DictionaryBTBRowEntry *matchedEntry = getValueChainByDictionaryBTBRowKey(table, key);

	DictionaryBTBRowValue *previous;
	DictionaryBTBRowValue *current;
	DictionaryBTBRowValue *match = NULL;

	if (matchedEntry != NULL) {
		for (current = matchedEntry -> value; current != NULL; previous = current, current = current -> next)
			if (table -> compareValues (value, current -> value) == 0) {
				match = current;
				break;
			}

		if (match != NULL) {
			if (match == matchedEntry -> value) {
				if (match -> next == NULL)
					removeDictionaryBTBRowEntriesByKey (table, key);
				else {
					matchedEntry -> value = matchedEntry -> value -> next;
					free (match);
				}
			} else {
				previous -> next = current -> next;
				free (matchedEntry -> value);
			}
		}
	}
}
*/
//#endif /* GLOBAL_ADT_DICTIONARY_H_ */
