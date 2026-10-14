/**********************************************************************
*
*       C Source:               messaggi.c
*       Subsystem:              1
*       Description:
*       %created_by:    zanna %
*       %date_created:  Wed Jun 11 13:50:16 1997 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: messaggi.c-3 %  (%full_filespec: messaggi.c-3:csrc:1 %)";
#endif
/*
 * messaggi.c
 *				funzioni relative all' inizializzazione delle stringhe
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "parametri.h"
#include "messages.h"
#include "messaggi.h"

MESSAGES mStr[maxIndxMessages];

extern char *FILES_PATH;

char *getMessageString();
char *getVar();
char *getVarMessage();
int strcpyMalloc (char **, char *);
int existFile (char *);
int loadMessagesString (char *, int );
int freeMessageStrings ();

/******************************************************************/
int initMessStruct ()
{
	strcpy (mStr[_DEFAULT_TITLE].label,"DEFAULT_TITLE");
	strcpy (mStr[_DEFAULT_INIT].label,"DEFAULT_INIT");
	strcpy (mStr[_INIT_SIM].label,"INIT_SIM");
	strcpy (mStr[_SEL_VAR_TITLE].label,"SEL_VAR_TITLE");
	strcpy (mStr[_LOAD_FILESEL].label,"LOAD_FILESEL");
	strcpy (mStr[_SAVE_FILESEL].label,"SAVE_FILESEL");
	strcpy (mStr[_IC_COMMENT_REQ].label,"IC_COMMENT_REQ");
	strcpy (mStr[_OPER_FALLITA].label,"OPER_FALLITA");
	strcpy (mStr[_FALLITA].label,"FALLITA");
	strcpy (mStr[_ABORT_TASK].label,"ABORT_TASK");
	strcpy (mStr[_START_TASK].label,"START_TASK");
	strcpy (mStr[_ABORTING].label,"ABORTING");
	strcpy (mStr[_ERRORE_INIT].label,"ERRORE_INIT");
	strcpy (mStr[_ERRORE_AGGANCIO].label,"ERRORE_AGGANCIO");
	strcpy (mStr[_ERROR_SIM].label,"ERROR_SIM");
	strcpy (mStr[_REINIT].label,"REINIT");
	strcpy (mStr[_NOT_EXECUTABLE].label,"NOT_EXECUTABLE");
	strcpy (mStr[_BAD_STATE].label,"BAD_STATE");
	strcpy (mStr[_NO_BT_LIST].label,"NO_BT_LIST");
	strcpy (mStr[_NAME_RESERVED].label,"NAME_RESERVED");
	strcpy (mStr[_NOT_EXISTENT].label,"NOT_EXISTENT");
	strcpy (mStr[_NOT_ACCESSIBLE].label,"NOT_ACCESSIBLE");
	strcpy (mStr[_VAR_NOT_EXIST].label,"VAR_NOT_EXIST");
	strcpy (mStr[_ENTRY_NOT_SEL].label,"ENTRY_NOT_SEL");
	strcpy (mStr[_NO_MORE_ENTRIES].label,"NO_MORE_ENTRIES");
	strcpy (mStr[_NOT_PERMITTED].label,"NOT_PERMITTED");
	strcpy (mStr[_OUT_OF_PATH].label,"OUT_OF_PATH");
	strcpy (mStr[_FIELD_NOT_VALID].label,"FIELD_NOT_VALID");
	strcpy (mStr[_ALREADYIN].label,"ALREADYIN");
	strcpy (mStr[_WARN_CUT_CI].label,"WARN_CUT_CI");
	strcpy (mStr[_WARN_REPLACE_CI].label,"WARN_REPLACE_CI");
	strcpy (mStr[_RESTORE_DEF_CI].label,"RESTORE_DEF_CI");
	strcpy (mStr[_CUT_CI].label,"CUT_CI");
	strcpy (mStr[_COPY_CI].label,"COPY_CI");
	strcpy (mStr[_CHECK_CI].label,"CHECK_CI");
	strcpy (mStr[_PASTE_CI].label,"PASTE_CI");
	strcpy (mStr[_SAVE_CI].label,"SAVE_CI");
	strcpy (mStr[_EMPTY_IC].label,"EMPTY_IC");
	strcpy (mStr[_SAVE_BT].label,"SAVE_BT");
	strcpy (mStr[_LOAD_CI].label,"LOAD_CI");
	strcpy (mStr[_LOAD_BT].label,"LOAD_BT");
	strcpy (mStr[_DEFAULT_SNAP].label,"DEFAULT_SNAP");
	strcpy (mStr[_RUN_SIM].label,"RUN_SIM");
	strcpy (mStr[_FREEZE_SIM].label,"FREEZE_SIM");
	strcpy (mStr[_BACKTRACK_SIM].label,"BACKTRACK_SIM");
	strcpy (mStr[_REPLAY_SIM].label,"REPLAY_SIM");
	strcpy (mStr[_STEP_SIM].label,"STEP_SIM");
	strcpy (mStr[_SHUTDOWN_SIM].label,"SHUTDOWN_SIM");
	strcpy (mStr[_AING_SELECTION].label,"AING_SELECTION");
	strcpy (mStr[_NEW_SESSION_NAME].label,"NEW_SESSION_NAME");
	strcpy (mStr[_NEW_STAT_REQ].label,"NEW_STAT_REQ");
	strcpy (mStr[_NEW_DELTAT_BT].label,"NEW_DELTAT_BT");
	strcpy (mStr[_NEW_SPEED].label,"NEW_SPEED");
	strcpy (mStr[_MAX_SIM_TIME].label,"MAX_SIM_TIME");
	strcpy (mStr[_ENTRY_AVAILABLE].label,"ENTRY_AVAILABLE");
	strcpy (mStr[_CLEAR_SIM].label,"CLEAR_SIM");
	strcpy (mStr[_READ_SNAP_VIEW].label,"READ_SNAP_VIEW");
	strcpy (mStr[_READ_BT_VIEW].label,"READ_BT_VIEW");
	strcpy (mStr[_STEPSCALE].label,"STEPSCALE");
	strcpy (mStr[_LAST_BT_READ].label,"LAST_BT_READ");
	strcpy (mStr[_VALIDATE_BT_OK].label,"VALIDATE_BT_OK");
	strcpy (mStr[_NEW_VAR_CR].label,"NEW_VAR_CR");
	strcpy (mStr[_NEW_STEP_CR].label,"NEW_STEP_CR");
	strcpy (mStr[_PROGRAM_ACT].label,"PROGRAM_ACT");
	strcpy (mStr[_SAVE_FILE_CR].label,"SAVE_FILE_CR");
	strcpy (mStr[_SAVE_FILE_PREV].label,"SAVE_FILE_PREV");
	strcpy (mStr[_SAVE_FILE_SCEN].label,"SAVE_FILE_SCEN");
	strcpy (mStr[_LOAD_FILE_CR].label,"LOAD_FILE_CR");
	strcpy (mStr[_LOAD_FILE_PREV].label,"LOAD_FILE_PREV");
	strcpy (mStr[_LOAD_FILE_SCEN].label,"LOAD_FILE_SCEN");
	strcpy (mStr[_ENTRY_NUMBER].label,"ENTRY_NUMBER");
	strcpy (mStr[_ACT_PERT_LABEL].label,"ACT_PERT_LABEL");
	strcpy (mStr[_UNACT_PERT_LABEL].label,"UNACT_PERT_LABEL");
	strcpy (mStr[_PERT_FREQ_UPD].label,"PERT_FREQ_UPD");
	strcpy (mStr[_PERTRATE].label,"PERTRATE");
	strcpy (mStr[_RUN_LABEL].label,"RUN_LABEL");
	strcpy (mStr[_FREEZE_LABEL].label,"FREEZE_LABEL");
	strcpy (mStr[_STEP_LABEL].label,"STEP_LABEL");
	strcpy (mStr[_SIMTIME_LABEL].label,"SIMTIME_LABEL");
	strcpy (mStr[_RITARDO_LABEL].label,"RITARDO_LABEL");
	strcpy (mStr[_SPEED_LABEL].label,"SPEED_LABEL");
	strcpy (mStr[_REPLAY_MODE_LABEL].label,"REPLAY_MODE_LABEL");
	strcpy (mStr[_REPLAY_ACTIVE].label,"REPLAY_ACTIVE");
	strcpy (mStr[_REPLAY_STOP].label,"REPLAY_STOP");
	strcpy (mStr[_BT_ACT_LOAD].label,"BT_ACT_LOAD");
	strcpy (mStr[_DELTAT_REC_BT].label,"DELTAT_REC_BT");
	strcpy (mStr[_MALF].label,"MALF");
	strcpy (mStr[_NOT_CONFIGURED].label,"NOT_CONFIGURED");
	strcpy (mStr[_FREM].label,"FREM");
	strcpy (mStr[_MALF_CONF_READ].label,"MALF_CONF_READ");
	strcpy (mStr[_FREM_CONF_READ].label,"FREM_CONF_READ");
	strcpy (mStr[_MALF_SUMMARY].label,"MALF_SUMMARY");
	strcpy (mStr[_FREM_SUMMARY].label,"FREM_SUMMARY");
	strcpy (mStr[_TYPE_NOT_EXIST].label,"TYPE_NOT_EXIST");
	strcpy (mStr[_MALF_SCEN_LIST].label,"MALF_SCEN_LIST");
	strcpy (mStr[_FREM_SCEN_LIST].label,"FREM_SCEN_LIST");
	strcpy (mStr[_MALF_SCEN_TITLE].label,"MALF_SCEN_TITLE");
	strcpy (mStr[_DELAY_LABEL].label,"DELAY_LABEL");
	strcpy (mStr[_DURATION_LABEL].label,"DURATION_LABEL");
	strcpy (mStr[_RAMP_LABEL].label,"RAMP_LABEL");
	strcpy (mStr[_MALF_LIST].label,"MALF_LIST");
	strcpy (mStr[_FREM_LIST].label,"FREM_LIST");
	strcpy (mStr[_NEW_MALF_SCEN].label,"NEW_MALF_SCEN");
	strcpy (mStr[_NEW_FREM_SCEN].label,"NEW_FREM_SCEN");
	strcpy (mStr[_SYSTEM_LIST].label,"SYSTEM_LIST");
	strcpy (mStr[_BLOCK_LIST].label,"BLOCK_LIST");
	strcpy (mStr[_VAR_TO_SELECT].label,"VAR_TO_SELECT");
	strcpy (mStr[_VAR_SELECTED].label,"VAR_SELECTED");
	strcpy (mStr[_CONF_ACCEPT].label,"CONF_ACCEPT");
	strcpy (mStr[_SELVAR_CONFIRM].label,"SELVAR_CONFIRM");
	strcpy (mStr[_CONF_DISCARD].label,"CONF_DISCARD");
	strcpy (mStr[_SELECT_ALL].label,"SELECT_ALL");
	strcpy (mStr[_SELECT_VAR].label,"SELECT_VAR");
	strcpy (mStr[_UNSELECT_VAR].label,"UNSELECT_VAR");
	strcpy (mStr[_SAVEFILTER].label,"SAVEFILTER");
	strcpy (mStr[_APPLYFILTER].label,"APPLYFILTER");
	strcpy (mStr[_NEW_FILTER].label,"NEW_FILTER");
	strcpy (mStr[_ATTN_FILTER1].label,"ATTN_FILTER1");
	strcpy (mStr[_ATTN_FILTER2].label,"ATTN_FILTER2");
	strcpy (mStr[_KKS_NIMP_LABEL].label,"KKS_NIMP_LABEL");
	strcpy (mStr[_KKS_SYST_LABEL].label,"KKS_SYST_LABEL");
	strcpy (mStr[_KKS_SSYST_LABEL].label,"KKS_SSYST_LABEL");
	strcpy (mStr[_KKS_COMP_LABEL].label,"KKS_COMP_LABEL");
	strcpy (mStr[_KKS_PROG_LABEL].label,"KKS_PROG_LABEL");
	strcpy (mStr[_KKS_APPLY_FILTER].label,"KKS_APPLY_FILTER");
	strcpy (mStr[_TIPO_PRESSIONE].label,"TIPO_PRESSIONE");
	strcpy (mStr[_TIPO_TEMPERATURA].label,"TIPO_TEMPERATURA");
	strcpy (mStr[_TIPO_ENTALPIA].label,"TIPO_ENTALPIA");
	strcpy (mStr[_TIPO_PORTATA].label,"TIPO_PORTATA");
	strcpy (mStr[_TIPO_POTENZA].label,"TIPO_POTENZA");
	strcpy (mStr[_TIPO_GIRI].label,"TIPO_GIRI");
	strcpy (mStr[_TIPO_ALZVALV].label,"TIPO_ALZVALV");
	strcpy (mStr[_TIPO_AINGABLE].label,"TIPO_AINGABLE");
	strcpy (mStr[_TIPO_INPUT].label,"TIPO_INPUT");
	strcpy (mStr[_TIPO_OUTPUT].label,"TIPO_OUTPUT");
	strcpy (mStr[_PROC_START_OK].label,"PROC_START_OK");
	strcpy (mStr[_PROC_START_Quit].label,"PROC_START_Quit");
	strcpy (mStr[_PROC_START_LABEL].label,"PROC_START_LABEL");
	strcpy (mStr[_PROC_START_DISPLAY].label,"PROC_START_DISPLAY");
	strcpy (mStr[_OPTION_APPLY_LABEL].label,"OPTION_APPLY_LABEL");
	strcpy (mStr[_APPLYLABEL].label,"APPLYLABEL");
	strcpy (mStr[_QUITLABEL].label,"QUITLABEL");
	strcpy (mStr[_EXITLABEL].label,"EXITLABEL");
	strcpy (mStr[_ACT_LABEL].label,"ACT_LABEL");
	strcpy (mStr[_DEACT_LABEL].label,"DEACT_LABEL");
	strcpy (mStr[_FileMenuLabel].label,"FileMenuLabel");
	strcpy (mStr[_EditMenuLabel].label,"EditMenuLabel");
	strcpy (mStr[_ShowMenuLabel].label,"ShowMenuLabel");
	strcpy (mStr[_HelpMenuLabel].label,"HelpMenuLabel");
	strcpy (mStr[_SaveMenuLabel].label,"SaveMenuLabel");
	strcpy (mStr[_SaveMenuLabel2].label,"SaveMenuLabel2");
	strcpy (mStr[_OpenMenuLabel].label,"OpenMenuLabel");
	strcpy (mStr[_OpenMenuLabel2].label,"OpenMenuLabel2");
	strcpy (mStr[_QuitMenuLabel].label,"QuitMenuLabel");
	strcpy (mStr[_NewMenuLabel].label,"NewMenuLabel");
	strcpy (mStr[_CutMenuLabel].label,"CutMenuLabel");
	strcpy (mStr[_AddItemLabel].label,"AddItemLabel");
	strcpy (mStr[_AddItemLabel2].label,"AddItemLabel2");
	strcpy (mStr[_ClearLabel].label,"ClearLabel");
	strcpy (mStr[_MaxTimeLabel].label,"MaxTimeLabel");
	strcpy (mStr[_MaxSpeedLabel].label,"MaxSpeedLabel");
	strcpy (mStr[_SetSpeedLabel].label,"SetSpeedLabel");
	strcpy (mStr[_StepScalingLabel].label,"StepScalingLabel");
	strcpy (mStr[_SnapshotLabel].label,"SnapshotLabel");
	strcpy (mStr[_BacktrackLabel].label,"BacktrackLabel");
	strcpy (mStr[_ContRecLabel].label,"ContRecLabel");
	strcpy (mStr[_IandTactionLog].label,"IandTactionLog");
	strcpy (mStr[_PerformanceLabel].label,"PerformanceLabel");
	strcpy (mStr[_ShutdownLabel].label,"ShutdownLabel");
	strcpy (mStr[_InitIcLabel].label,"InitIcLabel");
	strcpy (mStr[_InitBtLabel].label,"InitBtLabel");
	strcpy (mStr[_STARTUP_ERR].label,"STARTUP_ERR");
	strcpy (mStr[_MALF_SELECTION].label,"MALF_SELECTION");
	strcpy (mStr[_FREM_SELECTION].label,"FREM_SELECTION");
	strcpy (mStr[_CompressLabel].label,"CompressLabel");
	strcpy (mStr[_ExpandLabel].label,"ExpandLabel");
	strcpy (mStr[_MasterMenuLabel].label,"MasterMenuLabel");
	strcpy (mStr[_PrintMenuLabel].label,"PrintMenuLabel");
	strcpy (mStr[_CopyMenuLabel].label,"CopyMenuLabel");
	strcpy (mStr[_PasteMenuLabel].label,"PasteMenuLabel");
	strcpy (mStr[_ViewMenuLabel].label,"ViewMenuLabel");
	strcpy (mStr[_LoadMenuLabel].label,"LoadMenuLabel");
	strcpy (mStr[_CheckMenuLabel].label,"CheckMenuLabel");
	strcpy (mStr[_VAL_ACT_LABEL].label,"VAL_ACT_LABEL");

	strcpy (mStr[_LoadDefaultLabel].label,"Load default");
	strcpy (mStr[_SelectNewLabel].label,"Select new ...");
	strcpy (mStr[_ClearAllLabel].label,"Clear all ");
	strcpy (mStr[_CheckAllLabel].label,"Check all");
	strcpy (mStr[_LOADLABEL].label,"LOAD");
	strcpy (mStr[_SAVELABEL].label,"SAVE");
	strcpy (mStr[_VIEWLABEL].label,"VIEW");
	strcpy (mStr[_INITLABEL].label,"INIT");
	strcpy (mStr[_InitMenuLabel].label,"Init");
	
}
/******************************************************************/
int messStructDefault ()
{
	strcpyMalloc (&mStr[_DEFAULT_TITLE].string,DEFAULT_TITLE);
	strcpyMalloc (&mStr[_DEFAULT_INIT].string,DEFAULT_INIT);
	strcpyMalloc (&mStr[_INIT_SIM].string,INIT_SIM);
	strcpyMalloc (&mStr[_SEL_VAR_TITLE].string,SEL_VAR_TITLE);
	strcpyMalloc (&mStr[_LOAD_FILESEL].string,LOAD_FILESEL);
	strcpyMalloc (&mStr[_SAVE_FILESEL].string,SAVE_FILESEL);
	strcpyMalloc (&mStr[_IC_COMMENT_REQ].string,IC_COMMENT_REQ);
	strcpyMalloc (&mStr[_OPER_FALLITA].string,OPER_FALLITA);
	strcpyMalloc (&mStr[_FALLITA].string,FALLITA);
	strcpyMalloc (&mStr[_ABORT_TASK].string,ABORT_TASK);
	strcpyMalloc (&mStr[_START_TASK].string,START_TASK);
	strcpyMalloc (&mStr[_ABORTING].string,ABORTING);
	strcpyMalloc (&mStr[_ERRORE_INIT].string,ERRORE_INIT);
	strcpyMalloc (&mStr[_ERRORE_AGGANCIO].string,ERRORE_AGGANCIO);
	strcpyMalloc (&mStr[_ERROR_SIM].string,ERROR_SIM);
	strcpyMalloc (&mStr[_REINIT].string,REINIT);
	strcpyMalloc (&mStr[_NOT_EXECUTABLE].string,NOT_EXECUTABLE);
	strcpyMalloc (&mStr[_BAD_STATE].string,BAD_STATE);
	strcpyMalloc (&mStr[_NO_BT_LIST].string,NO_BT_LIST);
	strcpyMalloc (&mStr[_NAME_RESERVED].string,NAME_RESERVED);
	strcpyMalloc (&mStr[_NOT_EXISTENT].string,NOT_EXISTENT);
	strcpyMalloc (&mStr[_NOT_ACCESSIBLE].string,NOT_ACCESSIBLE);
	strcpyMalloc (&mStr[_VAR_NOT_EXIST].string,VAR_NOT_EXIST);
	strcpyMalloc (&mStr[_ENTRY_NOT_SEL].string,ENTRY_NOT_SEL);
	strcpyMalloc (&mStr[_NO_MORE_ENTRIES].string,NO_MORE_ENTRIES);
	strcpyMalloc (&mStr[_NOT_PERMITTED].string,NOT_PERMITTED);
	strcpyMalloc (&mStr[_OUT_OF_PATH].string,OUT_OF_PATH);
	strcpyMalloc (&mStr[_FIELD_NOT_VALID].string,FIELD_NOT_VALID);
	strcpyMalloc (&mStr[_ALREADYIN].string,ALREADYIN);
	strcpyMalloc (&mStr[_WARN_CUT_CI].string,WARN_CUT_CI);
	strcpyMalloc (&mStr[_WARN_REPLACE_CI].string,WARN_REPLACE_CI);
	strcpyMalloc (&mStr[_RESTORE_DEF_CI].string,RESTORE_DEF_CI);
	strcpyMalloc (&mStr[_CUT_CI].string,CUT_CI);
	strcpyMalloc (&mStr[_COPY_CI].string,COPY_CI);
	strcpyMalloc (&mStr[_CHECK_CI].string,CHECK_CI);
	strcpyMalloc (&mStr[_PASTE_CI].string,PASTE_CI);
	strcpyMalloc (&mStr[_SAVE_CI].string,SAVE_CI);
	strcpyMalloc (&mStr[_EMPTY_IC].string,EMPTY_IC);
	strcpyMalloc (&mStr[_SAVE_BT].string,SAVE_BT);
	strcpyMalloc (&mStr[_LOAD_CI].string,LOAD_CI);
	strcpyMalloc (&mStr[_LOAD_BT].string,LOAD_BT);
	strcpyMalloc (&mStr[_DEFAULT_SNAP].string,DEFAULT_SNAP);
	strcpyMalloc (&mStr[_RUN_SIM].string,RUN_SIM);
	strcpyMalloc (&mStr[_FREEZE_SIM].string,FREEZE_SIM);
	strcpyMalloc (&mStr[_BACKTRACK_SIM].string,BACKTRACK_SIM);
	strcpyMalloc (&mStr[_REPLAY_SIM].string,REPLAY_SIM);
	strcpyMalloc (&mStr[_STEP_SIM].string,STEP_SIM);
	strcpyMalloc (&mStr[_SHUTDOWN_SIM].string,SHUTDOWN_SIM);
	strcpyMalloc (&mStr[_AING_SELECTION].string,AING_SELECTION);
	strcpyMalloc (&mStr[_NEW_SESSION_NAME].string,NEW_SESSION_NAME);
	strcpyMalloc (&mStr[_NEW_STAT_REQ].string,NEW_STAT_REQ);
	strcpyMalloc (&mStr[_NEW_DELTAT_BT].string,NEW_DELTAT_BT);
	strcpyMalloc (&mStr[_NEW_SPEED].string,NEW_SPEED);
	strcpyMalloc (&mStr[_MAX_SIM_TIME].string,MAX_SIM_TIME);
	strcpyMalloc (&mStr[_ENTRY_AVAILABLE].string,ENTRY_AVAILABLE);
	strcpyMalloc (&mStr[_CLEAR_SIM].string,CLEAR_SIM);
	strcpyMalloc (&mStr[_READ_SNAP_VIEW].string,READ_SNAP_VIEW);
	strcpyMalloc (&mStr[_READ_BT_VIEW].string,READ_BT_VIEW);
	strcpyMalloc (&mStr[_STEPSCALE].string,STEPSCALE);
	strcpyMalloc (&mStr[_LAST_BT_READ].string,LAST_BT_READ);
	strcpyMalloc (&mStr[_VALIDATE_BT_OK].string,VALIDATE_BT_OK);
	strcpyMalloc (&mStr[_NEW_VAR_CR].string,NEW_VAR_CR);
	strcpyMalloc (&mStr[_NEW_STEP_CR].string,NEW_STEP_CR);
	strcpyMalloc (&mStr[_PROGRAM_ACT].string,PROGRAM_ACT);
	strcpyMalloc (&mStr[_SAVE_FILE_CR].string,SAVE_FILE_CR);
	strcpyMalloc (&mStr[_SAVE_FILE_PREV].string,SAVE_FILE_PREV);
	strcpyMalloc (&mStr[_SAVE_FILE_SCEN].string,SAVE_FILE_SCEN);
	strcpyMalloc (&mStr[_LOAD_FILE_CR].string,LOAD_FILE_CR);
	strcpyMalloc (&mStr[_LOAD_FILE_PREV].string,LOAD_FILE_PREV);
	strcpyMalloc (&mStr[_LOAD_FILE_SCEN].string,LOAD_FILE_SCEN);
	strcpyMalloc (&mStr[_ENTRY_NUMBER].string,ENTRY_NUMBER);
	strcpyMalloc (&mStr[_ACT_PERT_LABEL].string,ACT_PERT_LABEL);
	strcpyMalloc (&mStr[_UNACT_PERT_LABEL].string,UNACT_PERT_LABEL);
	strcpyMalloc (&mStr[_PERT_FREQ_UPD].string,PERT_FREQ_UPD);
	strcpyMalloc (&mStr[_PERTRATE].string,PERTRATE);
	strcpyMalloc (&mStr[_RUN_LABEL].string,RUN_LABEL);
	strcpyMalloc (&mStr[_FREEZE_LABEL].string,FREEZE_LABEL);
	strcpyMalloc (&mStr[_STEP_LABEL].string,STEP_LABEL);
	strcpyMalloc (&mStr[_SIMTIME_LABEL].string,SIMTIME_LABEL);
	strcpyMalloc (&mStr[_RITARDO_LABEL].string,RITARDO_LABEL);
	strcpyMalloc (&mStr[_SPEED_LABEL].string,SPEED_LABEL);
	strcpyMalloc (&mStr[_REPLAY_MODE_LABEL].string,REPLAY_MODE_LABEL);
	strcpyMalloc (&mStr[_REPLAY_ACTIVE].string,REPLAY_ACTIVE);
	strcpyMalloc (&mStr[_REPLAY_STOP].string,REPLAY_STOP);
	strcpyMalloc (&mStr[_BT_ACT_LOAD].string,BT_ACT_LOAD);
	strcpyMalloc (&mStr[_DELTAT_REC_BT].string,DELTAT_REC_BT);
	strcpyMalloc (&mStr[_MALF].string,MALF);
	strcpyMalloc (&mStr[_NOT_CONFIGURED].string,NOT_CONFIGURED);
	strcpyMalloc (&mStr[_FREM].string,FREM);
	strcpyMalloc (&mStr[_MALF_CONF_READ].string,MALF_CONF_READ);
	strcpyMalloc (&mStr[_FREM_CONF_READ].string,FREM_CONF_READ);
	strcpyMalloc (&mStr[_MALF_SUMMARY].string,MALF_SUMMARY);
	strcpyMalloc (&mStr[_FREM_SUMMARY].string,FREM_SUMMARY);
	strcpyMalloc (&mStr[_TYPE_NOT_EXIST].string,TYPE_NOT_EXIST);
	strcpyMalloc (&mStr[_MALF_SCEN_LIST].string,MALF_SCEN_LIST);
	strcpyMalloc (&mStr[_FREM_SCEN_LIST].string,FREM_SCEN_LIST);
	strcpyMalloc (&mStr[_MALF_SCEN_TITLE].string,MALF_SCEN_TITLE);
	strcpyMalloc (&mStr[_DELAY_LABEL].string,DELAY_LABEL);
	strcpyMalloc (&mStr[_DURATION_LABEL].string,DURATION_LABEL);
	strcpyMalloc (&mStr[_RAMP_LABEL].string,RAMP_LABEL);
	strcpyMalloc (&mStr[_MALF_LIST].string,MALF_LIST);
	strcpyMalloc (&mStr[_FREM_LIST].string,FREM_LIST);
	strcpyMalloc (&mStr[_NEW_MALF_SCEN].string,NEW_MALF_SCEN);
	strcpyMalloc (&mStr[_NEW_FREM_SCEN].string,NEW_FREM_SCEN);
	strcpyMalloc (&mStr[_SYSTEM_LIST].string,SYSTEM_LIST);
	strcpyMalloc (&mStr[_BLOCK_LIST].string,BLOCK_LIST);
	strcpyMalloc (&mStr[_VAR_TO_SELECT].string,VAR_TO_SELECT);
	strcpyMalloc (&mStr[_VAR_SELECTED].string,VAR_SELECTED);
	strcpyMalloc (&mStr[_CONF_ACCEPT].string,CONF_ACCEPT);
	strcpyMalloc (&mStr[_SELVAR_CONFIRM].string,SELVAR_CONFIRM);
	strcpyMalloc (&mStr[_CONF_DISCARD].string,CONF_DISCARD);
	strcpyMalloc (&mStr[_SELECT_ALL].string,SELECT_ALL);
	strcpyMalloc (&mStr[_SELECT_VAR].string,SELECT_VAR);
	strcpyMalloc (&mStr[_UNSELECT_VAR].string,UNSELECT_VAR);
	strcpyMalloc (&mStr[_SAVEFILTER].string,SAVEFILTER);
	strcpyMalloc (&mStr[_APPLYFILTER].string,APPLYFILTER);
	strcpyMalloc (&mStr[_NEW_FILTER].string,NEW_FILTER);
	strcpyMalloc (&mStr[_ATTN_FILTER1].string,ATTN_FILTER1);
	strcpyMalloc (&mStr[_ATTN_FILTER2].string,ATTN_FILTER2);
	strcpyMalloc (&mStr[_KKS_NIMP_LABEL].string,KKS_NIMP_LABEL);
	strcpyMalloc (&mStr[_KKS_SYST_LABEL].string,KKS_SYST_LABEL);
	strcpyMalloc (&mStr[_KKS_SSYST_LABEL].string,KKS_SSYST_LABEL);
	strcpyMalloc (&mStr[_KKS_COMP_LABEL].string,KKS_COMP_LABEL);
	strcpyMalloc (&mStr[_KKS_PROG_LABEL].string,KKS_PROG_LABEL);
	strcpyMalloc (&mStr[_KKS_APPLY_FILTER].string,KKS_APPLY_FILTER);
	strcpyMalloc (&mStr[_TIPO_PRESSIONE].string,TIPO_PRESSIONE);
	strcpyMalloc (&mStr[_TIPO_TEMPERATURA].string,TIPO_TEMPERATURA);
	strcpyMalloc (&mStr[_TIPO_ENTALPIA].string,TIPO_ENTALPIA);
	strcpyMalloc (&mStr[_TIPO_PORTATA].string,TIPO_PORTATA);
	strcpyMalloc (&mStr[_TIPO_POTENZA].string,TIPO_POTENZA);
	strcpyMalloc (&mStr[_TIPO_GIRI].string,TIPO_GIRI);
	strcpyMalloc (&mStr[_TIPO_ALZVALV].string,TIPO_ALZVALV);
	strcpyMalloc (&mStr[_TIPO_AINGABLE].string,TIPO_AINGABLE);
	strcpyMalloc (&mStr[_TIPO_INPUT].string,TIPO_INPUT);
	strcpyMalloc (&mStr[_TIPO_OUTPUT].string,TIPO_OUTPUT);
	strcpyMalloc (&mStr[_PROC_START_OK].string,PROC_START_OK);
	strcpyMalloc (&mStr[_PROC_START_Quit].string,PROC_START_Quit);
	strcpyMalloc (&mStr[_PROC_START_LABEL].string,PROC_START_LABEL);
	strcpyMalloc (&mStr[_PROC_START_DISPLAY].string,PROC_START_DISPLAY);
	strcpyMalloc (&mStr[_OPTION_APPLY_LABEL].string,OPTION_APPLY_LABEL);
	strcpyMalloc (&mStr[_APPLYLABEL].string,APPLYLABEL);
	strcpyMalloc (&mStr[_QUITLABEL].string,QUITLABEL);
	strcpyMalloc (&mStr[_EXITLABEL].string,EXITLABEL);
	strcpyMalloc (&mStr[_ACT_LABEL].string,ACT_LABEL);
	strcpyMalloc (&mStr[_DEACT_LABEL].string,DEACT_LABEL);
	strcpyMalloc (&mStr[_FileMenuLabel].string,FileMenuLabel);
	strcpyMalloc (&mStr[_EditMenuLabel].string,EditMenuLabel);
	strcpyMalloc (&mStr[_ShowMenuLabel].string,ShowMenuLabel);
	strcpyMalloc (&mStr[_HelpMenuLabel].string,HelpMenuLabel);
	strcpyMalloc (&mStr[_SaveMenuLabel].string,SaveMenuLabel);
	strcpyMalloc (&mStr[_SaveMenuLabel2].string,SaveMenuLabel2);
	strcpyMalloc (&mStr[_OpenMenuLabel].string,OpenMenuLabel);
	strcpyMalloc (&mStr[_OpenMenuLabel2].string,OpenMenuLabel2);
	strcpyMalloc (&mStr[_QuitMenuLabel].string,QuitMenuLabel);
	strcpyMalloc (&mStr[_NewMenuLabel].string,NewMenuLabel);
	strcpyMalloc (&mStr[_CutMenuLabel].string,CutMenuLabel);
	strcpyMalloc (&mStr[_AddItemLabel].string,AddItemLabel);
	strcpyMalloc (&mStr[_AddItemLabel2].string,AddItemLabel2);
	strcpyMalloc (&mStr[_ClearLabel].string,ClearLabel);
	strcpyMalloc (&mStr[_MaxTimeLabel].string,MaxTimeLabel);
	strcpyMalloc (&mStr[_MaxSpeedLabel].string,MaxSpeedLabel);
	strcpyMalloc (&mStr[_SetSpeedLabel].string,SetSpeedLabel);
	strcpyMalloc (&mStr[_StepScalingLabel].string,StepScalingLabel);
	strcpyMalloc (&mStr[_SnapshotLabel].string,SnapshotLabel);
	strcpyMalloc (&mStr[_BacktrackLabel].string,BacktrackLabel);
	strcpyMalloc (&mStr[_ContRecLabel].string,ContRecLabel);
	strcpyMalloc (&mStr[_IandTactionLog].string,IandTactionLog);
	strcpyMalloc (&mStr[_PerformanceLabel].string,PerformanceLabel);
	strcpyMalloc (&mStr[_ShutdownLabel].string,ShutdownLabel);
	strcpyMalloc (&mStr[_InitIcLabel].string,InitIcLabel);
	strcpyMalloc (&mStr[_InitBtLabel].string,InitBtLabel);
	strcpyMalloc (&mStr[_STARTUP_ERR].string,STARTUP_ERR);
	strcpyMalloc (&mStr[_MALF_SELECTION].string,MALF_SELECTION);
	strcpyMalloc (&mStr[_FREM_SELECTION].string,FREM_SELECTION);
	strcpyMalloc (&mStr[_CompressLabel].string,CompressLabel);
	strcpyMalloc (&mStr[_ExpandLabel].string,ExpandLabel);
	strcpyMalloc (&mStr[_MasterMenuLabel].string,MasterMenuLabel);
	strcpyMalloc (&mStr[_PrintMenuLabel].string,PrintMenuLabel);
	strcpyMalloc (&mStr[_CopyMenuLabel].string,CopyMenuLabel);
	strcpyMalloc (&mStr[_PasteMenuLabel].string,PasteMenuLabel);
	strcpyMalloc (&mStr[_ViewMenuLabel].string,ViewMenuLabel);
	strcpyMalloc (&mStr[_LoadMenuLabel].string,LoadMenuLabel);
	strcpyMalloc (&mStr[_CheckMenuLabel].string,CheckMenuLabel);
	strcpyMalloc (&mStr[_VAL_ACT_LABEL].string,VAL_ACT_LABEL);

   strcpyMalloc (&mStr[_LoadDefaultLabel].string,LoadDefaultLabel);
   strcpyMalloc (&mStr[_SelectNewLabel].string,SelectNewLabel);
   strcpyMalloc (&mStr[_ClearAllLabel].string,ClearAllLabel);
   strcpyMalloc (&mStr[_CheckAllLabel].string,CheckAllLabel);
   strcpyMalloc (&mStr[_LOADLABEL].string,LOADLABEL);
   strcpyMalloc (&mStr[_SAVELABEL].string,SAVELABEL);
   strcpyMalloc (&mStr[_VIEWLABEL].string,VIEWLABEL);
   strcpyMalloc (&mStr[_INITLABEL].string,INITLABEL);
   strcpyMalloc (&mStr[_InitMenuLabel].string,InitMenuLabel);
}
/******************************************************************/
int strcpyMalloc (char **s1, char *s2)
{
	*s1 = (char *)malloc(strlen(s2)+1);
	strcpy (*s1,s2);
	return (strlen(s2)+1);
}
/******************************************************************/
int caricaMessages (int lingua)
{
FILE *fp;
char nome[MAX_PATH_LEN];

	initMessStruct ();
	messStructDefault ();  

	switch (lingua)
		{
		case DEFAULT_TEXT:
			sprintf (nome,"%s/%s",FILES_PATH,"BancoLabelsStrings_ENG");
			break;
		case ENGLISH_TEXT:
			sprintf (nome,"%s/%s",FILES_PATH,"BancoLabelsStrings_ENG");
			break;
		case ITALIAN_TEXT:
			sprintf (nome,"%s/%s",FILES_PATH,"BancoLabelsStrings_ITA");
			break;
		}
/*
 * se il file non esiste o lingua non selezionata
 *		mantiene il default
 */
	if (!existFile(nome))
		{
		fprintf (stderr,"MESSAGES: file [%s] non esiste\n",nome);
		return (-1);
		}

	loadMessagesString (nome, lingua);
	
}
/******************************************************************/
int loadMessagesString (char *nome, int lingua)
{
FILE *fp;
char riga[200];
char varLab[MAX_NOME_LEN];
char varMess[MAX_STRINGA_LEN];
int i = 0;


	freeMessageStrings ();
	fp = fopen (nome,"r");

	while (fgets(riga,MAX_STRINGA_LEN,fp) != NULL)
		{
		strcpy (varLab,getVar(riga));
		for (i=0; i<maxIndxMessages; i++)
			{
			if (!strcmp(varLab,mStr[i].label))	/* trovata la label	*/
				{
				free (mStr[i].string);
				strcpyMalloc (&mStr[i].string,getMessageString(fp,varLab));
				}
			}
		}
}
/******************************************************************/
int getNumMessages (char *fname)
{
FILE *fp;
char riga[200];
int nMess = 0;

	if (!existFile(fname))
		return (-1);
	fp = fopen (fname,"r");

	while (fgets(riga,MAX_STRINGA_LEN,fp) != NULL)
		nMess++;

	return (nMess);
}
/******************************************************************/
/*
 * ritorna la dimensione delle stringhe
 */
