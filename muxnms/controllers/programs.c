/*
    programs Controller for esp-html-mvc (esp-html-mvc)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "esp.h" 
#include "communicate.h"
#include "getPrograms.h"

extern ClsProgram_st clsProgram;

static char* substr(const char*str,unsigned start, unsigned end)
{
   unsigned n = end - start;
   static char stbuf[256];
   strncpy(stbuf, str + start, n);
   stbuf[n] = 0;
   return stbuf;
}

static void getprg(HttpConn *conn) { 
	MprJson *jsonparam = httpGetParams(conn);
    char ip[16] = "192.168.1.134";//param("ip"); 
    char *inChn = mprGetJson(jsonparam, "inch"); 
	int inCh = atoi(inChn);
	char pProg[20480] = {0}; 	
    getprgsJson(ip, inCh, pProg);
	render(pProg);
    
} 
/*制表准备工作*/
static void maketable(HttpConn *conn) { 
	int i = 0, j = 0, prgindex = 0;
	char str[5] = {0};
	char idstr[4] = {0};
	MprJson *jsonparam = mprParseJson(espGetQueryString(conn));
	//提取要制表的节目信息
	for(i=0; i<clsProgram._intChannelCntMax; i++){
		sprintf(str, "inCh%d", i);
		if( 0 != mprGetJsonLength(mprGetJsonObj(jsonparam, str ))){		
			for(j=0;j< mprGetJsonLength(mprGetJsonObj(jsonparam, str )); j++){
				sprintf(idstr, "id%d", i);
				prgindex = atoi(mprGetJson(mprGetJsonObj(jsonparam, str ), idstr));
				printf("===ch===>>>>%d======index=====>>>%d\n", i, prgindex);	
			}
		}
	}	
	
	printf("===%s===>>>>%s\n", espGetQueryString(conn), mprGetJson(mprGetJsonObj(jsonparam, "inCh2" ), "id1"));	
	render("");
    
} 


static void common(HttpConn *conn) {
	
	
}

static void espinit() {	
	int i=0;
	ChannelProgramSt *pst = NULL;
	//全局变量初始化
	clsProgram._outChannelCntMax = 2;
	clsProgram._intChannelCntMax = 8;
	clsProgram.prgNum_min = 1;
	clsProgram.prgPid_min = 0x100;
	clsProgram.prgPid_max = 0xfff;
	clsProgram.subPrgPid_min = 0x1000;
	clsProgram.subPrgPid_max = 0x1ff0;
	//给全局变量申请内存
	for(i=0; i<clsProgram._intChannelCntMax; i++){
		pst = malloc(sizeof(ChannelProgramSt));
		memset(pst, 0, sizeof(ChannelProgramSt));
		pst->channelId = i + 1;		
		list_append(&(clsProgram.inPrgList), pst);
	}	
	for(i=0; i<clsProgram._outChannelCntMax; i++){
		pst = malloc(sizeof(ChannelProgramSt));
		memset(pst, 0, sizeof(ChannelProgramSt));
		list_append(&(clsProgram.outPrgList), pst);
	}	
	printf("======>>>>esp init!!!!!!!\n");
}

/*
    Dynamic module initialization
 */
ESP_EXPORT int esp_controller_muxnms_programs(HttpRoute *route, MprModule *module) {
    espDefineBase(route, common);
	espinit();	
    espDefineAction(route, "programs-cmd-getprg", getprg);
	espDefineAction(route, "programs-cmd-maketable", maketable);
    
#if SAMPLE_VALIDATIONS
    Edi *edi = espGetRouteDatabase(route);
    ediAddValidation(edi, "present", "programs", "title", 0);
    ediAddValidation(edi, "unique", "programs", "title", 0);
    ediAddValidation(edi, "banned", "programs", "body", "(swear|curse)");
    ediAddValidation(edi, "format", "programs", "phone", "/^\\(?([0-9]{3})\\)?[-. ]?([0-9]{3})[-. ]?([0-9]{4})$/");
#endif
    return 0;
}
