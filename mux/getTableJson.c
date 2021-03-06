#include <string.h>
#include "datastructdef.h"
#include "cJSON.h"
#include "list.h"

extern ClsProgram_st clsProgram;
extern ClsMux_st *pclsMux;
extern ClsParams_st *pdb;

void getTableJson(int channel, char *outprg, int flag, char *lan){
	char str[100] = {0};
	char idstr[100] = {0};
	int i = 0, j = 0, k = 0;
	BufferUn_st *pat;
	pat_senction_st *p_pat;	
	char* prgjsonstring;
	cJSON *tablesarray, *tablejson, *prgarray, *prgjson, *subTablearray, *subTablejson, *streamsarray, *streamjson;
	tablesarray = cJSON_CreateArray();
	cJSON_AddItemToArray(tablesarray,tablejson = cJSON_CreateObject());
	cJSON_AddNumberToObject(tablejson, "sts", flag);//制表状态码
	cJSON_AddTrueToObject(tablejson,"folder");
	cJSON_AddFalseToObject(tablejson,"expanded");
	cJSON_AddStringToObject(tablejson,"key", "id1.1");
	cJSON_AddStringToObject(tablejson,"title", "PAT");
	cJSON_AddStringToObject(tablejson,"icon", "img/channel_out.ico");
	if(pdb->pvalueTree->poutChnArray[channel-1].isNeedSend_pat){
		list_get(&(pclsMux->table_pat), channel-1, &pat);
		p_pat = (pat_senction_st*)malloc(sizeof(pat_senction_st));
		if(ParsePat(pat->pbuf, 5, p_pat)){
			cJSON_AddItemToObject(tablejson, "children", subTablearray = cJSON_CreateArray());	
			for(i=1;i<4;i++){
				cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
				cJSON_AddTrueToObject(subTablejson,"folder");
				cJSON_AddFalseToObject(subTablejson,"expanded");
				sprintf(idstr, "id1.1.1.%d", i);//1.1.1.1
				cJSON_AddStringToObject(subTablejson,"key", idstr);
				switch(i){
					case 1:
						memset(str, 0, sizeof(str));
						if(!strcmp(lan, "zh-CN")){
							sprintf(str,"版本：%02d", p_pat->version_number);
						}else{
							sprintf(str,"Version:%02d", p_pat->version_number);
						}
						cJSON_AddStringToObject(subTablejson,"title", str);
						cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
						break;
					case 2: 
						memset(str, 0, sizeof(str));
						sprintf(str,"TSID 0x%04x", p_pat->transport_stream_id);
						cJSON_AddStringToObject(subTablejson,"title", str);
						cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
						break;
					case 3:
						memset(str, 0, sizeof(str));
						sprintf(str,"%d Maps", p_pat->patPrg_tListLen);
						cJSON_AddStringToObject(subTablejson,"title", str);
						cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
						break;
					default:
						break;
				}
			}
			patPrg_t* p_program = p_pat->patPrg_tList;
			for(i=0;i<p_pat->patPrg_tListLen;i++)
			{
				cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
				cJSON_AddTrueToObject(subTablejson,"folder");
				cJSON_AddFalseToObject(subTablejson,"expanded");
				sprintf(idstr, "id1.1.1.%d", i+4);//1.1.0.4
				cJSON_AddStringToObject(subTablejson,"key", idstr);
				if(i == 0){
					memset(str, 0, sizeof(str));
					if(!strcmp(lan, "zh-CN")){
						sprintf(str,"网络ID (0x%04x)", p_program->pid);
					}else{
						sprintf(str,"NETWORKID (0x%04x)", p_program->pid);
					}
					cJSON_AddStringToObject(subTablejson,"title", str);
					cJSON_AddStringToObject(subTablejson,"icon", "img/notebook.ico");
				}else{
					memset(str, 0, sizeof(str));
					if(!strcmp(lan, "zh-CN")){
						sprintf(str,"业务 (0x%04x) PMTPid=(0x%04x)", p_program->program_number, p_program->pid);
					}else{
						sprintf(str,"Business (0x%04x) PMTPid=(0x%04x)", p_program->program_number, p_program->pid);
					}
					cJSON_AddStringToObject(subTablejson,"title", str);
					cJSON_AddStringToObject(subTablejson,"icon", "img/notebook.ico");
				}
				p_program++;
			}
			free(p_pat);
		}else{
			//pat表为空
			printf("-------ParsePat Failed------\n");
			free(p_pat);
		}
	}
	cJSON_AddItemToArray(tablesarray,tablejson = cJSON_CreateObject());
	cJSON_AddTrueToObject(tablejson,"folder");
	cJSON_AddFalseToObject(tablejson,"expanded");
	cJSON_AddStringToObject(tablejson,"key", "id1.2");
	cJSON_AddStringToObject(tablejson,"title", "PMT");
	cJSON_AddStringToObject(tablejson,"icon", "img/channel_out.ico");
	BufferUn_st *sdt = NULL;
	if(pdb->pvalueTree->poutChnArray[channel-1].isNeedSend_pmt){
		pmt_senction_st *p_pmt;
		sdt_senction_st *p_sdt;
		BufferUn_st *outPMTBuffer;
		list_t *tablePmt;
		DataStream_t *pdataStreamList;
		cJSON_AddItemToObject(tablejson, "children", prgarray = cJSON_CreateArray());
		list_get(&(pclsMux->table_pmtList), channel-1, &tablePmt);	
		list_get(&(pclsMux->table_sdt), channel-1, &sdt);
		if(sdt != NULL){
			p_sdt = (sdt_senction_st*)malloc(sizeof(sdt_senction_st));
			ParseSdt(sdt->pbuf, 5, p_sdt);	
		}				
		for(i=0;i<list_len(tablePmt);i++){
			list_get(tablePmt,i,&outPMTBuffer);
			p_pmt = (pmt_senction_st*)malloc(sizeof(pmt_senction_st));
			int isPmtCrypto = 0;
			if(ParsePmt(outPMTBuffer->pbuf, 5, p_pmt)){//pmt				
				cJSON_AddItemToArray(prgarray,prgjson = cJSON_CreateObject());
				memset(idstr, 0, sizeof(idstr));
				sprintf(idstr, "id1.2.%d", i+1);//1.2.1
				cJSON_AddTrueToObject(prgjson,"folder");
				cJSON_AddFalseToObject(prgjson,"expanded");
				cJSON_AddStringToObject(prgjson,"key", idstr);
				memset(str, 0, sizeof(str));
				memset(idstr, 0, sizeof(idstr));
				OutPsiTable_inSdt_search_prgInfo(p_sdt, p_pmt->program_number, idstr, &isPmtCrypto);
				sprintf(str,"PMT for SID 0x%04x - %s", p_pmt->program_number, idstr );
				cJSON_AddStringToObject(prgjson,"title", str);
				cJSON_AddStringToObject(prgjson,"icon", "img/notebook.ico");
				cJSON_AddItemToObject(prgjson, "children", subTablearray = cJSON_CreateArray());
				for(j=1;j<8;j++){ //TODO					
					switch(j){
						case 1:
							cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
							cJSON_AddTrueToObject(subTablejson,"folder");
							cJSON_AddFalseToObject(subTablejson,"expanded");
							sprintf(idstr, "id1.2.%d.%d", i, j);//1.2.1.1
							cJSON_AddStringToObject(subTablejson,"key", idstr);
							memset(str, 0, sizeof(str));
							if(!strcmp(lan, "zh-CN")){
								sprintf(str,"版本：%02d", p_pmt->version_number );
							}else{
								sprintf(str,"Version: %02d", p_pmt->version_number );
							}
							cJSON_AddStringToObject(subTablejson,"title", str);
							cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
							break;
						case 2: 
							cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
							cJSON_AddTrueToObject(subTablejson,"folder");
							cJSON_AddFalseToObject(subTablejson,"expanded");
							sprintf(idstr, "id1.2.%d.%d", i, j);//1.2.1.1
							cJSON_AddStringToObject(subTablejson,"key", idstr);
							memset(str, 0, sizeof(str));
							sprintf(str,"PCR pid 0x%04x", p_pmt->pcrPid );
							cJSON_AddStringToObject(subTablejson,"title", str);
							cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
							break;
						case 3:
							cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
							cJSON_AddTrueToObject(subTablejson,"folder");
							cJSON_AddFalseToObject(subTablejson,"expanded");
							sprintf(idstr, "id1.2.%d.%d", i, j);//1.2.1.1
							cJSON_AddStringToObject(subTablejson,"key", idstr);
							memset(str, 0, sizeof(str));
							if(!strcmp(lan, "zh-CN")){
								sprintf(str,"业务ID 0x%04x", p_pmt->program_number );
							}else{
								sprintf(str,"Business ID 0x%04x", p_pmt->program_number );
							}
							cJSON_AddStringToObject(subTablejson,"title", str);
							cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
							break;
						case 4:
							cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
							cJSON_AddTrueToObject(subTablejson,"folder");
							cJSON_AddFalseToObject(subTablejson,"expanded");
							sprintf(idstr, "id1.2.%d.%d", i, j);//1.2.1.1
							cJSON_AddStringToObject(subTablejson,"key", idstr);
							cJSON_AddStringToObject(subTablejson,"title", "maximun bitrate descriptor");
							cJSON_AddStringToObject(subTablejson,"icon", "img/channel_in.ico");
							break;
						case 5:
							cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
							cJSON_AddTrueToObject(subTablejson,"folder");
							cJSON_AddFalseToObject(subTablejson,"expanded");
							sprintf(idstr, "id1.2.%d.%d", i, j);//1.2.1.1
							cJSON_AddStringToObject(subTablejson,"key", idstr);
							cJSON_AddStringToObject(subTablejson,"title", "multiplex buffer utilization descriptor");
							cJSON_AddStringToObject(subTablejson,"icon", "img/channel_in.ico");
							break;
						case 6:
							pdataStreamList = p_pmt->pdataStreamList;
							for(k=0;k<p_pmt->pdataStreamListLen;k++){
								if(2 == pdataStreamList->streamType){
									cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
									cJSON_AddTrueToObject(subTablejson,"folder");
									cJSON_AddFalseToObject(subTablejson,"expanded");
									sprintf(idstr, "id1.2.%d.%d", i, j);//1.2.1.1
									cJSON_AddStringToObject(subTablejson,"key", idstr);
									memset(str, 0, sizeof(str));
									if(!strcmp(lan, "zh-CN")){
										sprintf(str,"流 %d pid(0x%x) MPEG2 Video", 2, pdataStreamList->outPid);
									}else{
										sprintf(str,"Stream %d pid(0x%x) MPEG2 Video", 2, pdataStreamList->outPid);
									}
									cJSON_AddStringToObject(subTablejson,"title", str);
									cJSON_AddStringToObject(subTablejson,"icon", "img/favicon.ico");
									
									cJSON_AddItemToObject(subTablejson, "children", streamsarray = cJSON_CreateArray());
									cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
									sprintf(idstr, "id1.2.%d.%d.1", i, k+6);//1.2.1.1
									cJSON_AddStringToObject(streamjson,"title", "video stream descriptor");
									cJSON_AddTrueToObject(streamjson,"folder");
									cJSON_AddFalseToObject(streamjson,"expanded");
									cJSON_AddStringToObject(streamjson,"key", idstr);
									cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
									
									cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
									sprintf(idstr, "id1.2.%d.%d.2", i, k+6);//1.2.1.1
									cJSON_AddStringToObject(streamjson,"title", "data stream alignment descriptor");
									cJSON_AddTrueToObject(streamjson,"folder");
									cJSON_AddFalseToObject(streamjson,"expanded");
									cJSON_AddStringToObject(streamjson,"key", idstr);
									cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
									
									cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
									sprintf(idstr, "id1.2.%d.%d.3", i, k+6);//1.2.1.1
									cJSON_AddStringToObject(streamjson,"title", "maximum bitrate descriptor");
									cJSON_AddTrueToObject(streamjson,"folder");
									cJSON_AddFalseToObject(streamjson,"expanded");
									cJSON_AddStringToObject(streamjson,"key", idstr);
									cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
									
									cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
									sprintf(idstr, "id1.2.%d.%d.4", i, k+6);//1.2.1.1
									cJSON_AddStringToObject(streamjson,"title", "stream identifier descriptor");
									cJSON_AddTrueToObject(streamjson,"folder");
									cJSON_AddFalseToObject(streamjson,"expanded");
									cJSON_AddStringToObject(streamjson,"key", idstr);
									cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
								}
								pdataStreamList++;
							}						
							break;
						case 7:
							pdataStreamList = p_pmt->pdataStreamList;
							for(k=0;k<p_pmt->pdataStreamListLen;k++){
								if(4 == pdataStreamList->streamType){
									cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
									cJSON_AddTrueToObject(subTablejson,"folder");
									cJSON_AddFalseToObject(subTablejson,"expanded");
									sprintf(idstr, "id1.2.%d.%d", i, j);//1.2.1.1
									cJSON_AddStringToObject(subTablejson,"key", idstr);
									memset(str, 0, sizeof(str));
									if(!strcmp(lan, "zh-CN")){
										sprintf(str,"流 %d pid(0x%x) MPEG2 Audio", 4, pdataStreamList->outPid);
									}else{
										sprintf(str,"Stream %d pid(0x%x) MPEG2 Audio", 4, pdataStreamList->outPid);
									}
									
									cJSON_AddStringToObject(subTablejson,"title", str);
									cJSON_AddStringToObject(subTablejson,"icon", "img/audio.ico");
									
									//stream json
									cJSON_AddItemToObject(subTablejson, "children", streamsarray = cJSON_CreateArray());
									cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
									sprintf(idstr, "id1.2.%d.%d.1", i, k+7);//1.2.1.1
									cJSON_AddStringToObject(streamjson,"title", "ISO 639 language descriptor");
									cJSON_AddTrueToObject(streamjson,"folder");
									cJSON_AddFalseToObject(streamjson,"expanded");
									cJSON_AddStringToObject(streamjson,"key", idstr);
									cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
									
									cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
									sprintf(idstr, "id1.2.%d.%d.2", i, k+7);//1.2.1.1
									cJSON_AddStringToObject(streamjson,"title", "audio stream descriptor");
									cJSON_AddTrueToObject(streamjson,"folder");
									cJSON_AddFalseToObject(streamjson,"expanded");
									cJSON_AddStringToObject(streamjson,"key", idstr);
									cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
									
									cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
									sprintf(idstr, "id1.2.%d.%d.3", i, k+7);//1.2.1.1
									cJSON_AddStringToObject(streamjson,"title", "maximum bitrate descriptor");
									cJSON_AddTrueToObject(streamjson,"folder");
									cJSON_AddFalseToObject(streamjson,"expanded");
									cJSON_AddStringToObject(streamjson,"key", idstr);
									cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
								}
								pdataStreamList++;
							}						
							break;
						default:
							
							break;
						
					}
				}					
				
			}else{
				
			}
			free(p_pmt);
		}
	}
	cJSON_AddItemToArray(tablesarray,tablejson = cJSON_CreateObject());
	cJSON_AddTrueToObject(tablejson,"folder");
	cJSON_AddFalseToObject(tablejson,"expanded");
	cJSON_AddStringToObject(tablejson,"key", "id1.3");
	cJSON_AddStringToObject(tablejson,"title", "SDT");
	cJSON_AddStringToObject(tablejson,"icon", "img/channel_out.ico");	
	sdt_senction_st *p_sdt;
	if(pdb->pvalueTree->poutChnArray[channel-1].isNeedSend_sdt){
		list_get(&(pclsMux->table_sdt), channel-1, &sdt);
		p_sdt = (sdt_senction_st*)malloc(sizeof(sdt_senction_st));	
		if(ParseSdt(sdt->pbuf, 5, p_sdt)){ //sdt
			cJSON_AddItemToObject(tablejson, "children", subTablearray = cJSON_CreateArray());	
			for(i=1;i<4;i++){ 
				cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
				cJSON_AddTrueToObject(subTablejson,"folder");
				cJSON_AddFalseToObject(subTablejson,"expanded");
				sprintf(idstr, "id1.3.%d", i);//1.3.1
				cJSON_AddStringToObject(subTablejson,"key", idstr);
				switch(i){
					case 1:
						memset(str, 0, sizeof(str));
						if(!strcmp(lan, "zh-CN")){
							sprintf(str,"版本：%02d", p_sdt->version_number );
						}else{
							sprintf(str,"Version：%02d", p_sdt->version_number );
						}
						cJSON_AddStringToObject(subTablejson,"title", str);
						cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
						break;
					case 2: 
						memset(str, 0, sizeof(str));
						if(!strcmp(lan, "zh-CN")){
							sprintf(str,"传输流ID 0x%04x", p_sdt->transport_stream_id );
						}else{
							sprintf(str,"StreamID 0x%04x", p_sdt->transport_stream_id );
						}
						cJSON_AddStringToObject(subTablejson,"title", str);
						cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
						break;
					case 3:
						memset(str, 0, sizeof(str));
						if(!strcmp(lan, "zh-CN")){
							sprintf(str,"原始网络ID (0x%04x)", p_sdt->original_network_id );
						}else{
							sprintf(str,"OriNetworkID (0x%04x)", p_sdt->original_network_id );
						}
						cJSON_AddStringToObject(subTablejson,"title", str);
						cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
						break;
					default:
						
						break;
				}
			}
			sdtPrgName_st* p_last_sdtPrgName_t = p_sdt->nameList;
			for(i=0;i<p_sdt->nameListLen;i++)
			{
				cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
				cJSON_AddTrueToObject(subTablejson,"folder");
				cJSON_AddFalseToObject(subTablejson,"expanded");
				sprintf(idstr, "id1.3.%d", i+4);//1.3.4.1
				cJSON_AddStringToObject(subTablejson,"key", idstr);
				memset(str, 0, sizeof(str));
				if(!strcmp(lan, "zh-CN")){
					sprintf(str,"业务[0x%04x] EIT Sched[%s] EIT PF[%s] RunningStatus[Running]", p_last_sdtPrgName_t->service_id, p_last_sdtPrgName_t->EIT_schedule_flag==0?"NOT Present":"Present", p_last_sdtPrgName_t->EIT_present_following_flag==0?"NOT Present":"Present");
				}else{
					sprintf(str,"Business[0x%04x] EIT Sched[%s] EIT PF[%s] RunningStatus[Running]", p_last_sdtPrgName_t->service_id, p_last_sdtPrgName_t->EIT_schedule_flag==0?"NOT Present":"Present", p_last_sdtPrgName_t->EIT_present_following_flag==0?"NOT Present":"Present");
				}
				cJSON_AddStringToObject(subTablejson,"title", str);
				cJSON_AddStringToObject(subTablejson,"icon", "img/notebook.ico");
				
				cJSON_AddItemToObject(subTablejson, "children", streamsarray = cJSON_CreateArray());	
				//TODO
				//未添加自定义描述符
				cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());				
				cJSON_AddTrueToObject(streamjson,"folder");
				cJSON_AddFalseToObject(streamjson,"expanded");
				memset(idstr, 0, sizeof(idstr));
				sprintf(idstr, "id1.3.%d.%d", i+4, 1);//1.3.4.1
				cJSON_AddStringToObject(streamjson,"key", idstr);
				cJSON_AddStringToObject(streamjson,"title", "server descriptor");
				cJSON_AddStringToObject(streamjson,"icon", "img/channel_in.ico");
				p_last_sdtPrgName_t++;
			}
			free(p_sdt);
		}else{
			
		}
	}	
	cJSON_AddItemToArray(tablesarray,tablejson = cJSON_CreateObject());
	cJSON_AddTrueToObject(tablejson,"folder");
	cJSON_AddFalseToObject(tablejson,"expanded");
	cJSON_AddStringToObject(tablejson,"key", "id1.4");
	cJSON_AddStringToObject(tablejson,"title", "CAT");
	cJSON_AddStringToObject(tablejson,"icon", "img/channel_out.ico");
	if(pdb->pvalueTree->poutChnArray[channel-1].isNeedSend_cat){
	    cat_senction_st *p_cat;
	    BufferUn_st *cat = NULL;
	    list_get(&(pclsMux->table_cat), channel-1, &cat);
        p_cat = (cat_senction_st*)malloc(sizeof(cat_senction_st));
		if(ParseCat(cat->pbuf, 5, p_cat)){ //cat
		    cJSON_AddItemToObject(tablejson, "children", subTablearray = cJSON_CreateArray());
            cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
            cJSON_AddFalseToObject(subTablejson,"folder");
            cJSON_AddStringToObject(subTablejson,"key", "id1.4.1");
            memset(str, 0, sizeof(str));
			if(!strcmp(lan, "zh-CN")){
				sprintf(str,"版本：%02d", p_cat->version_number );
			}else{
				sprintf(str,"Version: %02d", p_cat->version_number );
			}
            cJSON_AddStringToObject(subTablejson,"title", str);
            cJSON_AddStringToObject(subTablejson,"icon", "img/star.ico");
            CA_descriptor *calist = p_cat->caIdenList;
            for(i=1;i<p_cat->caIdenListLen + 1;i++){
                cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
                cJSON_AddFalseToObject(subTablejson,"folder");
                memset(str, 0, sizeof(str));
                sprintf(str,"id1.4.%d", i+1);
                cJSON_AddStringToObject(subTablejson,"key", str);
                memset(str, 0, sizeof(str));
                sprintf(str,"CA descriptor SysID(0x%04x) PID(0x%04x)", calist->inCaSysId,  calist->inCaPid);
                cJSON_AddStringToObject(subTablejson,"title", str);
                cJSON_AddStringToObject(subTablejson,"icon", "img/notebook.ico");
                calist++;
            }
		}else{
		
		}
	}
	cJSON_AddItemToArray(tablesarray,tablejson = cJSON_CreateObject());
	cJSON_AddTrueToObject(tablejson,"folder");
	cJSON_AddTrueToObject(tablejson,"expanded");
	cJSON_AddStringToObject(tablejson,"key", "id1.5");
	cJSON_AddStringToObject(tablejson,"title", "NIT");
	cJSON_AddStringToObject(tablejson,"icon", "img/channel_out.ico");
	if(pdb->pvalueTree->poutChnArray[channel-1].isNeedSend_nit){
        Nit_section_t *nist = NULL;
        list_get(&clsProgram.NitSection, channel-1, &nist);
        if(nist != NULL){
            if(nist->networkId != 0x00){
                cJSON_AddItemToObject(tablejson, "children", subTablearray = cJSON_CreateArray());
                cJSON_AddItemToArray(subTablearray,subTablejson = cJSON_CreateObject());
                cJSON_AddTrueToObject(subTablejson,"folder");
                cJSON_AddTrueToObject(subTablejson,"expanded");
                cJSON_AddStringToObject(subTablejson,"key", "id1.5.1");
                cJSON_AddStringToObject(subTablejson,"icon", "img/channel_out.ico");
                memset(str, 0, sizeof(str));
				if(!strcmp(lan, "zh-CN")){
					sprintf(str,"网络段ID[0x%04x] 名称[%s]", nist->networkId, nist->nameList->data);
				}else{
					sprintf(str,"Network Section ID[0x%04x] Name[%s]", nist->networkId, nist->nameList->data);
				}
                cJSON_AddStringToObject(subTablejson,"title", str);
                //nit section steam
                if(nist->streamLoopLen > 0){
                    cJSON_AddItemToObject(subTablejson, "children", streamsarray = cJSON_CreateArray());
                    Nit_streamLoop_t *streamLoop = nist->streamLoop;
                    for(i=0;i<nist->streamLoopLen;i++){
                        cJSON_AddItemToArray(streamsarray,streamjson = cJSON_CreateObject());
                        cJSON_AddFalseToObject(streamjson,"folder");
                        cJSON_AddNumberToObject(streamjson,"streamid", streamLoop->streamId);
                        switch(streamLoop->BufferUn_stList->pbuf[0]){
                            case 0x44:
                                cJSON_AddStringToObject(streamjson,"type", "DVB-C");
                                break;
                            case 0x43:
                                cJSON_AddStringToObject(streamjson,"type", "DVB-S");
                                break;
                            case 0x5a:
                                cJSON_AddStringToObject(streamjson,"type", "DVB-T");
                                break;
                        }
                        memset(idstr, 0, sizeof(idstr));
                        sprintf(idstr, "id1.5.1.%d", i+1);
                        cJSON_AddStringToObject(streamjson,"key", idstr);
                        cJSON_AddStringToObject(streamjson,"icon", "img/favicon.ico");
                        memset(str, 0, sizeof(str));
						if(!strcmp(lan, "zh-CN")){
							sprintf(str,"传输流ID[0x%04x] 原始网络ID[0x%04x]", streamLoop->streamId, streamLoop->original_network_id);
						}else{
							sprintf(str,"StreamID[0x%04x] OriNetworkID[0x%04x]", streamLoop->streamId, streamLoop->original_network_id);
						}
						cJSON_AddStringToObject(streamjson,"title", str);
                        streamLoop++;
                    }
                }
            }
        }
	}	
	prgjsonstring = cJSON_PrintUnformatted(tablesarray);
		
	memcpy(outprg, prgjsonstring, strlen(prgjsonstring));
	printf("----TableJson Length-->>>%d\n",strlen(prgjsonstring));
	//释放内存	
	cJSON_Delete(tablesarray);		
	free(prgjsonstring);
}

