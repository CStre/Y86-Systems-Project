/**
 *      Lab Assignement 6
 *
 * Title:           MemoryStage
 * Files:           MemoryStage.C
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

#include <iostream>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "E.h"
#include "Stage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "WritebackStage.h"
#include "Status.h"
#include "Debug.h"

bool MemoryStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    M * mreg = (M *) pregs[MREG];
    W * wreg = (W *) pregs[WREG];

    uint64_t stat = (*mreg).getstat()->getOutput(),
            icode = (*mreg).geticode()->getOutput(),
            valE = (*mreg).getvalE()->getOutput(),
            dstE = (*mreg).getdstM()->getOutput(),
            dstM = (*mreg).getdstE()->getOutput(),
            valM = 0;
                        
    setWInput(wreg, stat, icode, valM, valE, dstE, dstM);
    return false;
}

void MemoryStage::doClockHigh(PipeReg ** pregs)
{
    M * mreg = (M *) pregs[MREG];
    W * wreg = (W *) pregs[WREG];
          
    mreg->getstat()->normal();
    mreg->geticode()->normal();
    mreg->getvalE()->normal();
    mreg->getdstE()->normal();
    mreg->getdstM()->normal();
    mreg->getvalA()->normal();   

    wreg->getstat()->normal();
    wreg->geticode()->normal();
    wreg->getvalE()->normal();
    wreg->getvalM()->normal();
    wreg->getdstE()->normal();
    wreg->getdstM()->normal();
}

void MemoryStage::setWInput(W * wreg, uint64_t stat, uint64_t icode, uint64_t valM,
    uint64_t valE, uint64_t dstE, uint64_t dstM) 
{
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalE()->setInput(valE);
    wreg->getvalM()->setInput(valM);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);
}
