/**********************************************************************
*
*       C Source:               XlPictograph.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Mon Jan 27 10:08:22 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlPictograph.c-10.1.4.2.4 %  (%full_filespec: XlPictograph.c-10.1.4.2.4:csrc:1 %)";
#endif
/*
   modulo XlPictograph.c
   tipo 
   release 5.2
   data 1/5/96
   reserved @(#)XlPictograph.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlPictograph.c	5.2\t1/5/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlPictograph.c - widget pictograph per stazioni
 */
#include <Xm/XmP.h>
#ifndef VMS
#include <X11/Xmu/Xmu.h>
#endif
#include <X11/StringDefs.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlPictographP.h>
#include <Xl/nullo.bmp>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH 10
#define DEFAULTHEIGHT 10

static int leggi_pict_program();


/* macro per la verifica sulla esistenza di un file
*/
struct stat Statbuf;
# define esiste(file)           (stat(file,&Statbuf)<0 ? 0:Statbuf.st_mode)

/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNcomponent,
        XlCComponent,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.component),
        XmRImmediate,
        ""
        },
	{
        XlNowOpen,
        XlCOwOpen,
        XmRBoolean,
        sizeof(Boolean),
        XtOffsetOf(XlPictographRec,pictograph.owOpen),
        XmRImmediate,
        False 
        },
	{
        XlNsubsystem,
        XlCSubsystem,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.subsystem),
        XmRImmediate,
        ""
        },
	{
        XlNnomeFilePict,
        XlCNomeFilePict,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.nome_file_pictograph),
        XmRImmediate,
        "null"
        },
        {
        XlNnomeProgram,
        XlCNomeProgram,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.nome_file_program),
        XmRImmediate,
        "null"
        },
        {
        XlNtopBorder,
        XlCTopBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.top_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNbottomBorder,
        XlCBottomBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.bottom_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNleftBorder,
        XlCLeftBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.left_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNrightBorder,
        XlCRightBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.right_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNframeWidth,
        XlCFrameWidth,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.frame.frameWidth),
        XmRImmediate,
        (int)0 
        },
        {
        XlNvarInputFrame,
        XlCVarInputFrame,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.frame.str_input),
        XmRImmediate,
        ""
        },
        {
        XlNowFrameColor,
        XlCOwFrameColor,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlPictographRec,pictograph.frame.owFrameColor),
        XmRString,
        XlDefaultColore1
        },
        {
        XlNvarFrameColor,
        XlCVarFrameColor,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlPictographRec,pictograph.frame.varFrameColor),
        XmRString,
        XlDefaultColore1
        },
        {
        XlNvarInputCambioColore1_1,
        XlCVarInputCambioColore1_1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_1,
        XlCFlagOffset1_1,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[0].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_2,
        XlCVarInputCambioColore1_2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_2,
        XlCFlagOffset1_2,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[1].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_3,
        XlCVarInputCambioColore1_3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[2].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_3,
        XlCFlagOffset1_3,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[2].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_4,
        XlCVarInputCambioColore1_4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[3].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_4,
        XlCFlagOffset1_4,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[3].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_5,
        XlCVarInputCambioColore1_5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[4].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_5,
        XlCFlagOffset1_5,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[4].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore1_6,
        XlCVarInputCambioColore1_6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[5].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset1_6,
        XlCFlagOffset1_6,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[5].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNcoloreDefault1,
        XlCColoreDefault1,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].defaultColor),
        XmRString,
        XlDefaultColore1
        },
        {
        XlNvarInputCambioColore2_1,
        XlCVarInputCambioColore2_1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset2_1,
        XlCFlagOffset2_1,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[0].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore2_2,
        XlCVarInputCambioColore2_2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset2_2,
        XlCFlagOffset2_2,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[1].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore2_3,
        XlCVarInputCambioColore2_3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[2].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset2_3,
        XlCFlagOffset2_3,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[2].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore2_4,
        XlCVarInputCambioColore2_4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[3].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset2_4,
        XlCFlagOffset2_4,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[3].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore2_5,
        XlCVarInputCambioColore2_5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[4].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset2_5,
        XlCFlagOffset2_5,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[4].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore2_6,
        XlCVarInputCambioColore2_6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[5].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset2_6,
        XlCFlagOffset2_6,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[5].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNcoloreDefault2,
        XlCColoreDefault2,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].defaultColor),
        XmRString,
        XlDefaultColore2
        },
        {
        XlNvarInputCambioColore3_1,
        XlCVarInputCambioColore3_1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset3_1,
        XlCFlagOffset3_1,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[0].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore3_2,
        XlCVarInputCambioColore3_2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset3_2,
        XlCFlagOffset3_2,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[1].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore3_3,
        XlCVarInputCambioColore3_3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[2].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset3_3,
        XlCFlagOffset3_3,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[2].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore3_4,
        XlCVarInputCambioColore3_4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[3].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset3_4,
        XlCFlagOffset3_4,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[3].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore3_5,
        XlCVarInputCambioColore3_5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[4].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset3_5,
        XlCFlagOffset3_5,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[4].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore3_6,
        XlCVarInputCambioColore3_6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[5].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset3_6,
        XlCFlagOffset3_6,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[5].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNcoloreDefault3,
        XlCColoreDefault3,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].defaultColor),
        XmRString,
        XlDefaultColore3
        },
        {
        XlNvarInputCambioColore4_1,
        XlCVarInputCambioColore4_1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset4_1,
        XlCFlagOffset4_1,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[0].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore4_2,
        XlCVarInputCambioColore4_2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset4_2,
        XlCFlagOffset4_2,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[1].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore4_3,
        XlCVarInputCambioColore4_3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[2].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset4_3,
        XlCFlagOffset4_3,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[2].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore4_4,
        XlCVarInputCambioColore4_4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[3].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset4_4,
        XlCFlagOffset4_4,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[3].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore4_5,
        XlCVarInputCambioColore4_5,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[4].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset4_5,
        XlCFlagOffset4_5,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[4].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNvarInputCambioColore4_6,
        XlCVarInputCambioColore4_6,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[5].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNflagOffset4_6,
        XlCFlagOffset4_6,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[5].FlagOffset),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNcoloreDefault4,
        XlCColoreDefault4,
        XtRAnimatedColor,
        sizeof(XlAnimatedColor),
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].defaultColor),
        XmRString,
        XlDefaultColore4
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
	{
        XlNtrasparent,
        XlCTrasparent,
        XlDtrasparent,
        XlROption,
        NULL,
        XlOtrasparent,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,xlcore.trasparent),
        sizeof(int),
        },
 	{
        XlNcomponent,
        XlCComponent,
        XlDcomponent,
        XlRComponent,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.component),
        sizeof(char*)
        },
 	{
        XlNsubsystem,
        XlCSubsystem,
        XlDsubsystem,
        XlRSubSystem,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.subsystem),
        sizeof(char*)
        },
 	{
        XlNnomeFilePict,
        XlCNomeFilePict,
        XlDnomeFilePict,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.nome_file_pictograph),
        sizeof(char*)
        },
        {
        XlNnomeProgram,
        XlCNomeProgram,
        XlDnomeProgram,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.nome_file_program),
        sizeof(char*)
        },
        {
        XlNtopBorder,
        XlCTopBorder,
        XlDtopBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.top_border),
        sizeof(int)
        },
        {
        XlNbottomBorder,
        XlCBottomBorder,
        XlDbottomBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.bottom_border),
        sizeof(int)
        },
        {
        XlNleftBorder,
        XlCLeftBorder,
        XlDleftBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.left_border),
        sizeof(int)
        },
        {
        XlNrightBorder,
        XlCRightBorder,
        XlDrightBorder,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.right_border),
        sizeof(int)
        },
        {
        XlNframeWidth,
        XlCFrameWidth,
        XlDframeWidth,
        XlRInt,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.frame.frameWidth),
        sizeof(int)
        },
        {
        XlNvarInputFrame,
        XlCVarInputFrame,
        XlDvarInputFrame,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.frame.input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNowFrameColor,
        XlCOwFrameColor,
        XlDowFrameColor,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.frame.owFrameColor),
        sizeof(XlAnimatedColor)
        },
        {
        XlNvarFrameColor,
        XlCVarFrameColor,
        XlDvarFrameColor,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlPictographRec,pictograph.frame.varFrameColor),
        sizeof(XlAnimatedColor)
        },
	{
        XlNvarInputCambioColore1_1,
        XlCVarInputCambioColore1_1,
        XlDvarInputCambioColore1_1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_1,
        XlCFlagOffset1_1,
        XlDflagOffset1_1,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[0].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_2,
        XlCVarInputCambioColore1_2,
        XlDvarInputCambioColore1_2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_2,
        XlCFlagOffset1_2,
        XlDflagOffset1_2,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[1].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_3,
        XlCVarInputCambioColore1_3,
        XlDvarInputCambioColore1_3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[2].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_3,
        XlCFlagOffset1_3,
        XlDflagOffset1_3,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[2].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_4,
        XlCVarInputCambioColore1_4,
        XlDvarInputCambioColore1_4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[3].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_4,
        XlCFlagOffset1_4,
        XlDflagOffset1_4,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[3].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_5,
        XlCVarInputCambioColore1_5,
        XlDvarInputCambioColore1_5,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[4].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_5,
        XlCFlagOffset1_5,
        XlDflagOffset1_5,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[4].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore1_6,
        XlCVarInputCambioColore1_6,
        XlDvarInputCambioColore1_6,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[5].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset1_6,
        XlCFlagOffset1_6,
        XlDflagOffset1_6,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].pict_var[5].FlagOffset),
        sizeof(int)
        },
        {
        XlNcoloreDefault1,
        XlCColoreDefault1,
        XlDcoloreDefault1,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[0].defaultColor),
        sizeof(XlAnimatedColor)
        },
        {
        XlNvarInputCambioColore2_1,
        XlCVarInputCambioColore2_1,
        XlDvarInputCambioColore2_1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset2_1,
        XlCFlagOffset2_1,
        XlDflagOffset2_1,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[0].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore2_2,
        XlCVarInputCambioColore2_2,
        XlDvarInputCambioColore2_2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset2_2,
        XlCFlagOffset2_2,
        XlDflagOffset2_2,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[1].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore2_3,
        XlCVarInputCambioColore2_3,
        XlDvarInputCambioColore2_3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[2].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset2_3,
        XlCFlagOffset2_3,
        XlDflagOffset2_3,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[2].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore2_4,
        XlCVarInputCambioColore2_4,
        XlDvarInputCambioColore2_4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[3].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset2_4,
        XlCFlagOffset2_4,
        XlDflagOffset2_4,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[3].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore2_5,
        XlCVarInputCambioColore2_5,
        XlDvarInputCambioColore2_5,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[4].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset2_5,
        XlCFlagOffset2_5,
        XlDflagOffset2_5,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[4].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore2_6,
        XlCVarInputCambioColore2_6,
        XlDvarInputCambioColore2_6,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[5].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset2_6,
        XlCFlagOffset2_6,
        XlDflagOffset2_6,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].pict_var[5].FlagOffset),
        sizeof(int)
        },
        {
        XlNcoloreDefault2,
        XlCColoreDefault2,
        XlDcoloreDefault2,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[1].defaultColor),
        sizeof(XlAnimatedColor)
        },
        {
        XlNvarInputCambioColore3_1,
        XlCVarInputCambioColore3_1,
        XlDvarInputCambioColore3_1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset3_1,
        XlCFlagOffset3_1,
        XlDflagOffset3_1,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[0].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore3_2,
        XlCVarInputCambioColore3_2,
        XlDvarInputCambioColore3_2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset3_2,
        XlCFlagOffset3_2,
        XlDflagOffset3_2,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[1].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore3_3,
        XlCVarInputCambioColore3_3,
        XlDvarInputCambioColore3_3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[2].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset3_3,
        XlCFlagOffset3_3,
        XlDflagOffset3_3,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[2].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore3_4,
        XlCVarInputCambioColore3_4,
        XlDvarInputCambioColore3_4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[3].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset3_4,
        XlCFlagOffset3_4,
        XlDflagOffset3_4,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[3].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore3_5,
        XlCVarInputCambioColore3_5,
        XlDvarInputCambioColore3_5,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[4].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset3_5,
        XlCFlagOffset3_5,
        XlDflagOffset3_5,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[4].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore3_6,
        XlCVarInputCambioColore3_6,
        XlDvarInputCambioColore3_6,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[5].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset3_6,
        XlCFlagOffset3_6,
        XlDflagOffset3_6,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].pict_var[5].FlagOffset),
        sizeof(int)
        },
        {
        XlNcoloreDefault3,
        XlCColoreDefault3,
        XlDcoloreDefault3,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[2].defaultColor),
        sizeof(XlAnimatedColor)
        },
        {
        XlNvarInputCambioColore4_1,
        XlCVarInputCambioColore4_1,
        XlDvarInputCambioColore4_1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset4_1,
        XlCFlagOffset4_1,
        XlDflagOffset4_1,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[0].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore4_2,
        XlCVarInputCambioColore4_2,
        XlDvarInputCambioColore4_2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset4_2,
        XlCFlagOffset4_2,
        XlDflagOffset4_2,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[1].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore4_3,
        XlCVarInputCambioColore4_3,
        XlDvarInputCambioColore4_3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[2].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset4_3,
        XlCFlagOffset4_3,
        XlDflagOffset4_3,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[2].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore4_4,
        XlCVarInputCambioColore4_4,
        XlDvarInputCambioColore4_4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[3].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset4_4,
        XlCFlagOffset4_4,
        XlDflagOffset4_4,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[3].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore4_5,
        XlCVarInputCambioColore4_5,
        XlDvarInputCambioColore4_5,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[4].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset4_5,
        XlCFlagOffset4_5,
        XlDflagOffset4_5,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[4].FlagOffset),
        sizeof(int)
        },
        {
        XlNvarInputCambioColore4_6,
        XlCVarInputCambioColore4_6,
        XlDvarInputCambioColore4_6,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[5].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNflagOffset4_6,
        XlCFlagOffset4_6,
        XlDflagOffset4_6,
        XlRInt,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].pict_var[5].FlagOffset),
        sizeof(int)
        },
        {
        XlNcoloreDefault4,
        XlCColoreDefault4,
        XlDcoloreDefault4,
        XlRAnimatedColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlPictographRec,pictograph.cambio_colore[3].defaultColor),
        sizeof(XlAnimatedColor)
        },
	};
         
