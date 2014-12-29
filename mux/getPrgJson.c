#include <string.h>

#include "getPrograms.h"
#include "cJSON.h"

void getprgsJson(char *ip, int inChn, char *outprg){
	int i=0;	
	char str[200] = {0};
	char idstr[20] = {0};
	char prgkey[20] = {0};
	list_t  prginfolist;    
	program_info_t *ptmpPrgInfo;
    getPrograms(ip, inChn, &prginfolist);	
	cJSON *prgjson,*channelsarray,*channeljson,*subprgjson,*subprgsarray,*streamjson,*streamsarray,*audiosarray,*prgsarray;//*prgsjson,
	char *prgjsonstring;
	/*prgsjson = cJSON_CreateObject();
	cJSON_AddStringToObject(prgsjson,"title", "输入通道");
	cJSON_AddStringToObject(prgsjson,"folder", "true");
	cJSON_AddStringToObject(prgsjson,"expanded", "true");
	cJSON_AddStringToObject(prgsjson,"key", "id1.0");
	cJSON_AddStringToObject(prgsjson,"icon", "img/book.ico");	
	cJSON_AddItemToObject(prgsjson, "children", channelsarray = cJSON_CreateArray());
	*/
	sprintf(prgkey, "id1.%d", inChn);//1.2
	channelsarray = cJSON_CreateArray();
	cJSON_AddItemToArray(channelsarray,channeljson = cJSON_CreateObject());
	cJSON_AddTrueToObject(channeljson,"folder");
	cJSON_AddTrueToObject(channeljson,"expanded");
	sprintf(idstr, "id1.%d", inChn);//1.2	
	cJSON_AddStringToObject(channeljson,"key", idstr);
	cJSON_AddStringToObject(channeljson,"icon", "img/channel_in.ico");
	
	cJSON_AddItemToObject(channeljson, "children", prgsarray = cJSON_CreateArray());
	
	for(i=0; i<list_len(&prginfolist); i++) {
		cJSON_AddItemToArray(prgsarray,prgjson = cJSON_CreateObject());
		list_get(&prginfolist, i, &ptmpPrgInfo);
		printf("------prginfo--->>>%x\n", ptmpPrgInfo->prgNum);
		//添加节目节点TITLE		
		sprintf(idstr, "id1.%d.%d", inChn, (i+1));//1.2.1	
		sprintf(str,"节目%d(0X%x):PID(0X%x) PCR_PID(0X%x) - %s",ptmpPrgInfo->prgNum, ptmpPrgInfo->prgNum, ptmpPrgInfo->pmtPid, ptmpPrgInfo->newPcrPid, ptmpPrgInfo->prgName );

		cJSON_AddStringToObject(prgjson,"title", str);
		cJSON_AddTrueToObject(prgjson,"folder");
		cJSON_AddFalseToObject(prgjson,"expanded");
		cJSON_AddStringToObject(prgjson,"prgkey", prgkey);
		cJSON_AddStringToObject(prgjson,"key", idstr);
		cJSON_AddStringToObject(prgjson,"icon", "img/notebook.ico");
		
		cJSON_AddItemToObject(prgjson, "children", subprgsarray = cJSON_CreateArray());
		//subprgjson
		cJSON_AddItemToArray(subprgsarray,subprgjson = cJSON_CreateObject());
		cJSON_AddStringToObject(subprgjson,"title", "maximun bitrate descriptor");
		cJSON_AddTrueToObject(subprgjson,"folder");
		cJSON_AddFalseToObject(subprgjson,"expanded");
		sprintf(idstr, "id1.%d.%d.1", inChn, (i+1));
		cJSON_AddStringToObject(subprgjson,"prgkey", prgkey);
		cJSON_AddStringToObject(subprgjson,"key", idstr);
		cJSON_AddStringToObject(subprgjson,"icon", "img/channel_in.ico");
		
		cJSON_AddItemToArray(subprgsarray,subprgjson = cJSON_CreateObject());	
		sprintf(idstr, "id1.%d.%d.2", inChn, (i+1));
		cJSON_AddStringToObject(subprgjson,"title", "multiplex buffer utilization descriptor");
		cJSON_AddTrueToObject(subprgjson,"folder");
		cJSON_AddFalseToObject(subprgjson,"expanded");
		cJSON_AddStringToObject(subprgjson,"prgkey", prgkey);
		cJSON_AddStringToObject(subprgjson,"key", idstr);
		cJSON_AddStringToObject(subprgjson,"icon", "img/channel_in.ico");

		int j=0, k=0;
		//PMT
		Commdes_t *tmpinfo = malloc(sizeof(Commdes_t));
		int offset = 0, flag = 0;
		for(j=0; j<ptmpPrgInfo->pmtDesListLen; j++) {
			memcpy(tmpinfo, ptmpPrgInfo->pmtDesList+offset, sizeof(Commdes_t) );
			//printf("-----%d-->>%s\n", j, tmpinfo->data);
			offset += 1;
		}
		free(tmpinfo);
		//stream
		offset = 0;
		DataStream_t *streaminfo = malloc(sizeof(DataStream_t));
		for(j=0; j<ptmpPrgInfo->pdataStreamListLen; j++) {			
			memcpy(streaminfo, ptmpPrgInfo->pdataStreamList+offset, sizeof(DataStream_t) );
			//printf("-----%x-->>%x\n", streaminfo->inPid, streaminfo->streamType);
			offset += 1;				
			if(2 == streaminfo->streamType){
				//MPEG2 VIDEO
				cJSON_AddItemToArray(subprgsarray,subprgjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.3", inChn, (i+1));
				sprintf(str,"MPEG2 VIDEO  [PID(0x%x)]",  streaminfo->inPid);
				cJSON_AddStringToObject(subprgjson,"title", str);
				cJSON_AddTrueToObject(subprgjson,"folder");
				cJSON_AddFalseToObject(subprgjson,"expanded");
				cJSON_AddStringToObject(subprgjson,"prgkey", prgkey);
				cJSON_AddStringToObject(subprgjson,"key", idstr);
				cJSON_AddStringToObject(subprgjson,"icon", "img/favicon.ico");
				
				cJSON_AddItemToObject(subprgjson, "children", streamsarray = cJSON_CreateArray());
				cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.3.1", inChn, (i+1));
				cJSON_AddStringToObject(streamjson,"title", "video stream descriptor");
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				cJSON_AddStringToObject(streamjson,"prgkey", prgkey);
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
				
				cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.3.2", inChn, (i+1));
				cJSON_AddStringToObject(streamjson,"title", "data stream alignment descriptor");
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				cJSON_AddStringToObject(streamjson,"prgkey", prgkey);
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
				
				cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.3.3", inChn, (i+1));
				cJSON_AddStringToObject(streamjson,"title", "maximum bitrate descriptor");
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				cJSON_AddStringToObject(streamjson,"prgkey", prgkey);
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
				
				cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.3.4", inChn, (i+1));
				cJSON_AddStringToObject(streamjson,"title", "stream identifier descriptor");
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				cJSON_AddStringToObject(streamjson,"prgkey", prgkey);
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
			
				//cJSON_AddItemToObject(subprgjson, "children", streamsarray);
			}else if(4 == streaminfo->streamType){
				//MPEG2 AUDIO
				cJSON_AddItemToArray(subprgsarray,subprgjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.4", inChn, (i+1));
				sprintf(str,"MPEG2 AUDIO  [PID(0x%x)]",  streaminfo->inPid);
				cJSON_AddStringToObject(subprgjson,"title", str);
				cJSON_AddTrueToObject(subprgjson,"folder");
				cJSON_AddFalseToObject(subprgjson,"expanded");
				cJSON_AddStringToObject(subprgjson,"prgkey", prgkey);
				cJSON_AddStringToObject(subprgjson,"key", idstr);
				cJSON_AddStringToObject(subprgjson,"icon", "img/audio.ico");
				
				//stream json
				cJSON_AddItemToObject(subprgjson, "children", audiosarray = cJSON_CreateArray());
				cJSON_AddItemToArray(audiosarray,streamjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.4.1", inChn, (i+1));
				cJSON_AddStringToObject(streamjson,"title", "ISO 639 language descriptor");
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				cJSON_AddStringToObject(streamjson,"prgkey", prgkey);
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
				
				cJSON_AddItemToArray(audiosarray,streamjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.4.2", inChn, (i+1));
				cJSON_AddStringToObject(streamjson,"title", "audio stream descriptor");
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				cJSON_AddStringToObject(streamjson,"prgkey", prgkey);
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
				
				cJSON_AddItemToArray(audiosarray,streamjson = cJSON_CreateObject());
				sprintf(idstr, "id1.%d.%d.4.3", inChn, (i+1));
				cJSON_AddStringToObject(streamjson,"title", "maximum bitrate descriptor");
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				cJSON_AddStringToObject(streamjson,"prgkey", prgkey);
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");				
			}				
			Commdes_t *desNodeinfo = malloc(sizeof(Commdes_t));
			for(k=0; k<streaminfo->desNodeLen; k++) {
				memcpy(desNodeinfo, streaminfo->desNode+flag, sizeof(Commdes_t) );
				//printf("-----%d-->>%s\n", k, desNodeinfo->data);
				flag += 1;
				
			}
			free(desNodeinfo);
		}	
		free(streaminfo);	
	}
	//添加通道节点TITLE
	sprintf(str,"通道%d(ASI-%d)  -  原始网络ID=0x%04X,传输流ID=0x%04X",ptmpPrgInfo->chnId, ptmpPrgInfo->chnId, ptmpPrgInfo->networkId, ptmpPrgInfo->streamId );
	cJSON_AddStringToObject(channeljson,"title", str);

	prgjsonstring = cJSON_PrintUnformatted(channelsarray);
	memcpy(outprg, prgjsonstring, strlen(prgjsonstring));
	//printf("------>>>%s\n",prgjsonstring);
	//释放内存	
	cJSON_Delete(channelsarray);		
	free(prgjsonstring);
	freePrograms(&prginfolist);
}



