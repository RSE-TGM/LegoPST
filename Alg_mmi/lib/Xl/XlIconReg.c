/**********************************************************************
*
*       C Source:               XlIconReg.c
*       Subsystem:              1
*       Description:
*       %created_by:    lopez %
*       %date_created:  Tue Jun 11 11:52:45 2002 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlIconReg.c-5.1.2 %  (%full_filespec: XlIconReg.c-5.1.2:csrc:1 %)";
#endif
/*
   modulo XlIconReg.c
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlIconReg.c	5.1
*/
/*
        Variabile per identificazione della versione
*/
static char SccsID[] = "@(#)XlIconReg.c	5.1\t11/13/95";
/*
        Fine sezione per SCCS
*/
/*
 *  XlIconReg.c - widget iconReg per stazioni
 */
#include <Xm/XmP.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Xl/XlP.h>
#include <X11/CoreP.h>
#include <Xl/XlCoreP.h>
#include <X11/CompositeP.h>
#include <Xl/XlIconRegP.h>
#include <Xl/XlPort.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */



/* lista delle risorse  */
static XtResource resources[]= {
        {
        XlNnomeFile,
        XlCNomeFile,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.nome_file_icon),
        XmRImmediate,
        "null"
        },
        {
        XlNiconRegType,
        XlCIconRegType,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIconRegRec,iconReg.iconRegType),
        XmRImmediate,
        (XtPointer)DEFAULT_TYPE_ICONREG
        },
        {
        XlNtopBorder,
        XlCTopBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIconRegRec,iconReg.top_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNbottomBorder,
        XlCBottomBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIconRegRec,iconReg.bottom_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNleftBorder,
        XlCLeftBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIconRegRec,iconReg.left_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNrightBorder,
        XlCRightBorder,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlIconRegRec,iconReg.right_border),
        XmRImmediate,
        (XtPointer)0
        },
        {
        XlNmodulName,
        XlCModulName,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.nome_modulo),
        XmRImmediate,
        "MMMM"
        },
        {
        XlNtagName,
        XlCTagName,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.nome_sigla),
        XmRImmediate,
        "0101"
        },
        {
        XlNiconDescr,
        XlCIconDescr,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.descrizione),
        XmRImmediate,
        ""
        },
        {
        XlNinputValue,
        XlCInputValue,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.inputValue),
        XmRImmediate,
        "no value"
        },
        {
        XlNobjectForm,
        XlCObjectForm,
        XmRPointer,
        sizeof(OlFormObject),
        XtOffsetOf(XlIconRegRec,iconReg.form),
        XmRImmediate,
        (XtPointer)NULL
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].automatico),
        XmRString,
        XlDefaultColoreAutomatico1
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].stimato),
        XmRString,
        XlDefaultColoreStimato1
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione1
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib1
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo1
        },
#endif
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso1
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto1
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto1
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo1
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo1
        },
     {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].altissimo),
        XmRString,
        XlDefaultColoreAltissimo1
        },
        {
        XlNvarInputCambioColore2,
        XlCVarInputCambioColore2,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico2,
        XlCColoreAutomatico2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].automatico),
        XmRString,
        XlDefaultColoreAutomatico2
        },
        {
        XlNcoloreStimato2,
        XlCColoreStimato2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].stimato),
        XmRString,
        XlDefaultColoreStimato2
        },
        {
        XlNcoloreFuoriScansione2,
        XlCColoreFuoriScansione2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione2
        },
        {
        XlNcoloreFuoriAttendib2,
        XlCColoreFuoriAttendib2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib2
        },
        {
        XlNcoloreDigSet2,
        XlCColoreDigSet2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo2,
        XlCValoreBassissimo2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo2
        },
#endif
        {
        XlNcoloreBassissimo2,
        XlCColoreBassissimo2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso2,
        XlCValoreBassissimoBasso2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso2
        },
#endif
        {
        XlNcoloreBassissimoBasso2,
        XlCColoreBassissimoBasso2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto2,
        XlCValoreBassoAlto2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto2
        },
#endif
        {
        XlNcoloreBassoAlto2,
        XlCColoreBassoAlto2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto2
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo2,
        XlCValoreAltoAltissimo2,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo2
        },
#endif
        {
        XlNcoloreAltoAltissimo2,
        XlCColoreAltoAltissimo2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo2
        },
        {
        XlNcoloreAltissimo2,
        XlCColoreAltissimo2,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].altissimo),
        XmRString,
        XlDefaultColoreAltissimo2
        },
        {
        XlNvarInputCambioColore3,
        XlCVarInputCambioColore3,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico3,
        XlCColoreAutomatico3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].automatico),
        XmRString,
        XlDefaultColoreAutomatico3
        },
        {
        XlNcoloreStimato3,
        XlCColoreStimato3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].stimato),
        XmRString,
        XlDefaultColoreStimato3
        },
        {
        XlNcoloreFuoriScansione3,
        XlCColoreFuoriScansione3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione3
        },
        {
        XlNcoloreFuoriAttendib3,
        XlCColoreFuoriAttendib3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib3
        },
        {
        XlNcoloreDigSet3,
        XlCColoreDigSet3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo3,
        XlCValoreBassissimo3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo3
        },
#endif
        {
        XlNcoloreBassissimo3,
        XlCColoreBassissimo3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso3,
        XlCValoreBassissimoBasso3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso3
        },
#endif
        {
        XlNcoloreBassissimoBasso3,
        XlCColoreBassissimoBasso3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto3,
        XlCValoreBassoAlto3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto3
        },
#endif
        {
        XlNcoloreBassoAlto3,
        XlCColoreBassoAlto3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto3
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo3,
        XlCValoreAltoAltissimo3,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo3
        },
#endif
        {
        XlNcoloreAltoAltissimo3,
        XlCColoreAltoAltissimo3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo3
        },
        {
        XlNcoloreAltissimo3,
        XlCColoreAltissimo3,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].altissimo),
        XmRString,
        XlDefaultColoreAltissimo3
        },
        {
        XlNvarInputCambioColore4,
        XlCVarInputCambioColore4,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].str_input),
        XmRImmediate,
        ""
        },
        {
        XlNcoloreAutomatico4,
        XlCColoreAutomatico4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].automatico),
        XmRString,
        XlDefaultColoreAutomatico4
        },
        {
        XlNcoloreStimato4,
        XlCColoreStimato4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].stimato),
        XmRString,
        XlDefaultColoreStimato4
        },
        {
        XlNcoloreFuoriScansione4,
        XlCColoreFuoriScansione4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].fuoriScansione),
        XmRString,
        XlDefaultColoreFuoriScansione4
        },
        {
        XlNcoloreFuoriAttendib4,
        XlCColoreFuoriAttendib4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].fuoriAttendib),
        XmRString,
        XlDefaultColoreFuoriAttendib4
        },
        {
        XlNcoloreDigSet4,
        XlCColoreDigSet4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].digitaleOn),
        XmRString,
        XlDefaultColoreDigSet4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo4,
        XlCValoreBassissimo4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo_val),
        XmRString,
        XlDefaultValoreBassissimo4
        },
#endif
        {
        XlNcoloreBassissimo4,
        XlCColoreBassissimo4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo),
        XmRString,
        XlDefaultColoreBassissimo4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso4,
        XlCValoreBassissimoBasso4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo_basso_val),
        XmRString,
        XlDefaultValoreBassissimoBasso4
        },
#endif
        {
        XlNcoloreBassissimoBasso4,
        XlCColoreBassissimoBasso4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo_basso),
        XmRString,
        XlDefaultColoreBassissimoBasso4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto4,
        XlCValoreBassoAlto4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].basso_alto_val),
        XmRString,
        XlDefaultValoreBassoAlto4
        },
