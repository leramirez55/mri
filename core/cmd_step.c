/* Copyright 2012 Adam Green (http://mbed.org/users/AdamGreen/)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/* Handler for single step gdb command. */
#include "platforms.h"
#include "cmd_common.h"
#include "cmd_continue.h"
#include "cmd_registers.h"
#include "cmd_step.h"


static uint32_t justAdvancedPastBreakpoint(uint32_t continueReturn);
/* Handle the 's' command which is sent from gdb to tell the debugger to single step over the next instruction in the
   currently halted program.
   
    Command Format:     sAAAAAAAA
    Response Format:    Blank until the next exception, at which time a 'T' stop response packet will be sent.

    Where AAAAAAAA is an optional value to be used for the Program Counter when restarting the program.
*/
uint32_t HandleSingleStepCommand(void)
{
    /* Single step is pretty much like continue except processor is told to only execute 1 instruction. */
    if (justAdvancedPastBreakpoint(HandleContinueCommand()))
    {
        /* Treat the advance as the single step and don't resume execution. */
        return Send_T_StopResponse();
    }

    Platform_EnableSingleStep();

    return (HANDLER_RETURN_RESUME_PROGRAM | HANDLER_RETURN_RETURN_IMMEDIATELY);
}

static uint32_t justAdvancedPastBreakpoint(uint32_t continueReturn)
{
    return continueReturn & HANDLER_RETURN_SKIPPED_OVER_BREAK;
}
