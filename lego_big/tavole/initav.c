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

/*
 initav.c
 Alloca in memoria e legge le tavole del vapore presenti nel file TAVOLE.DAT
*/
int main()
{
int iret;
ism01_(&iret);
if(iret == 98)
	printf("\nATTENZIONE: la shared memory non e' stata caricata!!!");
else
if(iret == 97)
	printf("\n TAVOLE DEL VAPORE GIA` PRESENTI\n\n");
else
	printf("\n TAVOLE DEL VAPORE CARICATE CORRETTAMENTE\n\n");
}