#endif
        {
        XlNcoloreBassoAlto4,
        XlCColoreBassoAlto4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].basso_alto),
        XmRString,
        XlDefaultColoreBassoAlto4
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo4,
        XlCValoreAltoAltissimo4,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].alto_altissimo_val),
        XmRString,
        XlDefaultValoreAltoAltissimo4
        },
#endif
        {
        XlNcoloreAltoAltissimo4,
        XlCColoreAltoAltissimo4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].alto_altissimo),
        XmRString,
        XlDefaultColoreAltoAltissimo4
        },
        {
        XlNcoloreAltissimo4,
        XlCColoreAltissimo4,
        XmRPixel,
        sizeof(Pixel),
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].altissimo),
        XmRString,
        XlDefaultColoreAltissimo4
        },
        };

/*
 definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[] = {
        {
        XlNnomeFile,
        XlCNomeFile,
        XlDnomeFile,
        XlRFile+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.nome_file_icon),
        sizeof(char*)
        },
        {
        XlNiconRegType,
        XlCIconRegType,
        XlDiconRegType,
        XlROption+LOCKED_PAG,
        NULL,
        XlOiconRegType,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.iconRegType),
        sizeof(int)
        },
        {
        XlNtopBorder,
        XlCTopBorder,
        XlDtopBorder,
        XlRInt+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.top_border),
        sizeof(int)
        },
        {
        XlNbottomBorder,
        XlCBottomBorder,
        XlDbottomBorder,
        XlRInt+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.bottom_border),
        sizeof(int)
        },
        {
        XlNleftBorder,
        XlCLeftBorder,
        XlDleftBorder,
        XlRInt+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.left_border),
        sizeof(int)
        },
        {
        XlNrightBorder,
        XlCRightBorder,
        XlDrightBorder,
        XlRInt+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.right_border),
        sizeof(int)
        },
        {
        XlNrotate,
        XlCRotate,
        XlDrotate,
        XlROption,
        NULL,
        XlOrotate,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,xlcomposite.rotate),
        sizeof(int),
        },
        {
        XlNassRotate,
        XlCAssRotate,
        XlDassRotate,
        XlROption,
        NULL,
        XlOassRotate,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,xlcomposite.ass_rotate),
        sizeof(int),
        },
        {
        XlNmodulName,
        XlCModulName,
        XlDmodulName,
        XlRForm+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.nome_modulo),
        sizeof(char*)
        },
        {
        XlNtagName,
        XlCTagName,
        XlDtagName,
        XlRText+LOCKED_PAG_LIB,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.nome_sigla),
        sizeof(char*)
        },
        {
        XlNiconDescr,
        XlCIconDescr,
        XlDiconDescr,
        XlRText,  
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.descrizione),
        sizeof(char*)
        },
        {
        XlNinputValue,
        XlCInputValue,
        XlDinputValue,
        XlRInputValue,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlIconRegRec,iconReg.inputValue),
        sizeof(char*)
        },
        {
        XlNvarInputCambioColore1,
        XlCVarInputCambioColore1,
        XlDvarInputCambioColore1,
        XlRTipoVarReg+LOCKED_PAG,
        XlRVarXX,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico1,
        XlCColoreAutomatico1,
        XlDcoloreAutomatico1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato1,
        XlCColoreStimato1,
        XlDcoloreStimato1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione1,
        XlCColoreFuoriScansione1,
        XlDcoloreFuoriScansione1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib1,
        XlCColoreFuoriAttendib1,
        XlDcoloreFuoriAttendib1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet1,
        XlCColoreDigSet1,
        XlDcoloreDigSet1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo1,
        XlCColoreBassissimo1,
        XlDcoloreBassissimo1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo1,
        XlCValoreBassissimo1,
        XlDvaloreBassissimo1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso1,
        XlCColoreBassissimoBasso1,
        XlDcoloreBassissimoBasso1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso1,
        XlCValoreBassissimoBasso1,
        XlDvaloreBassissimoBasso1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto1,
        XlCColoreBassoAlto1,
        XlDcoloreBassoAlto1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto1,
        XlCValoreBassoAlto1,
        XlDvaloreBassoAlto1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo1,
        XlCColoreAltoAltissimo1,
        XlDcoloreAltoAltissimo1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo1,
        XlCValoreAltoAltissimo1,
        XlDvaloreAltoAltissimo1,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo1,
        XlCColoreAltissimo1,
        XlDcoloreAltissimo1,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC1,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[0].altissimo),
        sizeof(Pixel)
        },
        {
        XlNvarInputCambioColore2,
        XlCVarInputCambioColore2,
        XlDvarInputCambioColore2,
        XlRTipoVarReg+LOCKED_PAG,
        XlRVarXX,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico2,
        XlCColoreAutomatico2,
        XlDcoloreAutomatico2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato2,
        XlCColoreStimato2,
        XlDcoloreStimato2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione2,
        XlCColoreFuoriScansione2,
        XlDcoloreFuoriScansione2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib2,
        XlCColoreFuoriAttendib2,
        XlDcoloreFuoriAttendib2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet2,
        XlCColoreDigSet2,
        XlDcoloreDigSet2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo2,
        XlCColoreBassissimo2,
        XlDcoloreBassissimo2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo2,
        XlCValoreBassissimo2,
        XlDvaloreBassissimo2,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso2,
        XlCColoreBassissimoBasso2,
        XlDcoloreBassissimoBasso2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso2,
        XlCValoreBassissimoBasso2,
        XlDvaloreBassissimoBasso2,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto2,
        XlCColoreBassoAlto2,
        XlDcoloreBassoAlto2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto2,
        XlCValoreBassoAlto2,
        XlDvaloreBassoAlto2,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo2,
        XlCColoreAltoAltissimo2,
        XlDcoloreAltoAltissimo2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo2,
        XlCValoreAltoAltissimo2,
        XlDvaloreAltoAltissimo2,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo2,
        XlCColoreAltissimo2,
        XlDcoloreAltissimo2,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC2,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[1].altissimo),
        sizeof(Pixel)
        },
        {
        XlNvarInputCambioColore3,
        XlCVarInputCambioColore3,
        XlDvarInputCambioColore3,
        XlRTipoVarReg+LOCKED_PAG,
        XlRVarXX,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico3,
        XlCColoreAutomatico3,
        XlDcoloreAutomatico3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato3,
        XlCColoreStimato3,
        XlDcoloreStimato3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione3,
        XlCColoreFuoriScansione3,
        XlDcoloreFuoriScansione3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib3,
        XlCColoreFuoriAttendib3,
        XlDcoloreFuoriAttendib3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet3,
        XlCColoreDigSet3,
        XlDcoloreDigSet3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo3,
        XlCColoreBassissimo3,
        XlDcoloreBassissimo3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo3,
        XlCValoreBassissimo3,
        XlDvaloreBassissimo3,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso3,
        XlCColoreBassissimoBasso3,
        XlDcoloreBassissimoBasso3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso3,
        XlCValoreBassissimoBasso3,
        XlDvaloreBassissimoBasso3,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto3,
        XlCColoreBassoAlto3,
        XlDcoloreBassoAlto3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto3,
        XlCValoreBassoAlto3,
        XlDvaloreBassoAlto3,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo3,
        XlCColoreAltoAltissimo3,
        XlDcoloreAltoAltissimo3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo3,
        XlCValoreAltoAltissimo3,
        XlDvaloreAltoAltissimo3,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo3,
        XlCColoreAltissimo3,
        XlDcoloreAltissimo3,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC3,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[2].altissimo),
        sizeof(Pixel)
        },
        {
        XlNvarInputCambioColore4,
        XlCVarInputCambioColore4,
        XlDvarInputCambioColore4,
        XlRTipoVarReg+LOCKED_PAG,
        XlRVarXX,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].input),
        sizeof(PUNT_VARINP)
        },
        {
        XlNcoloreAutomatico4,
        XlCColoreAutomatico4,
        XlDcoloreAutomatico4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].automatico),
        sizeof(Pixel)
        },
        {
        XlNcoloreStimato4,
        XlCColoreStimato4,
        XlDcoloreStimato4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].stimato),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriScansione4,
        XlCColoreFuoriScansione4,
        XlDcoloreFuoriScansione4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].fuoriScansione),
        sizeof(Pixel)
        },
        {
        XlNcoloreFuoriAttendib4,
        XlCColoreFuoriAttendib4,
        XlDcoloreFuoriAttendib4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].fuoriAttendib),
        sizeof(Pixel)
        },
        {
        XlNcoloreDigSet4,
        XlCColoreDigSet4,
        XlDcoloreDigSet4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].digitaleOn),
        sizeof(Pixel)
        },
        {
        XlNcoloreBassissimo4,
        XlCColoreBassissimo4,
        XlDcoloreBassissimo4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimo4,
        XlCValoreBassissimo4,
        XlDvaloreBassissimo4,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassissimoBasso4,
        XlCColoreBassissimoBasso4,
        XlDcoloreBassissimoBasso4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo_basso),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassissimoBasso4,
        XlCValoreBassissimoBasso4,
        XlDvaloreBassissimoBasso4,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].bassissimo_basso_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreBassoAlto4,
        XlCColoreBassoAlto4,
        XlDcoloreBassoAlto4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].basso_alto),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreBassoAlto4,
        XlCValoreBassoAlto4,
        XlDvaloreBassoAlto4,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].basso_alto_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltoAltissimo4,
        XlCColoreAltoAltissimo4,
        XlDcoloreAltoAltissimo4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].alto_altissimo),
        sizeof(Pixel)
        },
#ifndef SCADA_MMI
        {
        XlNvaloreAltoAltissimo4,
        XlCValoreAltoAltissimo4,
        XlDvaloreAltoAltissimo4,
        XlRFloat+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].alto_altissimo_val),
        sizeof(float)
        },
#endif
        {
        XlNcoloreAltissimo4,
        XlCColoreAltissimo4,
        XlDcoloreAltissimo4,
        XlRColor+LOCKED_PAG,
        NULL,
        NULL,
        XlRGrVCC4,
        XtOffsetOf(XlIconRegRec,iconReg.cambio_colore[3].altissimo),
        sizeof(Pixel)
        },
	};


         
/* dichiarazione dei metodi (methods) */
static void Initialize();
static void Destroy();
static void Resize();
static Boolean SetValues();
static Boolean Refresh();
static void Redisplay();
static Boolean Compile();
static XtGeometryResult GeometryManager();
static void InsertChild();
static void DeleteChild();
static Boolean CheckIconReg();
#if defined XPRINTER_USED
static Boolean Print();
#endif