int getSizeMessages (char *fname)
{
FILE *fp;
char riga[200];
char varMess[MAX_STRINGA_LEN];
int size = 0;

   if (!existFile(fname))
      return (-1);
   fp = fopen (fname,"r");

   while (fgets(riga,MAX_STRINGA_LEN,fp) != NULL)
		{
		strcpy (varMess,getVarMessage(riga));
		if (varMess[0] != NULL)
			size += (strlen(varMess)+1);
		}

   return (size);
}
/******************************************************************/
/*
 * ritorna la stringa configurata nel file
 */
char *getMessageString (FILE *fp, char *var)
{
int i;
char riga[200];
char varLab[MAX_NOME_LEN];
static char varMess[MAX_STRINGA_LEN];	/* static: se ne ritorna l'indirizzo */

	rewind (fp);
	while (fgets(riga,MAX_STRINGA_LEN,fp) != NULL)
		{
		strcpy (varLab, getVar(riga));
		if (varLab != NULL)
			if (!strcmp(var,varLab))
				{
				strcpy (varMess, getVarMessage(riga));
				if (varMess[0] != NULL)
					return (varMess);
				}
		}
	return ("NULL");
}
/******************************************************************/
/*
 * ritorna la variabile
 *	la variabile e' racchiusa tra * e :
 */
