/**********************************************************************
*
*       C Source:               XlEnv.c
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed May 16 17:22:55 2001 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlEnv.c-4 %  (%full_filespec: XlEnv.c-4:csrc:1 %)";
#endif
/*
   modulo XlEnv.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlEnv.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlEnv.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xl/Xl.h>
#if defined VMS
#include <ctype.h>
#include <unixlib.h>
#include <ssdef.h>
#include <lnmdef.h>
#include <descrip.h>
#include <processes.h>

typedef struct itmlst{
 short buflen;
 short code;
 char *bufadr;
 char *retlenadr;
 int end_list;} ITMLIST;
#else
#include <stdlib.h>
#include <unistd.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */
#endif

#if defined VMS || defined SCO_UNIX
double rint();
#endif


char *XlGetenv(char * variabile)
{
/*printf("XlGetenv:   %s = %s\n",variabile,getenv(variabile));*/
return(getenv(variabile));
}

Boolean XlPutenv(char *variabile, char* valore)
{
#if defined VMS
int lun_ritorno;
ITMLIST item_list;
struct dsc$descriptor_s name_desc;
struct dsc$descriptor_s tabella_desc;
int status;

/*
	controllo la stringa valore
*/
if(valore==NULL)
	return(False);
if(strlen(valore)==0)
	return(False);

item_list.end_list=0;
item_list.buflen=strlen(valore);
item_list.code=LNM$_STRING;
item_list.bufadr=valore;
item_list.retlenadr=(&lun_ritorno);

name_desc.dsc$b_class=DSC$K_CLASS_S;
name_desc.dsc$b_dtype=DSC$K_DTYPE_T;
name_desc.dsc$a_pointer=variabile;
name_desc.dsc$w_length=strlen(variabile);

tabella_desc.dsc$b_class=DSC$K_CLASS_S;
tabella_desc.dsc$b_dtype=DSC$K_DTYPE_T;
tabella_desc.dsc$a_pointer="LNM$PROCESS_TABLE";
tabella_desc.dsc$w_length=strlen("LNM$PROCESS_TABLE");

status=sys$crelnm(0,&tabella_desc,&name_desc,0,&item_list);
if((status!=SS$_NORMAL)||(status!=SS$_SUPERSEDE))
        {
        LIB$SIGNAL(status);
        return(True);
        }
valore[*(item_list.retlenadr)]=0;
return(False);
#else
char *appoggio;

appoggio=(char*)malloc(strlen(variabile)+strlen(valore)+5);
sprintf(appoggio,"%s=%s",variabile,valore);
if(putenv(appoggio)==0)
	return(True);
else
	{
	free(appoggio);
	return(False);
	}
#endif
}


char *XlConvPathVms(char *path_in)
{
static char path[1024];
#if defined VMS
char app_path[1024];

char *app;
int cont,k;
strcpy(path,path_in);
strcpy(app_path,path);
app=strstr(app_path,"!");
if (app == NULL) return (path_in);

strcpy(path,&app[2]);
app=strstr(path,"/");
cont=0;
while((app=strstr(app,"/"))!=NULL)
        {
        ++app;
        cont++;
        }
app=strstr(path,"/");
app[0]='[';
for(k=0;k<cont-1;k++)
        {
        app=strstr(app,"/");
        if(k!= cont-2)
                app[0]='.';
        else
                app[0]=']';
        ++app;
        }
app=strstr(path,"[");
strcpy(app_path,app);
app[0]=':';
app[1]=0;
strcat(path,app_path);
return(path);
#else
strncpy(path,path_in,sizeof(path)-1);
return(path);
#endif
}


#if defined VMS || defined SCO_UNIX
double rint( val)
double val;
{
if(val>0)
        {
        if((val-(int)val)>=0.5)
                return((double)((int)val+1.));
        else
                return((double)((int)val));
        }
else
        {
        if(((int)val-val)>0.5)
                return((double)((int)val-1.));
        else
                return((double)((int)val));
        }
}
#endif

