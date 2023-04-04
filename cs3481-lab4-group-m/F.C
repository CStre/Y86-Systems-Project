/**
 *      Lab Assignement 6
 *
 * Title:           F
 * Files:           F.C
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
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"


/*
 * F constructor
 *
 * initialize the F pipeline register 
*/
F::F()
{
   predPC = new PipeRegField();
}

/* return the predPC pipeline register field */
PipeRegField * F::getpredPC()
{
   return predPC;
}

/* 
 * dump
 *
 * outputs the current values of the F pipeline register
*/
void F::dump()
{
   dumpField("F: predPC: ", 3, predPC->getOutput(), true);
}
