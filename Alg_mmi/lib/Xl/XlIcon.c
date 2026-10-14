/**********************************************************************
*
*       C Source:               XlIcon.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jan 21 14:32:14 2003 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlIcon.c-3.1.2.2.3 %  (%full_filespec: XlIcon.c-3.1.2.2.3:csrc:1 %)";
#endif
/*
   modulo XlIcon.c
   tipo 
   release 5.2
   data 1/5/96
   reserved @(#)XlIcon.c	5.2
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlIcon.c	5.2\t1/5/96";
/*
        Fine sezione per SCCS
*/
/*
 *  XlIcon.c - widget icon per stazioni
 */
#include <Xm/XmP.h>
#ifndef VMS
#include <X11/Xmu/Xmu.h>
#endif
#include <X11/StringDefs.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Xl/XlCoreP.h>
#include <Xl/XlIconP.h>
#include <Xl/nullo.bmp>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */

#define DEFAULTWIDTH 10
#define DEFAULTHEIGHT 10





/* lista delle risorse  */
static XtResource resources[]= {
	{
        XlNcomponent,
        XlCComponent,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRec,icon.component),
        XmRImmediate,
        ""
        },
	{
        XlNsubsystem,
        XlCSubsystem,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRec,icon.subsystem),
        XmRImmediate,
        ""
        },
	{
        XlNnomeFile,
        XlCNomeFile,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRec,icon.nome_file_icon),
        XmRImmediate,
        "null"
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].automatico),
        XmRString,
        XlDefaultColoreAutomatico1
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].stimato),
        XmRString,
        XlDefaultColoreStimato1
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione1
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib1
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo1
        },
#endif
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso1
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto1
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo1
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo1
        },
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].altissimo),
        XmRString,
        XlDefaultColoreAltissimo1
        },
        {
        XlNvarInputCambioColore2,
        XlCVarInputCambioColore2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico2,
        XlCColoreAutomatico2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].automatico),
        XmRString,
        XlDefaultColoreAutomatico2
        },
        {
        XlNcoloreStimato2,
        XlCColoreStimato2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].stimato),
        XmRString,
        XlDefaultColoreStimato2
        },
        {
        XlNcoloreFuoriScansione2,
        XlCColoreFuoriScansione2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione2
        },
        {
        XlNcoloreFuoriAttendib2,
        XlCColoreFuoriAttendib2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib2
        },
        {
        XlNcoloreDigSet2,
        XlCColoreDigSet2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo2,
        XlCValoreBassissimo2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo2
        },
#endif
        {
        XlNcoloreBassissimo2,
        XlCColoreBassissimo2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso2,
        XlCValoreBassissimoBasso2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso2
        },
#endif
        {
        XlNcoloreBassissimoBasso2,
        XlCColoreBassissimoBasso2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto2,
        XlCValoreBassoAlto2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto2
        },
#endif
        {
        XlNcoloreBassoAlto2,
        XlCColoreBassoAlto2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo2,
        XlCValoreAltoAltissimo2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo2
        },
#endif
        {
        XlNcoloreAltoAltissimo2,
        XlCColoreAltoAltissimo2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo2
        },
        {
        XlNcoloreAltissimo2,
        XlCColoreAltissimo2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].altissimo),
        XmRString,
        XlDefaultColoreAltissimo2
        },
        {
        XlNvarInputCambioColore3,
        XlCVarInputCambioColore3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico3,
        XlCColoreAutomatico3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].automatico),
        XmRString,
        XlDefaultColoreAutomatico3
        },
        {
        XlNcoloreStimato3,
        XlCColoreStimato3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].stimato),
        XmRString,
        XlDefaultColoreStimato3
        },
        {
        XlNcoloreFuoriScansione3,
        XlCColoreFuoriScansione3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione3
        },
        {
        XlNcoloreFuoriAttendib3,
        XlCColoreFuoriAttendib3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib3
        },
        {
        XlNcoloreDigSet3,
        XlCColoreDigSet3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo3,
        XlCValoreBassissimo3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo3
        },
#endif
        {
        XlNcoloreBassissimo3,
        XlCColoreBassissimo3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso3,
        XlCValoreBassissimoBasso3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso3
        },
#endif
        {
        XlNcoloreBassissimoBasso3,
        XlCColoreBassissimoBasso3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto3,
        XlCValoreBassoAlto3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto3
        },
