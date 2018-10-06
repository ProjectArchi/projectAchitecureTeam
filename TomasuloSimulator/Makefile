Target =  OurToma

#source file
SRCS = TomasuloSimulator.c ./Global/Utils/PrintUtility.c ./CacheEmulator/EmulateCache.c ./CPU/CPU.c

# *.o file (compile file)
#OBJS = TomasuloSimulator.o ./Global/Utils/PrintUtility.o ./CacheEmulator/EmulateCache.o ./CPU/CPU.o
OBJS = $(SRCS:.c=.o)
OBJSToma = TomasuloSimulator.o
#OBJSPrint = ./Global/Utils/PrintUtility.o
#OBJSCache = ./CacheEmulator/EmulateCache.o
#OBJSCPU = ./CPU/CPU.o

#partial *.h (partial head file besides Toma.h)
HEAD = ./Global/ADT/Dictionary.h ./Global/ADT/CircularQueue.h ./Global/DataTypes.h

$(Target): $(OBJS)
	@echo "transfer" $(OBJS) "into" $(Target)
	gcc -o $@ $^

TomasuloSimulator.c: TomasuloSimulator.c ./Global/TomasuloSimulator.h
	@echo "transfer TomasuloSimulator.c into" $(OBJSToma)
	gcc -c $<

#PrintUtility.o: ./Global/Utils/PrintUtility.c $(HEAD)
#	@echo "transfer PrintUtility.c" into $(OBJSPrint)
#	gcc -c $<

#EmulateCache.o: ./CacheEmulator/EmulateCache.c $(HEAD)
#	@echo "transfer EmulateCache.c into" $(OBJSCache)
#	gcc -c $<

#CPU.o: ./CPU/CPU.c $(HEAD)
#	@echo "transfer CPU.c into" $(OBJSCPU)
#	gcc -c $<

clean:
	rm -rf $(Target) 
	find . -name "*.o"  | xargs rm -f
