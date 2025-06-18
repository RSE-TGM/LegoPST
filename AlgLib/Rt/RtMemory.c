/**********************************************************************
*
*       C Source:               %name%
*       Subsystem:              %subsystem%
*       Description:
*       %created_by:    %
*       %date_created:  %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: %  (%full_filespec: %)";
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <Rt/Rt.h>
#include <sys/types.h>
#include <unistd.h>


#define DIM_TAPPO_RTMEMORY	16
#define DIM_LUN_NOME_FILE	32
#define TAPPO_RTMEMORY		"asdfghjklqwerty"

#define REALLOC_RTPUNT		1000

typedef struct {
	void * punt;
	size_t    size;
	int line;
	char file[32];
        } PUNT_RTMEMORY;

PUNT_RTMEMORY *punt_rtmemory = NULL;
int num_punt_rtmemory = -1;

static void alloca_punt_rtmemory();
static void ins_tappo_rtmemory();

size_t RtMemoryAllocated(void * punt)
{
size_t ret = 0;
int i;

if(num_punt_rtmemory == -1)
	alloca_punt_rtmemory();

if(punt != NULL)
	{
	/*
		cerca la posizione del puntatore
	*/
	for(i=0;i<num_punt_rtmemory;i++)
		if(punt_rtmemory[i].punt == punt)
			break;
	if(i == num_punt_rtmemory)
		exit(fprintf(stderr,
			"Errore [RtMemoryAllocated] puntatore non trovato\n"));
	ret = punt_rtmemory[i].size;
	}
else
	{
	for(i=0;i<num_punt_rtmemory;i++)
		if(punt_rtmemory[i].punt != 0)
			ret += punt_rtmemory[i].size;
	}
return(ret);
}


void *RtMalloc(size_t size, char *file, int line)
{
char *ret = NULL;
int i;

if(num_punt_rtmemory == -1)
	alloca_punt_rtmemory();

ret=malloc(size + 2*DIM_TAPPO_RTMEMORY);
ins_tappo_rtmemory(ret,size + 2*DIM_TAPPO_RTMEMORY);
ret += DIM_TAPPO_RTMEMORY;

if(ret == NULL)
	{
	fprintf(stderr,
	"Errore [RtMalloc] impossibile eseguire la malloc size = %d pid=[%d]\n",
		size,getpid());
	return(NULL);
	}

/*
	cerca la prima posizione libera
*/
for(i=0;i<num_punt_rtmemory;i++)
	if(punt_rtmemory[i].punt == 0)
		break;
if(i == num_punt_rtmemory-10)
	alloca_punt_rtmemory();

punt_rtmemory[i].punt = ret;
punt_rtmemory[i].size = size;
punt_rtmemory[i].line = line;
strncpy(punt_rtmemory[i].file, file,DIM_LUN_NOME_FILE-1);

#if defined PRINT_DEBUG_MALLOC
printf("RtMalloc [%d]  size = %d/%d punt = %d file %s line %d\n",
		i,size,RtMemoryAllocated(NULL),ret,file,line);
#endif

return((void*)ret);
}

void *RtCalloc(size_t size,size_t num_ele,char *file, int line)
{
void *ret;

ret=RtMalloc(size*num_ele,file,line);

if(ret!=NULL)
	memset(ret,0,size*num_ele);

return(ret);
}

void *RtRealloc(void *punt_ing,size_t size,char *file, int line)
{
char  *ret;
char  *app_punt_ing;
int i;

if(num_punt_rtmemory == -1)
	alloca_punt_rtmemory();

if(punt_ing == NULL)
	{
	ret=RtMalloc(size,file,line);
#if defined PRINT_DEBUG_MALLOC
	printf("RtRealloc  size = %d/%d punt = %d\n",
		size,RtMemoryAllocated(NULL),ret);
#endif
	}
else
	{
	/*
		cerca la posizione del puntatore
	*/
	for(i=0;i<num_punt_rtmemory;i++)
		if(punt_rtmemory[i].punt == punt_ing)
			break;
	if(i == num_punt_rtmemory)
		exit(fprintf(stderr,"Errore [RtRealloc] puntatore non trovato\n"));
	app_punt_ing = (char *)punt_ing;
	app_punt_ing -= DIM_TAPPO_RTMEMORY;
	ret = realloc(app_punt_ing,size + 2*DIM_TAPPO_RTMEMORY);
	ins_tappo_rtmemory(ret,size + 2*DIM_TAPPO_RTMEMORY);
	ret += DIM_TAPPO_RTMEMORY;
	punt_rtmemory[i].punt = ret;
	punt_rtmemory[i].size = size;
	punt_rtmemory[i].line = line;
	strncpy(punt_rtmemory[i].file, file,DIM_LUN_NOME_FILE-1);
#if defined PRINT_DEBUG_MALLOC
	printf("RtRealloc [%d]  size = %d/%d punt = %d\n",
		i,size,RtMemoryAllocated(NULL),ret);
#endif
	}
return((void *)ret);
}