static void GetFrameGC();
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void ClassInitialize();
static void Initialize();
static void Destroy();
static void Redisplay();
static Boolean SetValues();
static Boolean Compile();
static Boolean Refresh();
static void DrawFrame(Widget,Boolean);
static void LoadProgram();
static void LoadPictograph();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* dichiarazione di funzioni varie */
static void BlinkProc(Widget, int, Boolean);
static int EsisteBck();
static BCK_OBJ *InsertBck();
static void DrawPictograph(Widget, int, Boolean,Pixel,Pixel,Pixel,Pixel);


/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

/* Inizializzazione del class record */
XlPictographClassRec xlPictographClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Pictograph",
    /* widget_size              */      sizeof(XlPictographRec),
    /* class_initialize         */      ClassInitialize,
    /* class_part_initialize    */      NULL,
    /* class_inited             */      FALSE,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      XtInheritRealize,
    /* actions                  */      NULL,
    /* num_actions              */      0,
    /* resources                */      resources,
    /* num_resources            */      XtNumber(resources),
    /* xrm_class                */      NULLQUARK,
    /* compress_motion          */      TRUE,
    /* compress_exposure        */      XtExposeCompressMultiple, /* TRUE*/
    /* compress_enterleave      */      TRUE,
    /* visible_interest         */      FALSE,
    /* destroy                  */      Destroy,
    /* resize                   */      XtInheritResize,
    /* expose                   */      Redisplay,
    /* set_values               */      SetValues,
    /* set_values_hook          */      NULL,
    /* set_values_almost        */      XtInheritSetValuesAlmost,
    /* get_values_hook          */      NULL,
    /* accept_focus             */      NULL,
    /* version                  */      XtVersion,
    /* callback_private         */      NULL,
    /* tm_table                 */      defaultTranslations,
    /* query_geometry           */      XtInheritQueryGeometry,
    /* display_accelerator      */      XtInheritDisplayAccelerator,
    /* extension                */      NULL
  },
  { /* legocore fields                           */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia  */   NULL,
    /* oggetto compilatore      */      NULL,
    /* informazioni per la configurazione */
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),
    /* refresh                  */      Refresh,
    /* compile                  */      Compile,
    /* read                     */      XtInheritRead,
    /* write                    */      XtInheritWrite,
    /* select                   */      XtInheritSelect,
    /* DrawBorserIfSelected     */      XtInheritDrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  },
  { /* pictograph fields */
    /* num_pictograph            */      0,
    /* sfondi                    */      NULL,
    /* num program               */      0,
    /* program                   */      NULL
  }
};

WidgetClass xlPictographWidgetClass = (WidgetClass) &xlPictographClassRec;

static void ClassInitialize()
{
   xlPictographClassRec.xlpictograph_class.programs=NULL;
   xlPictographClassRec.xlpictograph_class.num_program=0;
}

