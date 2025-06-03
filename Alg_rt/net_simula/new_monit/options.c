/*
 * options.c
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/ToggleBG.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/SeparatoG.h>
#include <Xm/Separator.h>
#include <Xm/Xm.h>

#include "UxXt.h"

#include "sim_param.h"
#include "sim_types.h"
#include "dispatcher.h"


extern int _MAX_SNAP_SHOT;
extern char *FILES_PATH;

#include "parametri.h"
#include "bistrutt.h"
#include "option.h"
extern int opt_interface_active;
int present_context;
#include "messaggi.h"
#include "refresh.h"
#include "programLauncher.h"

#include "option.h"
#include "filtri.h"
#include "tabelle_malf.h"
#include "banco_globals.h"

#include "richiestaDati.h"
#include "libdispatcher.h"

extern int isSuperuser;
extern int tempo_finale_impostato;

/*
 * IC protette per default
 */
int IcDefProt[] = {-1};
/* eliminata protezione di default dopo la parametrizzazione del numero di IC */
/*   
   {0,1,2,3,-1};
*/
Widget optionSelRc;
Widget optionSelLabel;
Widget optionInitCleartbg;
Widget optionInitBttbg;
Widget optionInitIctbg;
Widget optionSnapAvailtbg;
Widget optionSnapSavetbg;
Widget optionSnapDeletetbg;
Widget optionSnapCopyPastetbg;
Widget optionRecAvailtbg;
Widget optionRecBttbg;
Widget optionRecCrtbg;
Widget optionRecItaltbg;
Widget optionRecPerftbg;
Widget optionRecArchtbg;
Widget optionMffrAvailtbg;
Widget optionArchiveAvailtbg;
Widget optionShowAvailtbg;
Widget optionEnvironmentAvailtbg;
Widget optionDisplayText[MAX_DISPLAY];
Widget optionUserprogText[MAX_USERPROG];

/* si rende parametrico dopo le modifiche allo skedulatore 8apr97 fm
Widget optionIcProttb[MAX_SNAP_SHOT];
char icnum[MAX_SNAP_SHOT][4];
*******************************************/
Widget *optionIcProttb;
char *icnum;


extern VAL val;
extern Widget areaMessaggi;

extern OPTIONS_FLAGS options;
extern Boolean *IcProt;    /* punta alla struttura protezione IC */

extern char selectedCommand[MAX_USERPROG_LUN];
void optchg();
void selectCommand();
void selectIcProt();
int clear_prec ();
int add_opt_init (Widget);
int    add_opt_snap(Widget);
int add_opt_recording(Widget);
int add_opt_mffr(Widget);
int add_opt_archive(Widget);
int add_opt_show(Widget);
int add_opt_display(Widget);
int add_opt_userprog(Widget);
int add_opt_icprotect(Widget);
int SD_optload (int, char*, Boolean*);
int carica_options_default(OPTIONS_FLAGS *, Boolean *);
int SD_optsave (int, char*);
int aggiorna_opzioni_display(Widget, OPTIONS_FLAGS *);
int aggiorna_opzioni_userprog(Widget, OPTIONS_FLAGS *);
int aggiorna_opzioni_icprotect(Widget, OPTIONS_FLAGS *);
int SD_maxtime (int, float*);
int SD_stepscaling (int, float*);



