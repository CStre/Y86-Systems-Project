/**
 *      Lab Assignement 6
 *
 * Title:           DecodeStage
 * Files:           DecodeStage.C
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
#include "Instructions.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "E.h"
#include "Stage.h"
#include "DecodeStage.h"
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"

/*
 * doClockLow:
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register sets (F, D, E, M, W instances)
 * @param: stages - array of stages (FetchStage, DecodeStage, ExecuteStage,
 *         MemoryStage, WritebackStage instances)
 */
bool DecodeStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    D * dreg = (D *) pregs[DREG];
    E * ereg = (E *) pregs[EREG];
    uint64_t stat = (*dreg).getstat() -> getOutput(), 
        icode = (*dreg).geticode() -> getOutput(), 
        ifun = (*dreg).getifun() -> getOutput(), 
        valC = (*dreg).getvalC() -> getOutput(),
        valP = 0, valA = 0, valB = 0;
        //dstE = RNONE, dstM = RNONE, srcA = RNONE, srcB = RNONE;

        // Lab7 inclusion
        uint64_t dstE = controldstE(icode, dreg);
        uint64_t dstM = controldstM(icode, dreg);
        uint64_t srcA = controlsrcA(icode, dreg);
        uint64_t srcB = controlsrcB(icode, dreg);



   //code missing here to select the value of the PC
   //and fetch the instruction from memory
   //Fetching the instruction will allow the icode, ifun,
   //rA, rB, and valC to be set.
   //The lab assignment describes what methods need to be
   //written.

   //The value passed to setInput below will need to be changed
   //freg->getpredPC()->setInput(e_pc + 1);

   //provide the input values for the D register
   setEInput(ereg, stat, icode, ifun, valA, valB, valC, valP, dstE, dstM, srcA, srcB);
   return false;
}

/* doClockHigh
 * applies the appropriate control signal to the F
 * and D register intances
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
void DecodeStage::doClockHigh(PipeReg ** pregs)
{
   E * ereg = (E *) pregs[EREG];

   ereg->geticode()->normal();
   ereg->getifun()->normal();
   ereg->getvalC()->normal();
   ereg->getvalA()->normal();
   ereg->getvalB()->normal();
   ereg->getdstE()->normal();
   ereg->getdstM()->normal();
   ereg->getsrcA()->normal();
   ereg->getsrcB()->normal();
}

/* setEInput
 * provides the input to potentially be stored in the E register
 * during doClockHigh
 *
 * @param: ereg - pointer to the E register instance
 * @param: stat - value to be stored in the stat pipeline register within E
 * @param: icode - value to be stored in the icode pipeline register within E
 * @param: ifun - value to be stored in the ifun pipeline register within E
 * @param: valC - value to be stored in the valC pipeline register within E
 * @param: valA - value to be stored in the valA pipeline register within E
 * @param: valB - value to be stored in the valB pipeline register within E
 * @param: dstE - value to be stored in the dstE pipeline register within E
 * @param: dstM - value to be stored in the dstM pipeline register within E
 * @param: srcA - value to be stored in the srcA pipeline register within E
 * @param: srcB - value to be stored in the srcB pipeline register within E
*/
void DecodeStage::setEInput(E * ereg, uint64_t stat, uint64_t icode, uint64_t ifun, uint64_t valA, 
                            uint64_t valB, uint64_t valC, uint64_t valP, uint64_t dstE, uint64_t dstM, 
                            uint64_t srcA, uint64_t srcB) 
{
    ereg->getstat()->setInput(stat);
    ereg->geticode()->setInput(icode);
    ereg->getifun()->setInput(ifun);
    ereg->getvalC()->setInput(valC);
    ereg->getvalA()->setInput(valP);
    ereg->getvalB()->setInput(valB);
    ereg->getdstE()->setInput(dstE);
    ereg->getdstM()->setInput(dstM);
    ereg->getsrcA()->setInput(srcA);
    ereg->getsrcB()->setInput(srcB);
}

/*
HCL for srcA component

    word d_srcA = [
    D_icode in { IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ } : D_rA;
    D_icode in { IPOPQ, IRET } : RSP;
    1: RNONE;  //no register needed
];
*/
uint64_t DecodeStage::controlsrcA(uint64_t D_icode, D * dreg)
{
    uint64_t d_srcA = 0;
    uint64_t D_rA = dreg->getrA()->getOutput();
    switch (D_icode)
    {
        case IRRMOVQ:
        case IRMMOVQ:
        case IOPQ:
        case IPUSHQ:
            d_srcA = D_rA;
            return d_srcA;
        case IPOPQ:
        case IRET:
            d_srcA = RSP;
            return d_srcA;
        default:
            d_srcA = RNONE;
            return d_srcA;
    }
}

/*
//HCL for srcB component

word d_srcB = [

D_icode in { IOPQ, IRMMOVQ, IMRMOVQ } : D_rB;

D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RSP;

1: RNONE;   //no register needed

];
*/
uint64_t DecodeStage::controlsrcB(uint64_t D_icode, D * dreg)
{
    uint64_t d_srcB = 0;
    uint64_t D_rB = dreg->getrB()->getOutput();
    switch (D_icode)
    {
        case IOPQ:
        case IRMMOVQ:
        case IMRMOVQ:
            d_srcB = D_rB;
            return d_srcB;
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            d_srcB = RSP;
            return d_srcB;
        default:
            d_srcB = RNONE;
            return d_srcB;
    }
}

/*
//HCL for dstE component

word d_dstE = [

D_icode in { IRRMOVQ, IIRMOVQ, IOPQ} : D_rB;

D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RSP;

1: RNONE;

];
*/
uint64_t DecodeStage::controldstE(uint64_t D_icode, D * dreg)
{
    uint64_t d_dstE = 0;
    uint64_t D_rB = dreg->getrB()->getOutput();
    switch (D_icode)
    {
        case IRRMOVQ:
        case IIRMOVQ:
        case IOPQ:
            d_dstE = D_rB;
            return d_dstE;
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            d_dstE = RSP;
            return d_dstE;
        default:
        d_dstE = RNONE;
        return d_dstE;
    }
}

/*
//HCL for dstM component

word d_dstM = [

D_icode in { IMRMOVQ, IPOPQ } : D_rA;

1: RNONE;

];
*/
uint64_t DecodeStage::controldstM(uint64_t D_icode, D * dreg)
{
    uint64_t d_dstM = 0;
    uint64_t D_rA = dreg->getrA()->getOutput();
    switch (D_icode)
    {
        case IMRMOVQ:
        case IPOPQ:
            d_dstM = D_rA;
            return d_dstM;
        default:
            d_dstM = RNONE;
            return d_dstM;
    }
}

/*
//HCL for Sel+FwdA

word d_valA = [

1: d_rvalA;   //value from register file

];
*/
uint64_t DecodeStage::controlselFwdA(uint64_t D_icode, D * dreg)
{
    uint64_t d_valA = 0;
    return d_valA;
}

uint64_t DecodeStage::controlFwdB(uint64_t D_icode, D * dreg)
{
    uint64_t d_valA = 0;
    return d_valA;
}