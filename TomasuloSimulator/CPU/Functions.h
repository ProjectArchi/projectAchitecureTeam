/*tools for EXE and WB stages
*/

# include "../Global/TomasuloSimulator.h"

//Inner stage buffers for EXE/WB
typedef struct wbBuffer {
	int intFlag;		 //1 represent it contains a result, 0 represent not, same in the following
	int intResult;
//	int intDest;
	RSVRow *intRSV;
	
	int multFlag;
	int multResult;
//	int multDest;
	RSVRow *multRSV
	
	int lsFlag;		//0---no value; 1---Load Int; 2---Load float; -1---save int; -2---save float
	int lsIntResult;
	double lsFpResult;
//	int lsDest;
	int lsAddr;
	RSVRow *lsRSV;

	int fpaddFlag;
	double fpaddResult;
//	int fpaddDest;
	RSVRow *fpaddRSV;
	
	int fpmultFlag;
	double fpmultResult;
//	int fpmultDest;
	RSVRow *fpmultRSV;
	
	int fpdivFlag;
	double fpdivResult;
//	int fpdivDest;
	RSVRow *fpdivRSV;

	int buFlag;
	int buResult;
//	int buDest;
	RSVRow *buRSV
}wbBuffer;

//Unit count for unit latency;
typedef struct unit{
	int count;
	RSVRow *rsv;
}Unit;
CircularQueue *INT = createCircularQueue (1);
CircularQueue *MULT = createCircularQueue (4);
CircularQueue *LS = createCircularQueue (1);
CircularQueue *FPADD = createCircularQueue (3);
CircularQueue *FPMULT = createCircularQueue (4);
CircularQueue *FPDIV = createCircularQueue (1);
CircularQueue *BU = createCircularQueue (1);



//Commit function
void commit(){
	while(CDB > 0 && getHeadCircularQueue(ROB)->Busy == 1 && getHeadCircularQueue(ROB)->state == 1){
		getHeadCircularQueue(ROB)->Busy == 0;
		getHeadCircularQueue(ROB)->state == 0;
		h = dequeueCircular(ROB);
		Instruction *ins = h -> Instruction -> op;
		int rr = h -> RenamingRegister;
		CDB--;
		if(ins == BEQZ || ins == BNEZ || ins == BEQ || ins == BNE) {
			//clean when branch mis-predicted
			if (intRR[rr] != h -> Instructions -> predictFlag){
				//flush ROB
				int i = ROB -> head;
				while(i != tail){
					free(items[i]);
					i = (i + 1) % ROB -> size;
					ROB -> count--;
				}
				free(items[tail]);
				tail = head;
				//flush Register Status
				for (i = 0; i < 32; i++){
					intRenamingRegister[i] -> busy = 0;
					fpRenamingRegister[i] -> busy = 0;
				}
				//Move PC to right position
				if (intRR[rr] == 0){
					PC = h -> Instruction -> Immediate;
				}
				if (intRR[rr] == -1){
					PC = h -> Instruction -> InstructionPC + 4;
				}
			}
		} else if (ins == SD){
			int data = cpu -> intRenamingRegister[rr];
			*((int*)addrPtr) = h -> Destination;
			removeDictionaryEntriesByKey (dataCache, addrPtr);

			*((double*)valuePtr) = data;
			addDictionaryEntry (dataCache, addrPtr, valuePtr);
		} else if (ins == S_D){
			double data = cpu -> fpRenamingRegister[rr];
                        *((int*)addrPtr) = h -> Destination;
                        removeDictionaryEntriesByKey (dataCache, addrPtr);

                        *((double*)valuePtr) = data;
                        addDictionaryEntry (dataCache, addrPtr, valuePtr);
		} else if (ins == ADD.D || ins == SUB.D || ins == MUL.D || ins == DIV.D || ins == L.D){
			fpRegister[h -> Destination] = fpRenamingRegister[rr];
			int ad = h -> Destination;//silly one
			fpRegisterStatus[ad] -> Busy = 0;
		} else {
			intRegister[h -> Destination] = intRenamingRegister[rr];//silly one
			intRegisterStatus[ad] -> Busy = 0;
		}
		free(h);
	}
}

