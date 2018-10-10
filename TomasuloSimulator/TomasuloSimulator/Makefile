Target =  structTest

SRCS = structTest.c
OBJS = $(SRCS:.c=.o)

HEAD = ./Global/ADT/DictionaryRSRow.h ./Global/ADT/Dictionary.h ./Global/ADT/DictionaryRegsRow.h ./Global/ADT/CircularQueue.h ./Global/DataTypes.h ./OurForm.h ./Global/ADT/DictionaryBTB.h

$(Target): $(OBJS)
	@echo "transfer" $(OBJS) "into" $(Target)
	gcc -o $@ $^

$(OBJS):structTest.c $(HEAD)
	gcc -c -g $<

clean:
	rm -rf $(Target) 
	find . -name "*.o"  | xargs rm -f