/*  Su una riga malformata ritorna "" e non NULL: tutti i chiamanti fanno
    strcpy del risultato, e con NULL si schiantavano.  */
char *getVar (char *s)
{
static char app[MAX_NOME_LEN];	/* static: se ne ritorna l'indirizzo */
int i,k;

	i = 0;
	k = 0;
	while (s[i] != '*')	/* skippa fino a '*' */
		{
		if (s[i] == '\0')	/* formato non corretto */
			{
			fprintf (stderr,"getVar: formato non corretto (*):[%s]\n",s);
			return ("");
			}
		i++;
		}
	i++;						 /* skippa anche '*' */	

	while (s[i] != ':')	 /* copia fino a ':'	*/
		{
		if ((s[i] == ' ') || (s[i] == '\t'))
			{
			i++;				/* toglie i blank (senza i++ il ciclo non finiva) */
			continue;
			}
		if (s[i] == '\0')  /* formato non corretto */
         {
         fprintf (stderr,"getVar: formato non corretto (:):[%s]\n",s);
         return ("");
         }
		if (k < MAX_NOME_LEN-1)		/* oltre, si tronca */
			app[k++] = s[i];
		i++;	
		}
	app[k] = '\0';
	return (app);
}
/******************************************************************/
/*
 * ritorna il valore della variabile
 */