void  RtFree(void *ing, char *file, int line)
{
int i;
char *punt = ing;

if(num_punt_rtmemory == -1)
	alloca_punt_rtmemory();

if(punt==NULL)
	{
	fprintf(stderr,"Errore [RtFree] puntatore NULL file %s line %d\n",
			file,line);
	return;
	}
/*
	cerca la posizione del puntatore
*/
for(i=0;i<num_punt_rtmemory;i++)
	if(punt_rtmemory[i].punt == punt)
		break;
if(i == num_punt_rtmemory)
	{
	fprintf(stderr,"Errore [RtFree] puntatore=%s non trovato pid=[%d] file %s line %d\n",punt, getpid(),file,line);
	return;
	}

if(punt_rtmemory[i].size > 0)
	punt -= DIM_TAPPO_RTMEMORY;

free(punt);

#if defined PRINT_DEBUG_MALLOC
printf("RtFree [%d]  [%d] size = %d/%d file %s line %d\n       Allocazione file %s line %d\n",i,punt,punt_rtmemory[i].size,
	RtMemoryAllocated(NULL),file,line,punt_rtmemory[i].file,
	punt_rtmemory[i].line);
#endif
punt_rtmemory[i].punt = 0;
punt_rtmemory[i].size = 0;
punt_rtmemory[i].line = 0;
punt_rtmemory[i].file[0]=0;
}


static void alloca_punt_rtmemory()
{
int i;

   if(num_punt_rtmemory <0)
	num_punt_rtmemory  =0;

   num_punt_rtmemory += REALLOC_RTPUNT;
   punt_rtmemory = (PUNT_RTMEMORY*) realloc(punt_rtmemory,
			num_punt_rtmemory*sizeof(PUNT_RTMEMORY));
   memset(&punt_rtmemory[num_punt_rtmemory-REALLOC_RTPUNT],0,
		sizeof(PUNT_RTMEMORY)*REALLOC_RTPUNT);

   printf("ATTENZIONE: REALLOC PUNT MEMORY -> [%d]\n",num_punt_rtmemory);
}

Boolean RtCheckPointer(void *punt)
{
char *p;
int i;
Boolean ret = True;

#if defined DEBUG_MALLOC
if(num_punt_rtmemory == -1)
	alloca_punt_rtmemory();

if(punt!=NULL)
	{
	/*
		cerca la posizione del puntatore
	*/
	for(i=0;i<num_punt_rtmemory;i++)
		if(punt_rtmemory[i].punt == punt)
			break;
	if(i == num_punt_rtmemory)
		{
		fprintf(stderr,
	  	"Errore [RtCheckPointer] pointer non trovato pid=[%d]\n",
			getpid());
		return;
		}

	p = (char *) punt_rtmemory[i].punt;
	p -= DIM_TAPPO_RTMEMORY;
	if(strcmp(p,TAPPO_RTMEMORY)!= 0)
		{
		fprintf(stderr,"Errore [RtCheckPointer] pointer %d sporco in basso\n",
			punt_rtmemory[i].punt);
		ret = False;
		}

	p += (DIM_TAPPO_RTMEMORY + punt_rtmemory[i].size);

	if(strcmp(p,TAPPO_RTMEMORY)!= 0)
		{
		fprintf(stderr,"Errore [RtCheckPointer] pointer %d sporco in alto\n",
			punt_rtmemory[i].punt);
		ret = False;
		}
	}
else
	{
	for(i=0;i<num_punt_rtmemory;i++)
             if(punt_rtmemory[i].punt != NULL)
		{
#if defined PRINT_DEBUG_MALLOC
printf("RtCheckPointer [%d][%d] size = %d/%d  allocato file %s line %d\n",
	i,
	punt_rtmemory[i].punt,
	punt_rtmemory[i].size,
	RtMemoryAllocated(NULL),
	punt_rtmemory[i].file,
	punt_rtmemory[i].line);
#endif
		p = (char *) punt_rtmemory[i].punt;
		p -= DIM_TAPPO_RTMEMORY;
		if(strcmp(p,TAPPO_RTMEMORY)!= 0)
			{
			fprintf(stderr,
		        "Errore [RtCheckPointer] punt [%d] sporco in basso\n",
				punt_rtmemory[i].punt);
			ret = False;
			}

		p += (DIM_TAPPO_RTMEMORY + punt_rtmemory[i].size);

		if(strcmp(p,TAPPO_RTMEMORY)!= 0)
			{
			fprintf(stderr,
		         "Errore [RtCheckPointer] punt [%d] sporco in alto\n",
				punt_rtmemory[i].punt);
			ret = False;
			}
		}
	}
#endif

return(ret);

}

static void ins_tappo_rtmemory(char * punt, int size)
{
strcpy(punt,TAPPO_RTMEMORY);

punt += (size - DIM_TAPPO_RTMEMORY);

strcpy(punt,TAPPO_RTMEMORY);
}

char *RtNewString(char *stringa,char *file, int line)
{
char *ret;

if(stringa == NULL)
        return(NULL);

ret = RtMalloc(strlen(stringa)+1,file,line);
strcpy(ret,stringa);

return(ret);
}


void * RtIntRecord(void *punt,char *file, int line)
{
int i;

if(punt ==NULL)
	return(NULL);

if(num_punt_rtmemory == -1)
	alloca_punt_rtmemory();
/*
	cerca la prima posizione libera
*/
for(i=0;i<num_punt_rtmemory;i++)
	if(punt_rtmemory[i].punt == 0)
		break;
if(i == num_punt_rtmemory-10)
	alloca_punt_rtmemory();

punt_rtmemory[i].punt = punt;
punt_rtmemory[i].size = 0;
strncpy(punt_rtmemory[i].file, file,DIM_LUN_NOME_FILE-1);
punt_rtmemory[i].line = line;

return(punt);
}

#ifndef DEBUG_MALLOC
void *RtRecord(void *punt)
{
        return(punt);
}
#endif
