// CExoDebugInternal::Warning (s5).

#include <stdio.h>
#include <string.h>
#include "c/exo_base.h"

class CExoDebugInternal {
public:
    void Warning(int nLine, char* sFile, const char* sMessage);
};

// The bare CStr() calls are debugger-output statements compiled out of the
// release build.
// FUNCTION: SWKOTOR 0x005EC2B0
void CExoDebugInternal::Warning(int nLine, char* sFile, const char* sMessage)
{
    CExoString sWarning;
    if (sMessage == 0) {
        sWarning.Format("A warning has occured:\n\n File: %s\n\nLine #: %d\n\nPress Abort to quit, Retry to debug, or Ignore to continue", sFile, nLine);
    } else {
        sWarning.Format("A warning has occured:\n\n File: %s\n\nLine #: %d \n\nProgrammer says: %s\n\n"
                        "Press Abort to quit, Retry to debug, or Ignore to continue", sFile, nLine, sMessage);
    }
    FILE* pFile = fopen("Logs\\swcWarnings.txt", "at");
    if (pFile) {
        fprintf(pFile, sWarning.CStr());
    }
    CExoString sLine;
    CExoString sRest;
    char* sBase = strrchr(sFile, '\\');
    if (!sBase) {
        sBase = sFile;
    } else {
        sBase++;
    }
    sLine.Format("WARNING: %s(%d) Comment:", sBase, nLine);
    sLine.CStr();
    if (sMessage) {
        sRest = sMessage;
        int nLength = sRest.GetLength();
        while (nLength) {
            if (nLength <= 50) {
                sLine = sRest;
                sRest = "";
            } else {
                sLine = sRest.Left(50);
                int nCut = 50;
                char* p = strrchr(sLine.CStr(), ' ');
                if (p) {
                    nCut = p - sLine.CStr();
                }
                p = strrchr(sLine.CStr(), '\t');
                if (p) {
                    nCut = p - sLine.CStr();
                }
                p = strrchr(sLine.CStr(), '\r');
                if (p) {
                    nCut = p - sLine.CStr();
                }
                sLine = sRest.Left(nCut);
                sRest = sRest.Right(nLength - nCut);
            }
            sLine.CStr();
            nLength = sRest.GetLength();
        }
    }
    if (pFile) {
        fclose(pFile);
    }
}