/* dichiarazione di funzioni varie */
static int EsisteBck();
static BCK_OBJ *InsertBck();
static void LoadIcon();
static OlFormObject CaricaForm();


/* actions del widget XlIconReg */
static void Seleziona();
static void AddSelez();
static void MoveTasti();


/* translations  */
static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        <KeyPress>Left:move_tasti()\n\
        <KeyPress>Right:move_tasti()\n\
        <KeyPress>Up:move_tasti()\n\
        <KeyPress>Down:move_tasti()\n\
        Shift<Btn1Down>:       AddSelez()";

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"move_tasti",MoveTasti},
        {"AddSelez",AddSelez}
        };


#define superclass (&xlCompositeClassRec)
#define supersuperclass (&xlManagerClassRec)

#define expcoreclass (&xlCoreClassRec)


/* Inizializzazione del class record */
XlIconRegClassRec xlIconRegClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlCompositeClassRec,
    /* class_name               */      "IconReg",
    /* widget_size              */      sizeof(XlIconRegRec),
    /* class_initialize         */      NULL,
    /* class_part_initialize    */      NULL,
    /* class_inited             */      FALSE,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      XtInheritRealize,
    /* actions                  */      actions,
    /* num_actions              */      XtNumber(actions),
    /* resources                */      resources,
    /* num_resources            */      XtNumber(resources),
    /* xrm_class                */      NULLQUARK,
    /* compress_motion          */      TRUE,
    /* compress_exposure        */      XtExposeCompressMultiple, /* TRUE*/
    /* compress_enterleave      */      TRUE,
    /* visible_interest         */      FALSE,
    /* destroy                  */      Destroy,
    /* resize                   */      Resize,  /* XtInheritResize, */
    /* expose                   */      Redisplay/*XtInheritExpose */,
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
  { /* Constraint class fields   */
    /* geometry_manager          */ 	GeometryManager,
    /* change_managed            */     XtInheritChangeManaged,
    /* insert_child              */     InsertChild,
    /* delete_child              */     DeleteChild,
    /* extension                 */     NULL
  },
  { /* legomanager fields                           */
    /* oggetto pert             */      NULL,
    /* oggetto database         */      NULL,
    /* oggetto database_topologia */    NULL,
    /* oggetto compilatore      */      NULL,
    /* informazioni per la configurazione */
    /* confinfo                 */      confinfo,
    /* num_confinfo             */      XtNumber(confinfo),
    /* refresh                  */      Refresh,
    /* compile                  */      Compile,
    /* read                     */      XtInheritRead,
    /* write                    */      XtInheritWrite,
    /* select                   */      XtInheritSelect,
    /* drawBorderIfSelected     */      XtInheritDrawBorderIfSelected,
    /* selectRunTime            */      NULL,
#if defined XPRINTER_USED
    /* print                    */      Print,
#endif
  },
  { /* XlComposite fields */
    /* empty                    */      0
  },
  { /* XlIconReg fields */
    /* num_icone                    */   0,
    /* sfondi                    */      NULL,
    /* num_form                  */      0,
    /* elencoForm                */      NULL,
    /* checkIconReg              */      CheckIconReg
  }
};

WidgetClass xlIconRegWidgetClass = (WidgetClass) &xlIconRegClassRec;




static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request;
XtWidgetGeometry *reply;
{
return((*superclass->composite_class.geometry_manager)(w, request, reply));
}

static void InsertChild(w,args,num_args)
Widget w;
ArgList args;
Cardinal *num_args;
{
((*superclass->composite_class.insert_child)(w));
}

static void DeleteChild(w,args,num_args)
Widget w;
ArgList args;
Cardinal *num_args;
{
((*superclass->composite_class.delete_child)(w));
}


