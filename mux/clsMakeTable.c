
#include <stdio.h>
#include "EnumDef.h"
#include "datastructdef.h"
#include "list.h"

extern ClsMux_st *pclsMux;
extern ClsParams_st *pdb;

extern ClsProgram_st clsProgram;
static int isTableInit=0;
int MakeTable(int outChnId)
{
	ChannelProgramSt *outpst = NULL;
	list_get(&(clsProgram.outPrgList), outChnId-1, &outpst);
	buildTable(outChnId,pdb->pvalueTree->poutChnArray,outpst->prgNodes);

}

int buildTable(int outChnId, 	DatabaseOutputChannel_st *outChnArray,	list_t  prginfolist)
{
	//Error_psiTable errFlag;
	int outChnIndex = outChnId - 1;
	int rstPat;
	int i;


	ClsMuxInit(2,2);
	ClsMuxInit(2,2);


	//int streamId = 1;
	//int netWorkId = 16;
	int version = 2;
	unsigned char patTable[188];
	unsigned char pmtTable[188];
	Dev_prgInfo_st *ptmpPrgInfo;

	int streamId = outChnArray[outChnIndex].streamId;
	int netWorkId = outChnArray[outChnIndex].networkId;
	int oringinalNetworkId = outChnArray[outChnIndex].oringal_networkid;
	//int version = outChnArray[outChnIndex].version;

	// PAT

	/*
	if (outChnArray[outChnIndex].isAutoRaiseVersion) // �Զ����Ӱ汾��
	{
	version++;
	if (version > 31)
	version = 0;
	outChnArray[outChnIndex].version = version;
	//db.SettingSave();
	}
	*/
	BufferUn_st  *pbuff;


	rstPat=CreatePatStan(prginfolist,patTable, streamId, netWorkId, version);	


	list_get(&pclsMux->table_pat,outChnIndex,&pbuff);
	memcpy(pbuff->pbuf, patTable, sizeof(patTable));
	pbuff->bufLen=sizeof(patTable);
	//ist_set(&pclsMux->table_pat,outChnIndex,pbuff);

	if (!rstPat)
	{
		CleanOutputTable(outChnId);
		//ShowCreatTableErrorInfo(errFlag, "[PAT]");
		return 0;
	}
	else
	{	
		printf("make pat SUCCESSFULL---%d\n",rstPat);
	}


#if 0
	pat_senction_st *p_pat = (pat_senction_st*)malloc(sizeof(pat_senction_st));	
	rstPat=ParsePat(pbuff->pbuf, 5, p_pat);
	if(rstPat)
	{
		printPAT(p_pat);
	}

#endif


	list_t *tablePmt;
	list_get(&pclsMux->table_pmtList, outChnIndex, &tablePmt);
	while(list_len(tablePmt))
	{
		list_pop_tail(tablePmt);	
	}

	//for (i = 0; i < list_len(&prginfolist); i++)
	for (i = 0; i < 1; i++)
	{
		list_get(&prginfolist, i, &ptmpPrgInfo);	
		rstPat=CreatePmtStan(ptmpPrgInfo,pmtTable,version);

		if(rstPat)
		{
			printf("make pmt SUCCESSFULL---%d\n",rstPat);
			BufferUn_st *pmtbuff =(BufferUn_st*)malloc(sizeof(BufferUn_st));
			
			pmtbuff->pbuf=malloc(sizeof(pmtTable));
			memcpy(pmtbuff->pbuf, pmtTable, sizeof(pmtTable));
			pmtbuff->bufLen=sizeof(pmtTable);
			list_append(tablePmt,pmtbuff);	

		}
		else
		{
			return 0;

		}


#if 0

		BufferUn_st  *outPMTBuffer;
		list_get(&pclsMux->table_pmtList, outChnIndex, &tablePmt);
		list_get(tablePmt,i,&outPMTBuffer);
		pmt_senction_st *PMTS = (pmt_senction_st*)malloc(sizeof(pmt_senction_st));
		int rst=ParsePmt(outPMTBuffer->pbuf, 5, PMTS);
		printPMT(PMTS);
#endif

	}	

return 1;






}

void printPAT(pat_senction_st* PATS)
{
	int i;

	printf("crc32----%d\n",PATS->crc32);
	printf("current_next_indicator----%d\n",PATS->current_next_indicator);
	printf("flag_0----%d\n",PATS->flag_0);
	printf("last_section_number----%d\n",PATS->last_section_number);
	printf("reserved0----%d\n",PATS->reserved0);
	printf("reserved1----%d\n",PATS->reserved1);
	printf("section_length----%d\n",PATS->section_length);
	printf("section_number----%d\n",PATS->section_number);
	printf("section_syntax_indicator----%d\n",PATS->section_syntax_indicator);

	printf("table_id----%d\n",PATS->table_id);
	printf("transport_stream_id----%d\n",PATS->transport_stream_id);
	printf("version_number----%d\n",PATS->version_number);

	patPrg_t* p_program = PATS->patPrg_tList;
	for (i = 0; i < PATS->patPrg_tListLen; i++)
	{
		printf("patPrg_st pid----%d\n",p_program->pid);
		printf("patPrg_st program_number ----%d\n",p_program->program_number);
		printf("patPrg_st reserved----%d\n",p_program->reserved);
		p_program++;
	}


}