/* 
  istanzio i GC per il frame e quello di clear
  frame_gc owFrame_gc; il primo e' relativo alla variabile di controllo del frame;
                       il secondo e' relativo alla apertura della O/W; 
*/
static void GetFrameGC(w,modoPrint)
Widget w;
Boolean modoPrint;
{
   XGCValues values;
   unsigned long valuemask= GCForeground | GCBackground | GCLineWidth | GCLineStyle ;
   XlPictographWidget cw = (XlPictographWidget) w;

   values.line_width = cw->pictograph.frame.frameWidth;
   values.line_style = LineSolid;

#if defined XPRINTER_USED
   if(modoPrint)
   {
      values.foreground = XlPrintGetPixel(w,cw->pictograph.frame.varFrameColor.colore);
      values.background = XlPrintGetPixel(w,cw->core.background_pixel);
      cw->pictograph.frame.frame_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);

      values.foreground = XlPrintGetPixel(w,cw->pictograph.frame.owFrameColor.colore);
      cw->pictograph.frame.owFrame_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);

      values.foreground = XlPrintGetPixel(w,cw->core.background_pixel);
      values.background = XlPrintGetPixel(w,cw->core.background_pixel);
      cw->pictograph.clear_gc = XCreateGC(XlDisplay(w),XlWindow(w),
                                 valuemask, &values);
   }
   else
   {
      values.foreground = cw->pictograph.frame.varFrameColor.colore;
      values.background = cw->core.background_pixel;
      cw->pictograph.frame.frame_gc = XtGetGC(w, valuemask, &values);

      values.foreground = cw->pictograph.frame.owFrameColor.colore;
      cw->pictograph.frame.owFrame_gc = XtGetGC(w, valuemask, &values);

      values.foreground = cw->core.background_pixel;
      values.background = cw->core.background_pixel;
      cw->pictograph.clear_gc = XtGetGC(w, valuemask, &values);
   }
#else
   values.foreground = cw->pictograph.frame.varFrameColor.colore;
   values.background = cw->core.background_pixel;
   cw->pictograph.frame.frame_gc = XtGetGC(w, valuemask, &values);

   values.foreground = cw->pictograph.frame.owFrameColor.colore;
   cw->pictograph.frame.owFrame_gc = XtGetGC(w, valuemask, &values);

   values.foreground = cw->core.background_pixel;
   values.background = cw->core.background_pixel;
   cw->pictograph.clear_gc = XtGetGC(w, valuemask, &values);
#endif
}


/* =================================================================
   verifica nella struttura della classe la presenza di uno sfondo
   17/7/97 viene verificato oltre al nome amche lo screen perche'
           avendo nella struttura dei Pixel value questi sono legati
           allo screen.
   ritorno:
                posizione: se e' stata trovata
                -1 : se non e' stata trovata
   =================================================================
*/
static int EsisteProgram(w,nome_program)
Widget w;
char *nome_program;
{
   int num_program;
   int i;

   num_program=xlPictographClassRec.xlpictograph_class.num_program;

   for(i=0;i<num_program;i++)
        if(strcmp(xlPictographClassRec.xlpictograph_class.programs[i].nome_program,
                    nome_program)==0) 
           if( XtScreen(w) == xlPictographClassRec.xlpictograph_class.programs[i].scr )
                return(i);

   return(-1);
}


/* aggiorno la cache dei programmi nella classe dello widget 
*/
static void aggiorna_classe_prg(Widget w,XlPictographAnimaz *prg)
{
   int num_program;
   int nrow;
   int i,j,nval;
   int val;

   num_program = xlPictographClassRec.xlpictograph_class.num_program;
   nrow = prg->nrow;
   
   /* se nella cache non c'e' niente la XtRaelloc essendo il ptr pari a NULL 
      fa una semplice XtMalloc altrimenti alloca nuovo spazio e copia 
      il vecchio contenuto
   */    
   if( (xlPictographClassRec.xlpictograph_class.programs =
           (XlPictographAnimaz *)XtRealloc(
                      xlPictographClassRec.xlpictograph_class.programs,
                      sizeof(XlPictographAnimaz)*(num_program+1))) == NULL)
   {
         fprintf(stderr,"XlPictograph: Erroc in allocation of programs cache\n");
         return;
   }
   
   /* copio il nome del programma e il numero di righe nella cache 
      appena allocata
   */
   
   memcpy(&xlPictographClassRec.xlpictograph_class.programs[num_program],
          prg,sizeof(XlPictographAnimaz));


   /* alloco spazio per le righe
      +1 perche chiudo il vettore con idvar = -1
      (se non sivuole piu' chiudere il vettore con -1 e' necessario rivedere
       le allocazioni, la leggi_pict_program e principalmete la XlValueToBlink)
   */
   if( 
     (xlPictographClassRec.xlpictograph_class.programs[num_program].row =
      (XlPictographAnimazRow *) XtCalloc(nrow+1,sizeof(XlPictographAnimazRow))) == NULL) 
   {
         fprintf(stderr,"XlPictograph: Erroc in allocation of rows programs cache\n");
         return;
   }
  
   /* trasferisco i dati delle righe nella cache
   for(i=0;i<prg->nrow;i++)
   {
      xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].idvar =
      prg.row[i].idvar;
      xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].nval = 
      prg.row[i].nval;
      xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].animaz.colore = prg->row[i].animaz.colore;
      xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].animaz.blink_rate = prg->row[i].animaz.blink_rate;
      xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].animaz.trasparent = prg->row[i].animaz.trasparent;

   }
   */

   /* trasferisco i dati delle righe nella cache
      che contengono id della zona il colore animato , il gc
      e il numero dei valori di confronto
      nonche il vettore dei calori che pero' copio successivamente
   */
   for(i=0;i<prg->nrow;i++)
   {
     memcpy(&xlPictographClassRec.xlpictograph_class.programs[num_program].row[i],
          &prg->row[i],sizeof(XlPictographAnimazRow));
   }

   

   /* alloco spazio per il vettore dei valori associato 
      a ciascuna riga
   */
   for(i=0;i<prg->nrow;i++)
   {
      nval = prg->row[i].nval;

      if( (xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].val=
         (int *)XtCalloc(1,sizeof(int)*nval))==NULL)
      {
         fprintf(stderr,"XlPictograph: Error in malloc caching values\n");
         return;
      }

      for(j=0;j<nval;j++)
      {
         val = prg->row[i].val[j];
         xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].val[j] = val;

         val = xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].val[j];

     }
  }
  xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].idvar = -1;

  xlPictographClassRec.xlpictograph_class.num_program++;

}

/*****************************************************
 * LoadProgram
 * effettua la lettura del file program (.prg) associato
 * all'oggetto. Una volta caricato lo mette nella cache
 * della classe dell'oggetto. Di conseguenza l'oggetto  
 * successivo che fara' riferimento allo stesso program 
 * non carichera' il program ma semplicemente assnera'
 * l'indice di quello gia' presente nella cache.
 * - NB - il program caricato contenendo dei colori 
 *        in forma di Pixel e' legato allo screen
 *        percui oltre al nome del file program e' 
 *        identificato dallo SCREEN relativo all'oggetto  
 *        Per la implementazione attuale quindi un program
 *        puo' essere caricato piu' volte nella cache
 *        perche associato a piu' di uno screen
 *****************************************************/
static void LoadProgram(w)
Widget w;
{
   XlPictographWidget cw= (XlPictographWidget)w;
   XlPictographClassRec *wPictographClass;
   int num_program; 
   int pos_program;
   XlPictographAnimaz *prg;

   /* inizializzo a null 
   */
   cw->pictograph.program=NULL;

   /* verifica la esistenza del program nella cache
      e se esiste ne individua la posizione nel vettore 
      NB oltre al nome nel programma di ricerca viene verificato
         anche lo screen dell'oggetto.
   */
   pos_program=EsisteProgram(XtParent(w),cw->pictograph.nome_file_program);
   

/***
printf("\n\nDEBUG XlPictograph il programma e' nella cache indice = %d (se = -1 non in cache)\n",pos_program);
***/

   /* se il programma non e' gia' stato caricato
      lo carica
   */
   if(pos_program == -1)
   {
      /* leggo il file program e lo metto direttamente nella cache
      */

      /* se nella cache non c'e' niente la XtRealloc essendo il ptr pari a NULL
         fa una semplice XtMalloc altrimenti alloca nuovo spazio e copia
         il vecchio contenuto
      */

      num_program = xlPictographClassRec.xlpictograph_class.num_program;

/*            printf("\nNr. prog %d - size complessiva %d - ptr %d\n",num_program,
             sizeof(XlPictographAnimaz)*(num_program+1),
             xlPictographClassRec.xlpictograph_class.programs);   Max */




      
      if ((xlPictographClassRec.xlpictograph_class.programs = 
          (XlPictographAnimaz *)XtRealloc(xlPictographClassRec.xlpictograph_class.programs,
          sizeof(XlPictographAnimaz)*(num_program+1))) == NULL)
      {
        fprintf(stderr,"XlPictograph: Error in (Re)allocation of programs cache\n");
        return;
      }

      
      
      if( leggi_pict_program(w,cw->pictograph.nome_file_program,
                             &xlPictographClassRec.xlpictograph_class.programs[num_program]) )
      {
         fprintf(stderr,"Error reading pictograph program %s",
                       cw->pictograph.nome_file_program);

         XtFree(xlPictographClassRec.xlpictograph_class.programs);
         return;
      }

       xlPictographClassRec.xlpictograph_class.num_program++;

/***** fine prova
******************/

/* Carlo prova per problemi colori in pixelvalue
*/
      pos_program=EsisteProgram(XtParent(w),cw->pictograph.nome_file_program);
 
      wPictographClass=(XlPictographClassRec *)XtClass(w);
/*
Modifica by Fabio
*/	
      cw->pictograph.index_program=pos_program;
/*
      cw->pictograph.program = &xlPictographClassRec.xlpictograph_class.programs[pos_program];
*/

   }
   else
   {

      /* gestisco qui il caso che lo sfondo sia gia' presente
         in memoria
      */
      pos_program=EsisteProgram(XtParent(w),cw->pictograph.nome_file_program);
      wPictographClass=(XlPictographClassRec *)XtClass(w);
/*
Modifica by Fabio
*/
      cw->pictograph.index_program=pos_program;
/*
      cw->pictograph.program = &xlPictographClassRec.xlpictograph_class.programs[pos_program];
*/
   }

}

