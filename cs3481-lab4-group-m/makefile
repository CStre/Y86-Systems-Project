CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = PipeReg.o F.o D.o E.o M.o W.o FetchStage.o DecodeStage.o ExecuteStage.o \
MemoryStage.o WritebackStage.o PipeRegField.o yess.o Simulate.o Memory.o Tools.o \
RegisterFile.o ConditionCodes.o Loader.o
INC = ../include

.C.o:
	$(CC) $(CFLAGS) $< -o $@

yess: $(OBJ) 

PipeReg.o: PipeReg.C PipeReg.h

F.o: F.C F.h PipeRegField.h PipeReg.h

D.o: D.C D.h Instructions.h RegisterFile.h PipeReg.h PipeRegField.h Status.h

E.o: E.C E.h RegisterFile.h Instructions.h PipeRegField.h PipeReg.h Status.h

M.o: M.C M.h RegisterFile.h Instructions.h PipeRegField.h PipeReg.h Status.h

W.o: W.C W.h RegisterFile.h Instructions.h PipeRegField.h PipeReg.h Status.h

FetchStage.o: FetchStage.C FetchStage.h RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h \
			W.h Stage.h Status.h Debug.h

DecodeStage.o: DecodeStage.C DecodeStage.h RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h \
			W.h Stage.h Status.h Debug.h

ExecuteStage.o: ExecuteStage.C ExecuteStage.h RegisterFile.h PipeRegField.h PipeReg.h F.h D.h \
			M.h W.h Stage.h Status.h Debug.h

MemoryStage.o: MemoryStage.C MemoryStage.h RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h \
			W.h Stage.h MemoryStage.h Status.h Debug.h

WritebackStage.o: WritebackStage.C WritebackStage.h RegisterFile.h PipeRegField.h PipeReg.h F.h \
				D.h M.h W.h Stage.h Status.h Debug.h

PipeRegField.o: PipeRegField.C PipeRegField.h

yess.o: yess.C Debug.h Memory.h Loader.h RegisterFile.h ConditionCodes.h PipeReg.h Stage.h \
		Simulate.h

Simulate.o: Simulate.C Simulate.h PipeRegField.h PipeReg.h F.h D.h E.h M.h W.h Stage.h \
			ExecuteStage.h MemoryStage.h DecodeStage.h FetchStage.h WritebackStage.h Simulate.h \
			Memory.h RegisterFile.h ConditionCodes.h

Memory.o: Memory.h Tools.h

Tools.o: Tools.h

RegisterFile.o: RegisterFile.C RegisterFile.h Tools.h 

ConditionCodes.o: ConditionCodes.C ConditionCodes.h Tools.h 

Loader.o: Loader.C Loader.h Memory.h

clean:
	rm $(OBJ)

run:
	make clean
	make yess
	./run.sh
