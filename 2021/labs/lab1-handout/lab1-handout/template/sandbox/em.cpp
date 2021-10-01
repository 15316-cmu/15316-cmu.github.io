
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "pin.H"

VOID ImageLoad(IMG img, VOID *v)
{
    // Iterate through each section of the image that is being loaded into memory
    for(SEC sec= IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec) ) {
        
        // Iterate through each routine in the current section
        for(RTN rtn = SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn)) {

            RTN_Open(rtn);

            // Iterate through each instruction in the current routine
            for( INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins) ) {             

                // Instrument individual instructions here
            }

            // Instrument procedure calls and returns here

            RTN_Close(rtn);            
        }
    }
}

//
// Handle system call entry here
//
VOID SyscallEntry(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, VOID *v) 
{
}

//
// Handle system call return here
//
VOID SyscallExit(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, VOID *v)
{
}

//
// Place any cleanup code in here
//
VOID Fini(INT32 code, VOID *v)
{
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */  
INT32 Usage()
{
    PIN_ERROR( "This Pintool enforces a sandbox policy on the interpreter \n" 
              + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
    if (PIN_Init(argc, argv)) return Usage();

    // Read symbol data from the binary to support targeted
    // instrumentation of specific functions
    PIN_InitSymbols();

    // Every time a binary image is loaded into memory, call ImageLoad
    IMG_AddInstrumentFunction(ImageLoad, 0);
    // Each time a system call is invoked, call SyscallEntry
    PIN_AddSyscallEntryFunction(SyscallEntry, 0);
    // Each time a system call returns, call SyscallExit
    PIN_AddSyscallExitFunction(SyscallExit, 0);    
    // When the application exits, call Fini
    PIN_AddFiniFunction(Fini, 0);

    PIN_StartProgram();
    
    return 0;
}