/* load del file pictograph associata al pictograph
*/
static void LoadPictograph(w,width,height,top,bottom,left,right)
Widget w;
int *width;
int *height;
int top,bottom,left,right;
{
   XlPictographWidget cw= (XlPictographWidget)w;
   int dim[4];
   int pos_sfondo;
   int app_int;

   /* inizializzo a null lo sfondo
   */
   cw->pictograph.sfondo=NULL;


   /* verifica la esistenza di uno sfondo in memoria 
      e se esiste ne individua la posizione nel vettore 
   */
   if(!cw->xlcore.config)
      pos_sfondo=EsisteBck(XtParent(w),cw->pictograph.nome_file_pictograph,
                          cw->xlcore.rotate,cw->xlcore.ass_rotate,
                          cw->pictograph.top_border,cw->pictograph.bottom_border,
                          cw->pictograph.left_border,cw->pictograph.right_border);
   else
      pos_sfondo = -1;

   /* se lo sfondo non e' gia' stato caricato 
      lo carica
   */
   if(pos_sfondo == -1)
   {
      /* carico nel vettore dim i bordi
         ( top,bottom,left,right )
      */
      dim[0]= cw->pictograph.top_border;
      dim[1]= cw->pictograph.bottom_border;
      dim[2]= cw->pictograph.left_border;
      dim[3]= cw->pictograph.right_border;

      /* leggo il file ico 
      */
      leggi_file_bck(XtParent(w),cw->pictograph.nome_file_pictograph,
		"ico",&(cw->pictograph.sfondo),dim);

      /* se lo sfondo non e' stato caricato
         assegno dimensioni 100x100 allo widget
      */
      if(cw->pictograph.sfondo==NULL)
      {
	*width = 100;
	*height = 100;
	set_something_val(cw,XmNborderWidth,(XtArgVal) 1);
	return;
      }

      /* inserisco il nuovo sfondo nella lista
         degli sfondi caricati
      */
      cw->pictograph.sfondo=InsertBck(XtParent(w),cw->pictograph.sfondo,
                                       dim,cw->pictograph.nome_file_pictograph,
		                       cw->xlcore.rotate,cw->xlcore.ass_rotate,
                                       cw->pictograph.top_border,cw->pictograph.bottom_border,
                                       cw->pictograph.left_border,cw->pictograph.right_border);

      if(cw->pictograph.sfondo!=NULL)
      {
	*width = dim[2]-dim[0];
	*height = dim[3]-dim[1];

        /* sistema i bordi
        */
        *width = *width + left + right;
        *height = *height + top + bottom;
      }
      else
      {
	*width = 0;
	*height = 0;
      }

      /* effettuo le eventuali rotazioni 
      */
      ruota_bck(width,height,cw->pictograph.sfondo,
		cw->xlcore.rotate,cw->xlcore.ass_rotate,
                cw->pictograph.top_border,cw->pictograph.bottom_border,
                cw->pictograph.left_border,cw->pictograph.right_border);


   }
   else
   { 
      
      /* gestisco qui il caso che lo sfondo sia gia' presente
         in memoria
      */

      cw->pictograph.sfondo= xlPictographClassRec.xlpictograph_class.sfondi[pos_sfondo].sfondo;
        
      if(cw->pictograph.sfondo!=NULL)
      {
	*width = xlPictographClassRec.xlpictograph_class.sfondi[pos_sfondo].dim[2] - 
                         xlPictographClassRec.xlpictograph_class.sfondi[pos_sfondo].dim[0];
	*height = xlPictographClassRec.xlpictograph_class.sfondi[pos_sfondo].dim[3] - 
                         xlPictographClassRec.xlpictograph_class.sfondi[pos_sfondo].dim[1];
      }
      else
      {
	*width = 0;
	*height = 0;
      }

      /* sistema i bordi
      */
      *width = *width + left + right;
      *height = *height + top + bottom;

      if((cw->xlcore.rotate==ROTATE_90)||(cw->xlcore.rotate==ROTATE_270))
      {
       	app_int= *width;
       	*width = *height;
       	*height = app_int;
      }
   }

   return;
}


/* metodo Initializa oggetto Pictograph
*/
static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
   Arg arg[5];
   XlPictographWidget new = (XlPictographWidget)tnew;
   int width,height,x,y;
   int zoom;
	
   /* inizializzazione nuova istanza di Pictograph
   */
   new->pictograph.sfondo= (BCK_OBJ *)-1;
   new->pictograph.region=NULL;
   new->pictograph.primo_refresh=1;
   new->pictograph.owOpen = False;
   new->pictograph.frame.blink = False;
   new->pictograph.OldPixel1= -1;
   new->pictograph.OldPixel2= -1;
   new->pictograph.OldPixel3= -1;
   new->pictograph.OldPixel4= -1;

   new->pictograph.blinking[0] = False;
   new->pictograph.blinking[1] = False;
   new->pictograph.blinking[2] = False;
   new->pictograph.blinking[3] = False;

   new->pictograph.blink_freq[0] = 0.0;
   new->pictograph.blink_freq[1] = 0.0;
   new->pictograph.blink_freq[2] = 0.0;
   new->pictograph.blink_freq[3] = 0.0;

   GetFrameGC(tnew,False);

   /* carica il background del pictograph  
      (eventualmente dalla cache)
   */
   LoadPictograph(tnew,&width,&height,
        new->pictograph.top_border,new->pictograph.bottom_border,
        new->pictograph.left_border, new->pictograph.right_border);

   /* carica il programma
   */
   LoadProgram(tnew);

   
 
     
   

   if( (width>0) && (height>0) )
   {
      if(XlIsXlComposite(XtParent(tnew)))
      {
         zoom=XlGetZoom(XtParent(tnew));
         width = width * zoom/100;
         height = height * zoom/100;
      }

      width++;
      height++;

      XtSetArg(arg[0],XmNwidth,width);
      XtSetArg(arg[1],XmNheight,height);
      XtSetValues(tnew,arg,2);
   }

   new->pictograph.old_frameval = 0.0;
   new->pictograph.frame.input.valore = 0;
   new->pictograph.cambio_colore[0].pict_var[0].input.valore = 0.0;
   new->pictograph.cambio_colore[0].pict_var[1].input.valore = 0.0;
   new->pictograph.cambio_colore[0].pict_var[2].input.valore = 0.0;
   new->pictograph.cambio_colore[0].pict_var[3].input.valore = 0.0;
   new->pictograph.cambio_colore[0].pict_var[4].input.valore = 0.0;
   new->pictograph.cambio_colore[0].pict_var[5].input.valore = 0.0;
   new->pictograph.cambio_colore[1].pict_var[0].input.valore = 0.0;
   new->pictograph.cambio_colore[1].pict_var[1].input.valore = 0.0;
   new->pictograph.cambio_colore[1].pict_var[2].input.valore = 0.0;
   new->pictograph.cambio_colore[1].pict_var[3].input.valore = 0.0;
   new->pictograph.cambio_colore[1].pict_var[4].input.valore = 0.0;
   new->pictograph.cambio_colore[1].pict_var[5].input.valore = 0.0;
   new->pictograph.cambio_colore[2].pict_var[0].input.valore = 0.0;
   new->pictograph.cambio_colore[2].pict_var[1].input.valore = 0.0;
   new->pictograph.cambio_colore[2].pict_var[2].input.valore = 0.0;
   new->pictograph.cambio_colore[2].pict_var[3].input.valore = 0.0;
   new->pictograph.cambio_colore[2].pict_var[4].input.valore = 0.0;
   new->pictograph.cambio_colore[2].pict_var[5].input.valore = 0.0;
   new->pictograph.cambio_colore[3].pict_var[0].input.valore = 0.0;
   new->pictograph.cambio_colore[3].pict_var[1].input.valore = 0.0;
   new->pictograph.cambio_colore[3].pict_var[2].input.valore = 0.0;
   new->pictograph.cambio_colore[3].pict_var[3].input.valore = 0.0;
   new->pictograph.cambio_colore[3].pict_var[4].input.valore = 0.0;
   new->pictograph.cambio_colore[3].pict_var[5].input.valore = 0.0;


}




/* ===========================
   Metodo di Selezione  
   in configurazione
   Gestito dalle Translation
   =========================== */

#define superclass (&xlCoreClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
   /* Richiama la routine di select di XlCore per la gestione della
       selezione
   */
   (*superclass->xlcore_class.select) (w,0);
}


/* ===================================
   Metodo di aggiunta alla lista degli
   oggetti selezionati 
   in configurazione
   Gestito dalle Translation
   =================================== */

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
   /* richiama il metodo di select 
   */
   (*superclass->xlcore_class.select)(w,1);
}


/* ===================================
   Metodo di SetValues
   =================================== */