#endif
        {
        XlNcoloreBassoAlto3,
        XlCColoreBassoAlto3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo3,
        XlCValoreAltoAltissimo3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo3
        },
#endif
        {
        XlNcoloreAltoAltissimo3,
        XlCColoreAltoAltissimo3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo3
        },
        {
        XlNcoloreAltissimo3,
        XlCColoreAltissimo3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].altissimo),
        XmRString,
        XlDefaultColoreAltissimo3
        },
        {
        XlNvarInputCambioColore4,
        XlCVarInputCambioColore4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico4,
        XlCColoreAutomatico4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].automatico),
        XmRString,
        XlDefaultColoreAutomatico4
        },
        {
        XlNcoloreStimato4,
        XlCColoreStimato4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].stimato),
        XmRString,
        XlDefaultColoreStimato4
        },
        {
        XlNcoloreFuoriScansione4,
        XlCColoreFuoriScansione4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione4
        },
        {
        XlNcoloreFuoriAttendib4,
        XlCColoreFuoriAttendib4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib4
        },
        {
        XlNcoloreDigSet4,
        XlCColoreDigSet4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo4,
        XlCValoreBassissimo4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo4
        },
#endif
        {
        XlNcoloreBassissimo4,
        XlCColoreBassissimo4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso4,
        XlCValoreBassissimoBasso4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso4
        },
#endif
        {
        XlNcoloreBassissimoBasso4,
        XlCColoreBassissimoBasso4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto4,
        XlCValoreBassoAlto4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto4
        },
#endif
        {
        XlNcoloreBassoAlto4,
        XlCColoreBassoAlto4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo4,
        XlCValoreAltoAltissimo4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo4
        },
#endif
        {
        XlNcoloreAltoAltissimo4,
        XlCColoreAltoAltissimo4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo4
        },
        {
        XlNcoloreAltissimo4,
        XlCColoreAltissimo4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].altissimo),
        XmRString,
        XlDefaultColoreAltissimo4
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
        XtOffsetOf(XlIconRec,xlcore.trasparent),
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
        XtOffsetOf(XlIconRec,icon.component),
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
        XtOffsetOf(XlIconRec,icon.subsystem),
        sizeof(char*)
        },
 	{
        XlNnomeFile,
        XlCNomeFile,
        XlDnomeFile,
        XlRFile,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRec,icon.nome_file_icon),
        sizeof(char*)
        },
	{
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XlDvarInputCambioColore1,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XlDcoloreAutomatico1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].automatico),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XlDcoloreStimato1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].stimato),
        sizeof(Pixel)
        },