static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{
int width,height;
Arg arg[5];
Pixel back_parent;
XrmDatabase db;
char appo[50];




XlIconRegWidget new = (XlIconRegWidget)tnew;


/*
        inizializzo a -1 il campo sfondo la region e i campi OldPixel...
        per il refresh
*/
new->iconReg.sfondo= -1;
new->iconReg.region=NULL;
new->iconReg.OldPixel1= -1;
new->iconReg.OldPixel2= -1;
new->iconReg.OldPixel3= -1;
new->iconReg.OldPixel4= -1;

/*
	impongo a zero il border_width e il campo children_layout a uno
*/
new->core.border_width=0;
new->xlcomposite.childrenLayout=1;


LoadIcon(new,&width,&height,new->iconReg.top_border,new->iconReg.bottom_border,
	new->iconReg.left_border,new->iconReg.right_border);
if(width>0 && height >0)
        {
	new->core.width = width +1 ;
	new->core.height = height +1 ;
	if((new->xlcomposite.rotate==ROTATE_90)||
                (new->xlcomposite.rotate==ROTATE_270))
                        {
			new->xlmanager.width0 = height +1 ;
			new->xlmanager.height0 = width +1 ;
			}
		else
                        {
			new->xlmanager.width0 = width +1 ;
			new->xlmanager.height0 = height +1 ;
			}
        }

/*
	setta nel db le nuove risorse di width e height
*/
db = XrmGetDatabase(XtDisplay(XtParent(new)));
sprintf(appo,"%d",new->xlmanager.width0);
XlSetResourceByWidget(&db,(Widget)new,XlNwidth0,appo);
sprintf(appo,"%d",new->xlmanager.height0);
XlSetResourceByWidget(&db,(Widget)new,XlNheight0,appo);


/*
	Se l'icona di regolazione e' di tipo rimando il tagName
	e' cablato a NOME_ICONA_RIMANDO
*/
if(new->iconReg.iconRegType != NORMAL_ICONREG)
	{
	XtSetArg(arg[0],XlNtagName,NOME_ICONA_RIMANDO);
	XtSetValues(tnew,arg,1);
	}



/*
	Controllo le dimensioni dei campi:
		nome_modulo 
		nome_sigla
*/
if(strlen(new->iconReg.nome_modulo)>MAX_LUN_NOME_MODULO)
	new->iconReg.nome_modulo[MAX_LUN_NOME_MODULO]=0;
if(strlen(new->iconReg.nome_sigla)>MAX_LUN_NOME_SIGLA)
	new->iconReg.nome_sigla[MAX_LUN_NOME_SIGLA]=0;


/*
	Caricamento della form (oggetto ...)  o associazione ad una form 
	precedentemente caricata
*/
new->iconReg.form=NULL;
if(new->iconReg.iconRegType == NORMAL_ICONREG)
   if((new->iconReg.form=CaricaForm(new->iconReg.nome_modulo))==NULL)
	XlWarning("XlIconReg","Initialize",
             "no form loaded");



}

static void Resize(w)
Widget w;
{
XlIconRegWidget cw= (XlIconRegWidget)w;

(*superclass->core_class.resize) (w);
}

static void MoveTasti(wid,ev,params,num_params)
Widget wid;
XEvent *ev;
String *params;
Cardinal *num_params;
{
KeySym tasto;
Modifiers mod;
int x,y;


x=ev->xkey.x;
y=ev->xkey.y;
tasto=XtGetActionKeysym(ev,&mod);
if(tasto == XK_Left) x -=1;
if(tasto == XK_Right) x +=1;
if(tasto == XK_Up) y -=1;
if(tasto == XK_Down) y +=1;
XWarpPointer(XtDisplay(wid),XtWindow(wid),XtWindow(wid),
                (int)0,(int)0,(unsigned int)0,(unsigned int)0, x,y);
}

/* Flag per modalità connessione interfaccia tra pagine - globale per libXl.a */
Boolean StateInterfaceMode = False;

/* Variabili per gestione del feedback visivo delle connessioni */
static Widget selected_iconreg = NULL;    /* IconReg attualmente selezionata */
static GC green_gc = NULL;                /* GC per il quadratino verde */

/* Funzione per creare il GC verde se non esiste */
static void create_green_gc(Widget w)
{
    if (green_gc == NULL) {
        XGCValues values;
        values.foreground = 0x00FF00; /* Verde brillante */
        values.line_width = 2;
        values.line_style = LineSolid;
        green_gc = XtGetGC(w, GCForeground | GCLineWidth | GCLineStyle, &values);
    }
}

/* Funzione per disegnare il quadratino verde */
static void draw_green_square(Widget w)
{
    if (!XtIsRealized(w)) return;
    
    create_green_gc(w);
    
    /* Disegna un quadratino verde nell'angolo in alto a sinistra */
    XDrawRectangle(XtDisplay(w), XtWindow(w), green_gc, 2, 2, 8, 8);
    XFillRectangle(XtDisplay(w), XtWindow(w), green_gc, 3, 3, 6, 6);
}

/* Funzione per rimuovere il quadratino verde (ridisegnando l'area) */
static void clear_green_square(Widget w)
{
    if (!XtIsRealized(w)) return;
    
    /* Forza il ridisegno dell'area del quadratino */
    XClearArea(XtDisplay(w), XtWindow(w), 2, 2, 8, 8, True);
}

static void Seleziona(w,event,params,num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
// GUAG2025 Nuovo sistema di connessione basta un click sull'icona 
// Iconreg grande per connettere due porte di interfaccia tra gli schemi:
// se siamo in connect mode, si cerca una porta di interfaccia
// tra i figli di questa IconReg e si simula un click su di essa
// per gestire la connessione, altrimenti si prosegue con la selezione normale

/*
 NUOVO SISTEMA: Se siamo in connect mode, gestisci il workflow a due click
 con feedback visivo tramite quadratino verde
*/
if(StateInterfaceMode)
	{
	/* Cerca porte di interfaccia tra i figli di questa IconReg */
	WidgetList children;
	Cardinal num_children;
	int i;
	Boolean has_interface_port = False;
	
	XtVaGetValues(w, XmNchildren, &children, XmNnumChildren, &num_children, NULL);
	
	for(i = 0; i < num_children; i++)
		{
		extern Boolean XlIsInterfacePort(Widget);
		if(XlIsInterfacePort(children[i]))
			{
			has_interface_port = True;
			break;
			}
		}
	
	if(has_interface_port)
		{
		printf("DEBUG: IconReg %s ha porte di interfaccia, selected_iconreg=%p, w=%p\n", 
			   XtName(w), (void*)selected_iconreg, (void*)w);
		
		/* PRIMO CLICK: Se nessuna IconReg è selezionata */
		if(selected_iconreg == NULL)
			{
			/* Seleziona questa IconReg e disegna il quadratino verde */
			selected_iconreg = w;
			draw_green_square(w);
			printf("DEBUG: Primo click - IconReg %s (ptr=%p) selezionata con quadratino verde\n", 
				   XtName(w), (void*)w);
			return; /* Non procedere con la selezione normale */
			}
		/* SECONDO CLICK: Se questa è la stessa IconReg già selezionata */
		else if(selected_iconreg == w)
			{
			/* Deseleziona e rimuovi il quadratino verde */
			clear_green_square(w);
			selected_iconreg = NULL;
			printf("DEBUG: Click sulla stessa IconReg %s - deselezionata, quadratino rimosso\n", XtName(w));
			return; /* Non procedere con la selezione normale */
			}
		/* SECONDO CLICK: Su IconReg diversa - crea la connessione */
		else
			{
			/* Rimuovi i quadratini verdi da entrambe le IconReg */
			clear_green_square(selected_iconreg);
			clear_green_square(w);
			
			printf("DEBUG: Secondo click - connessione tra %s e %s, quadratini rimossi\n", 
				   XtName(selected_iconreg), XtName(w));
			
			/* Trova le porte nelle due IconReg e crea la connessione */
			Widget port1 = NULL, port2 = NULL;
			WidgetList first_children;
			Cardinal first_num_children;
			
			/* Trova la porta nella prima IconReg */
			XtVaGetValues(selected_iconreg, XmNchildren, &first_children, 
						  XmNnumChildren, &first_num_children, NULL);
			for(int j = 0; j < first_num_children; j++)
				{
				if(XlIsInterfacePort(first_children[j]))
					{
					port1 = first_children[j];
					printf("DEBUG: Trovata porta1 %s nella prima IconReg %s (ptr=%p)\n", 
						   XtName(port1), XtName(selected_iconreg), (void*)selected_iconreg);
					break;
					}
				}
			
			/* Trova la porta nella seconda IconReg */
			for(int k = 0; k < num_children; k++)
				{
				if(XlIsInterfacePort(children[k]))
					{
					port2 = children[k];
					printf("DEBUG: Trovata porta2 %s nella seconda IconReg %s (ptr=%p)\n", 
						   XtName(port2), XtName(w), (void*)w);
					break;
					}
				}
			
			/* Se entrambe le porte sono trovate, crea la connessione */
			if(port1 && port2)
				{
				printf("DEBUG: Iniziando connessione tra porte %s e %s\n", XtName(port1), XtName(port2));
				
				/* Prima chiamata: inizia la connessione dalla prima porta */
				XEvent fake_event1;
				String params1[1];
				Cardinal num_params1 = 0;
				
				fake_event1.type = ButtonPress;
				fake_event1.xbutton.button = Button1;
				fake_event1.xbutton.x = 0;
				fake_event1.xbutton.y = 0;
				fake_event1.xbutton.time = CurrentTime;
				fake_event1.xbutton.state = 0;
				fake_event1.xbutton.window = XtWindow(port1);
				
				printf("DEBUG: Chiamando Seleziona sulla prima porta %s\n", XtName(port1));
				XtCallActionProc(port1, "Seleziona", &fake_event1, params1, num_params1);
				
				/* Seconda chiamata: completa la connessione con la seconda porta */
				XEvent fake_event2;
				String params2[1];
				Cardinal num_params2 = 0;
				
				fake_event2.type = ButtonPress;
				fake_event2.xbutton.button = Button1;
				fake_event2.xbutton.x = 0;
				fake_event2.xbutton.y = 0;
				fake_event2.xbutton.time = CurrentTime;
				fake_event2.xbutton.state = 0;
				fake_event2.xbutton.window = XtWindow(port2);
				
				printf("DEBUG: Chiamando Seleziona sulla seconda porta %s\n", XtName(port2));
				XtCallActionProc(port2, "Seleziona", &fake_event2, params2, num_params2);
				}
			
			/* Reset dello stato */
			selected_iconreg = NULL;
			return; /* Non proseguire con selezione normale */
			}
		}
	}

/*
 Comportamento normale: Richiama la routine di select di XlIconReg 
 per la gestione della selezione
*/
(*supersuperclass->xlmanager_class.select) (w,0);
}