static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
   XlPictographWidget curcw= (XlPictographWidget) current;
   XlPictographWidget newcw= (XlPictographWidget) new;
   Boolean do_redisplay = False;


   if( curcw->pictograph.owOpen != newcw->pictograph.owOpen )
   {
/*
           if( curcw.pictograph.frame.blink )
*/
           if( newcw->pictograph.owOpen == False )
           {
/*
printf("Rimuovo il blink del frame dal pictograph durante la SetValues\n");
*/
              newcw->pictograph.frame.blink = False;
              XlRemoveBlink(new,5);
           }
      do_redisplay=True;
   }

   if( strcmp(curcw->pictograph.nome_file_program,newcw->pictograph.nome_file_program) )
   {
      Compile(newcw);
      do_redisplay = True; 
   }

   return(do_redisplay);
}

/* ===================================
   Metodo di Destroy
   =================================== 
*/

static void Destroy(w)
Widget w;
{
   float bfreq[4];
   XlPictographWidget cw= (XlPictographWidget) w;


   if( cw->pictograph.frame.frame_gc )
     XtReleaseGC(w,cw->pictograph.frame.frame_gc);
   if( cw->pictograph.frame.owFrame_gc )
     XtReleaseGC(w,cw->pictograph.frame.owFrame_gc);
   if( cw->pictograph.clear_gc )
     XtReleaseGC(w,cw->pictograph.clear_gc);

                                                                                                                
   /* elimina, se necessario, il widget dalla lista di blink 
   */
   if( cw->pictograph.blinking[0] )
      XlRemoveBlink(cw,0);

   if( cw->pictograph.blinking[1] )
      XlRemoveBlink(cw,1);

   if( cw->pictograph.blinking[2] )
      XlRemoveBlink(cw,2);

   if( cw->pictograph.blinking[2] )
      XlRemoveBlink(cw,3);

   if( cw->pictograph.frame.blink )
      XlRemoveBlink(cw,5);
}


/* ===================================
   Metodo di Compilazione
   =================================== 
*/

