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


#ifndef IP_FS_H               // Avoid multiple inclusion.
#define IP_FS_H

#include "SEGGER.h"

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  //
  // Read only file operations. These have to be present on ANY file system, even the simplest one.
  //
  void* (*pfOpenFile)             (const char* sFilename);
  int   (*pfCloseFile)            (void* hFile);
  int   (*pfReadAt)               (void* hFile, void* pBuffer, U32 Pos, U32 NumBytes);
  long  (*pfGetLen)               (void* hFile);
  //
  // Directory query operations.
  //
  void  (*pfForEachDirEntry)      (void* pContext, const char* sDir, void (*pf)(void*, void*));
  void  (*pfGetDirEntryFileName)  (void* pFileEntry, char* sFileName, U32 SizeOfBuffer);
  U32   (*pfGetDirEntryFileSize)  (void* pFileEntry, U32* pFileSizeHigh);
  U32   (*pfGetDirEntryFileTime)  (void* pFileEntry);
  int   (*pfGetDirEntryAttributes)(void* pFileEntry);
  //
  // Write file operations.
  //
  void* (*pfCreate)               (const char* sFileName);
  void* (*pfDeleteFile)           (const char* sFilename);
  int   (*pfRenameFile)           (const char* sOldFilename, const char* sNewFilename);
  int   (*pfWriteAt)              (void* hFile, void* pBuffer, U32 Pos, U32 NumBytes);
  //
  // Additional directory operations
  //
  int   (*pfMKDir)                (const char* sDirName);
  int   (*pfRMDir)                (const char* sDirName);
  //
  // Additional operations
  //
  int   (*pfIsFolder)             (const char* sPath);
  int   (*pfMove)                 (const char* sOldFilename, const char* sNewFilename);
} IP_FS_API;

typedef struct {
  const          char* sPath;
  const unsigned char* pData;
        unsigned int   FileSize;
} IP_FS_READ_ONLY_FILE_ENTRY;

typedef struct IP_FS_READ_ONLY_FILE_HOOK_STRUCT IP_FS_READ_ONLY_FILE_HOOK;
struct IP_FS_READ_ONLY_FILE_HOOK_STRUCT {
  IP_FS_READ_ONLY_FILE_HOOK* pNext;
  IP_FS_READ_ONLY_FILE_ENTRY FileEntry;
};

/*********************************************************************
*
*       API functions
*
**********************************************************************
*/

#define IP_FS_FS                    IP_FS_emFile
#define IP_FS_FS_AllowHiddenAccess  IP_FS_emFile_AllowHiddenAccess
#define IP_FS_FS_DenyHiddenAccess   IP_FS_emFile_DenyHiddenAccess

extern const IP_FS_API IP_FS_ReadOnly;                  // Read-only file system, typically located in flash memory.
extern const IP_FS_API IP_FS_Win32;                     // File system interface for Win32.
extern const IP_FS_API IP_FS_Linux;                     // File system interface for Linux
extern const IP_FS_API IP_FS_emFile;                    // Target file system (emFile), shows and allows access to hidden files.
extern const IP_FS_API IP_FS_emFile_AllowHiddenAccess;  // Target file system (emFile), does not show hidden files but allows access to them.
extern const IP_FS_API IP_FS_emFile_DenyHiddenAccess;   // Target file system (emFile), does not show hidden files and does not allow access to them.

//
// Helper functions for Read Only file system layer.
//
void IP_FS_READ_ONLY_ClrFileHooks(void);
void IP_FS_READ_ONLY_AddFileHook (IP_FS_READ_ONLY_FILE_HOOK* pHook, const char* sPath, const unsigned char* pData, unsigned int FileSize);

//
// Helper functions for Win32 file system layer.
//
void IP_FS_WIN32_ConfigBaseDir(const char* sDir);


#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

#endif                        // Avoid multiple inclusion.

/*************************** End of file ****************************/