static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
/* richiama il metodo di select */
(*supersuperclass->xlmanager_class.select)(w,1);
}


static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
char *app_char;
Arg comp_args[2];
XlIconRegWidget curcw= (XlIconRegWidget) current;
XlIconRegWidget newcw= (XlIconRegWidget) new;
Boolean do_redisplay = False;
WidgetList widget_figli;
Cardinal num_figli;
int i;



if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
        newcw->xlmanager.width0=curcw->xlmanager.width0;

if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
        newcw->xlmanager.height0=curcw->xlmanager.height0;


/*
if((curcw->core.x != newcw->core.x)|| (newcw->core.y=curcw->core.height))
   {
   XtVaGetValues( curcw,XmNnumChildren, &num_figli,
                XmNchildren, &widget_figli, NULL );
   for(i=0;i<num_figli;i++)
	if(XlIsPort(widget_figli[i]))
		XlPortPosizionaLabel(widget_figli[i]);
   }
*/

if(curcw->iconReg.nome_sigla != newcw->iconReg.nome_sigla)
        {
/*	XtFree(curcw->iconReg.nome_sigla); */
        app_char=newcw->iconReg.nome_sigla;
        newcw->iconReg.nome_sigla=
                XtNewString(app_char);
        }

if(curcw->iconReg.cambio_colore[0].str_input != 
	newcw->iconReg.cambio_colore[0].str_input)
        {
/*	free(curcw->iconReg.cambio_colore[0].str_input); */
        app_char=newcw->iconReg.cambio_colore[0].str_input;
        newcw->iconReg.cambio_colore[0].str_input= XtNewString(app_char);
        }
if(curcw->iconReg.cambio_colore[1].str_input != 
	newcw->iconReg.cambio_colore[1].str_input)
        {
/*	free(curcw->iconReg.cambio_colore[1].str_input); */
        app_char=newcw->iconReg.cambio_colore[1].str_input;
        newcw->iconReg.cambio_colore[1].str_input= XtNewString(app_char);
        }
if(curcw->iconReg.cambio_colore[2].str_input != 
	newcw->iconReg.cambio_colore[2].str_input)
        {
/*	free(curcw->iconReg.cambio_colore[2].str_input); */
        app_char=newcw->iconReg.cambio_colore[2].str_input;
        newcw->iconReg.cambio_colore[2].str_input= XtNewString(app_char);
        }
if(curcw->iconReg.cambio_colore[3].str_input != 
	newcw->iconReg.cambio_colore[3].str_input)
        {
/*	free(curcw->iconReg.cambio_colore[3].str_input); */
        app_char=newcw->iconReg.cambio_colore[3].str_input;
        newcw->iconReg.cambio_colore[3].str_input= XtNewString(app_char);
        }

if(curcw->iconReg.descrizione != newcw->iconReg.descrizione)
        {
/*	XtFree(curcw->iconReg.descrizione); */
        app_char=newcw->iconReg.descrizione;
        newcw->iconReg.descrizione= XtNewString(app_char);
        }

if(curcw->iconReg.inputValue != newcw->iconReg.inputValue)
        {
/*	XtFree(curcw->iconReg.inputValue);
*/
        app_char=newcw->iconReg.inputValue;
        newcw->iconReg.inputValue= XtNewString(app_char);
        }

if(curcw->iconReg.nome_file_icon != newcw->iconReg.nome_file_icon)
        {
/*
	XtFree(curcw->iconReg.nome_file_icon);
*/
        app_char=newcw->iconReg.nome_file_icon;
        newcw->iconReg.nome_file_icon= XtNewString(app_char);
        }

if(curcw->iconReg.nome_modulo != newcw->iconReg.nome_modulo)
        {
/*
	XtFree(curcw->iconReg.nome_modulo);
*/
        app_char=newcw->iconReg.nome_modulo;
        newcw->iconReg.nome_modulo= XtNewString(app_char);
	/*
	   Caricamento della form (oggetto ...)  o associazione ad una form 
	   precedentemente caricata
	*/
	newcw->iconReg.form=NULL;
	if(newcw->iconReg.iconRegType == NORMAL_ICONREG)
   	 if((newcw->iconReg.form=CaricaForm(newcw->iconReg.nome_modulo))==NULL)
		XlWarning("XlIconReg","SetValues","no form loaded");
	}

return do_redisplay;
}

static void Destroy(w)
Widget w;
{
XlIconRegWidget cw= (XlIconRegWidget) w;

/*
	se in config libera lo sfondo
*/
/*
if(cw->xlmanager.config)
	libera_lista_bck(cw->iconReg.sfondo);
*/
}