static Boolean Compile(Widget w)
{
   static char *icon_dir=NULL;
   Boolean errors=False;
   XlPictographWidget cw= (XlPictographWidget)w;
   char nome_file[255];

   if(!XtIsWidget(w))
   {
      XlWarning("XlPictograph","Compile",
                "il parametro in ingresso non e' un Widget");
      return(False);
   }

   /* get della icon directory
   */
   if(icon_dir==NULL)
   {
      icon_dir=getenv("LEGOMMI_ICO");
      if(icon_dir==NULL)
      {
         XlErrComp(w,"Compile","Environment variable LEGOMMI_ICO not defined",NULL);
         errors=True;
         XlEvError(cw,True);
      }
   }

   /* il nome del file e' dato dalla risorsa nomeProgram
      con estensione .prg
   */
   sprintf(nome_file,"%s/%s.prg",icon_dir,cw->pictograph.nome_file_program);

   if( !esiste(nome_file) )
   {
      XlErrComp(w,"Compile","Program file not exist",NULL);
      errors=True;
      XlEvError(cw,True);
   }


#ifdef NON_SERVE?

    questa parte dovrebbe essere rivista per gestire 
    la nuova struttura delle variabili di animazione

   /* test variabile cambio colore 1 
   */

   if(XlEsisteCambioColore(cw->pictograph.sfondo,1) &&
      (strlen(cw->pictograph.cambio_colore[0].str_input)==0))
   {
        XlErrComp(w,"Compile","Input variable 1 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
   }

   /* test variabile cambio colore 2 
   */

   if(XlEsisteCambioColore(cw->pictograph.sfondo,2) &&
      (strlen(cw->pictograph.cambio_colore[1].str_input)==0))
   {
        XlErrComp(w,"Compile","Input variable 2 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
   }

   /* test variabile cambio colore 3 
   */

   if(XlEsisteCambioColore(cw->pictograph.sfondo,3) &&
      (strlen(cw->pictograph.cambio_colore[2].str_input)==0))
   {
        XlErrComp(w,"Compile","Input variable 3 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
   }

   /* test variabile cambio colore 4 
   */

   if(XlEsisteCambioColore(cw->pictograph.sfondo,4) &&
      (strlen(cw->pictograph.cambio_colore[3].str_input)==0))
   {
        XlErrComp(w,"Compile","Input variable 4 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
   }
#endif

   if(xlPictographClassRec.xlcore_class.compilatore==NULL)
   {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
   }

   if(!(*superclass->xlcore_class.compile) (w,
        xlPictographClassRec.xlcore_class.compilatore))
   {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
   }

   XlEvError(cw,errors);

   return(True);
}

/* =======================================================================
   XlPictograph Refresh Method:
       utilizzato run_time
           Effettua il ridisegno del XlPictograph 
           Nel caso di blink  questa routine setta i colori e la frequenza
           di blink il ridisegno lo fa la BlinkProc 
   ========================================================================
*/
static Boolean Refresh(Widget w)
{
   XlPictographWidget cw= (XlPictographWidget)w;
   int width=cw->core.width;
   int height=cw->core.height;
   int wi,he;
   XExposeEvent event;
   XmDrawingAreaCallbackStruct str;
   float blink_freq[4];
   Pixel RetPixel[4];
   Boolean flagRi[4],flagVari[4];


   /* se siamo in configurazione non devo fare niente
   */
   if(cw->xlcore.config == True)
      return(True);
   if(!XtIsWidget(w))
   {
      XlWarning("XlPictograph","Refresh","il parametro in ingresso non e' un Widget");
      return(False);
   }

   if(xlPictographClassRec.xlcore_class.database==NULL)
   {
      XlWarning("XlPictograph","Refresh","database non definito");
      return(False);
   }

   /* inizializzo 
   */
   if(cw->pictograph.primo_refresh== 1) /* primo refresh */
   {
      cw->pictograph.blinking[0] = False;
      cw->pictograph.blinking[1] = False;
      cw->pictograph.blinking[2] = False;
      cw->pictograph.blinking[3] = False;

      cw->pictograph.blink_freq[0] = 0.0;
      cw->pictograph.blink_freq[1] = 0.0;
      cw->pictograph.blink_freq[2] = 0.0;
      cw->pictograph.blink_freq[3] = 0.0;
   }

   /* test flag su var0
   */
   flagVari[0] = XlValueToBlinkPixel(w,0,&RetPixel[0],&blink_freq[0]);

   /* sistemo il colore 
   */

   if( flagVari[0] )
   {

      /* lo aggiungo nella lista solo se non sta gia blinkando
         se sta gia blinkando puo darsi che lo debba fare ad una frequenza
         diversa, in questo caso rimuovo e riaggiungo con una nuova frequenza
      */
      if( !cw->pictograph.blinking[0] )
      {
         cw->pictograph.blink_freq[0] = blink_freq[0];
         cw->pictograph.blinking[0] = True;
         XlAddBlink(cw,0,blink_freq[0],(void*)BlinkProc);
      }
      else
      {
         if( cw->pictograph.blinking[0] && (cw->pictograph.blink_freq[0] != blink_freq[0]) )
         {
            XlRemoveBlink(cw,0);
            cw->pictograph.blink_freq[0] = blink_freq[0];
            cw->pictograph.blinking[0] = True;
            XlAddBlink(cw,0,blink_freq[0],(void*)BlinkProc);
         }
      }
   }
   else
   {
     /* se prima blinkava rimuovo il blink
     */
     if( cw->pictograph.blinking[0] )
         XlRemoveBlink(cw,0);
     cw->pictograph.blinking[0] = False;
     cw->pictograph.blink_freq[0] = 0;

   }


   /* test flag su var1
   */
   flagVari[1] = XlValueToBlinkPixel(w,1,&RetPixel[1],&blink_freq[1]);


   if( flagVari[1] )
   {

      /* lo aggiungo nella lista solo se non sta gia blinkando
         se sta gia blinkando puo darsi che lo debba fare ad una frequenza
         diversa, in questo caso rimuovo e riaggiungo con una nuova frequenza
      */
      if( !cw->pictograph.blinking[1] )
      {
         cw->pictograph.blink_freq[1] = blink_freq[1];
         cw->pictograph.blinking[1] = True;
         XlAddBlink(cw,1,blink_freq[1],(void*)BlinkProc);
      }
      else
      {
         if(cw->pictograph.blinking[1] && (cw->pictograph.blink_freq[1] != blink_freq[1]) )
         {
            XlRemoveBlink(cw,1);
            cw->pictograph.blink_freq[1] = blink_freq[1];
            cw->pictograph.blinking[1] = True;
            XlAddBlink(cw,1,blink_freq[1],(void*)BlinkProc);
         }
      }
   }
   else
   {
     /* se prima blinkava rimuovo il blink
     */
     if( cw->pictograph.blinking[1] )
         XlRemoveBlink(cw,1);
     cw->pictograph.blinking[1] = False;
     cw->pictograph.blink_freq[1] = 0;
   }

   /* test flag su var2
   */
   flagVari[2] = XlValueToBlinkPixel(w,2,&RetPixel[2],&blink_freq[2]);

   if( flagVari[2] )
   {


      /* lo aggiungo nella lista solo se non sta gia blinkando
         se sta gia blinkando puo darsi che lo debba fare ad una frequenza
         diversa, in questo caso rimuovo e riaggiungo con una nuova frequenza
      */
      if( !cw->pictograph.blinking[2] )
      {
         cw->pictograph.blink_freq[2] = blink_freq[2];
         cw->pictograph.blinking[2] = True;
         XlAddBlink(cw,2,blink_freq[2],(void*)BlinkProc);
      }
      else
      {
         if(cw->pictograph.blinking[2] && (cw->pictograph.blink_freq[2] != blink_freq[2]))
         {
            XlRemoveBlink(cw,2);
            cw->pictograph.blink_freq[2] = blink_freq[2];
            cw->pictograph.blinking[2] = True;
            XlAddBlink(cw,2,blink_freq[2],(void*)BlinkProc);
         }
      }
   }
   else
   {
     /* se prima blinkava rimuovo il blink
     */
     if( cw->pictograph.blinking[2] )
         XlRemoveBlink(cw,2);
     cw->pictograph.blinking[2] = False;
     cw->pictograph.blink_freq[2] = 0;
   }

   /* test flag su var3
   */
   flagVari[3] = XlValueToBlinkPixel(w,3,&RetPixel[3],&blink_freq[3]);


   if( flagVari[3] )
   {

      /* lo aggiungo nella lista solo se non sta gia blinkando
         se sta gia blinkando puo darsi che lo debba fare ad una frequenza
         diversa, in questo caso rimuovo e riaggiungo con una nuova frequenza
      */
      if( !cw->pictograph.blinking[3] )
      {
         cw->pictograph.blink_freq[3] = blink_freq[3];
         cw->pictograph.blinking[3] = True;
         XlAddBlink(cw,3,blink_freq[3],(void*)BlinkProc);
      }
      else
      {
         if(cw->pictograph.blinking[3] && (cw->pictograph.blink_freq[3] != blink_freq[3]))
         {
            XlRemoveBlink(cw,3);
            cw->pictograph.blink_freq[3] = blink_freq[3];
            cw->pictograph.blinking[3] = True;
            XlAddBlink(cw,3,blink_freq[3],(void*)BlinkProc);
         }
      }
   }
   else
   {
     /* se prima blinkava rimuovo il blink
     */
     if( cw->pictograph.blinking[3] )
         XlRemoveBlink(cw,3);
     cw->pictograph.blinking[3] = False;
     cw->pictograph.blink_freq[3] = 0;
   }

   /* faccio il refresh del core
   */ 
   if(!(*superclass->xlcore_class.refresh) (w))
   {
      XlWarning("XlPictograph","Refresh","errore refresh classe superiore");
      return(False);
   }

   /* ridisegno la icona, solo se non deve blinkare
      altrimenti resetto solo i colori
   */
   if( !(cw->pictograph.blinking[0] || cw->pictograph.blinking[1] ||
         cw->pictograph.blinking[2] || cw->pictograph.blinking[3]) || 
         (cw->pictograph.primo_refresh==1) )
   {
      cw->pictograph.primo_refresh = 0;

      DrawPictograph(w,-1,False,
                       RetPixel[0],
                       RetPixel[1],
                       RetPixel[2],
                       RetPixel[3]);
   }
   else
   {
      cw->pictograph.OldPixel1 = RetPixel[0];
      cw->pictograph.OldPixel2 = RetPixel[1];
      cw->pictograph.OldPixel3 = RetPixel[2];
      cw->pictograph.OldPixel4 = RetPixel[3];
   }

/* se richiesto disegno il frame 
   questo pezo di codide lo faccio se la Ow e aperta 
   oppure se il valore della variabile di controllo e' cambiato
*/
   /* verifico se la O/W e' aperta
   */
   if( cw->pictograph.owOpen )  
   {
     /* disegno il frame eventualmente blinkate
     */
     if( cw->pictograph.frame.owFrameColor.blink_rate > 0.0 )
     {
        if( !cw->pictograph.frame.blink )
        {
/*
printf("La Ow risulta aperta e senza blink\nAggiungo il blink del frame \n");
*/
          cw->pictograph.frame.blink = True;
          XlAddBlink(cw,5,cw->pictograph.frame.owFrameColor.blink_rate,(void*)BlinkProc);
        }
     }
     else
        DrawFrame(w,0);
   }
   else if (cw->pictograph.frame.input.valore != cw->pictograph.old_frameval)
   {

/*
printf("O/W chiusa\n");
*/
      /* la O/W e' chiusa 
         se la variabile del frame e' a 1 deve essere disegnato il frame  
         altrimenti il frame non ci deve essere 
      */
      if( cw->pictograph.frame.input.valore )
      {
           if( cw->pictograph.frame.varFrameColor.blink_rate > 0.0 )
           {
              if(!cw->pictograph.frame.blink)
              {
                 cw->pictograph.frame.blink = True;
                 XlAddBlink(cw,5,cw->pictograph.frame.varFrameColor.blink_rate,
                                (void*)BlinkProc);
              }
           }
           else
              DrawFrame(w,0);
      }
      else /* non e' aperta non e' settata la variabile il frame non ci vuole */
      {
           /* se blinkava devo rimuovere il blink
           */
           if( cw->pictograph.frame.blink )
           {
/*
printf("Rimuovo il blink del frame dal pictograph\n");
*/
              cw->pictograph.frame.blink = False;
              XlRemoveBlink(cw,5);
           }

           DrawFrame(w,2);
      }
   }
   
   /* aggiorno il valore precedente alla variabile del frame
   */
   cw->pictograph.old_frameval = cw->pictograph.frame.input.valore;
          
   return(True);
}

/* ================================ 
   Disegna la icona del pictograph
   ================================
*/
static void DrawPictograph(Widget w,int modo,Boolean On,
	Pixel RetPixel1, Pixel RetPixel2, Pixel RetPixel3, Pixel RetPixel4)
{
   XlPictographWidget cw= (XlPictographWidget)w;
   int width=cw->core.width;
   int height=cw->core.height;
   XExposeEvent event;
   Pixel AppPixel1=RetPixel1;
   Pixel AppPixel2=RetPixel2;
   Pixel AppPixel3=RetPixel3;
   Pixel AppPixel4=RetPixel4;
   float blink_freq;

   event.x=0;
   event.y=0;
   event.width=width;
   event.height=height;
   event.window=XtWindow(cw);

#if defined BELL_ON
   if((modo>=0)&&(On==False))
	XBell(XtDisplay(cw),40);
#endif

   if((modo==0) && cw->pictograph.blinking[0] )
     if( On )
     {
	AppPixel1=cw->core.background_pixel;
        cw->pictograph.OldPixel1 = AppPixel1;
     }
     else
     {
        XlValueToBlinkPixel(w,0,&AppPixel1,&blink_freq);
        cw->pictograph.OldPixel1 = AppPixel1;
     }
   
   if((modo==1)&&cw->pictograph.blinking[1] )
     if( On )
     {
        AppPixel2=cw->core.background_pixel;
        cw->pictograph.OldPixel2 = AppPixel2;
     }
     else
     {
        XlValueToBlinkPixel(w,1,&AppPixel2,&blink_freq);
        cw->pictograph.OldPixel2 = AppPixel2;
     }

   if((modo==2)&&cw->pictograph.blinking[2]  )
     if( On )
     {
	AppPixel3=cw->core.background_pixel;
        cw->pictograph.OldPixel3 = AppPixel3;
     }
     else
     {
        XlValueToBlinkPixel(w,2,&AppPixel3,&blink_freq);
        cw->pictograph.OldPixel3 = AppPixel3;
     }

   if((modo==3)&&cw->pictograph.blinking[3] )
     if( On )
     {
	AppPixel4=cw->core.background_pixel;
        cw->pictograph.OldPixel4 = AppPixel4;
     }
     else
     {
        XlValueToBlinkPixel(w,3,&AppPixel4,&blink_freq);
        cw->pictograph.OldPixel4 = AppPixel4;
     }
   
   if(modo == -1)
	(*superclass->core_class.expose) (w,&event,NULL);

/* chiamo la routine disegna_bck
*/

   if(cw->pictograph.sfondo)
	disegna_bck_icon((Widget)cw,cw->pictograph.sfondo,
		AppPixel1,AppPixel2,AppPixel3,AppPixel4,False);
   
}



/*========================
  Metofo di Redisplay
  =======================
*/
static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
   XlPictographWidget cw= (XlPictographWidget)w;
   register int x,y;
   unsigned int width,height;
   XmDrawingAreaCallbackStruct str;

   if(event)
   {
        x=event->x;
        y=event->y;
        width=event->width;
        height=event->height;
   }
   else
   {
        x=0;
        y=0;
        width=cw->core.width;
        height=cw->core.height;
   }


   /* Richiamo il metodo di Redisplay di XlCore per la gestione della
      trasparenza e del contorno di selezione dell'oggetto
   */
   (*superclass->core_class.expose) (w,event,NULL);

   if(cw->pictograph.sfondo)
   {
	str.event=(XEvent*)event;
	disegna_bck_icon(w,cw->pictograph.sfondo,
                        cw->pictograph.OldPixel1,cw->pictograph.OldPixel2,
                        cw->pictograph.OldPixel3,cw->pictograph.OldPixel4,False);
   }

   /* Richiamo del metodo di disegno del bordo se l'oggetto
      e' selezionato
   */
   (*superclass->xlcore_class.drawBorderIfSelected) (w);

   /* ridisegno il frame run time in caso di expose
   */
   if( (cw->xlcore.config!=True) && cw->pictograph.frame.input.valore )
      DrawFrame(w,0);

}
 
/* =================================================================
   verifica nella struttura della classe la presenza di uno sfondo
   ritorno:
		posizione: se e' stata trovata
		-1 : se non e' stata trovata	
   =================================================================
*/
static int EsisteBck(w,nome_pictograph,rotate,ass_rotate,top,bottom,left,right)
Widget w;
char *nome_pictograph;
int rotate, ass_rotate;
int top,bottom,left,right;
{
   int num_pictograph;
   int i;

   num_pictograph=xlPictographClassRec.xlpictograph_class.num_pictograph;

   for(i=0;i<num_pictograph;i++)
	if((strcmp(xlPictographClassRec.xlpictograph_class.sfondi[i].nome_pictograph,
                    nome_pictograph)==0) &&
	   (rotate == xlPictographClassRec.xlpictograph_class.sfondi[i].rotate) &&
    	   (ass_rotate == xlPictographClassRec.xlpictograph_class.sfondi[i].ass_rotate) &&
           ( top ==  xlPictographClassRec.xlpictograph_class.sfondi[i].top_border) &&
           ( bottom ==  xlPictographClassRec.xlpictograph_class.sfondi[i].bottom_border) &&
           ( left == xlPictographClassRec.xlpictograph_class.sfondi[i].left_border) &&
           ( right == xlPictographClassRec.xlpictograph_class.sfondi[i].right_border) &&	
	   (XtDisplay(w) == xlPictographClassRec.xlpictograph_class.sfondi[i].display))
		return(i);

   return(-1);
}


/* =================================================================
	inserisce uno sfondo nella struttura della classe
   =================================================================
*/
static BCK_OBJ *InsertBck(w,new_sfondo,dim,nome_pictograph,rotate,ass_rotate,
top,bottom,left,right)
Widget w;
BCK_OBJ *new_sfondo;
int dim[4];
char *nome_pictograph;
int rotate, ass_rotate;
int top,bottom,left,right;
{
   int num_pictograph;
   int size_sfondo;
   int num_b=1;

   num_pictograph = xlPictographClassRec.xlpictograph_class.num_pictograph+1;
   xlPictographClassRec.xlpictograph_class.num_pictograph=num_pictograph;

   if(num_pictograph==1)
	xlPictographClassRec.xlpictograph_class.sfondi=NULL;

   xlPictographClassRec.xlpictograph_class.sfondi= 
                    (XlPictographSfondo*)XtRealloc( xlPictographClassRec.xlpictograph_class.sfondi,
		                                     sizeof(XlPictographSfondo)*(num_pictograph));

   while(new_sfondo[num_b-1].tipo>0)
	num_b++;

   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].sfondo =
                        (BCK_OBJ *)XtMalloc(sizeof(BCK_OBJ)*num_b);

   strcpy(xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].nome_pictograph,nome_pictograph);

   if(new_sfondo!=NULL)
     memcpy(xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].sfondo,new_sfondo,
             sizeof(BCK_OBJ)*num_b);

   memcpy(xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].dim,dim,4*sizeof(int));

   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].rotate=rotate;
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].ass_rotate=ass_rotate;
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].top_border=top;
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].bottom_border=bottom;
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].left_border=left;
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].right_border=right;
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].num_obj=num_b;
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].display=XtDisplay(w);
   xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].contatore=(num_pictograph-1);
   XtFree(new_sfondo);
   return(xlPictographClassRec.xlpictograph_class.sfondi[num_pictograph-1].sfondo);
}

