// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END


#include "gfx/legato/common/legato_error.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#if LE_ASSERT_ENABLE == 1

#define LE_ERRORMSG_SIZE 196

static char errorMsg[LE_ERRORMSG_SIZE];

const char* leGetErrorMessage()
{
    return errorMsg;
}

void leSetErrorMessage(const char* msg)
{
    size_t len;

    errorMsg[0] = '\0';

    if(msg == NULL)
        return;

    len = strlen(msg);

    if(len >= LE_ERRORMSG_SIZE - 1)
    {
        memcpy(errorMsg, msg, LE_ERRORMSG_SIZE - 1);
        errorMsg[LE_ERRORMSG_SIZE - 1] = '\0';
    }
    else
    {
        strcpy(errorMsg, msg);
    }

//#ifdef WIN32
    printf("%s\n", errorMsg);
//#endif

    while(1){ }
}

void leSprintfErrorMessage(const char* fmt, ...)
{
    va_list(args);
    va_start(args, fmt);

    vsnprintf(errorMsg, LE_ERRORMSG_SIZE, fmt, args);

    va_end(args);

//#ifdef WIN32
    printf("%s\n", errorMsg);
//#endif

    while(1){ }
}

void leAssert()
{
    printf("%s\n", errorMsg);

    while(1){ }
    //exit(0);
}

#endif