void writeBack(){
	if (CDB > 0 && wbBuffer -> intFlag == 1){
		int Res = wbBuffer -> intResult;
		int Dest = wbBuffer -> intRSV -> Destination;
		wbBuffer -> intRSV -> Busy = 0;
		ROB -> items[Dest] -> State = 1;
		intRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
		DictionaryRSVRowEntry *tmp = RSV -> head;
		while(tmp != NULL){
			if(tmp -> value -> qj == Dest){
				tmp -> value -> vj = Res;
				tmp -> value -> qj = 0;
			}
			if(tmp -> value -> qk == Dest){
                                tmp -> value -> vk = Res;
                                tmp -> value -> qk = 0;
                        }
			tmp == tmp -> next;
		}
		CDB--;
		wbBuffer -> intFlag = 0;
		Unit *unit = dequeueCircular(INT);
		free(unit);
	}
	if (CDB > 0 && wbBuffer -> multFlag == 1){
		int Res = wbBuffer -> multResult;
                int Dest = wbBuffer -> multRSV -> Destination;
                wbBuffer -> multRSV -> Busy = 0;
		ROB -> items[Dest] -> State = 1;
                intRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
                DictionaryRSVRowEntry *tmp = RSV -> head;
                while(tmp != NULL){
                        if(tmp -> value -> qj == Dest){
                                tmp -> value -> vj = Res;
                                tmp -> value -> qj = 0;
                        }
                        if(tmp -> value -> qk == Dest){
                                tmp -> value -> vk = Res;
                                tmp -> value -> qk = 0;
                        }
                        tmp == tmp -> next;
                }
                CDB--;
		wbBuffer -> multFlag = 0;
		Unit *unit = dequeueCircular(MULT);
		free(unit);
	}
	if (CDB > 0 && wbBuffer -> lsFlag != 0){
		if (lsFlag == 1){
			int Res = wbBuffer -> lsIntResult;
			int Dest = wbBuffer -> lsRSV -> Destination;
			wbBuffer -> lsRSV -> Busy = 0;
			ROB -> items[Dest] -> State = 1;
                	intRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
                	DictionaryRSVRowEntry *tmp = RSV -> head;
                	while(tmp != NULL){
                        	if(tmp -> value -> qj == Dest){
                                	tmp -> value -> vj = Res;
                                	tmp -> value -> qj = 0;
                        	}
                        	if(tmp -> value -> qk == Dest){
                                	tmp -> value -> vk = Res;
                                	tmp -> value -> qk = 0;
                        	}
                        	tmp == tmp -> next;
                	}
		}
		if (lsFlag == 2){
			double Res = wbBuffer -> lsFpResult;
			int Dest = wbBuffer -> lsRSV -> Destination;
                        wbBuffer -> lsRSV -> Busy = 0;
                        ROB -> items[Dest] -> State = 1;
                        fpRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
                        DictionaryRSVRowEntry *tmp = RSV -> head;
                        while(tmp != NULL){
                                if(tmp -> value -> qj == Dest){
                                        tmp -> value -> vj = Res;
                                        tmp -> value -> qj = 0;
                                }
                                if(tmp -> value -> qk == Dest){
                                        tmp -> value -> vk = Res;
                                        tmp -> value -> qk = 0;
                                }
                                tmp == tmp -> next;
                        }
		}
		if (lsFlag == -1){
			int Res = wbBuffer -> lsIntResult;
			int Dest = wbBuffer -> lsRSV -> Destination;
			int Addr = wbBuffer -> lsAddr;
			wbBuffer -> lsRSV -> Busy = 0;
			ROB -> items[Dest] -> State = 1;
			ROB -> items[Dest] -> Destination = lsAddr;
                        intRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
		}
		if (lsFlag == -2){
                        double Res = wbBuffer -> lsFpResult;
                        int Dest = wbBuffer -> lsRSV -> Destination;
			int Addr = wbBuffer -> lsAddr;
                        wbBuffer -> lsRSV -> Busy = 0;
                        ROB -> items[Dest] -> State = 1;
			ROB -> items[Dest] -> Destination = lsAddr;
                        fpRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
                }
		CDB--;
		wbBuffer -> lsFlag = 0;
		Unit *unit = dequeueCircular(LS);
		free(unit);
	}
	
	if(CDB > 0 && wbBuffer -> fpaddFlag == 1){
		double Res = wbBuffer -> fpaddResult;
                int Dest = wbBuffer -> fpaddRSV -> Destination;
                wbBuffer -> fpaddRSV -> Busy = 0;
                ROB -> items[Dest] -> State = 1;
                fpRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
                DictionaryRSVRowEntry *tmp = RSV -> head;
                while(tmp != NULL){
                        if(tmp -> value -> qj == Dest){
                                tmp -> value -> vj = Res;
                                tmp -> value -> qj = 0;
                        }
                        if(tmp -> value -> qk == Dest){
                                tmp -> value -> vk = Res;
                                tmp -> value -> qk = 0;
                        }
                        tmp == tmp -> next;
                }
                CDB--;
		wbBuffer -> fpaddFlag = 0;
		Unit *unit = dequeueCircular(FPADD);
		free(unit);
	}

	if (CDB > 0 && wbBuffer -> fpmultFlag == 1 ){
		double Res = wbBuffer -> fpmultResult;
                int Dest = wbBuffer -> fpmultRSV -> Destination;
                wbBuffer -> fpmultRSV -> Busy = 0;
                ROB -> items[Dest] -> State = 1;
                fpRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
                DictionaryRSVRowEntry *tmp = RSV -> head;
                while(tmp != NULL){
                        if(tmp -> value -> qj == Dest){
                                tmp -> value -> vj = Res;
                                tmp -> value -> qj = 0;
                        }
                        if(tmp -> value -> qk == Dest){
                                tmp -> value -> vk = Res;
                                tmp -> value -> qk = 0;
                        }
                        tmp == tmp -> next;
                }
                CDB--;
		wbBuffer -> fpmultFlag = 0;
		Unit *unit = dequeueCircular(FPMULT);
		free(unit);
	}
	
	if (CDB > 0 && wbBuffer -> fpdivFlag == 1 ){
                double Res = wbBuffer -> fpdivResult;
                int Dest = wbBuffer -> fpdivRSV -> Destination;
                wbBuffer -> fpdivRSV -> Busy = 0;
                ROB -> items[Dest] -> State = 1;
                fpRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
                DictionaryRSVRowEntry *tmp = RSV -> head;
                while(tmp != NULL){
                        if(tmp -> value -> qj == Dest){
                                tmp -> value -> vj = Res;
                                tmp -> value -> qj = 0;
                        }
                        if(tmp -> value -> qk == Dest){
                                tmp -> value -> vk = Res;
                                tmp -> value -> qk = 0;
                        }
                        tmp == tmp -> next;
                }
                CDB--;
		wbBuffer -> fpdivFlag = 0;
		Unit *unit = dequeueCircular(FPDIV);
		free(unit);
        }

	if (CDB > 0 && wbBuffer -> buFlag == 1){
		int Res = wbBuffer -> buResult;
		int Dest = wbBuffer -> buRSV -> Destination;;
		wbBuffer -> buRSV -> Busy = 0;
		ROB -> items[Dest] -> State = 1;
                intRenamingRegister[ROB -> items[Dest] -> RenamingRegister] = Res;
		CDB--;
		wbBuffer -> buFlag = 0;
		Unit *unit = dequeueCircular(BU);
		free(unit);
	}
}