void printPMT(pmt_senction_st* PMTS)
{

	int i;
	int j;

	printf("crc32----%d\n",PMTS->crc32);
	printf("current_next_indicator----%d\n",PMTS->current_next_indicator);
	printf("flag_0----%d\n",PMTS->flag_0);
	printf("last_section_number----%d\n",PMTS->last_section_number);
	printf("pcrPid----%d\n",PMTS->pcrPid);
	printf("program_info_length----%d\n",PMTS->program_info_length);
	printf("program_number----%d\n",PMTS->program_number);

	printf("reserved0----%d\n",PMTS->reserved0);
	printf("reserved1----%d\n",PMTS->reserved1);
	printf("reserved2----%d\n",PMTS->reserved2);
	printf("reserved3----%d\n",PMTS->reserved3);
	printf("section_length----%d\n",PMTS->section_length);
	printf("section_number----%d\n",PMTS->section_number);
	printf("section_syntax_indicator----%d\n",PMTS->section_syntax_indicator);

	printf("table_id----%d\n",PMTS->table_id);
	printf("version_number----%d\n",PMTS->version_number);


	int k;

	Commdes_t* p_program = PMTS->pmtDesList;

	for (i = 0; i < PMTS->pmtDesListLen; i++)
	{

		printf("Commdes_st index----%d\n",p_program->index);
		printf("Commdes_st tag ----%d\n",p_program->tag);
		printf("Commdes_st userNew----%d\n",p_program->userNew);
		printf("Commdes_st dataLen %d \n",p_program->dataLen);



		unsigned char *ptmp=p_program->data;

		for (k = 0; k < p_program->dataLen; k++)
		{	  

			printf(" %d  ",*ptmp++);		  

		}




		p_program++;

	}


	DataStream_t* p_last_DataStream_t = PMTS->pdataStreamList;

	for (i = 0; i < PMTS->pdataStreamListLen; i++)
	{
		printf("DataStream_st inChn----%d\n",p_last_DataStream_t->inChn);
		printf("DataStream_st index ----%d\n",p_last_DataStream_t->index);
		printf("DataStream_st inPid----%d\n",p_last_DataStream_t->inPid);
		printf("DataStream_st outPid----%d\n",p_last_DataStream_t->outPid);
		printf("DataStream_st streamType----%d\n",p_last_DataStream_t->streamType);


		p_program = p_last_DataStream_t->desNode;
		for (j = 0; j< p_last_DataStream_t->desNodeLen; j++)
		{
			printf("Commdes_st index----%d\n",p_program->index);
			printf("Commdes_st tag ----%d\n",p_program->tag);
			printf("Commdes_st userNew----%d\n",p_program->userNew);
			printf("Commdes_st dataLen is :%d  vList: ",p_program->dataLen);


			unsigned char *ptmp=p_program->data;

			for (k = 0; k < p_program->dataLen; k++)
			{

				printf(" %d  ",*ptmp++);			

			}

			p_program++;
		}

		p_last_DataStream_t++;
	}
}

int AutoMakeNewPid(int outChannel)
{

}

int MakePidMapTable(int outChannel)
{

}

int CleanOutputTable(int outChannel)
{
	/*
	BufferUn_st  *pbuff;
	int outChnIndex=outChannel-1;
	list_get(&pclsMux->table_pat,outChnIndex,&pbuff);

	if(pbuff!=NULL)
	{
	list_append(&pclsMux->table_pat,NULL);
	}
	*/

}



void ClsMuxInit(int _outMaxNum,int treeView_inLength)
{
	int i;
	if(isTableInit)
	{

		printf("pclsMuxb is already not null****\n");
	}
	else
	{	
		printf("go to init pclsMuxb****\n");

		if (_outMaxNum > 0)
		{
			pclsMux=(ClsMux_st*)malloc(sizeof(ClsMux_st));
			list_init(&pclsMux->table_pat);
			list_init(&pclsMux->table_pmtList);
		}
		for (i = 0; i < treeView_inLength; i++)
		{
			if (_outMaxNum > 0)
			{
				BufferUn_st *pbuff =(BufferUn_st*)malloc(sizeof(BufferUn_st));
				pbuff->pbuf=malloc(188);
				pbuff->bufLen=999;
				list_append(&pclsMux->table_pat,pbuff);


				list_t *table_pmt=(list_t*)malloc(sizeof(list_t));
				list_init(table_pmt);
				list_append(&pclsMux->table_pmtList,table_pmt);

			}
		}	
		isTableInit=1;

	}

}