static Boolean Refresh(w)
Widget w;
{
XlIconRegWidget cw= (XlIconRegWidget)w;
int i;
XlCoreClassRec *wclass;
XlManagerClassRec *wclassM;
XExposeEvent event;
Pixel RetPixel1;
Pixel RetPixel2;
Pixel RetPixel3;
Pixel RetPixel4;
int width=cw->core.width;
int height=cw->core.height;
Boolean ret = True;


if(cw->xlmanager.config==True)
        return(True);

if(!XtIsWidget(w))
        {
        XlWarning("XlIconReg","Refresh",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

if(xlIconRegClassRec.xlmanager_class.database==NULL)
        {
        XlWarning("XlIconReg","Refresh","database non definito");
        return(False);
        }

if(!(*supersuperclass->xlmanager_class.refresh) (w))
        {
        XlWarning("XlIconReg","Refresh","errore refresh classe superiore");
	ret = False;
        }

/*
	Refresh dei figli
*/
if(!(*superclass->xlmanager_class.refresh) (w))
        {
        XlWarning("XlIconReg","Refresh","errore refresh classe superiore");
	ret = False;
        }



/*
        Richiamo il metodo di Redisplay di XlCore per la gestione della
        trasparenza e del contorno di selezione dell'oggetto
*/
event.x=0;
event.y=0;
event.width=width;
event.height=height;


RetPixel1=XlFlagToPixel(cw->iconReg.cambio_colore[0]);
RetPixel2=XlFlagToPixel(cw->iconReg.cambio_colore[1]);
RetPixel3=XlFlagToPixel(cw->iconReg.cambio_colore[2]);
RetPixel4=XlFlagToPixel(cw->iconReg.cambio_colore[3]);


if((RetPixel1!=cw->iconReg.OldPixel1)||(RetPixel2!=cw->iconReg.OldPixel2)||
        (RetPixel3!=cw->iconReg.OldPixel3)||(RetPixel4!=cw->iconReg.OldPixel4))
        {
        (*superclass->core_class.expose) (w,&event,NULL);
        /*
                chiamare la routine
                                        disegna_bck
        */
        if(cw->iconReg.sfondo)
                {
                disegna_bck_icon(cw,cw->iconReg.sfondo,
                        RetPixel1,RetPixel2,RetPixel3,RetPixel4,False);
                }
        cw->iconReg.OldPixel1=RetPixel1;
        cw->iconReg.OldPixel2=RetPixel2;
        cw->iconReg.OldPixel3=RetPixel3;
        cw->iconReg.OldPixel4=RetPixel4;
        }

return(ret);
}

static Boolean Compile(w)
Widget w;
{
int i;
XlCoreClassRec *wclass;
XlManagerClassRec *wclassM;
Boolean ret=True;
Boolean errors=False;

XlIconRegWidget cw= (XlIconRegWidget)w;


if(!XtIsWidget(w))
        {
        XlWarning("XlIconReg","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }

XlEvError(cw,False);


if(!XlIsInterfaceIconReg((Widget)cw))
	{
	/* test variabile cambio colore 1 */
	if(XlEsisteCambioColore(cw->iconReg.sfondo,1) &&
	   (strlen(cw->iconReg.cambio_colore[0].str_input)==0))
        	{
        	XlErrComp(w,"Compile","Input variable 1 not defined",NULL);
        	errors=True;
        	XlEvError(cw,True);
        	}
	/* test variabile cambio colore 2 */
	if(XlEsisteCambioColore(cw->iconReg.sfondo,2) &&
	   (strlen(cw->iconReg.cambio_colore[1].str_input)==0))
        	{
        	XlErrComp(w,"Compile","Input variable 2 not defined",NULL);
        	errors=True;
        	XlEvError(cw,True);
        	}
	/* test variabile cambio colore 3 */
	if(XlEsisteCambioColore(cw->iconReg.sfondo,3) &&
	   (strlen(cw->iconReg.cambio_colore[2].str_input)==0))
        	{
        	XlErrComp(w,"Compile","Input variable 3 not defined",NULL);
        	errors=True;
        	XlEvError(cw,True);
        	}
	/* test variabile cambio colore 4 */
	if(XlEsisteCambioColore(cw->iconReg.sfondo,4) &&
	   (strlen(cw->iconReg.cambio_colore[3].str_input)==0))
        	{
        	XlErrComp(w,"Compile","Input variable 4 not defined",NULL);
        	errors=True;
        	XlEvError(cw,True);
        	}
	}


if(xlIconRegClassRec.xlmanager_class.compilatore==NULL)
        {
        XlErrComp(w,"Compile","Compiler not defined",NULL);
        XlEvError(cw,True);
        return(False);
        }

if(!(*supersuperclass->xlmanager_class.compile) (w,
        xlIconRegClassRec.xlmanager_class.compilatore))
        {
        XlErrComp(w,"Compile","Error in upper-class",NULL);
        XlEvError(cw,True);
	ret=False;
        }

/*
	Compila i figli
*/
if(!(*superclass->xlmanager_class.compile) (w,
        xlIconRegClassRec.xlmanager_class.compilatore))
        {
        XlErrComp(w,"Compile","Error in upper-class",NULL);
        XlEvError(cw,True);
	ret=False;
        }
return(ret);
}


static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
XlIconRegWidget cw= (XlIconRegWidget)w;
CoreWidgetClass wclass;
register int x,y;
unsigned int width,height;
XmDrawingAreaCallbackStruct str;
int i;


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
        richiama la routine di expose del padre dell'oggetto
*/
if(cw->xlmanager.trasparent==True)
        {
        event->x+=cw->core.x;
        event->y+=cw->core.y;
        event->width +=2;
        event->height +=2;
        event->window=XtWindow(XtParent(cw));
        str.event=(XEvent*)event;
        str.window=XtWindow(XtParent(cw));
        XtCallCallbacks(XtParent(cw),XmNexposeCallback,&str);
        }
else
	{
	/*
		ridisegna lo sfondo
	*/
/*         per la trasparenza scommentare e aggiungere il Redisplay dei figli

	printf("XFillRectangle [XlIconReg] %d %d %d %d\n",x,y,width,height);
	XFillRectangle(XtDisplay(cw),XtWindow(cw),
		cw->xlmanager.sfondo_gc,x,y,width,height);
*/
	}

if(cw->iconReg.sfondo)
        {
        str.event=(XEvent*)event;
	disegna_bck_icon(cw,cw->iconReg.sfondo,
                        cw->iconReg.OldPixel1,cw->iconReg.OldPixel2,
			cw->iconReg.OldPixel3,cw->iconReg.OldPixel4,False);
        }
/*
         Raise dei figli non XlPort
*/
for(i=0;i<cw->composite.num_children;i++)
	{
        if(!XlIsPort(cw->composite.children[i]))
		XRaiseWindow(XtDisplay(cw->composite.children[i]),
				XtWindow(cw->composite.children[i]));
/*
	wclass=XtClass(cw->composite.children[i]);
        (*wclass->core_class.expose)(cw->composite.children[i],NULL,NULL);
*/
	}

/*
        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*supersuperclass->xlmanager_class.drawBorderIfSelected) (w);

}

      
/*
 XlIsIconReg(w) funzione per sapere se l'oggetto w
 e' del tipo XlIconReg
*/
Boolean XlIsIconReg(Widget w)
{
CoreWidgetClass wclass;
wclass=XtClass(w);
if(strcmp(wclass->core_class.class_name,"IconReg")==0)
	return(True);
else
	return(False);
}

/*
	XlIsInterfaceIconReg(w) funzione per sapere se l'oggetto w
	e' del tipo IconReg di interfaccia
*/
Boolean XlIsInterfaceIconReg(Widget w)
{
XlIconRegWidget cw= (XlIconRegWidget)w;

	if(!XlIsIconReg(w))
		return(False);
        if(cw->iconReg.iconRegType != NORMAL_ICONREG)
		return(True);
	else
		return(False);
}

/*
	XlInterfaceIconRegIsConnected(w) funzione per sapere se
	l'oggetto e' del tipo IconReg di interfaccia ed e' connesso
*/
Boolean XlInterfaceIconRegIsConnected(Widget w)
{
XlIconRegWidget cw= (XlIconRegWidget)w;
WidgetList *wlist;
Cardinal num;
Arg arg[2];
int i;

if(!XlIsIconReg(w))
	return(False);
if(!XlIsInterfaceIconReg(w))
	return(False);
/*
	ricava l'elenco dei figli
*/
XtSetArg(arg[0],XmNnumChildren,&num);
XtSetArg(arg[1],XmNchildren,&wlist);
XtGetValues(w,arg,2);

/*
	cerca le porte di interfaccia e verifica se sono connesse
*/
for(i=0;i<num;i++)
	if(XlIsInterfacePort(wlist[i]))
		if(XlPortIsConnected(wlist[i]))
			return(True);

return(False);
}




static void LoadIcon(w,width,height,top,bottom,left,right)
Widget w;
int *width;
int *height;
int top,bottom,left,right;
{
XlIconRegWidget cw= (XlIconRegWidget)w;
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
int app_width,app_height;

/*
        setto il valore di default per il ritorno
*/
cw->iconReg.sfondo=NULL;

/*
if(!cw->xlmanager.config)
*/
pos_sfondo=EsisteBck(XtParent(w),cw->iconReg.nome_file_icon,
	cw->xlcomposite.rotate,cw->xlcomposite.ass_rotate,
	cw->iconReg.top_border,cw->iconReg.bottom_border,
	cw->iconReg.left_border, cw->iconReg.right_border);
/*
else
        pos_sfondo = -1;
*/



if(pos_sfondo == -1)
        {
        /*
		carico nel vettore dim i bordi
		( top,bottom,left,right )
	*/
	dim[0]= cw->iconReg.top_border;
	dim[1]= cw->iconReg.bottom_border;
	dim[2]= cw->iconReg.left_border;
	dim[3]= cw->iconReg.right_border;
	
        if(!leggi_file_bck(XtParent(w),cw->iconReg.nome_file_icon,
                "ico",&(cw->iconReg.sfondo),dim))
                {
                *width = 100;
                *height = 100;
                set_something_val(cw,XmNborderWidth,(XtArgVal)1);
                return;
                }
/*
	if(!cw->xlmanager.config)
		{
*/
/*		printf("\n INSERIMENTO dello sfondo nella classe\n");
		printf("\n DIM ELEM.GRAF = %d",sizeof(BCK_OBJ)); */
             cw->iconReg.sfondo=InsertBck(XtParent(w),cw->iconReg.sfondo,
			dim,cw->iconReg.nome_file_icon,
      			cw->xlcomposite.rotate,cw->xlcomposite.ass_rotate,
			cw->iconReg.top_border,cw->iconReg.bottom_border,
			cw->iconReg.left_border, cw->iconReg.right_border);
/*
		}
*/
        if(cw->iconReg.sfondo!=NULL)
                {
                *width = (Dimension)dim[2]-dim[0];
                *height = (Dimension)dim[3]-dim[1];
		/*
			sistema i bordi
		*/
		*width = *width + left + right;
		*height = *height + top + bottom;
                }
        else
                {
                *width = 0;
                *height = 0;
                }
        ruota_bck(width,height,cw->iconReg.sfondo,cw->xlcomposite.rotate,
		cw->xlcomposite.ass_rotate,
		cw->iconReg.top_border,cw->iconReg.bottom_border,
		cw->iconReg.left_border,cw->iconReg.right_border);
        }
else
        {
/* printf("\n RECUPERO dello sfondo dalla cache"); */
        cw->iconReg.sfondo= xlIconRegClassRec.xliconReg_class.sfondi[pos_sfondo].sfondo;

        if(cw->iconReg.sfondo!=NULL)
                {
                *width = xlIconRegClassRec.xliconReg_class.sfondi[pos_sfondo].dim[2]- xlIconRegClassRec.xliconReg_class.sfondi[pos_sfondo].dim[0];
                *height = xlIconRegClassRec.xliconReg_class.sfondi[pos_sfondo].dim[3]- xlIconRegClassRec.xliconReg_class.sfondi[pos_sfondo].dim[1];
                }
        else
                {
                *width = 0;
                *height = 0;
                }
	/*
		sistema i bordi
	*/
	*width = *width + left + right;
	*height = *height + top + bottom;
        if((cw->xlcomposite.rotate==ROTATE_90)||(cw->xlcomposite.rotate==ROTATE_270))
                {
                app_int= *width;
                *width = *height;
                *height = app_int;
                }
        }
return;
}


/*
        verifica nella struttura della classe la presenza di uno sfondo
        ritorno:
                posizione: se e' stata trovata
                -1 : se non e' stata trovata
*/
static int EsisteBck(w,nome_icona,rotate,ass_rotate,top,bottom,left,right)
Widget w;
char *nome_icona;
int rotate, ass_rotate,top,bottom,left,right;
{
int num_icone;
int i;

num_icone=xlIconRegClassRec.xliconReg_class.num_icone;

for(i=0;i<num_icone;i++)
        if((strcmp(xlIconRegClassRec.xliconReg_class.sfondi[i].nome_icona,nome_icona)==0)&&
                (rotate == xlIconRegClassRec.xliconReg_class.sfondi[i].rotate)&&
                (ass_rotate == xlIconRegClassRec.xliconReg_class.sfondi[i].ass_rotate) &&
                (top == xlIconRegClassRec.xliconReg_class.sfondi[i].top_border) &&
                (bottom == xlIconRegClassRec.xliconReg_class.sfondi[i].bottom_border) &&
                (left == xlIconRegClassRec.xliconReg_class.sfondi[i].left_border) &&
                (right == xlIconRegClassRec.xliconReg_class.sfondi[i].right_border) &&
                (XtDisplay(w) == xlIconRegClassRec.xliconReg_class.sfondi[i].display))
                return(i);
return(-1);
}
/*
        inserisce un sfondo nella struttura della classe
*/
static BCK_OBJ *InsertBck(w,new_sfondo,dim,nome_icona,rotate,ass_rotate,
	top,bottom,left,right)
Widget w;
BCK_OBJ *new_sfondo;
int dim[4];
char *nome_icona;
int rotate, ass_rotate,top,bottom,left,right;
{
int num_icone;
int size_sfondo;
int num_b=1;

num_icone=xlIconRegClassRec.xliconReg_class.num_icone+1;
xlIconRegClassRec.xliconReg_class.num_icone=num_icone;


if(num_icone==1)
        xlIconRegClassRec.xliconReg_class.sfondi=NULL;

xlIconRegClassRec.xliconReg_class.sfondi= (XlIconRegSfondo*)XtRealloc(
                        xlIconRegClassRec.xliconReg_class.sfondi,
                                sizeof(XlIconRegSfondo)*(num_icone));
/****
while(new_sfondo[num_b-1].tipo>0)
        num_b++;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].sfondo=(BCK_OBJ *)XtMalloc(sizeof(BCK_OBJ)*num_b);
strcpy(xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].nome_icona,nome_icona);
if(new_sfondo!=NULL)
  memcpy(xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].sfondo,new_sfondo,sizeof(BCK_OBJ)*num_b);
*****/
strcpy(xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].nome_icona,nome_icona);
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].sfondo=new_sfondo;
memcpy(xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].dim,dim,4*sizeof(int));
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].rotate=rotate;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].ass_rotate=ass_rotate;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].top_border=top;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].bottom_border=bottom;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].left_border=left;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].right_border=right;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].num_obj=num_b;
xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].display=XtDisplay(w);
return(xlIconRegClassRec.xliconReg_class.sfondi[num_icone-1].sfondo);
}

