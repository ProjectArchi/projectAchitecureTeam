/*
 * DictionaryRegsRow.h
 *
 *  Created on: Oct 2, 2015
 *      Author: DebashisGanguly
 */

#ifndef GLOBAL_ADT_DICTIONARY_H_
#define GLOBAL_ADT_DICTIONARY_H_

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

}RegsRow;
*/


typedef struct _RegSROW{
    int* Key;
    int reorderNum;
    int busy;
}RegsRow;



typedef int (*_getHashCode)(int *key);
typedef int (*_compareValues)(void *value1, void *value2);

typedef struct _dictionaryValue {
	void *value;
	struct _dictionaryValue *next;
} DictionaryRegsRowValue;

typedef struct _dictionaryRegsRowEntry {
	int hashKey;
	int *key;
	RegsRow *value;
	void *next;
} DictionaryRegsRowEntry;

typedef struct _dictionaryRegsRow {
	DictionaryRegsRowEntry *head;
	_getHashCode getHashCode;
	_compareValues compareValues;
} DictionaryRegsRow;

static inline DictionaryRegsRow *createDictionaryRegsRow (_getHashCode getHashCode, _compareValues compareValues) {
	DictionaryRegsRow *table = (DictionaryRegsRow *)malloc (sizeof(DictionaryRegsRow));
	table -> head = NULL;
	table -> getHashCode = getHashCode;
	table -> compareValues = compareValues;
	return table;
}


static inline DictionaryRegsRowValue *createDictionaryRegsRowValue (void *value)  {
	DictionaryRegsRowValue *newValue = (DictionaryRegsRowValue *)malloc (sizeof(DictionaryRegsRowValue));
	newValue -> value = value;
	newValue -> next = NULL;
	return newValue;
}

static inline DictionaryRegsRowEntry *createDictionaryRegsRowEntry (DictionaryRegsRow *table, int *key, RegsRow *value) {
	DictionaryRegsRowEntry *newEntry = (DictionaryRegsRowEntry *)malloc (sizeof(DictionaryRegsRowEntry));
	newEntry -> hashKey = table -> getHashCode (key);
	newEntry -> key = key;
	newEntry -> value = value;
	newEntry -> next = NULL;
	printf("Entry Address is %d\n", newEntry);
	return newEntry;
}

static inline DictionaryRegsRowEntry *getValueChainByDictionaryRegsRowKey (DictionaryRegsRow *table, int *key) {
	DictionaryRegsRowEntry *match = NULL;
	DictionaryRegsRowEntry *current;

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

static inline void addValueToDictionaryRegsRowChain (DictionaryRegsRowEntry *existing, void *value, _compareValues compareValues) {
	DictionaryRegsRowValue *previous;
	DictionaryRegsRowValue *current;
	DictionaryRegsRowValue *match = NULL;

	for (current = existing -> value; current != NULL; previous = current, current = current -> next)
		if (compareValues (value, current -> value) == 0) {
			match = current;
		}

	if (match == NULL)
		previous -> next = createDictionaryRegsRowValue (value);
}

static inline void addDictionaryRegsRowEntry (DictionaryRegsRow *table, int *key, RegsRow *value) {
	DictionaryRegsRowEntry *existing;

	if (table != NULL) {
		if (table -> head == NULL) {
			table -> head = createDictionaryRegsRowEntry(table, key, value);
		} else {
			if ((existing = getValueChainByDictionaryRegsRowKey(table, key)) != NULL) {
				addValueToDictionaryRegsRowChain (existing, value, table -> compareValues);
			} else {
				DictionaryRegsRowEntry *previous;
				DictionaryRegsRowEntry *current;

				for (current = table -> head; current != NULL; previous = current, current = current -> next);

				previous -> next = createDictionaryRegsRowEntry (table, key, value);
			}
		}
	}
}

static inline void removeDictionaryRegsRowEntriesByKey (DictionaryRegsRow *table, int *key) {
	DictionaryRegsRowEntry *previous;
	DictionaryRegsRowEntry *current;
	DictionaryRegsRowEntry *match = NULL;

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
static inline void removeDictionaryRegsRowEntryByKeyValue (DictionaryRegsRow *table, int *key, void *value) {
	DictionaryRegsRowEntry *matchedEntry = getValueChainByDictionaryRegsRowKey(table, key);

	DictionaryRegsRowValue *previous;
	DictionaryRegsRowValue *current;
	DictionaryRegsRowValue *match = NULL;

	if (matchedEntry != NULL) {
		for (current = matchedEntry -> value; current != NULL; previous = current, current = current -> next)
			if (table -> compareValues (value, current -> value) == 0) {
				match = current;
				break;
			}

		if (match != NULL) {
			if (match == matchedEntry -> value) {
				if (match -> next == NULL)
					removeDictionaryRegsRowEntriesByKey (table, key);
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
#endif /* GLOBAL_ADT_DICTIONARY_H_ */
