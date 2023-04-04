/**
 *      Lab Assignement 6
 *
 * Title:           ExecuteStage
 * Files:           ExecuteStage.C
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
#include <string>
#include <cstdint>
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

bool ExecuteStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    E * ereg = (E *) pregs[EREG];
    M * mreg = (M *) pregs[MREG];

    uint64_t stat = (*ereg).getstat()->getOutput(),
            icode = (*ereg).geticode()->getOutput(),
            dstE = (*ereg).getdstE()->getOutput(),
            dstM = (*ereg).getdstM()->getOutput(),
            valA = 0, valB = 0, Cnd = 0, valE = 0;

    
    setMInput(mreg, stat, icode, Cnd, valE, valA, valB, dstE, dstM);
    return false;
}

void ExecuteStage::doClockHigh(PipeReg ** pregs)
{
    E * ereg = (E *) pregs[EREG];
    M * mreg = (M *) pregs[MREG];
    
    ereg->getstat()->normal();
    ereg->geticode()->normal();
    ereg->getdstE()->normal();
    ereg->getdstM()->normal();

    mreg->getstat()->normal();
    mreg->geticode()->normal();
    mreg->getvalA()->normal();
    mreg->getCnd()->normal();
    mreg->getdstE()->normal();
    mreg->getdstM()->normal();
       
}

void ExecuteStage::setMInput(M * mreg, uint64_t stat, uint64_t icode, uint64_t Cnd,
    uint64_t valE, uint64_t valA, uint64_t valB, uint64_t dstE, uint64_t dstM) 
{
    mreg->getstat()->setInput(stat);
    mreg->geticode()->setInput(icode);
    mreg->getCnd()->setInput(Cnd);
    mreg->getvalE()->setInput(valE);
    mreg->getvalA()->setInput(valA);
    mreg->getdstE()->setInput(dstE);
    mreg->getdstM()->setInput(dstM);
}