static OlFormObject CaricaForm(char *nome)
{
OlFormObject ret = NULL;
static char *path_form=NULL;
char  file_form[300];
int k;
int num_form = xlIconRegClassRec.xliconReg_class.num_form;



/*
	Verifico se la form e' presente nella classe
*/
if(num_form != 0)
   {
   for(k=0;k<num_form; k++)
     if(strcmp(nome,
	 xlIconRegClassRec.xliconReg_class.elencoForm[k].nome_form)==0)
	    break;
   if(k<num_form)
      {
      ret=xlIconRegClassRec.xliconReg_class.elencoForm[k].elencoForm;
      return(ret);
      }
   }

/*
	Caricamento form: creazione oggetto OlForm
*/
if(path_form == NULL)
	path_form=XlGetenv("LEGOCAD_USER");

if(path_form == NULL)
	{
	XlWarning("XlIconReg","CaricaForm",
             "LEGOCAD_USER not defined");
	return(ret);
	}

#if defined VMS
path_form[strlen(path_form)-1]=0;
sprintf(file_form,"%s.legocad.libut_reg.libreg]%s.frm",path_form,nome);
#else
sprintf(file_form,"%s/legocad/libut_reg/libreg/%s.frm",path_form,nome);
#endif
ret=OlCreateForm(file_form);

if(ret == NULL)
	return(ret);

/*
	Carico la form nella classe
*/
++ num_form;
xlIconRegClassRec.xliconReg_class.elencoForm=(XlPortElencoForm*)XtRealloc(
			xlIconRegClassRec.xliconReg_class.elencoForm,
			num_form*sizeof(XlPortElencoForm));
strcpy(xlIconRegClassRec.xliconReg_class.elencoForm[num_form-1].nome_form,nome);
xlIconRegClassRec.xliconReg_class.elencoForm[num_form-1].elencoForm=ret;
xlIconRegClassRec.xliconReg_class.num_form = num_form;

/*
fprintf(stderr,"--------------------------------------------\n");
for(k=0;k<num_form; k++)
     fprintf(stderr,"form[%d] -> %s   punt = %d\n",k,
	xlIconRegClassRec.xliconReg_class.elencoForm[k].nome_form,
	xlIconRegClassRec.xliconReg_class.elencoForm[k].elencoForm);
fprintf(stderr,"--------------------------------------------\n");
*/

	

return(ret);
}