char *XlGetDir(char *lista)
{
char comando[300];
FILE *fp;
char riga[FILENAME_MAX],riga2[FILENAME_MAX];
char *ritorno=NULL;
char *app;
char *riga_app;
int k;

/*
	system non supportata
*/
if(system(NULL)==0)
	return(NULL);

#if defined VMS

	sprintf(comando,"dir/column=1/version=1/output=__lista__.lista");
	if (lista!=NULL)
		sprintf(comando,"%s %s",comando,lista);
#else
	if (lista!=NULL)
		sprintf(comando,"ls -1 %s > __lista__.lista",lista);
else
		sprintf(comando,"ls -1 > __lista__.lista");
#endif
	system(comando);

	if((fp=fopen("__lista__.lista","r"))==NULL)
		return(NULL);
#if defined VMS
	if(fscanf(fp,"\n%s %s\n\n",riga,riga2)==EOF)
		{
		system("delete __lista__.lista;*");
		return(NULL);
		}
#endif
	while(fscanf(fp,"%s\n",riga)!=EOF)
		{
#if defined VMS
		if(strcmp("Total",riga)==0)
			break;
		app=strstr(riga,";");
		if(app!=NULL)
			memset(app,0,1);
		riga_app = riga;
#endif
#if defined UNIX
		riga_app = riga;
            printf("\t riga=%s [%d]\n\n",riga,strlen(riga));
		app=strstr(riga,"/");
		if(app!=NULL)
			for(k=(strlen(riga)-1);k>=0;k--)
				if(riga[k]=='/')
					{
					riga_app=&riga[k+1];
					break;
					}
#endif
		if (ritorno == NULL)
			ritorno = calloc (strlen(riga_app)+2,sizeof(char));
		else 
			ritorno=realloc
				(ritorno,strlen(ritorno)+strlen(riga_app)+2);
		strcat (ritorno,riga_app);
		strcat (ritorno,"\n");
		}
	fclose(fp);
#if defined VMS
	for(k=0;k<strlen(ritorno);k++)
		ritorno[k]=tolower(ritorno[k]);
#endif

#if defined VMS
	system("delete __lista__.lista;*");
#else
	system("rm __lista__.lista");
#endif

return(ritorno);
}

Boolean XlRename(char * file_1, char * file_2)
{
char comando[500];
/*
	system non supportata
*/
if(system(NULL)==0)
	return(False);

#if defined VMS
	sprintf(comando,"rename/nolog %s %s",file_1,file_2);
#else
	sprintf(comando,"mv %s %s",file_1,file_2);
#endif
	system(comando);
return(True);
}

Boolean XlCopy(char * file_1, char * file_2)
{
char comando[500];
/*
	system non supportata
*/
if(system(NULL)==0)
	return(False);

#if defined VMS
	sprintf(comando,"copy/nolog %s %s",file_1,file_2);
#else
	sprintf(comando,"cp %s %s",file_1,file_2);
#endif

	system(comando);
return(True);
}

Boolean XlRemove(char * file)
{
char comando[500];
/*
	system non supportata
*/
if(system(NULL)==0)
	return(False);

#if defined VMS
	sprintf(comando,"delete/nolog %s;*",file);
#else
	sprintf(comando,"rm %s",file);
#endif

	system(comando);
return(True);
}


int  XlChDir(char *path)
{
        return(chdir(XlConvDirVms(path)));
}

char *XlConvDirVms(char *path_in)
{
#if defined VMS
char app_path[1024];
static char path[1024];
char *app;
int cont,k;
strcpy(path,path_in);
strcpy(app_path,path);
app=strstr(app_path,"!");
if (app == NULL) return (path_in);
strcpy(path,&app[2]);
app=strstr(path,"/");
cont=0;
while((app=strstr(app,"/"))!=NULL)
        {
        ++app;
        cont++;
        }
app=strstr(path,"/");
app[0]='[';
for(k=0;k<cont-1;k++)
        {
        app=strstr(app,"/");
        app[0]='.';
        ++app;
        }
app=strstr(path,"[");
strcpy(app_path,app);
app[0]=':';
app[1]=0;
strcat(path,app_path);
strcat(path,"]");
printf("XlConvDirVms ret = %s\n",path);
return(path);
#else
return(path_in);
#endif
}


char *XlUnconvPathVms(char *path_in)
{
#if defined UNIX
return(path_in);
#else
char path[1024];
char path_r[1024];
int i,k,lun;
char *app;
char *macchina;
Boolean fine_conv = False;
char *ret;
        printf("XlUnconvPathVms => %s\n",path_in);
        app=strstr(path_in,":");
        strcpy(path,path_in);
        if(app == NULL)
                sprintf(&path[0],"%s%s",getenv("SYS$SYSDEVICE"),path_in);
        else
                {
                app=strstr(path_in,"$");
                if( app == NULL)
                        {
                        macchina = getenv("SYS$SYSDEVICE");
                        app = strstr(macchina,"$");
                        *app =0;
                        sprintf(&path[0],"%s$%s",macchina,path_in);
                        }
                }
        /*  elab. path */
        lun=strlen(path);
        k=0;
        for(i=0;i<lun;i++)
            {
            if(path[i] == ']')
                {
                fine_conv = True;
                path_r[k]='/'; k++;continue;
                }
            if((path[i] == '.')&& !fine_conv)
                {
                path_r[k]='/'; k++;continue;
                }
            if((path[i] == ':')&& !fine_conv)
                {
                path_r[k]='/'; k++;i++;continue;
                }
            if((path[i] == '$')&& !fine_conv)
                {
                path_r[k]='!'; k++;path_r[k]='/'; k++;continue;
                }
            path_r[k]=path[i];
            k++;
            }
        path_r[k] = 0;
        ret = (char*) malloc(strlen(path_r)+1);
        strcpy(ret,path_r);
return(ret);
#endif
}