void executeStage(){

	DictionaryEntry *dataCacheElement;
	
	DictionaryRSVRowEntry *tmpEntry;
	RSVRow *tmpRSV;

	//INT unit execution
	if(getCountCircularQueue(INT) == 0){
		//Check input
		tmpEntry = RSV -> head;
                int i;
                for(i = 0; i < 4; i++){
                        if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                Unit *newINT = (Unit *)malloc(sizeof(Unit));
                                newInt -> count = 1;
                                newInt -> rsv = tmpEntry -> value;
                                enqueueCircular(INT, newINT);
                                break;
                        }
                        tmpEntry = tmpEntry -> next;
                }
	}
	//do execution
	if (getCountCircularQueue(INT) > 0)
		if(getHeadCircularQueue(INT) -> count == 1 && wbBuffer -> intFlag == 0){
			tmpRSV = getHeadCircualrQueue(INT) -> rsv;
			wbBuffer -> intRSV = tmpRSV;
			wbBuffer -> intFlag = 1;
			switch(tmpRSV -> op){
				case ANDI:
					wbBuffer -> intResult = tmpRSV -> vj & tmpRSV -> A;
					break;
				case AND:
					wbBuffer -> intResult = tmpRSV -> vj & tmpRSV -> vk;
					break;
				case ORI:
					wbBuffer -> intResult = tmpRSV -> vj | tmpRSV -> A;
					break;
				case OR:
					wbBuffer -> intResult = tmpRSV -> vj | tmpRSV -> vk;
					break;
				case SLTI:
					wbBuffer -> intResult = tmpRSV -> vj < tmpRSV -> A ? 1 : 0;
					break;
				case SLTI:
					wbBuffer -> intResult = tmpRSV -> vj < tmpRSV -> vk ? 1 : 0;
					break;
				case DADDI:
					wbBuffer -> intResult = tmpRSV -> vj + tmpRSV -> A;
					break;
				case DADD:
					wbBuffer -> intResult = tmpRSV -> vj + tmpRSV -> vk;
					break;
				case DSUB:
					wbBuffer -> intResult = tmpRSV -> vj - tmpRSV -> vk;
					break;
			}
		}
	}

	//MULT unit execution
	if(getCountCircularQueue(MULT) > 0){
		if(getHeadCircularQueue(MULT) -> count < 4){
			//do count++
			int i = MULT -> head;
			int k = getCountCircularQueue(MULT);
			while(k > 0){
				MULT -> items[i] -> count++;
				i = (i + 1) % MULT -> size;
				k--;
			}
			//check unit output
			if(getHeadCircularQueue(MULT) -> count = 4){
				tmpRSV = getHeadCircularQueue(MULT) -> rsv;
                        	wbBuffer -> multRSV = tmpRSV;
                        	wbBuffer -> multResult = tmpRSV -> vj * tmpRSV -> vk;
				wbBuffer -> multFlag = 1;
			}
			//check unit input
			tmpEntry = RSV -> head;
			for(i = 0; i < 4; i++){
				tmpEntry = empEntry -> next;
			}
			for(i = 0; i < 2; i++){
				if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                	Unit *newMULT = (Unit *)malloc(sizeof(Unit));
                                	newMULT -> count = 1;
                                	newMULT -> rsv = tmpEntry -> value;
                                	enqueueCircular(MULT, newMULT);
                                	break;
                        	}
                        	tmpEntry = tmpEntry -> next;
			}
		}
	} else {
		//check unit input
                        tmpEntry = RSV -> head;
                        for(i = 0; i < 4; i++){
                                tmpEntry = empEntry -> next;
                        }
                        for(i = 0; i < 2; i++){
                                if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                        Unit *newMULT = (Unit *)malloc(sizeof(Unit));
                                        newMULT -> count = 1;
                                        newMULT -> rsv = tmpEntry -> value;
                                        enqueueCircular(MULT, newMULT);
                                        break;
                                }
                                tmpEntry = tmpEntry -> next;
                        }
	}



	//FPadd unit execution
	if(getCountCircularQueue(FPADD) > 0){
		if(getHeadCircularQueue(FPADD) -> count < 3){
			//do count++
			int i = FPADD -> head;
			int k = getCountCircularQueue(FPADD);
			while(k > 0){
				FPADD -> items[i] -> count++;
				i = (i + 1) % FPADD -> size;
				k--;
			}
			//check unit output
			if(getHeadCircularQueue(FPADD) -> count = 3){
				tmpRSV = getHeadCircularQueue(FPADD) -> rsv;
                        	wbBuffer -> fpaddRSV = tmpRSV;
				switch(tmpRSV -> op){
					case ADD.D: 
						wbBuffer -> fpaddResult = tmpRSV -> vj + tmpRSV -> vk;
						break;
					case SUB.D:
                                                wbBuffer -> fpaddResult = tmpRSV -> vj - tmpRSV -> vk;
                                                break;
				}
				wbBuffer -> fpaddFlag = 1;
			}
			//check unit input
			tmpEntry = RSV -> head;
			for(i = 0; i < 10; i++){
				tmpEntry = empEntry -> next;
			}
			for(i = 0; i < 3; i++){
				if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                	Unit *newFPADD = (Unit *)malloc(sizeof(Unit));
                                	newFPADD -> count = 1;
                                	newFPADD -> rsv = tmpEntry -> value;
                                	enqueueCircular(FPADD, newFPADD);
                                	break;
                        	}
                        	tmpEntry = tmpEntry -> next;
			}
		}
	} else {
		//check unit input
		tmpEntry = RSV -> head;
                        for(i = 0; i < 10; i++){
                                tmpEntry = empEntry -> next;
                        }
                        for(i = 0; i < 3; i++){
                                if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                        Unit *newFPADD = (Unit *)malloc(sizeof(Unit));
                                        newFPADD -> count = 1;
                                        newFPADD -> rsv = tmpEntry -> value;
                                        enqueueCircular(FPADD, newFPADD);
                                        break;
                                }
                                tmpEntry = tmpEntry -> next;
			}
	}	
	


	
	//FPmult unit execution
        if(getCountCircularQueue(FPMULT) > 0){
                if(getHeadCircularQueue(FPMULT) -> count < 4){
                        //do count++
                        int i = FPMULT -> head;
                        int k = getCountCircularQueue(FPMULT);
                        while(k > 0){
                                FPMULT -> items[i] -> count++;
                                i = (i + 1) % FPMULT -> size;
                                k--;
                        }
                        //check unit output
                        if(getHeadCircularQueue(FPMULT) -> count = 3){
                                tmpRSV = getHeadCircularQueue(FPMULT) -> rsv;
                                wbBuffer -> fpmultRSV = tmpRSV;
                                wbBuffer -> fpmultResult = tmpRSV -> vj * tmpRSV -> vk;
                                wbBuffer -> fpmultFlag = 1;
                        }
                        //check unit input
                        tmpEntry = RSV -> head;
                        for(i = 0; i < 13; i++){
                                tmpEntry = empEntry -> next;
                        }
                        for(i = 0; i < 4; i++){
                                if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                        Unit *newFPMULT = (Unit *)malloc(sizeof(Unit));
                                        newFPMULT -> count = 1;
                                        newFPMULT -> rsv = tmpEntry -> value;
                                        enqueueCircular(FPMULT, newFPMULT);
                                        break;
                                }
                                tmpEntry = tmpEntry -> next;
                        }
                }
        } else {
                //check unit input
                tmpEntry = RSV -> head;
                for(i = 0; i < 13; i++){
                	tmpEntry = empEntry -> next;
                }
		for(i = 0; i < 4; i++){
			if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
				Unit *newFPMULT = (Unit *)malloc(sizeof(Unit));
				newFPMULT -> count = 1;
				newFPMULT -> rsv = tmpEntry -> value;
				enqueueCircular(FPMULT, newFPMULT);
				break;
			}
			tmpEntry = tmpEntry -> next;
		}
	}
		
	//FPdiv unit execution
	if(getCountCircularQueue(FPDIV) > 0){
		if(getHeadCircularQueue(FPDIV) -> count < 8){
                	getHeadCircularQueue(FPDIV) -> count++;
			if(getHeadCircularQueue(FPDIV) -> count == 8){
                        	tmpRSV = getHeadCircualrQueue(FPDIV) -> rsv;
                        	wbBuffer -> fpdivRSV = tmpRSV;
				wbBuffer -> fpdivResult = tmpRSV -> vj / tmpRSV -> vk;
                        	wbBuffer -> fpdivFlag = 1;	
			}
		}
	} else {
		//check unit input
                tmpEntry = RSV -> head;
                for(i = 0; i < 17; i++){
                        tmpEntry = empEntry -> next;
                }
                for(i = 0; i < 2; i++){
                        if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                Unit *newFPDIV = (Unit *)malloc(sizeof(Unit));
                                newFPDIV -> count = 1;
                                newFPDIV -> rsv = tmpEntry -> value;
                                enqueueCircular(FPDIV, newFPDIV);
                                break;
                        }
                        tmpEntry = tmpEntry -> next;
                }
	}

	
	//LS unit execution
	if (getCountCircularQueue(LS) > 0){
		if(getHeadCircularQueue(LS) -> count == 1 && wbBuffer -> lsFlag == 0){
			tmpRSV = getHeadCircularQueue(LS) -> rsv;
			wbBuffer -> lsRSV = tmpRSV
			switch(tmpRSV -> op){
				case LD:
					wbBuffer -> lsAddr = tmpRSV -> vj + rmpRSV -> A;
					//Check memory address confliction
					int conflict = 0;
					int i = ROB -> head;
					while(i != (ROB -> tail + 1) % ROB -> size){
						if (ROB -> items[i] -> Instruction -> op == SD && ROB -> items[i] -> Destination == wbBuffer -> lsAddr){
							conflict = 1;
							break;
						}
						i++;
					}
					//load value if no confliction
					if (conflict == 0){
						wbBuffer -> lsFlag = 1;
						*((int*)addrPtr) = wbBuffer -> lsAddr;
						dataCacheElement = getValueChainByDictionaryKey (dataCache, addrPtr);
		
						valuePtr = dataCacheElement -> value -> value;
						wbBuffer -> lsIntResult =  (int) *((double*)valuePtr);
					}
					break;
				case L_D:
					wbBuffer -> lsAddr = tmpRSV -> vk + rmpRSV -> A;
                                        //Check memory address confliction
					int conflict = 0;
                                        int i = ROB -> head;
                                        while(i != (ROB -> tail + 1) % ROB -> size){
                                                if (ROB -> items[i] -> Instruction -> op == S_D && ROB -> items[i] -> Destination == wbBuffer -> lsAddr){
                                                        conflict = 1;
                                                        break;
                                                }
                                                i++;
                                        }
					//load value if no confliction
                                        if (conflict == 0){
                                                wbBuffer -> lsFlag = 2;
                                                *((int*)addrPtr) = wbBuffer -> lsAddr;
                                                dataCacheElement = getValueChainByDictionaryKey (dataCache, addrPtr);

                                                valuePtr = dataCacheElement -> value -> value;
                                                wbBuffer -> lsFpResult = *((double*)valuePtr);
                                        }
                                        break;
				case SD:
					wbBuffer -> lsAddr = tmpRSV -> vj + rmpRSV -> A;
					wbBuffer -> lsIntResult = tmpRSV -> vk;
					wbBuffer -> lsFlag = -1;
					break;
				case S_D:
                                        wbBuffer -> lsAddr = tmpRSV -> vj + rmpRSV -> A;
                                        wbBuffer -> lsFpResult = tmpRSV -> vk;
                                        wbBuffer -> lsFlag = -2;
                                        break;
			}
		}
	} else {
		//check unit input
                tmpEntry = RSV -> head;
                for(i = 0; i < 6; i++){
                        tmpEntry = empEntry -> next;
                }
                for(i = 0; i < 4; i++){
                        if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                Unit *newLS = (Unit *)malloc(sizeof(Unit));
                                newLS -> count = 1;
                                newLS -> rsv = tmpEntry -> value;
                                enqueueCircular(LS, newLS);
                                break;
                        }
                        tmpEntry = tmpEntry -> next;
                }
	}	
	
	//BU unit execution
	if (getCountCircularQueue(BU) > 0){
		if(getHeadCircularQueue(BU) -> count == 1 && wbBuffer -> lsFlag == 0){
			tmpRSV = getHeadCircularQueue(BU) -> rsv;
			wbBuffer -> lsRSV = tmpRSV;
			//get result 0---taken -1---not taken
			switch(tmpRSV -> op){
				case BNE:
					wbBuffer -> buResult = tmpRSV -> vj != tmpRSV -> vk ? 0 : -1;
					break;
				case BNEZ:
					wbBuffer -> buResult = tmpRSV -> vj != 0 ? 0 : -1;
					break;
				case BEQ:
                                        wbBuffer -> buResult = tmpRSV -> vj == tmpRSV -> vk ? 0 : -1;
                                        break;
                                case BEQZ:
                                        wbBuffer -> buResult = tmpRSV -> vj == 0 ? 0 : -1;
					break;
			}
			//Update BTB
			if (wbBuffer -> buResult == 0 && ROB -> items[tmpRSV -> Destination] -> Instruction -> predictFlag == -1){
				int i;
				for (i = 0; i < 16; i++){
					if (BTB[i] -> InstructionPC == -1){
						BTB[i] -> InstructionPC = ROB -> items[tmpRSV -> Destination] -> Instruction -> InstructionPC;
						BTB[i] -> PredictedPC = tmpRSV -> A;
						break;
					}
				}
			}
			if (wbBuffer -> buResult == -1 && ROB -> items[tmpRSV -> Destination] -> Instruction -> predictFlag == 0){
                                int i;
                                for (i = 0; i < 16; i++){
                                        if (BTB[i] -> InstructionPC == ROB -> items[tmpRSV -> Destination] -> Instruction -> InstructionPC){
                                                BTB[i] -> InstructionPC = -1;
                                                BTB[i] -> PredictedPC = -1;
                                                break;
                                        }
                                }
                        }
		}
	} else {
		//check unit input
                tmpEntry = RSV -> head;
                for(i = 0; i < 19; i++){
                        tmpEntry = empEntry -> next;
                }
                for(i = 0; i < 2; i++){
                        if (tmpEntry -> value -> Busy == 1 && tmpEntry -> value -> qj == 0 && tmpEntry -> value -> qk == k){
                                Unit *newBU = (Unit *)malloc(sizeof(Unit));
                                newBU -> count = 1;
                                newBU -> rsv = tmpEntry -> value;
                                enqueueCircular(BU, newBU);
                                break;
                        }
                        tmpEntry = tmpEntry -> next;
                }
	}
	
}