/********************************************************************/
int carica_options (parent, item)
Widget parent;
int item;
{
static int first_time = 1;

if (opt_interface_active)
	clear_prec ();

first_time = 0;
opt_interface_active = 1;
present_context = item;
	
switch (item)
	{
	case OPTION_INIT:
		add_opt_init (parent);
		break;
	case OPTION_SNAPSHOT:
		add_opt_snap (parent);
		break;
	case OPTION_RECORDING:
      add_opt_recording (parent);
      break;
   case OPTION_MFFR:
      add_opt_mffr (parent);
      break;
   case OPTION_ARCHIVE:
      add_opt_archive (parent);
      break;
   case OPTION_SHOW:
      add_opt_show (parent);
      break;
   case OPTION_DISPLAY:
      add_opt_display (parent);
      break;
	case OPTION_USERPROG:
		add_opt_userprog (parent);
		break;
	case OPTION_ICPROTECT:
		add_opt_icprotect (parent);
		break;
	}
	return (0);
}
/********************************************************************/
int clear_prec ()
{
	if (XtIsManaged(optionSelRc))  
		XtDestroyWidget(optionSelRc);
	return(0);
}
/********************************************************************/
int add_opt_init (p)
Widget p;             /* parent    */
{
	optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio 
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );


      optionInitCleartbg = XtVaCreateManagedWidget( "optionInitCleartbg",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Clear" ),
			XmNset,	options.options_init.clear,
         NULL );

      optionInitBttbg = XtVaCreateManagedWidget( "optionInitBttbg", 
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Init from BT" ),
			XmNset, options.options_init.from_bt,
         NULL );
		XtUnmanageChild (optionInitBttbg);

      optionInitIctbg = XtVaCreateManagedWidget( "optionInitIctbg",  
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Init from IC" ),
			XmNset,		options.options_init.from_ci,
         NULL );
		XtUnmanageChild (optionInitIctbg);

		XtAddCallback (optionInitCleartbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionInitBttbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionInitIctbg,XmNvalueChangedCallback,optchg,NULL);

	return(0);
}
/********************************************************************/
int add_opt_snap (p)
Widget p;
{
Boolean available;

	available = options.options_snapshot.active;

   optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );

      optionSnapAvailtbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Active" ),
			XmNset,  	options.options_snapshot.active,
         NULL );

		XtVaCreateManagedWidget( "",
			xmSeparatorGadgetClass, optionSelRc,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
			NULL);

		optionSnapSavetbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Save" ),
			XmNset, 		options.options_snapshot.save,
			XmNsensitive,	available,
         NULL );
		XtUnmanageChild (optionSnapSavetbg); /* tolta la possibilita' */

		optionSnapDeletetbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Cut & Paste" ),
			XmNset,		options.options_snapshot.delete,
			XmNsensitive,	available,
         NULL );

		optionSnapCopyPastetbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Copy & Paste" ),
			XmNset,		options.options_snapshot.copy_and_paste,
			XmNsensitive,	available,
         NULL );

		XtAddCallback (optionSnapAvailtbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionSnapSavetbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionSnapDeletetbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionSnapCopyPastetbg,XmNvalueChangedCallback,optchg,NULL);
		return (0);
}
/********************************************************************/
int add_opt_recording (p)
Widget p;
{
Boolean available;

	available = options.options_recording.active;

   optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );

      optionRecAvailtbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Active" ),
			XmNset,		options.options_recording.active,
         NULL );

      XtVaCreateManagedWidget( "",
         xmSeparatorGadgetClass, optionSelRc,
			XmNleftAttachment, XmATTACH_FORM,
			XmNrightAttachment, XmATTACH_FORM,
         NULL);

		optionRecBttbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Backtrack" ),
			XmNset,		options.options_recording.backtrack,
			XmNsensitive,	available,
         NULL );

		optionRecCrtbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Continuos recording" ),
			XmNset,		options.options_recording.cr,
			XmNsensitive,	available,
         NULL );

		optionRecItaltbg  = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "I&T action log" ),
			XmNset,		options.options_recording.ital,
			XmNsensitive,	available,
         NULL );
		XtUnmanageChild (optionRecItaltbg);

		optionRecPerftbg   = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Performance analisys"),
			XmNset,		options.options_recording.performance,
			XmNsensitive,	available,
         NULL );
		XtUnmanageChild (optionRecPerftbg);

		optionRecArchtbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Archive"),
			XmNset,     options.options_recording.archive,
			XmNsensitive,	available,
         NULL );
		XtUnmanageChild (optionRecArchtbg);

		XtAddCallback (optionRecAvailtbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionRecBttbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionRecCrtbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionRecItaltbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionRecPerftbg,XmNvalueChangedCallback,optchg,NULL);
		XtAddCallback (optionRecArchtbg,XmNvalueChangedCallback,optchg,NULL);

		return (0);
}
/*******************************************************************/
int add_opt_mffr (p)
Widget p;
{
Boolean available;

	available = options.options_mffr.active;

   optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );

      optionMffrAvailtbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Active" ),
			XmNset,		options.options_mffr.active,
         NULL );

		XtAddCallback (optionMffrAvailtbg,XmNvalueChangedCallback,optchg,NULL);

	return (0);
}
/*******************************************************************/
int add_opt_archive (p)
Widget p;
{
Boolean available;

	available =  options.options_archive.active;

   optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );

      optionArchiveAvailtbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Active" ),
			XmNset,		options.options_archive.active,
         NULL );

		XtAddCallback (optionArchiveAvailtbg,XmNvalueChangedCallback,optchg,NULL);

   return (0);
}
/*******************************************************************/
int add_opt_show (p)
Widget p;
{
Boolean available;

   available =  options.options_show.active;

   optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );

      optionShowAvailtbg = XtVaCreateManagedWidget( "",
         xmToggleButtonGadgetClass, optionSelRc,
         RES_CONVERT( XmNlabelString, "Active" ),
			XmNset,		options.options_show.active,
         NULL );

		XtAddCallback (optionShowAvailtbg,XmNvalueChangedCallback,optchg,NULL);

   return (0);
}
/*******************************************************************/
int add_opt_display (p)
Widget p;
{
Boolean available;
int i;

   optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );

		for (i=0; i<MAX_DISPLAY; i++)
			{
			optionDisplayText[i] = XtVaCreateManagedWidget( "",
         	xmTextWidgetClass, optionSelRc,
         	XmNwidth, 300,
         	XmNvalue, options.options_display.display_name[i],
         	NULL );
			}

	XtVaSetValues (optionDisplayText[0],XmNeditable,False,NULL );
   return (0);
}
/*******************************************************************/
int add_opt_userprog (p)
Widget p;
{
Boolean available;
int i;

   optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_TIGHT,
         XmNnumColumns, 1,
         NULL );