/*  Come getVar: "" e non NULL su una riga malformata.  */
char *getVarMessage (char *s)
{
static char varMess[MAX_STRINGA_LEN];	/* static: se ne ritorna l'indirizzo */
int i,k;

	i=0;
	k=0;

	while (s[i] != '"')   /* skippa fino a '"'  */
		{
      if (s[i] == '\0') /* formato non corretto */
         {
         fprintf (stderr,"getVar: formato non corretto (:):[%s]\n",s);
         return ("");
         }
      i++;
      }
   i++;

	while (s[i] != '"')   /* copia fino a '"'  */
      {
      if (s[i] == '\0') /* formato non corretto */
         {
         fprintf (stderr,"getVar: formato non corretto (:):[%s]\n",s);
         return ("");
         }
		if (k < MAX_STRINGA_LEN-1)	/* oltre, si tronca */
			varMess[k++] = s[i];
      i++;
      }
	varMess[k] = '\0';
	return (varMess);
}
/***************************************************************/
/*
 * libera la memoria occupata dalle stringhe
 *		la prima volta le stringhe non sono allocate
 */
int freeMessageStrings ()
{
static int prima_volta = 1;
int i;

	if (prima_volta)
		{
		prima_volta = 0;
		return (0);
		}

	for (i=0; i<maxIndxMessages; i++)
		free (mStr[i].string);
	return (1);
}