int OutPsiTable_inSdt_search_prgInfo(sdt_senction_st *p_sdt, int seriveid, char *prgName, int *isCrpto)
{
	int i = 0, j = 0;
	if (p_sdt->nameList != NULL && p_sdt->nameListLen > 0)
	{
		sdtPrgName_st *nameList = p_sdt->nameList;
		for(i=0;i<p_sdt->nameListLen;i++){
			if (nameList->service_id == seriveid)
			{
				memcpy(isCrpto, &nameList->free_CA_mode, sizeof(nameList->free_CA_mode));
				if (nameList->desList != NULL && nameList->desListLen > 0)
				{
					Commdes_t *desList = nameList->desList;
					for(j=0;j<nameList->desListLen;j++){
						if (desList->tag == 0x48) // service_tag
						{
							int iTagAddr = 0;
							int sdtDes_descriptor_length = desList->dataLen;//.data[iTagAddr++];
							int serviceType = desList->data[iTagAddr++];
							int providerNameLen = desList->data[iTagAddr++];							
							iTagAddr += providerNameLen;
							int serviceNameLen = desList->data[iTagAddr++];
							if (serviceNameLen > 0)
							{
								memcpy(prgName, desList->data+iTagAddr, serviceNameLen);
							}else{
								memcpy(prgName, "Undefined", 6);
							}
						}
						desList++;
					}
				}
				return 1;
			}
			nameList++;
		}		
	}

	return 0;
}

 