/* ================================================================
   procedura per gestione del Blink
   Parametri:
      Widget w;    pictograph da far blinkare
      int ind;     zona animata del pictograph che deve blinkare
      Boolean On;  Blink on/off
   ================================================================
*/
static void BlinkProc(Widget w,int ind,Boolean On)
{
   XlPictographWidget cw= (XlPictographWidget) w;

   if( ind != 5)
      DrawPictograph(w,ind,On,cw->pictograph.OldPixel1,cw->pictograph.OldPixel2,
              cw->pictograph.OldPixel3,cw->pictograph.OldPixel4);
   else
      DrawFrame(w,On);
}


/*=======================
        metodo di stampa
  =======================
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
   XlPictographWidget cw= (XlPictographWidget)w;
   extern Display *printer;

   (*superclass->xlcore_class.print) (w);

/**
   if(cw->pictograph.frame.frame_gc)
        XtReleaseGC(XlDisplay(w),cw->pictograph.frame.frame_gc);
   if(cw->pictograph.frame.owFrame_gc)
        XtReleaseGC(XlDisplay(w),cw->pictograph.frame.owFrame_gc);
   if(cw->pictograph.clear_gc)
        XtReleaseGC(XlDisplay(w),cw->pictograph.clear_gc);

   GetFrameGC(w,True);
***/

   disegna_bck_icon(w,cw->pictograph.sfondo,
          cw->pictograph.OldPixel1,cw->pictograph.OldPixel2,
          cw->pictograph.OldPixel3,cw->pictograph.OldPixel4,True);
/***
   if(cw->pictograph.frame.frame_gc)
        XFreeGC(XlDisplay(w),cw->pictograph.frame.frame_gc);
   if(cw->pictograph.frame.owFrame_gc)
        XFreeGC(XlDisplay(w),cw->pictograph.frame.owFrame_gc);
   if(cw->pictograph.clear_gc)
        XFreeGC(XlDisplay(w),cw->pictograph.clear_gc);

   GetFrameGC(w,True);
***/

   return(True);
}
#endif




/* inserisce il prg nella cache
*/
int insert_prg_cache(Widget w)
{
   XlPictographWidget cw= (XlPictographWidget)w;
   int num_program = xlPictographClassRec.xlpictograph_class.num_program;
   int i,nrow,nval;

   /* alloco memoria per contenere il vettore dei programs
   */
   if( !num_program )
   {
      if( (xlPictographClassRec.xlpictograph_class.programs = 
                 (XlPictographAnimaz *)XtCalloc(1,sizeof(XlPictographAnimaz)))==NULL)
      {
         fprintf(stderr,"XlPictograph: Erroc in malloc of programs cache\n");
         return(1);
      }
   }
   else
   {
      num_program++;
      if( (xlPictographClassRec.xlpictograph_class.programs =
               (XlPictographAnimaz *)XtRealloc(xlPictographClassRec.xlpictograph_class.programs,
                         sizeof(XlPictographAnimaz)*num_program)) == NULL)
      {
         fprintf(stderr,"XlPictograph: Erroc in realloc of programs cache\n");
         return(1);
      }
   }
    

   /* copio i dati nella cache appena allocata
      i dati copiati sono il nome del program e il numero di righe
      (viene copiato anche il puntatore al vettore delle righe
       ma non dve essere usato perche appartiene all'oggetto, per cui 
       dopo alloco memoria per questo vettore di righe)
   
   memcpy( &xlPictographClassRec.xlpictograph_class.programs[num_program], 
           cw->pictograph.program, sizeof(XlPictographAnimaz));
   */

   strcpy(xlPictographClassRec.xlpictograph_class.programs[num_program].nome_program,
          cw->pictograph.program->nome_program);
   xlPictographClassRec.xlpictograph_class.programs[num_program].nrow = 
          cw->pictograph.program->nrow;
 
/***
printf("xlPictographClassRec.nome_program=%s\n,xlPictographClassRec.nrow=%d\n",
xlPictographClassRec.xlpictograph_class.programs[num_program].nome_program,
xlPictographClassRec.xlpictograph_class.programs[num_program].nrow);
***/

   nrow = xlPictographClassRec.xlpictograph_class.programs[num_program].nrow;
   /* alloco memoria per contenere le righe del program nella cache
   */
   if( (xlPictographClassRec.xlpictograph_class.programs[num_program].row =
         (XlPictographAnimazRow *)XtCalloc( nrow,sizeof(XlPictographAnimazRow)))== NULL)
   {
      fprintf(stderr,"XlPictograph: Error in malloc caching row\n");
      return(1);
   }

   /* copio i dati relativi alle righe 
   */
   memcpy( xlPictographClassRec.xlpictograph_class.programs[num_program].row, 
           cw->pictograph.program->row, sizeof(XlPictographAnimazRow)*nrow);

   /* a sua volta nelle righe c'e' un vettore di valori
      per cui devo allocare e copiare                
   */

   /* alloco memoria per contenere le righe del program nella cache
   */
   for(i=0;i<nrow;i++)
   {
      nval = cw->pictograph.program->row[i].nval;
      if( (xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].val=
         (int *)XtCalloc(nval,sizeof(int)))==NULL)
      {
         fprintf(stderr,"XlPictograph: Error in malloc caching values\n");
         return(1);
      }

      memcpy( &xlPictographClassRec.xlpictograph_class.programs[num_program].row[i].val, 
           &cw->pictograph.program->row[i].val, sizeof(int)*nval);

   }

   /* incrementoil contatore della cache
   */
   xlPictographClassRec.xlpictograph_class.num_program++;

   return(0);
}



