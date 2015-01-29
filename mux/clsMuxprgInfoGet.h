

#include "EnumDef.h"
#include "list.h"

#ifndef _CLSMUX_PRGINFO_GET_H_
#define _CLSMUX_PRGINFO_GET_H_

ErrorTypeEm GetPidOffset(char *ip, int outChannel, unsigned int *outPid);
ErrorTypeEm SendOutPrgMuxMap(char *ip, int outChannel, list_t *pmuxPrgInfoList);
ErrorTypeEm SendTable_pmt(char *ip, int outChannel, list_t *pmtList);
ErrorTypeEm SendTable_psi(char *ip, int outChannel, PsiTableType tableType, unsigned char *ptableBytes, int length);
#endif