char *XlUnconvDirVms(char *path_in)
{
#if defined UNIX
return(path_in);
#else
char path[1024];
char path_r[1024];
int i,k,lun;
char *app;
char *macchina;
Boolean fine_conv = False;
char *ret;
        printf("XlUnconvDirVms => %s\n",path_in);
        app=strstr(path_in,":");
        strcpy(path,path_in);
        if(app == NULL)
                sprintf(&path[0],"%s%s",getenv("SYS$SYSDEVICE"),path_in);
        else
                {
                app=strstr(path_in,"$");
                if( app == NULL)
                        {
                        macchina = getenv("SYS$SYSDEVICE");
                        app = strstr(macchina,"$");
                        *app =0;
                        sprintf(&path[0],"%s$%s",macchina,path_in);
                        }
                }
        /*  elab. path */
        lun=strlen(path);
        k=0;
        for(i=0;i<lun;i++)
            {
            if(path[i] == ']')
                break;
            if((path[i] == '.')&& !fine_conv)
                {
                path_r[k]='/'; k++;continue;
                }
            if((path[i] == ':')&& !fine_conv)
                {
                path_r[k]='/'; k++;i++;continue;
                }
            if((path[i] == '$')&& !fine_conv)
                {
                path_r[k]='!'; k++;path_r[k]='/'; k++;continue;
                }
            path_r[k]=path[i];
            k++;
            }
        path_r[k] = 0;
        ret = (char*) malloc(strlen(path_r)+1);
        strcpy(ret,path_r);
return(ret);
#endif
}
                                                    
                  
/*
   funzione introdotta per superare i limiti
   imposti dalla esecuzione della ls, quando 
   la lista di file ritornata e' troppo lunga
   Problema nativo del s.o.
   Argomenti:
	char *path:	nella forma "/usr/user/pippo"
	char *ext	nella forma "ico" "bmp" etc.
*/
char *XlGetDir2(char *path,char *ext)
{
char comando[400];
FILE *fp;
char riga[FILENAME_MAX],riga2[FILENAME_MAX];
char *ritorno=NULL;
char *app;
char *riga_app;
int k;
#if defined VMS
char lista[400];
#endif


/*
	system non supportata
*/
if(system(NULL)==0)
	return(NULL);

#if defined VMS

	sprintf(lista,"%s/*.%s",path,ext);
	sprintf(comando,"dir/column=1/version=1/output=__lista__.lista");
	if (lista!=NULL)
		sprintf(comando,"%s %s",comando,lista);
#else
	if (path!=NULL && ext!=NULL )
		sprintf(comando,"ls -1 %s | grep .%s > __lista__.lista",path,ext);
else
		sprintf(comando,"ls -1 > __lista__.lista");
#endif
	system(comando);

	if((fp=fopen("__lista__.lista","r"))==NULL)
		return(NULL);
#if defined VMS
	if(fscanf(fp,"\n%s %s\n\n",riga,riga2)==EOF)
		{
		system("delete __lista__.lista;*");
		return(NULL);
		}
#endif
	while(fscanf(fp,"%s\n",riga)!=EOF)
		{
#if defined VMS
		if(strcmp("Total",riga)==0)
			break;
		app=strstr(riga,";");
		if(app!=NULL)
			memset(app,0,1);
		riga_app = riga;
#endif
#if defined UNIX
		riga_app = riga;
            printf("\t riga=%s [%d]\n\n",riga,strlen(riga));
		app=strstr(riga,"/");
		if(app!=NULL)
			for(k=(strlen(riga)-1);k>=0;k--)
				if(riga[k]=='/')
					{
					riga_app=&riga[k+1];
					break;
					}
#endif
		if (ritorno == NULL)
			ritorno = calloc (strlen(riga_app)+2,sizeof(char));
		else 
			ritorno=realloc
				(ritorno,strlen(ritorno)+strlen(riga_app)+2);
		strcat (ritorno,riga_app);
		strcat (ritorno,"\n");
		}
	fclose(fp);
#if defined VMS
	for(k=0;k<strlen(ritorno);k++)
		ritorno[k]=tolower(ritorno[k]);
#endif

#if defined VMS
	system("delete __lista__.lista;*");
#else
	system("rm __lista__.lista");
#endif

return(ritorno);
}
