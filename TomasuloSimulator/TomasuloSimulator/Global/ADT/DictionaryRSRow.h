/*
 * DictionaryRSRow.h
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

/*
typedef struct _RegSROW{
    int* Field;
    int reorderNum;
    int busy;
}RegsRow;
*/


typedef int (*_getHashCode)(int *key);
typedef int (*_compareValues)(void *value1, void *value2);

typedef struct _dictionaryValue {
	void *value;
	struct _dictionaryValue *next;
} DictionaryRSRowValue;

typedef struct _dictionaryRSRowEntry {
	int hashKey;
	int *key;
	RSRow *value;
	void *next;
} DictionaryRSRowEntry;

typedef struct _dictionaryRSRow {
	DictionaryRSRowEntry *head;
	_getHashCode getHashCode;
	_compareValues compareValues;
} DictionaryRSRow;

static inline DictionaryRSRow *createDictionaryRSRow (_getHashCode getHashCode, _compareValues compareValues) {
	DictionaryRSRow *table = (DictionaryRSRow *)malloc (sizeof(DictionaryRSRow));
	table -> head = NULL;
	table -> getHashCode = getHashCode;
	table -> compareValues = compareValues;
	return table;
}


static inline DictionaryRSRowValue *createDictionaryRSRowValue (void *value)  {
	DictionaryRSRowValue *newValue = (DictionaryRSRowValue *)malloc (sizeof(DictionaryRSRowValue));
	newValue -> value = value;
	newValue -> next = NULL;
	return newValue;
}

static inline DictionaryRSRowEntry *createDictionaryRSRowEntry (DictionaryRSRow *table, int *key, RSRow *value) {
	DictionaryRSRowEntry *newEntry = (DictionaryRSRowEntry *)malloc (sizeof(DictionaryRSRowEntry));
	newEntry -> hashKey = table -> getHashCode (key);
	newEntry -> key = key;
	newEntry -> value = value;
	newEntry -> next = NULL;
//	printf("Entry Address is %d\n", newEntry);
	return newEntry;
}

static inline DictionaryRSRowEntry *getValueChainByDictionaryRSRowKey (DictionaryRSRow *table, int *key) {
	DictionaryRSRowEntry *match = NULL;
	DictionaryRSRowEntry *current;

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

static inline void addValueToDictionaryRSRowChain (DictionaryRSRowEntry *existing, void *value, _compareValues compareValues) {
	DictionaryRSRowValue *previous;
	DictionaryRSRowValue *current;
	DictionaryRSRowValue *match = NULL;

	for (current = existing -> value; current != NULL; previous = current, current = current -> next)
		if (compareValues (value, current -> value) == 0) {
			match = current;
		}

	if (match == NULL)
		previous -> next = createDictionaryRSRowValue (value);
}

static inline void addDictionaryRSRowEntry (DictionaryRSRow *table, int *key, RSRow *value) {
	DictionaryRSRowEntry *existing;

	if (table != NULL) {
		if (table -> head == NULL) {
			table -> head = createDictionaryRSRowEntry(table, key, value);
		} else {
			if ((existing = getValueChainByDictionaryRSRowKey(table, key)) != NULL) {
				addValueToDictionaryRSRowChain (existing, value, table -> compareValues);
			} else {
				DictionaryRSRowEntry *previous;
				DictionaryRSRowEntry *current;

				for (current = table -> head; current != NULL; previous = current, current = current -> next);

				previous -> next = createDictionaryRSRowEntry (table, key, value);
			}
		}
	}
}

static inline void removeDictionaryRSRowEntriesByKey (DictionaryRSRow *table, int *key) {
	DictionaryRSRowEntry *previous;
	DictionaryRSRowEntry *current;
	DictionaryRSRowEntry *match = NULL;

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
static inline void removeDictionaryRSRowEntryByKeyValue (DictionaryRSRow *table, int *key, void *value) {
	DictionaryRSRowEntry *matchedEntry = getValueChainByDictionaryRSRowKey(table, key);

	DictionaryRSRowValue *previous;
	DictionaryRSRowValue *current;
	DictionaryRSRowValue *match = NULL;

	if (matchedEntry != NULL) {
		for (current = matchedEntry -> value; current != NULL; previous = current, current = current -> next)
			if (table -> compareValues (value, current -> value) == 0) {
				match = current;
				break;
			}

		if (match != NULL) {
			if (match == matchedEntry -> value) {
				if (match -> next == NULL)
					removeDictionaryRSRowEntriesByKey (table, key);
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
