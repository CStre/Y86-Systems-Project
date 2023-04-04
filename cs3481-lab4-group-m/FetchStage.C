/**
 *      Lab Assignement 6
 *
 * Title:           FetchStage
 * Files:           FetchStage.C
 * Semester:        Spring 2023
 * Course:          CS_3481
 * Professor:       Dr. Hamza
 * 
 * @author:         Bryan Trejo,
 *                  Collin Streitman
 * 
 * Group Name:      Group M
 * @version:        4/04/2023
 */

#include <string>
#include <cstdint>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Memory.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "Instructions.h"
#include "Stage.h"
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"
#include "Tools.h"


/*
 * doClockLow:
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register sets (F, D, E, M, W instances)
 * @param: stages - array of stages (FetchStage, DecodeStage, ExecuteStage,
 *         MemoryStage, WritebackStage instances)
 */
bool FetchStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];
   M * mreg = (M *) pregs[MREG];
   W * wreg = (W *) pregs[WREG];
   uint64_t f_pc = 0, icode = 0, ifun = 0, valC = 0, valP = 0;
   uint64_t rA = RNONE, rB = RNONE, stat = SAOK;

   //code missing here to select the value of the PC
   //and fetch the instruction from memory
   //Fetching the instruction will allow the icode, ifun,
   //rA, rB, and valC to be set.
   //The lab assignment describes what methods need to be
   //written.

   bool error = false;
   f_pc = selectPC(freg, mreg, wreg);
   uint64_t bytes = Memory::getInstance()->getByte(f_pc, error);
   if (!error)
   {
      icode = bytes >> 4;
      ifun = bytes & 0xf;
   }
   valP = PCincrement(f_pc, needRegIds(icode), needValC(icode));

   //The value passed to setInput below will need to be changed
   freg->getpredPC()->setInput(predictPC(icode, valC, valP));

   //provide the input values for the D register
   setDInput(dreg, stat, icode, ifun, rA, rB, valC, valP);
   return false;
}

/* doClockHigh
 * applies the appropriate control signal to the F
 * and D register intances
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
void FetchStage::doClockHigh(PipeReg ** pregs)
{
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];

   freg->getpredPC()->normal();
   dreg->getstat()->normal();
   dreg->geticode()->normal();
   dreg->getifun()->normal();
   dreg->getrA()->normal();
   dreg->getrB()->normal();
   dreg->getvalC()->normal();
   dreg->getvalP()->normal();
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(D * dreg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->getstat()->setInput(stat);
   dreg->geticode()->setInput(icode);
   dreg->getifun()->setInput(ifun);
   dreg->getrA()->setInput(rA);
   dreg->getrB()->setInput(rB);
   dreg->getvalC()->setInput(valC);
   dreg->getvalP()->setInput(valP);
}

uint64_t FetchStage::selectPC(F * freg, M * mreg, W * wreg)
{
   // word f_pc = [
   //    M_icode == IJXX && !M_Cnd : M_valA;
   //    W_icode == IRET : W_valM;
   //    1: F_predPC;
   // ];

   //uint64_t icode = mreg->geticode()->getOutput();
   uint64_t W_icode = wreg->geticode()->getOutput();
   uint64_t M_Cnd = mreg->getCnd()->getOutput();
   uint64_t M_valA = mreg->getvalA()->getOutput();
   uint64_t W_valM = wreg->getvalM()->getOutput();
   uint64_t F_predPC = freg->getpredPC()->getOutput();
   
   uint64_t M_icode = 0;

   if(M_icode == IJXX && !M_Cnd)
   {
      return M_valA;
   } 
   else if(W_icode == IRET)
   {
      return W_valM;
   }
   else
   {
      return F_predPC;
   }
}

bool FetchStage::needRegIds(uint64_t f_icode)
{
   //bool need_regids = f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ };

   if(f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ || f_icode == IRRMOVQ 
      || f_icode == IOPQ || f_icode == IPUSHQ || f_icode == IPOPQ)
      {
         return true;
      }
      else
      {
         return false;
      }
}

bool FetchStage::needValC(uint64_t f_icode)
{
   //bool need_valC = f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };

   if(f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ || f_icode == IJXX || 
      f_icode == ICALL)
      {
         return true;
      }
      else
      {
         return false;
      }
}

uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP)
{
   // word f_predPC = [
   //    f_icode in { IJXX, ICALL } : f_valC;
   //    1: f_valP;
   // ] ;

   if(f_icode == IJXX || f_icode == ICALL)
   {
      return f_valC;
   }
   else
   {
      return f_valP;
   }
}

uint64_t FetchStage::PCincrement(uint64_t f_pc, bool regID, bool valC)
{

   if(regID == true && valC ==true)
   {
      return f_pc += 0xa;
   } 
   else if(regID == false && valC ==false)
   {
      return f_pc += 0x1;
   }
   else if (regID == true && valC == false)
   {
      return f_pc += 0x2; 
   } 
   else if (regID == false && valC == true)
   {
      return f_pc += 0x9; 
   }
   else 
      return 1;
}
     