/*
 * label blank per riservare spazio
 */
      optionSelLabel = XtVaCreateManagedWidget( "",
         xmLabelWidgetClass, optionSelRc,
         NULL );

      for (i=0; i<MAX_USERPROG ; i++)
         {
         optionUserprogText[i] = XtVaCreateManagedWidget( "",
            xmTextWidgetClass, optionSelRc,
            XmNwidth, 300,
            XmNvalue, options.options_user_progs.userprog[i],
            NULL );
         }

   return (0);
}
/*******************************************************************/
int add_opt_icprotect (p)
Widget p;
{
int i,j;
char stringa[5];
static int primo_giro=1;

	if (primo_giro)
		{
		optionIcProttb = (Widget *) XtMalloc (sizeof(Widget)*_MAX_SNAP_SHOT);
		icnum = (char *)malloc(sizeof(char)*_MAX_SNAP_SHOT*4);
		primo_giro = 0;
		}

	optionSelRc = XtVaCreateManagedWidget( "optionSelRc",
         xmRowColumnWidgetClass, p,
         XmNentryAlignment, XmALIGNMENT_BEGINNING,
         XmNpacking, XmPACK_COLUMN,
         XmNnumColumns, 10,
         NULL );
/*
 * label blank per riservare spazio
 */
      for (i=0; i<_MAX_SNAP_SHOT; i++)
         {
			sprintf (icnum+i*4,"%d",i);
			sprintf (stringa,"%d",i+1);
			*(optionIcProttb+i) = XtVaCreateManagedWidget( "",
         		xmToggleButtonWidgetClass, optionSelRc,
         		XmNset,     *(IcProt+i),
					RES_CONVERT( XmNlabelString,stringa), 
         		NULL );
			for (j=0; IcDefProt[j]>=0; j++)
         	{
         	if (i == IcDefProt[j])
            	{
            	XtVaSetValues (*(optionIcProttb+i),XmNsensitive,False,NULL);
            	break;
            	}
         	}
			XtAddCallback (*(optionIcProttb+i),
								XmNvalueChangedCallback, selectIcProt,
								icnum+i*4);
			}
		return (0);
}
/*******************************************************************/
int read_options()
{
static int primo_giro = 1;

	if (primo_giro)
		{
		IcProt = (Boolean *)malloc(sizeof(Boolean)*_MAX_SNAP_SHOT);
		primo_giro = 0;
		}
	if (SD_optload(BANCO, (char*)&options, IcProt) > 0)
		fprintf (stderr,"OPZIONI caricate\n");
	else
		fprintf (stderr,"*** errore load opzioni\n");
	return(0);
}
/*******************************************************************/
int lettura_opzioni(opt, IcDati)
char *opt;
Boolean *IcDati;
{
FILE *fp;
char path[400];
int n_snap;

	strcpy (path, FILES_PATH);
	strcat (path,"/");
	strcat (path,OPTION_FILE);

//	printf ("apre file %s\n",path);

	if ((fp = fopen (path,"r")) == NULL)
		{
		printf ("il file %s non esiste: setta defaults\n",path);
		carica_options_default((OPTIONS_FLAGS *)opt,IcDati);
// GUAG2025
//		if (SD_optsave(BANCO, opt, IcDati) > 0)
		if (SD_optsave(BANCO, opt) > 0)
			fprintf (stderr,"OPZIONI salvate\n");
		else
			fprintf (stderr,"*** errore save opzioni\n");
		}
	else
		{
		fread (opt, sizeof(OPTIONS_FLAGS), 1, fp);
		fread (&n_snap,sizeof(int), 1, fp); /* _MAX_SNAP_SHOT di quando e' stato salvato */
		if (_MAX_SNAP_SHOT < n_snap)
			n_snap = _MAX_SNAP_SHOT;
		fread (IcDati, sizeof(Boolean)*n_snap, 1, fp);
		fclose(fp);
		}
	return(0);
}
/*******************************************************************/
int carica_options_default(opt, IcDati)
OPTIONS_FLAGS *opt;
Boolean *IcDati;
{
int i,j;
	opt->options_init.clear = True;
	opt->options_init.from_bt = False;
	opt->options_init.from_ci = False;

	opt->options_snapshot.active = True;
	opt->options_snapshot.save = True;
	opt->options_snapshot.delete = True;
	opt->options_snapshot.copy_and_paste = True;
	
	opt->options_recording.active = True;
	opt->options_recording.backtrack = True;
	opt->options_recording.cr = True;
	opt->options_recording.ital = False;
	opt->options_recording.performance = False;
	opt->options_recording.archive = False;
	
	opt->options_mffr.active = False;

	opt->options_archive.active = True;

	opt->options_show.active = True;

	opt->options_environment.active = False;

	strcpy (opt->options_display.display_name[0],DEFAULT_DISPLAY);
	for (i=1; i<MAX_DISPLAY; i++)
		strcpy (opt->options_display.display_name[i],"");

	for (i=0; i<MAX_USERPROG; i++)
      strcpy (opt->options_user_progs.userprog[i],"");

	for (i=0; i< _MAX_SNAP_SHOT; i++)
		{
		*(IcDati+i) = False;
		for (j=0; IcDefProt[j]>=0; j++)
			{
			if (i == IcDefProt[j])
				{
				*(IcDati+i) = True;
				break;
				}
			}
		}

	opt->optionsFilterCr.ByAllVar = False;
	opt->optionsFilterCr.ByModBlock = True;
	opt->optionsFilterCr.ByMod = False;
	opt->optionsFilterCr.ByKks = False;
	opt->optionsFilterCr.ByType = False;

   opt->optionsFilterAing.ByAllVar = False;
   opt->optionsFilterAing.ByModBlock = True;
   opt->optionsFilterAing.ByMod = False;
   opt->optionsFilterAing.ByKks = False;
   opt->optionsFilterAing.ByType = True;

	opt->optionsFilterMalf.ByAllVar = False;
   opt->optionsFilterMalf.ByModBlock = True;
   opt->optionsFilterMalf.ByMod = False;
   opt->optionsFilterMalf.ByKks = True;
   opt->optionsFilterMalf.ByType = False;

   opt->optionsFilterFrem.ByAllVar = False;
   opt->optionsFilterFrem.ByModBlock = True;
   opt->optionsFilterFrem.ByMod = False;
   opt->optionsFilterFrem.ByKks = True;
   opt->optionsFilterFrem.ByType = True;
	return(0);
}
/*******************************************************************/
int write_options(opt,IcDati)
char *opt;
Boolean *IcDati;
{
FILE *fp;
char path[400];

   strcpy (path, FILES_PATH);
   strcat (path,"/");
   strcat (path,OPTION_FILE);

   printf ("apre file %s\n",path);

   if ((fp = fopen (path,"w")) == NULL)
      {
		printf ("OPT: non apre il file in scrittura\n");
		return(-1);
		}
	fwrite (opt, sizeof (OPTIONS_FLAGS), 1, fp);
	fwrite (&_MAX_SNAP_SHOT, sizeof (int), 1, fp);
	fwrite (IcDati, sizeof (Boolean)*_MAX_SNAP_SHOT, 1, fp);

	fclose (fp);

	return(0);
}
/*******************************************************************/
void optchg (w, data, reason)
Widget w;
char *data;
XmToggleButtonCallbackStruct *reason;
{
Boolean stato;
Boolean app;

	stato = reason->set;
/*
 * init
 */
	if (w == optionInitCleartbg) options.options_init.clear = stato;
	if (w == optionInitBttbg) options.options_init.from_bt = stato;
	if (w == optionInitIctbg) options.options_init.from_ci = stato;

/*
 * snap
 */
	if (w == optionSnapAvailtbg)
		{
		if (!stato)
			{
			options.options_snapshot.active = stato;
/*			options.options_snapshot.save = stato;
			options.options_snapshot.delete = stato;
			options.options_snapshot.copy_and_paste = stato;
			XtVaSetValues (optionSnapSavetbg, 
									XmNset, stato, XmNsensitive, False, NULL);
			XtVaSetValues (optionSnapDeletetbg, 
									XmNset, stato, XmNsensitive, False, NULL);
			XtVaSetValues (optionSnapCopyPastetbg, 
									XmNset, stato, XmNsensitive, False, NULL); */
			}
		else
			{
			options.options_snapshot.active = stato;
/*			XtVaSetValues (optionSnapSavetbg, 
                           XmNsensitive, True, NULL);
         XtVaSetValues (optionSnapDeletetbg, 
                           XmNsensitive, True, NULL);
         XtVaSetValues (optionSnapCopyPastetbg, 
                           XmNsensitive, True, NULL); */
			}
		}
	if (w == optionSnapSavetbg) options.options_snapshot.save = stato;	
	if (w == optionSnapDeletetbg) options.options_snapshot.delete = stato;
	if (w == optionSnapCopyPastetbg)	options.options_snapshot.copy_and_paste = stato;						

/*
 * recording
 */
	if (w == optionRecAvailtbg)
		{
		if (!stato)
			{
			options.options_recording.active = stato;
			options.options_recording.backtrack = stato;
			options.options_recording.cr = stato;
			options.options_recording.ital = stato;
			options.options_recording.performance = stato;
			options.options_recording.archive = stato;
			XtVaSetValues (optionRecBttbg, 
									XmNset, stato, XmNsensitive, False, NULL);
			XtVaSetValues (optionRecCrtbg, 
									XmNset, stato, XmNsensitive, False, NULL);
			XtVaSetValues (optionRecItaltbg, 
									XmNset, stato, XmNsensitive, False, NULL);
			XtVaSetValues (optionRecPerftbg, 
									XmNset, stato, XmNsensitive, False, NULL);
			XtVaSetValues (optionRecArchtbg, 
									XmNset, stato, XmNsensitive, False, NULL);
			}
		else
			{
			options.options_recording.active = stato;
			XtVaSetValues (optionRecBttbg, 
                           XmNsensitive, True, NULL);
         XtVaSetValues (optionRecCrtbg, 
                           XmNsensitive, True, NULL);
         XtVaSetValues (optionRecItaltbg, 
                           XmNsensitive, True, NULL);
         XtVaSetValues (optionRecPerftbg, 
                           XmNsensitive, True, NULL);
         XtVaSetValues (optionRecArchtbg, 
                           XmNsensitive, True, NULL);
			}
		}
	if (w == optionRecBttbg) options.options_recording.backtrack = stato;
	if (w == optionRecCrtbg) options.options_recording.cr = stato;
	if (w == optionRecItaltbg) options.options_recording.ital = stato;
	if (w == optionRecPerftbg) options.options_recording.performance = stato;
	if (w == optionRecArchtbg) options.options_recording.archive = stato;
/*
 * mffr
 */
	if (w == optionMffrAvailtbg)
		{
		if (!stato)
			{
			options.options_mffr.active = stato;
			}
		else
			{
			options.options_mffr.active = stato;
			}
		}
/*
 * archive
 */
	if (w == optionArchiveAvailtbg)
      {
      if (!stato)
         {
         options.options_archive.active = stato;
         }
      else
         {
         options.options_archive.active = stato;
         }
      }

/*
 * show
 */
   if (w == optionShowAvailtbg)
      {
      if (!stato)
         {
         options.options_show.active = stato;
         }
      else
         {
         options.options_show.active = stato;
         }
      }

/*
 * environment
 */
	if (w == optionEnvironmentAvailtbg)
      {
      if (!stato)
         {
         options.options_environment.active = stato;
         }
      else
         {
         options.options_environment.active = stato;
         }
      }
}
/**************************************************************/
int aggiorna_opzioni (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
	aggiorna_opzioni_init (w, p);
	aggiorna_opzioni_snap (w, p);  
	aggiorna_opzioni_recording (w, p);
	aggiorna_opzioni_mffr (w, p);
	aggiorna_opzioni_show (w, p);
	if (present_context == OPTION_DISPLAY)
		aggiorna_opzioni_display (w, p);
   if (present_context == OPTION_USERPROG)
      aggiorna_opzioni_userprog (w, p);
   if (present_context == OPTION_ICPROTECT)
      aggiorna_opzioni_icprotect (w, p);
	/*aggiorna_opzioni_programs (w, p);  */
}
/**************************************************************/
int aggiorna_opzioni_display (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
int i;
char *dummy;

	for (i=1;i<MAX_DISPLAY;i++)
		{
		dummy = XmTextGetString (optionDisplayText[i]);
		strcpy (p->options_display.display_name[i],dummy);
		XtFree (dummy);
		}
	return (0);
}
/**************************************************************/
int aggiorna_opzioni_userprog (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
int i;
char *dummy;

   for (i=0;i<MAX_USERPROG;i++)
      {
      dummy = XmTextGetString (optionUserprogText[i]);
      strcpy (p->options_user_progs.userprog[i],dummy);
      XtFree (dummy);
      }
   return (0);
}
/**************************************************************/
int aggiorna_opzioni_icprotect (w, p)
Widget w;
OPTIONS_FLAGS *p;
{
int i;

	for (i=0; i< _MAX_SNAP_SHOT; i++)
		*(IcProt+i) = XmToggleButtonGetState (*(optionIcProttb+i));

	return (0);
}
/**************************************************************/
int creaBottoniLauncher (w)     /* crea la lista bottoni per comandi */
Widget w;                       /* chiamante                         */
{
int i;
_UxCprogramLauncher *Ctx;
   Ctx = (_UxCprogramLauncher *) UxGetContext(w);

	for (i=0; i<MAX_USERPROG; i++)
		{
		Ctx->UxprogLaunchText[i] = XtVaCreateManagedWidget( "ToggleButton",
         xmToggleButtonWidgetClass, Ctx->UxrowColumn4,
         XmNwidth, 40,
         XmNheight, 10,
         RES_CONVERT( XmNlabelString, "" ),
         XmNsensitive, FALSE,
         NULL );
   	UxPutContext( Ctx->UxprogLaunchText[i], (char *) Ctx );
		}
	return (0);
}
/**************************************************************/
int loadPrograms (w)    /* carica la lista dei prog configurati  */
Widget w;               /* chiamante                             */
{
int i;
_UxCprogramLauncher *Ctx;
	Ctx = (_UxCprogramLauncher *) UxGetContext(w);

	for (i=0; i<MAX_USERPROG; i++)
		{
		if (strcmp(options.options_user_progs.userprog[i],""))
			{
			strcpy (Ctx->Uxcomandi[i],options.options_user_progs.userprog[i]);
			XtVaSetValues (Ctx->UxprogLaunchText[i],
				XmNsensitive, True,
				RES_CONVERT(XmNlabelString, Ctx->Uxcomandi[i]),NULL);
			XtAddCallback (Ctx->UxprogLaunchText[i],
								XmNvalueChangedCallback,selectCommand,
								Ctx->Uxcomandi[i]);
			}
		else
			XtUnmanageChild (Ctx->UxprogLaunchText[i]);
		}
	return (0);
}
/**************************************************************/
void selectCommand (w, data, reason)
Widget w;
char *data;
XmToggleButtonCallbackStruct *reason;
{
Boolean stato;

	stato = reason->set;

	if (!stato)   /* deselezione      */
		return;

	strcpy (selectedCommand,data);
	printf ("comando [%s]\n",selectedCommand);
	return;
}
/**************************************************************/
void selectIcProt (w, data, reason)
Widget w;
char *data;
XmToggleButtonCallbackStruct *reason;
{
Boolean stato;
	stato = reason->set;

	printf ("[%s] stato = %d\n",data,stato);
	return;
}
/**************************************************************/
int richiesta_velocita (Widget w)
//Widget w;
{
char risp [10];
char old_val[10];
float new_val;
char *messaggio;
int esito;

#ifndef OLD_STAT
   sprintf (old_val,"%4.1f",statistiche.stato_parametri.timescaling_sim);
#else
   sprintf (old_val,"%4.1f",val.actual.timescaling_sim);
#endif
   create_richiestaDati (w, NEW_SPEED, old_val, risp);
   new_val = (float)atof(risp);
   printf ("risp = %s: %f\n",risp,new_val);

   esito = 0;
   messaggio = malloc (strlen(NEW_SPEED)+strlen(OPER_FALLITA)+20);

   if (SD_timescaling (BANCO, &new_val) >0)
      sprintf (messaggio,"%s :%4.1f",NEW_SPEED,new_val);
   else
      {
      sprintf (messaggio,"%s :%4.1f %s",NEW_SPEED,new_val,OPER_FALLITA);
      esito = -1;
      attenzione(w,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);
}
/**************************************************************/
int richiesta_maxtime (w)
Widget w;
{
char risp [10];
char old_val[10];
float new_val;
char *messaggio;
int esito;
int ret;

   esito = 0;

#ifndef OLD_STAT
   sprintf (old_val,"%4.1f",statistiche.stato_parametri.max_time);
#else
   sprintf (old_val,"%4.1f",val.actual.max_time);
#endif
   if ((ret = create_richiestaDati (w, MAX_SIM_TIME, old_val, risp)) < 0)
		{
/* richiesto quit	*/
		return (esito);
		}
   new_val = (float)atof(risp);
   printf ("risp = %s: %f\n",risp,new_val);

   messaggio = malloc (strlen(MAX_SIM_TIME)+strlen(OPER_FALLITA)+20);

	if (SD_maxtime (BANCO, &new_val) >0)
		{
      sprintf (messaggio,"%s :%4.1f",MAX_SIM_TIME,new_val);
		tempo_finale_impostato = 1; /* flag di max time			*/
		}
   else
      {
      sprintf (messaggio,"%s :%4.1f %s",MAX_SIM_TIME,new_val,OPER_FALLITA);
      esito = -1;
      attenzione(w,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);
}
/**************************************************************/
int richiesta_stepscale (w)
Widget w;
{
char risp [10];
char old_val[10];
float new_val;
char *messaggio;
int esito;

#ifndef OLD_STAT
   sprintf (old_val,"%4.1f",statistiche.stato_parametri.stepscaling_sim);
#else
   sprintf (old_val,"%4.1f",val.actual.stepscaling_sim);
#endif
   create_richiestaDati (w, STEPSCALE, old_val, risp);
   new_val = (float)atof(risp);
   printf ("risp = %s: %f\n",risp,new_val);

   esito = 0;
   messaggio = malloc (strlen(STEPSCALE)+strlen(OPER_FALLITA)+20);

   if (SD_stepscaling (BANCO, &new_val) >0)
      sprintf (messaggio,"%s :%4.1f",STEPSCALE,new_val);
   else
      {
      sprintf (messaggio,"%s :%4.1f %s",STEPSCALE,new_val,OPER_FALLITA);
      esito = -1;
      attenzione(w,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);
}
/**************************************************************/
int set_max_speed (w)
Widget w;
{
char risp [10];
char old_val[10];
float new_val;
char *messaggio;
int esito;

   new_val = -1.0;
   esito = 0;
   messaggio = malloc (strlen(NEW_SPEED)+strlen(OPER_FALLITA)+20);

   if (SD_timescaling (BANCO, &new_val) >0)
      sprintf (messaggio,"%s :%4.1f",NEW_SPEED,new_val);
   else
      {
      sprintf (messaggio,"%s :%4.1f %s",NEW_SPEED,new_val,OPER_FALLITA);
      esito = -1;
      attenzione(w,messaggio);
      }

   add_message (areaMessaggi,messaggio,LIVELLO_1);
   free (messaggio);
   return(esito);
}
