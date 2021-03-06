#include <stdio.h>
#include "freePrograms.h"


int freeProgramsMalloc(Dev_prgInfo_st * data)
{
	int i = 0, j=0;
	int pmtDesListLen = data->pmtDesListLen;
	for(i=0; i < pmtDesListLen; i++){		
			free(data->pmtDesList[i].data);
			data->pmtDesList[i].data = NULL;		
	}
	if(data->pmtDesListLen>0){
		free(data->pmtDesList);
		data->pmtDesList = NULL;
	}
	int pdataStreamListLen = data->pdataStreamListLen;
	if(pdataStreamListLen>0){
		for(i=0; i< pdataStreamListLen; i++){
			for(j=0; j < data->pdataStreamList[i].desNodeLen; j++)
			{				
				if(data->pdataStreamList[i].desNode[j].dataLen){
					free(data->pdataStreamList[i].desNode[j].data);
					data->pdataStreamList[i].desNode[j].data = NULL;					
				}												
			}
			if(data->pdataStreamList[i].desNodeLen>0){
				free(data->pdataStreamList[i].desNode);
				data->pdataStreamList[i].desNode = NULL;
			}		
		}		
		free(data->pdataStreamList);
		data->pdataStreamList = NULL;
	}
	if(data->prgNameLen > 0){
		free(data->prgName);
		data->prgName = NULL;
	}
	if(data->providerNameLen > 0){
		free(data->providerName);
		data->providerName = NULL;
	}
	if(data->psdtDesListLen > 0){
		for(i=0; i< data->psdtDesListLen; i++){			
			if(data->psdtDesList[i].dataLen){
				free(data->psdtDesList[i].data);
				data->psdtDesList[i].data = NULL;					
			}		
		}		
		free(data->psdtDesList);
		data->psdtDesList = NULL;
	}
}

int freeUserProgramsMalloc(User_prgInfo_t * data)
{
	int i = 0, j=0;
	int pmtDesListLen = data->pmtDesListLen;
	for(i=0; i < pmtDesListLen; i++){
			free(data->pmtDesList[i].data);
			data->pmtDesList[i].data = NULL;
	}
	if(data->pmtDesListLen>0){
		free(data->pmtDesList);
		data->pmtDesList = NULL;
	}
	int pdataStreamListLen = data->pdataStreamListLen;
	if(pdataStreamListLen>0){
		for(i=0; i< pdataStreamListLen; i++){
			for(j=0; j < data->pdataStreamList[i].desNodeLen; j++)
			{
				if(data->pdataStreamList[i].desNode[j].dataLen){
					free(data->pdataStreamList[i].desNode[j].data);
					data->pdataStreamList[i].desNode[j].data = NULL;
				}
			}
			if(data->pdataStreamList[i].desNodeLen>0){
				free(data->pdataStreamList[i].desNode);
				data->pdataStreamList[i].desNode = NULL;
			}
		}
		free(data->pdataStreamList);
		data->pdataStreamList = NULL;
	}
	if(data->prgNameLen > 0){
		free(data->prgName);
		data->prgName = NULL;
	}
	if(data->providerNameLen > 0){
		free(data->providerName);
		data->providerName = NULL;
	}
	if(data->psdtDesListLen > 0){
		for(i=0; i< data->psdtDesListLen; i++){
			if(data->psdtDesList[i].dataLen){
				free(data->psdtDesList[i].data);
				data->psdtDesList[i].data = NULL;
			}
		}
		free(data->psdtDesList);
		data->psdtDesList = NULL;
	}
}

int freeUserPrograms(list_t *prginfolist)
{
	if(!prginfolist) {
		printf("prginfolist==NULL not need to free.\n");
		return -1;
	}


	User_prgInfo_t *ptmpPrgInfo = NULL;
	int i;
	int len = list_len(prginfolist);
	for(i= len -1 ;i>-1;i--){
		list_get(prginfolist,i, &ptmpPrgInfo);
		//free ptemp
		if(ptmpPrgInfo != NULL){
			freeUserProgramsMalloc(ptmpPrgInfo);
			list_pop(prginfolist,i);
			ptmpPrgInfo = NULL;
		}

	}

	prginfolist = NULL;


	return 0;
}

int freePrograms(list_t *prginfolist)
{
	if(!prginfolist) {
		printf("prginfolist==NULL not need to free.\n");
		return -1;
	}


	Dev_prgInfo_st *ptmpPrgInfo = NULL;
	int i;
	int len = list_len(prginfolist);
	for(i= len -1 ;i>-1;i--){
		list_get(prginfolist,i, &ptmpPrgInfo);
		//free ptemp
		if(ptmpPrgInfo != NULL){
			freeProgramsMalloc(ptmpPrgInfo);			
			list_pop(prginfolist,i);
			ptmpPrgInfo = NULL;
		}		
		
	}

	prginfolist = NULL;

	
	return 0;
}

int freecanode(list_t *caIdenList){
    int i = 0;
    CA_descriptor *ca = NULL;
    if(caIdenList != NULL){
        if(list_len(caIdenList)>0){
            for(i=list_len(caIdenList)-1;i>-1;i--){
                list_get(caIdenList, i, &ca);
                if(ca->private_data_byte_len>0){
                    free(ca->private_data_byte);
                    ca->private_data_byte = NULL;
                    ca->private_data_byte_len = 0;
                }
                free(ca);
                ca = NULL;
                list_pop(caIdenList, i);
            }
        }
        //free(caIdenList);
        caIdenList = NULL;
    }

}