#endif
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XlDcoloreFuoriScansione1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XlDcoloreFuoriAttendib1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XlDcoloreDigSet1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XlDcoloreBassissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XlDvaloreBassissimo1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XlDcoloreBassissimoBasso1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XlDvaloreBassissimoBasso1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XlDcoloreBassoAlto1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XlDvaloreBassoAlto1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XlDcoloreAltoAltissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XlDvaloreAltoAltissimo1,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XlDcoloreAltissimo1,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRec,icon.cambio_colore[0].altissimo),
        sizeof(Pixel)
        },
	{
        XlNvarInputCambioColore2,
        XlCVarInputCambioColore2,
        XlDvarInputCambioColore2,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico2,
        XlCColoreAutomatico2,
        XlDcoloreAutomatico2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato2,
        XlCColoreStimato2,
        XlDcoloreStimato2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione2,
        XlCColoreFuoriScansione2,
        XlDcoloreFuoriScansione2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib2,
        XlCColoreFuoriAttendib2,
        XlDcoloreFuoriAttendib2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet2,
        XlCColoreDigSet2,
        XlDcoloreDigSet2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo2,
        XlCColoreBassissimo2,
        XlDcoloreBassissimo2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo2,
        XlCValoreBassissimo2,
        XlDvaloreBassissimo2,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso2,
        XlCColoreBassissimoBasso2,
        XlDcoloreBassissimoBasso2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso2,
        XlCValoreBassissimoBasso2,
        XlDvaloreBassissimoBasso2,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto2,
        XlCColoreBassoAlto2,
        XlDcoloreBassoAlto2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto2,
        XlCValoreBassoAlto2,
        XlDvaloreBassoAlto2,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo2,
        XlCColoreAltoAltissimo2,
        XlDcoloreAltoAltissimo2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo2,
        XlCValoreAltoAltissimo2,
        XlDvaloreAltoAltissimo2,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo2,
        XlCColoreAltissimo2,
        XlDcoloreAltissimo2,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRec,icon.cambio_colore[1].altissimo),
        sizeof(Pixel)
        },
	{
        XlNvarInputCambioColore3,
        XlCVarInputCambioColore3,
        XlDvarInputCambioColore3,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico3,
        XlCColoreAutomatico3,
        XlDcoloreAutomatico3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato3,
        XlCColoreStimato3,
        XlDcoloreStimato3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione3,
        XlCColoreFuoriScansione3,
        XlDcoloreFuoriScansione3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib3,
        XlCColoreFuoriAttendib3,
        XlDcoloreFuoriAttendib3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet3,
        XlCColoreDigSet3,
        XlDcoloreDigSet3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo3,
        XlCColoreBassissimo3,
        XlDcoloreBassissimo3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo3,
        XlCValoreBassissimo3,
        XlDvaloreBassissimo3,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso3,
        XlCColoreBassissimoBasso3,
        XlDcoloreBassissimoBasso3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso3,
        XlCValoreBassissimoBasso3,
        XlDvaloreBassissimoBasso3,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto3,
        XlCColoreBassoAlto3,
        XlDcoloreBassoAlto3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto3,
        XlCValoreBassoAlto3,
        XlDvaloreBassoAlto3,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo3,
        XlCColoreAltoAltissimo3,
        XlDcoloreAltoAltissimo3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo3,
        XlCValoreAltoAltissimo3,
        XlDvaloreAltoAltissimo3,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo3,
        XlCColoreAltissimo3,
        XlDcoloreAltissimo3,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRec,icon.cambio_colore[2].altissimo),
        sizeof(Pixel)
        },
	{
        XlNvarInputCambioColore4,
        XlCVarInputCambioColore4,
        XlDvarInputCambioColore4,
        XlRTipoVarInp,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico4,
        XlCColoreAutomatico4,
        XlDcoloreAutomatico4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato4,
        XlCColoreStimato4,
        XlDcoloreStimato4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione4,
        XlCColoreFuoriScansione4,
        XlDcoloreFuoriScansione4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione4,
        XlCColoreFuoriScansione4,
        XlDcoloreFuoriScansione4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet4,
        XlCColoreDigSet4,
        XlDcoloreDigSet4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo4,
        XlCColoreBassissimo4,
        XlDcoloreBassissimo4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo4,
        XlCValoreBassissimo4,
        XlDvaloreBassissimo4,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso4,
        XlCColoreBassissimoBasso4,
        XlDcoloreBassissimoBasso4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso4,
        XlCValoreBassissimoBasso4,
        XlDvaloreBassissimoBasso4,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto4,
        XlCColoreBassoAlto4,
        XlDcoloreBassoAlto4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto4,
        XlCValoreBassoAlto4,
        XlDvaloreBassoAlto4,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo4,
        XlCColoreAltoAltissimo4,
        XlDcoloreAltoAltissimo4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo4,
        XlCValoreAltoAltissimo4,
        XlDvaloreAltoAltissimo4,
        XlRFloat,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo4,
        XlCColoreAltissimo4,
        XlDcoloreAltissimo4,
        XlRColor,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRec,icon.cambio_colore[3].altissimo),
        sizeof(Pixel)
        },
	};
         
/* dichiarazione dei metodi (methods) */
static void Seleziona();
static void AddSelez();
static void Initialize();
static void Destroy();
static void Redisplay();
static Boolean SetValues();
static Boolean Compile();
static Boolean Refresh();
#if defined XPRINTER_USED && !defined LINUX
static Boolean Print();
#endif

/* dichiarazione di funzioni varie */
static void BlinkProc(Widget, int, Boolean);
static int EsisteBck();
static BCK_OBJ *InsertBck();
static void DrawIcona(Widget, int, Boolean,Pixel,Pixel,Pixel,Pixel);


/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez}
        };

/* Inizializzazione del class record */
XlIconClassRec xlIconClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCoreClassRec,
    /* class_name               */      "Icon",
    /* widget_size              */      sizeof(XlIconRec),
    /* class_initialize         */      NULL,
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
#if defined XPRINTER_USED && !defined LINUX
    /* print                    */      Print,