/* routine di lettura file program .prg
   contenenti le istruzioni di animazione di un pictograph
   Gestisce anche la cache dei program

     formato del file da leggere .prg

      num_prog nrighe
      prog @1 nval ival1 ival2 ival3 ... ivaln #color1 fblink_freq
      prog @2 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
      prog @2 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
      prog @4 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq

   NOTA: il vettore restituito e' terminato con un idvar = -1; 

*/



/***********************************************************************
 * leggi_pict_program
 * legge il file program (.prg) e ne memorizza i dati nella struttura 
 *
 *   formato del file da leggere .prg
 *
 *    num_prog nrighe
 *    prog @1 nval ival1 ival2 ival3 ... ivaln #color1 fblink_freq
 *    prog @2 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
 *    prog @2 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
 *    prog @4 nval ival1 ival2 ival3 ... ivaln #color2 fblink_freq
 *
 * - NB - viene anche associato lo SCREEN per cui questo program e' 
 *        stato caricato
 *  NOTA: il vettore restituito e' terminato con un idvar = -1; 
 ******************************************************+*****************/

static int leggi_pict_program(Widget w,char *nome_pict,XlPictographAnimaz *pict)
{
   unsigned short dummy;
   static char *icon_dir=NULL;
   char nome_file[255];
   char tok[20],idvar[5],colore[20], str_blink_rate[10];
   double blink_rate;
   int nrighe,ret,i,j;
   FILE *fp;
   int loaded;

   if(icon_dir==NULL)
   {
      icon_dir=getenv("LEGOMMI_ICO");
      if(icon_dir==NULL)
         return(1);
   }

   /* il nome del file e' dato dalla risorsa nomeProgram
      con estensione .prg
   */
   sprintf(nome_file,"%s/%s.prg",icon_dir,nome_pict);
   if( !strlen(nome_file) )
   {
      fprintf(stderr,"Error in filename pictograph animation file: %s\n",nome_file);
      return(1);
   }

   /* apertura del file
   */
   if( (fp = fopen(nome_file,"r")) == NULL)
   {
      fprintf(stderr,"Error opening pictograph animation file: %s\n",nome_file);
      return(1);
   }

   /* lettura prima riga (num_prog 5)
   */
   if( !fscanf(fp,"%s %d",tok,&nrighe) )
   {
      fprintf(stderr,"Error reading pictograph animation file: %s\n",nome_file);
      fclose(fp);
      return(1);
   }
   /* verifica del token
   */
   if( strcmp(tok,"num_prog" ) )
   {
     fprintf(stderr,"Error in format pictograph animation file: %s\n",nome_file);
      fclose(fp);
     return(1);
   }

   if( !nrighe )
   {
     fprintf(stderr,"Error in format pictograph animation file: %s\n",nome_file);
      fclose(fp);
     return(1);
   }

#if 0
/* #ifdef MODIFICA*/
   /* alloco memoria per la struttura
   */
   if(((*pict)=(XlPictographAnimaz *)XtCalloc(1,sizeof(XlPictographAnimaz)))==NULL)
   {
      fprintf(stderr,"Error in malloc XlPictographAnimaz\n");
      fclose(fp);
      return(1);
   }
#endif

   /********************************
       - NB -
      assegno il nome e lo SCREEN
   ********************************/

   strcpy(pict->nome_program,nome_pict);
   pict->scr = XtScreen(w);

   /* assegno il numero di righe
   */
   pict->nrow = nrighe;

   /* alloco memoria per le righe dei valori
      una in piu' perche chiudo il vettore con -1
   */
   if( ((pict->row)=(XlPictographAnimazRow *)XtCalloc(nrighe+1,
                               sizeof(XlPictographAnimazRow)))==NULL)
   {
      fprintf(stderr,"Error in malloc XlPictographAnimaz\n");
      fclose(fp);
      return(1);
   }

   /* leggo ogni riga del file
   */
   for(i=0;i<nrighe;i++)
   {
      ret=fscanf(fp,"%s %s %d",tok,idvar,&pict->row[i].nval);
      
/*      printf ("\n\n%3d) ret %d tok <%s> idvar <%s>  nval <%d>\n",i+1,ret,tok,idvar,pict->row[i].nval);  Max */
      
      if( !ret || (ret== EOF))
      {
         fprintf(stderr,"Error reading pictograph animation file: %s line: %d\n",
                             nome_file,i+1);
         fclose(fp);
         return(1);
      }


      /* il token deve essere 'prog'
      */
      if( strcmp(tok,"prog") )
      {
         fprintf(stderr,"Error reading pictograph animation file: %s line: %d\n",
                             nome_file,i+1);
         fclose(fp);
         return(1);
      }


      /* indice zona associata 0,1,2,3...n
      */
      pict->row[i].idvar = atoi(&idvar[1]) -1;

      /* se ci sono valori specificati  li alloco e li leggo
         altrimenti leggo solo il colore
      */
      if( pict->row[i].nval )
      {
         if((pict->row[i].val=(int *)XtCalloc(pict->row[i].nval,sizeof(int)))==NULL)
         {
            fprintf(stderr,"Error in malloc value\n");
            fclose(fp);
            return(1);
         }

         /* leggo la lista di valori
         */
/*         printf ("    ");  Max */
         
         for(j=0;j<pict->row[i].nval;j++)
         {
            ret=fscanf(fp,"%d",&pict->row[i].val[j]) ;
            if( !ret || (ret== EOF))
            {
               fprintf(stderr,"Error reading pictograph animation file: %s line: %d\n",
                             nome_file,i+1);
               fclose(fp);
               return(1);
            }
/*            printf ("(ret %d: %d) ", ret, pict->row[i].val[j]);  Max */
         }
/*         printf ("\n");  Max */
      }

      /* leggo il colore e la blink freq e trasparenza
      */
      ret = fscanf(fp,"%s %s %d",colore, str_blink_rate, pict->row[i].animaz.trasparent);
      blink_rate = (float) atof(str_blink_rate);
      pict->row[i].animaz.blink_rate = blink_rate;
/*      printf("ret %d colore %s blink %f traspare %d\n", ret, colore, blink_rate,
             pict->row[i].animaz.trasparent)  Max */;
      if( !ret || (ret== EOF))
      {
         fprintf(stderr,"Error reading pictograph animation file: %s line: %d\n",
                           nome_file,i+1);
         fclose(fp);
         return(1);
      }


      /* converto il colore in pixel
      */
      XlCvtStrToPixel(XtDisplay(w),colore,&pict->row[i].animaz.colore,&dummy,
                      &dummy,&dummy);

   }

   /***************************************
     termino il vettore con -1 in idvar
     ATT.NE e' importantissimo che il vettore
     sia terminato con -1 sempre perche' 
     le routine di conversione XlValueToBlinkPixel etc
     fanno uno while(idvar != -1)
   *****************************************/

   pict->row[i].idvar = -1;


   fclose(fp);
   return(0);
}


/*
 * Routine di disegno del frame sull'oggetto XlPictograph
 * Il frame viene disegnato quando viene aperta la OperationWindow
 * associata all'oggetto o quando e' attiva la variabile di animazione
 * associata al frame.
*/

static void DrawFrame(Widget w,Boolean blink)
{
   XlPictographWidget cw= (XlPictographWidget) w;
   unsigned int width=cw->core.width;
   unsigned int height=cw->core.height;
   int frameWidth = cw->pictograph.frame.frameWidth;
   GC gc_set=NULL;
 
   /* non faccio niente se sono inconfigurazione
   */
   if(cw->xlcore.config==True)
        return;

   if( (width == 0) || (height == 0) )
      return;

   /* disegno il frame
      x=y=0; spessore frame = configurabile
   */


   /* setto il colore in base al valore della variabile
      che ha priorita' minore rispetto alla owOpen
   */
   if( blink == 2)
      gc_set = cw->pictograph.clear_gc;
   else
   {
      if( cw->pictograph.frame.input.valore )
         gc_set = cw->pictograph.frame.frame_gc;

      /* setto il colore in base alla apertura della operationWindow
      */
      if( cw->pictograph.owOpen )
         gc_set = cw->pictograph.frame.owFrame_gc;

      /* il colore deve essere trasparente se il blink e' off
         oppure se il frame non e' attivo
      */
      if( blink == 1 )
         gc_set = cw->pictograph.clear_gc;
   }

                       
/* se il gc non e' settato in modo diverso
   lo setto come clear
*/
   if( gc_set == NULL )     
     gc_set = cw->pictograph.clear_gc;

   /* disegno il rettangolo rappresentante il frame 
         che deve essere contenuto tutto dentro lo widget per
         cui aggiusto x,y,width,heigth in base allo spessore del frame stesso
   */
   XDrawRectangle(XlDisplay(w),XlWindow(w),gc_set,
                     0+(frameWidth/2),0+(frameWidth/2),
                     width-frameWidth,height-frameWidth);


}