/*
Funzione interna per controllo oggetto di regolaz.
*/
static Boolean CheckIconReg(Widget w)
{
XlIconRegWidget cw= (XlIconRegWidget)w;
Boolean ret = True;
WidgetList widget_figli;
Cardinal num_figli,num_porte;
int i;
Widget p[2];


/*
	controllo che esista la form associata per le IconReg non di
	interfaccia e che le porte contenute abbiano associato un nome,
	che siano state assegnate le variabili cambio colore necessarie
	allo sfondo e quindi la consistenza dei figli di tipo porta
	e dispReg
*/
if(cw->iconReg.iconRegType == NORMAL_ICONREG)
   {
   /* test presenza form associata */
   if(cw->iconReg.form==NULL)
        {
        fprintf(stderr,"WARNING: Impossible to associate a form to IconReg\n");
	ret = False;
        }

   /* test variabile cambio colore 1 */
        if(XlEsisteCambioColore(cw->iconReg.sfondo,1) &&
           (strlen(cw->iconReg.cambio_colore[0].str_input)==0))
	ret = False;
   /* test variabile cambio colore 2 */
        if(XlEsisteCambioColore(cw->iconReg.sfondo,2) &&
           (strlen(cw->iconReg.cambio_colore[1].str_input)==0))
	ret = False;
   /* test variabile cambio colore 3 */
        if(XlEsisteCambioColore(cw->iconReg.sfondo,3) &&
           (strlen(cw->iconReg.cambio_colore[2].str_input)==0))
	ret = False;
   /* test variabile cambio colore 4 */
        if(XlEsisteCambioColore(cw->iconReg.sfondo,4) &&
           (strlen(cw->iconReg.cambio_colore[3].str_input)==0))
	ret = False;

   XtVaGetValues( cw,XmNnumChildren, &num_figli,
                XmNchildren, &widget_figli, NULL );
   for(i=0;i<num_figli;i++)
        {
	if(XlIsPort(widget_figli[i]))
		{
		if(!XlCheckPort(widget_figli[i]))
                        {
			ret = False;
                        }
		}
	if(XlIsDispReg(widget_figli[i]))
		{
		if(!XlCheckDispReg(widget_figli[i]))
                        {
                        fprintf(stderr,"WARNING: var CambioColore for DispReg=%s not correctly set\n",XtName(widget_figli[i]));
			ret = False;
                        }
		}
	}
   }
else
   {
   /*
	per le icone di interfaccia verifico che abbiano solo due porte
	(una di ingresso e una di uscita), e che almeno una sia di 
	interfaccia.
   */
   XtVaGetValues( cw,XmNnumChildren, &num_figli,
                XmNchildren, &widget_figli, NULL );
   num_porte = 0;
   for(i=0;i<num_figli;i++)
	if(XlIsPort(widget_figli[i]))
		{
		p[num_porte]=widget_figli[i];
		++ num_porte;
		if (num_porte == 2)
			break;
		}
   if(num_porte!=2)
        {
        fprintf(stderr,"WARNING: Interfaceobj =%s must have  2 ports !!!",XtName(cw));
	ret=False;
        }
   else
	{
   	if(!((XlIsInterfacePort(p[0])&& !XlIsInterfacePort(p[1]))||
		(XlIsInterfacePort(p[1])&& !XlIsInterfacePort(p[0]))))
                        {
                        fprintf(stderr,"WARNING: Interfaceobj =%s: need at least 1 port !!!",XtName(cw));
			ret = False;
                        }

   	if(!((XlPortIsInput(p[0])&& XlPortIsOutput(p[1]))||
		(XlPortIsInput(p[1])&& XlPortIsOutput(p[0]))))
                        {
                        fprintf(stderr,"WARNING: Interfaceobj =%s: need an input and an output port !!!",XtName(cw));
			ret = False;
                        }
	}
   }

XlEvError(cw,!ret);
return(ret);
}

/*
Metodo richiamabile da esterno per effettuare check su oggetto regolaz.
*/
Boolean XlCheckIconReg(Widget w)
{
XlIconRegWidget cw= (XlIconRegWidget)w;

return((*xlIconRegClassRec.xliconReg_class.checkIconReg)(cw));
}

char * XlGetInterfaceIconRegPagConnect(Widget w)
{
XlIconRegWidget cw= (XlIconRegWidget)w;
static char ret[255];
WidgetList widget_figli;
Cardinal num_figli;
int i;
char *app_char;
char *res;

/*
	cerco la porta di interfaccia
*/
ret[0]=0;
XtVaGetValues( cw,XmNnumChildren, &num_figli,
                XmNchildren, &widget_figli, NULL );
for(i=0;i<num_figli;i++)
	if(XlIsInterfacePort(widget_figli[i]))
	   {
	   XtVaGetValues( widget_figli[i],XlNportNameConnect,&app_char,NULL);
	   printf("-----> [%s]\n",app_char);
	   res=XtNewString(app_char);
	   app_char=strstr(res," ");
	   if(app_char == NULL) return(ret);
	   ++ app_char;
	   strcpy(ret,app_char);
	   app_char=strstr(ret," ");
	   *app_char=0;
	   return(ret);
	   }

return(ret);
}

/*
        metodo di stampa
*/
#if defined XPRINTER_USED
static Boolean Print(Widget w)
{
extern Display *printer;
XlIconRegWidget cw= (XlIconRegWidget)w;

/*
	richiama il metodo di stampa del composite
	che stampera' i figli
*/
(*superclass->xlmanager_class.print) (w);

disegna_bck_icon(cw,cw->iconReg.sfondo,
          cw->iconReg.OldPixel1,cw->iconReg.OldPixel2,
	  cw->iconReg.OldPixel3,cw->iconReg.OldPixel4,True);

return(True);
}
#endif