#endif
  },
  { /* icon fields */
    /* num_icone                    */   0,
    /* sfondi                    */      NULL
  }
};

WidgetClass xlIconWidgetClass = (WidgetClass) &xlIconClassRec;



static void LoadIcon(w,width,height)
Widget w;
int *width;
int *height;
{
XlIconWidget cw= (XlIconWidget)w;
Pixmap pix;
int xh,yh;
int screen_num=DefaultScreen(XtDisplay(w));
int ret;
Arg args[2];
char path_icon[1024];
char *disegni_dir;
FILE *fp;
int dim[4];
int pos_sfondo;
int num_b,app_int;

/*
	setto il valore di default per il ritorno
*/
cw->icon.sfondo=NULL;


if(!cw->xlcore.config)
pos_sfondo=EsisteBck(XtParent(w),cw->icon.nome_file_icon,cw->xlcore.rotate,cw->xlcore.ass_rotate);
else
	pos_sfondo = -1;



if(pos_sfondo == -1)
	{
        /*
                carico nel vettore dim i bordi
                ( top,bottom,left,right )
        */
        dim[0]= 0;
        dim[1]= 0;
        dim[2]= 0;
        dim[3]= 0;
	leggi_file_bck(XtParent(w),cw->icon.nome_file_icon,
		"ico",&(cw->icon.sfondo),dim);
	if(cw->icon.sfondo==NULL)
		{
		*width = 100;
		*height = 100;
		set_something_val(cw,XmNborderWidth,(XtArgVal)1);
		return;
		}
	cw->icon.sfondo=InsertBck(XtParent(w),cw->icon.sfondo,dim,cw->icon.nome_file_icon,
		cw->xlcore.rotate,cw->xlcore.ass_rotate);
	if(cw->icon.sfondo!=NULL)
		{
		*width = dim[2]-dim[0];
		*height = dim[3]-dim[1];
		}
	else
		{
		*width = 0;
		*height = 0;
		}
        ruota_bck(width,height,cw->icon.sfondo,
		cw->xlcore.rotate,cw->xlcore.ass_rotate,0,0,0,0);
	}
else
	{ 
	cw->icon.sfondo= xlIconClassRec.xlicon_class.sfondi[pos_sfondo].sfondo;
        
	if(cw->icon.sfondo!=NULL)
		{
		*width = xlIconClassRec.xlicon_class.sfondi[pos_sfondo].dim[2]-
				xlIconClassRec.xlicon_class.sfondi[pos_sfondo].dim[0];
		*height = xlIconClassRec.xlicon_class.sfondi[pos_sfondo].dim[3]-
				xlIconClassRec.xlicon_class.sfondi[pos_sfondo].dim[1];
		}
	else
		{
		*width = 0;
		*height = 0;
		}
        if((cw->xlcore.rotate==ROTATE_90)||(cw->xlcore.rotate==ROTATE_270))
        	{
        	app_int= *width;
        	*width = *height;
        	*height = app_int;
        	}
	}
return;
}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
Arg arg[5];
XlIconWidget new = (XlIconWidget)tnew;
int width,height;
Pixel back_parent;
int zoom;

/*
	inizializzo a -1 il campo sfondo la region e i campi OldPixel...
	per il refresh
*/
new->icon.sfondo= -1;
new->icon.region=NULL;
new->icon.primo_refresh=1;
new->icon.OldPixel1= -1;
new->icon.OldPixel2= -1;
new->icon.OldPixel3= -1;
new->icon.OldPixel4= -1;


LoadIcon(new,&width,&height);
if(width>0 && height >0)
	{
        if(XlIsXlComposite(XtParent(new)))
                {
                zoom=XlGetZoom(XtParent(new));
                width = width * zoom/100;
                height = height * zoom/100;
                }
	width++;
	height++;
	XtSetArg(arg[0],XmNwidth,width);
	XtSetArg(arg[1],XmNheight,height);
	XtSetValues(tnew,arg,2);
	}

}

#define superclass (&xlCoreClassRec)

static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/*
 Richiama la routine di select di XlCore per la gestione della
 selezione
*/
(*superclass->xlcore_class.select) (w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
(*superclass->xlcore_class.select)(w,1);
}



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
XlIconWidget curcw= (XlIconWidget) current;
XlIconWidget newcw= (XlIconWidget) new;
Boolean do_redisplay = False;


return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlIconWidget cw= (XlIconWidget) w;


/* elimina, se necessario, il widget dalla lista di blink */
if(XlFlagRiIsOn(cw->icon.cambio_colore[0]))
        XlRemoveBlink(cw,0);
if(XlFlagRiIsOn(cw->icon.cambio_colore[1]))
        XlRemoveBlink(cw,1);
if(XlFlagRiIsOn(cw->icon.cambio_colore[2]))
        XlRemoveBlink(cw,2);
if(XlFlagRiIsOn(cw->icon.cambio_colore[3]))
        XlRemoveBlink(cw,3);

}


static Boolean Compile(Widget w)
{
Boolean errors=False;
XlIconWidget cw= (XlIconWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlIcon","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
/* test variabile cambio colore 1 */
if(XlEsisteCambioColore(cw->icon.sfondo,1) &&
      (strlen(cw->icon.cambio_colore[0].str_input)==0))
        {
        XlErrComp(w,"Compile","Input variable 1 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
        }
/* test variabile cambio colore 2 */
if(XlEsisteCambioColore(cw->icon.sfondo,2) &&
      (strlen(cw->icon.cambio_colore[1].str_input)==0))
        {
        XlErrComp(w,"Compile","Input variable 2 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
        }
/* test variabile cambio colore 3 */
if(XlEsisteCambioColore(cw->icon.sfondo,3) &&
      (strlen(cw->icon.cambio_colore[2].str_input)==0))
        {
        XlErrComp(w,"Compile","Input variable 3 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
        }
/* test variabile cambio colore 4 */
if(XlEsisteCambioColore(cw->icon.sfondo,4) &&
      (strlen(cw->icon.cambio_colore[3].str_input)==0))
        {
        XlErrComp(w,"Compile","Input variable 4 not defined",NULL);
        errors=True;
        XlEvError(cw,True);
        }
if(xlIconClassRec.xlcore_class.compilatore==NULL)
        {
	XlErrComp(w,"Compile","Compiler not defined",NULL);
	XlEvError(cw,True);
        return(False);
        }

if(!(*superclass->xlcore_class.compile) (w,
     xlIconClassRec.xlcore_class.compilatore))
        {
	XlErrComp(w,"Compile","Error in upper-class",NULL);
	XlEvError(cw,True);
        return(False);
        }
XlEvError(cw,errors);
return(True);
}

static Boolean Refresh(Widget w)
{
XlIconWidget cw= (XlIconWidget)w;
int width=cw->core.width;
int height=cw->core.height;
int wi,he;
XExposeEvent event;
XmDrawingAreaCallbackStruct str;
Boolean old_blink_0;
Boolean old_blink_1;
Boolean old_blink_2;
Boolean old_blink_3;
Pixel RetPixel1;
Pixel RetPixel2;
Pixel RetPixel3;
Pixel RetPixel4;



if(cw->xlcore.config == True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlIcon","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlIconClassRec.xlcore_class.database==NULL)
        {
        XlWarning("XlIcon","Refresh","database non definito");
        return(False);
        }

if(cw->icon.primo_refresh== 1) /* primo refresh */
        {
        cw->icon.primo_refresh = 0;
        if(XlFlagRiIsOn(cw->icon.cambio_colore[0]))
                                XlAddBlink(cw,0,FREQUENZA_BASE,(void*)BlinkProc);
        if(XlFlagRiIsOn(cw->icon.cambio_colore[1]))
                                XlAddBlink(cw,1,FREQUENZA_BASE,(void*)BlinkProc);
        if(XlFlagRiIsOn(cw->icon.cambio_colore[2]))
                                XlAddBlink(cw,2,FREQUENZA_BASE,(void*)BlinkProc);
        if(XlFlagRiIsOn(cw->icon.cambio_colore[3]))
                                XlAddBlink(cw,3,FREQUENZA_BASE,(void*)BlinkProc);
        }

old_blink_0=XlFlagRiIsOn(cw->icon.cambio_colore[0]);
old_blink_1=XlFlagRiIsOn(cw->icon.cambio_colore[1]);
old_blink_2=XlFlagRiIsOn(cw->icon.cambio_colore[2]);
old_blink_3=XlFlagRiIsOn(cw->icon.cambio_colore[3]);

if(!(*superclass->xlcore_class.refresh) (w))
        {
        XlWarning("XlIcon","Refresh","errore refresh classe superiore");
        return(False);
        }

if(XlFlagRiIsOn(cw->icon.cambio_colore[0]) != old_blink_0)
        {
        if(old_blink_0) XlRemoveBlink(cw,0);
        else XlAddBlink(cw,0,FREQUENZA_BASE,(void*)BlinkProc);
	cw->icon.OldPixel1= -1;
        }
if(XlFlagRiIsOn(cw->icon.cambio_colore[1]) != old_blink_1)
        {
        if(old_blink_1) XlRemoveBlink(cw,1);
        else XlAddBlink(cw,1,FREQUENZA_BASE,(void*)BlinkProc);
	cw->icon.OldPixel2= -1;
        }
if(XlFlagRiIsOn(cw->icon.cambio_colore[2]) != old_blink_2)
        {
        if(old_blink_2) XlRemoveBlink(cw,2);
        else XlAddBlink(cw,2,FREQUENZA_BASE,(void*)BlinkProc);
	cw->icon.OldPixel3= -1;
        }
if(XlFlagRiIsOn(cw->icon.cambio_colore[3]) != old_blink_3)
        {
        if(old_blink_3) XlRemoveBlink(cw,3);
        else XlAddBlink(cw,3,FREQUENZA_BASE,(void*)BlinkProc);
	cw->icon.OldPixel4= -1;
        }

RetPixel1=XlFlagToPixel(cw->icon.cambio_colore[0]);
RetPixel2=XlFlagToPixel(cw->icon.cambio_colore[1]);
RetPixel3=XlFlagToPixel(cw->icon.cambio_colore[2]);
RetPixel4=XlFlagToPixel(cw->icon.cambio_colore[3]);


DrawIcona(w,-1,False,RetPixel1,RetPixel2,RetPixel3,RetPixel4);

return(True);
}

static void DrawIcona(Widget w,int modo,Boolean On,
	Pixel RetPixel1, Pixel RetPixel2, Pixel RetPixel3, Pixel RetPixel4)
{
XlIconWidget cw= (XlIconWidget)w;
int width=cw->core.width;
int height=cw->core.height;
int wi,he;
XExposeEvent event;
Pixel AppPixel1=RetPixel1;
Pixel AppPixel2=RetPixel2;
Pixel AppPixel3=RetPixel3;
Pixel AppPixel4=RetPixel4;

/*
	Richiamo il metodo di Redisplay di XlCore per la gestione della
	trasparenza e del contorno di selezione dell'oggetto
*/
event.x=0;
event.y=0;
event.width=width;
event.height=height;



#if defined BELL_ON
if((modo>=0)&&(On==False))
	XBell(XtDisplay(cw),40);
#endif

if((modo==0)&&(On==True))
	AppPixel1=cw->core.background_pixel;
if((modo==1)&&(On==True))
	AppPixel2=cw->core.background_pixel;
if((modo==2)&&(On==True))
	AppPixel3=cw->core.background_pixel;
if((modo==3)&&(On==True))
	AppPixel4=cw->core.background_pixel;

if((RetPixel1!=cw->icon.OldPixel1)||(RetPixel2!=cw->icon.OldPixel2)||
	(RetPixel3!=cw->icon.OldPixel3)||(RetPixel4!=cw->icon.OldPixel4)||
		(modo>=0))
	{
	if(modo == -1)
		(*superclass->core_class.expose) (w,&event,NULL);
	cw->icon.OldPixel1=RetPixel1;
	cw->icon.OldPixel2=RetPixel2;
	cw->icon.OldPixel3=RetPixel3;
	cw->icon.OldPixel4=RetPixel4;
	/*
		chiamare la routine
					disegna_bck
	*/
	if(cw->icon.sfondo)
		{
		disegna_bck_icon(cw,cw->icon.sfondo,
			AppPixel1,AppPixel2,AppPixel3,AppPixel4,False);
		}
	}
}




static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlIconWidget cw= (XlIconWidget)w;
register int x,y;
unsigned int width,height;
int wi,he;
GC RetGC;
XmDrawingAreaCallbackStruct str;
/*
event->width+=5;
event->height+=5;
*/

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


/*
	Richiamo il metodo di Redisplay di XlCore per la gestione della
	trasparenza e del contorno di selezione dell'oggetto
*/
(*superclass->core_class.expose) (w,event,NULL);
if(cw->icon.sfondo)
	{
	str.event=(XEvent*)event;
	disegna_bck_icon(cw,cw->icon.sfondo,
                        cw->icon.OldPixel1,cw->icon.OldPixel2,
                        cw->icon.OldPixel3,cw->icon.OldPixel4,False);
	}
/*
	Richiamo del metodo di disegno del bordo se l'oggetto
	e' selezionato
*/
(*superclass->xlcore_class.drawBorderIfSelected) (w);
}

/*
	verifica nella struttura della classe la presenza di uno sfondo
	ritorno:
		posizione: se e' stata trovata
		-1 : se non e' stata trovata
*/
static int EsisteBck(w,nome_icona,rotate,ass_rotate)
Widget w;
char *nome_icona;
int rotate, ass_rotate;
{
int num_icone;
int i;

num_icone=xlIconClassRec.xlicon_class.num_icone;

for(i=0;i<num_icone;i++)
	if((strcmp(xlIconClassRec.xlicon_class.sfondi[i].nome_icona,nome_icona)==0)&&
		(rotate == xlIconClassRec.xlicon_class.sfondi[i].rotate)&&
		(ass_rotate == xlIconClassRec.xlicon_class.sfondi[i].ass_rotate)&&
		(XtDisplay(w) == xlIconClassRec.xlicon_class.sfondi[i].display))
		return(i);
return(-1);
}

/*
	inserisce un sfondo nella struttura della classe
*/
static BCK_OBJ *InsertBck(w,new_sfondo,dim,nome_icona,rotate,ass_rotate)
Widget w;
BCK_OBJ *new_sfondo;
int dim[4];
char *nome_icona;
int rotate, ass_rotate;
{
int num_icone;
int size_sfondo;
int num_b=1;

num_icone=xlIconClassRec.xlicon_class.num_icone+1;
xlIconClassRec.xlicon_class.num_icone=num_icone;


if(num_icone==1)
	xlIconClassRec.xlicon_class.sfondi=NULL;

xlIconClassRec.xlicon_class.sfondi= (XlIconSfondo*)XtRealloc(
			xlIconClassRec.xlicon_class.sfondi,
				sizeof(XlIconSfondo)*(num_icone));
while(new_sfondo[num_b-1].tipo>0)
	num_b++;
xlIconClassRec.xlicon_class.sfondi[num_icone-1].sfondo=(BCK_OBJ *)XtMalloc(sizeof(BCK_OBJ)*num_b);
strcpy(xlIconClassRec.xlicon_class.sfondi[num_icone-1].nome_icona,nome_icona);
if(new_sfondo!=NULL)
  memcpy(xlIconClassRec.xlicon_class.sfondi[num_icone-1].sfondo,new_sfondo,sizeof(BCK_OBJ)*num_b);
memcpy(xlIconClassRec.xlicon_class.sfondi[num_icone-1].dim,dim,4*sizeof(int));
xlIconClassRec.xlicon_class.sfondi[num_icone-1].rotate=rotate;
xlIconClassRec.xlicon_class.sfondi[num_icone-1].ass_rotate=ass_rotate;
xlIconClassRec.xlicon_class.sfondi[num_icone-1].num_obj=num_b;
xlIconClassRec.xlicon_class.sfondi[num_icone-1].display=XtDisplay(w);
XtFree(new_sfondo);
return(xlIconClassRec.xlicon_class.sfondi[num_icone-1].sfondo);
}

static void BlinkProc(Widget w,int ind,Boolean On)
{
XlIconWidget cw= (XlIconWidget) w;

	DrawIcona(w,ind,On,cw->icon.OldPixel1,cw->icon.OldPixel2,
		cw->icon.OldPixel3,cw->icon.OldPixel4);
}


/*
        metodo di stampa
*/
#if !defined LINUX
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
XlIconWidget cw= (XlIconWidget)w;
extern Display *printer;

(*superclass->xlcore_class.print) (w);

disegna_bck_icon(cw,cw->icon.sfondo,
          cw->icon.OldPixel1,cw->icon.OldPixel2,
          cw->icon.OldPixel3,cw->icon.OldPixel4,True);
return(True);
}
#endif
#endif
