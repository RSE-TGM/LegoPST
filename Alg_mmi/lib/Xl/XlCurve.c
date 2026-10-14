/*********************************************************************
*
*	C Source:		XlCurve.c
*	Instance:		1
*
*	%created_by:	lomgr %
*	%date_created:	Thu Feb  3 15:04:58 2005 %
*
**********************************************************************/
#ifndef lint
static char *_csrc = "@(#) %filespec: XlCurve.c-4.2.4 %  (%full_filespec: XlCurve.c-4.2.4:csrc:1 %)";
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>

#include "UxXt.h"

#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include <Xm/PushB.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <X11/Shell.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>

#include <Xm/XmP.h>

#include <Xl/XlCurveP.h>
#include <ctype.h>
#include <Xl/XlProto.h>   /* dichiarazioni prima implicite: vedi il file */



#define MIN_WIDTH	5
#define MIN_HEIGHT	5

/* Prototyping funzioni esterne */
extern Boolean RecoveryRangeZoom();
extern int calc_start_pos();


/*
	Lista delle risorse
*/
static XtResource resources[]= {

        {
        XlNtimePeriodKeys,
        XlCTimePeriodKeys,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.timePeriodKeys),
        XtRImmediate,
        (XtPointer) 1,
        },
        {
        XlNvalueRangeKeys,
        XlCValueRangeKeys,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.valueRangeKeys),
        XtRImmediate,
        (XtPointer) 1,
        },
        {
        XlNconfigurationButton,
        XlCConfigurationButton,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.configurationButton),
        XtRImmediate,
        (XtPointer) 1,
        },
        {
        XlNsaveButton,
        XlCSaveButton,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.saveButton),
        XtRImmediate,
        (XtPointer) 1,
        },
        {
        XlNsaveGroupsButton,
        XlCSaveGroupsButton,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.saveGroupsButton),
        XtRImmediate,
        (XtPointer) 1,
        },
        {
        XlNgroupsButton,
        XlCGroupsButton,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.groupsButton),
        XtRImmediate,
        (XtPointer) 1,
        },
        {
        XlNlegend,
        XlCLegend,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.legend),
        XtRImmediate,
        (XtPointer) 1,
        },
        {
        XlNnumCurvesInd,
        XlCNumCurvesInd,
        XtRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.numCurvesInd),
        XtRImmediate,
        (XtPointer) 4,
        },
	{
	XlNcurveFieldColor,
	XlCcurveFieldColor,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.curveFieldColor),
	XmRString,
	"black",
	},
	{
	XlNgridColor,
	XlCgridColor,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.gridColor),
	XmRString,
	"white smoke",
	},
	{
	XlNrefcursColor,
	XlCrefcursColor,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.refcursColor),
	XmRString,
	"white smoke",
	},
	{
	XlNgraphAreaColor,
	XlCgraphAreaColor,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.graphAreaColor),
	XmRString,
	"#a0a050",
	},
	{
	XlNlegendFieldColor,
	XlClegendFieldColor,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.legendColor),
	XmRString,
	"white smoke",
	},
	{
	XlNmainFieldColor,
	XlCmainFieldColor,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.mainColor),
	XmRString,
	"#7d87aa",
	},
	{
	XlNkeysColor,
	XlCkeysColor,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.keysColor),
	XmRString,
	"white smoke",
	},
	{
	XlNfontLegend,
	XlCfontLegend,
	XmRString,
	sizeof(char *),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.legendFont),
	XmRImmediate,
        "fixed",
	},
	{
	XlNtimeAxisFont,
	XlCtimeAxisFont,
	XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.timeAxisFont),
	XmRImmediate,
        "fixed",
	},
	{
	XlNvalueAxisFont,
	XlCvalueAxisFont,
	XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.valueAxisFont),
	XmRImmediate,
	"fixed",
	},
	{
        XlNcurveDataSourceType,
        XlCcurveDataSourceType,
        XtRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.curveSourceType),
        XtRImmediate,
        (XtPointer) 0,
        },
	{
	XlNcurveDataSource,
	XlCcurveDataSource,
	XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.curveSource),
	XtRImmediate,
        "", 
        },
	{
	XlNvar1Info,
	XlCvar1Info,
	XmRString,
	sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[0]),
	XtRImmediate,
	"LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
	},
        {
        XlNvar2Info,
        XlCvar2Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[1]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar3Info,
        XlCvar3Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[2]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar4Info,
        XlCvar4Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[3]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar5Info,
        XlCvar5Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[4]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar6Info,
        XlCvar6Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[5]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar7Info,
        XlCvar7Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[6]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar8Info,
        XlCvar8Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[7]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar9Info,
        XlCvar9Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[8]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNvar10Info,
        XlCvar10Info,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[9]),
        XtRImmediate,
        "LEGOname|KKS|KKSdescr|0.0|1.0|UnitMis",
        },
        {
        XlNfactScaleValue,
        XlCfactScaleValue,
        XmRFloat,
        sizeof(float),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.factScaleValue),
        XtRImmediate,
        (XtPointer)DEFAULT_SCA_VAL,
        },
        {
        XlNrangeTimeZoom,
        XlCrangeTimeZoom,
        XmRString,
        sizeof(char *),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.rangeTimeZoom),
        XtRImmediate,
        "0|0|0|0|0|0|0|0|0|0|",
        },
        {
        XlNrangeTimeStart,
        XlCrangeTimeStart,
        XmRInt,
        sizeof(int),
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.rangeTimeStart),
        XtRImmediate,
	(XtPointer)DEF_TIME_RANGE,
        },
	{
	XlNcurveColor1,
	XlCcurveColor1,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[0]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor2,
	XlCcurveColor2,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[1]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor3,
	XlCcurveColor3,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[2]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor4,
	XlCcurveColor4,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[3]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor5,
	XlCcurveColor5,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[4]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor6,
	XlCcurveColor6,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[5]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor7,
	XlCcurveColor7,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[6]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor8,
	XlCcurveColor8,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[7]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor9,
	XlCcurveColor9,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[8]),
	XmRString,
	"WhiteSmoke",
	},
	{
	XlNcurveColor10,
	XlCcurveColor10,
	XmRPixel,
	sizeof(Pixel),
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[9]),
	XmRString,
	"WhiteSmoke",
	},
};


/*
	Definizione delle confinfo per il configuratore degli oggetti
*/
static XlConfInfo confinfo[]= {
        {
        XlNtimePeriodKeys,
        XlCTimePeriodKeys,
        XlDtimePeriodKeys,
        XlROption,
        NULL,
        XlOtimePeriodKeys,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.timePeriodKeys),
        sizeof(int)
        },
        {
        XlNvalueRangeKeys,
        XlCValueRangeKeys,
        XlDvalueRangeKeys,
        XlROption,
        NULL,
        XlOvalueRangeKeys,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.valueRangeKeys),
        sizeof(int)
        },
        {
        XlNconfigurationButton,
        XlCConfigurationButton,
        XlDconfigurationButton,
        XlROption,
        NULL,
        XlOconfigurationButton,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.configurationButton),
        sizeof(int)
        },
        {
        XlNsaveButton,
        XlCSaveButton,
        XlDsaveButton,
        XlROption,
        NULL,
        XlOsaveButton,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.saveButton),
        sizeof(int)
        },
        {
        XlNsaveGroupsButton,
        XlCSaveGroupsButton,
        XlDsaveGroupsButton,
        XlROption,
        NULL,
        XlOsaveGroupsButton,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.saveGroupsButton),
        sizeof(int)
        },
        {
        XlNgroupsButton,
        XlCGroupsButton,
        XlDgroupsButton,
        XlROption,
        NULL,
        XlOgroupsButton,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.groupsButton),
        sizeof(int)
        },
        {
        XlNlegend,
        XlCLegend,
        XlDlegend,
        XlROption,
        NULL,
        XlOlegend,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.legend),
        sizeof(int)
        },
        {
        XlNnumCurvesInd,
        XlCNumCurvesInd,
        XlDnumCurvesInd,
        XlROption,
        NULL,
        XlOnumCurvesInd,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.numCurvesInd),
        sizeof(int)
        },
	{
	XlNcurveFieldColor,
	XlCcurveFieldColor,
	XlDcurveFieldColor,
	XlRColor,
	NULL,
	NULL,
	XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.curveFieldColor),	
	sizeof(Pixel)
	},
	{
	XlNgridColor,
	XlCgridColor,
	XlDgridColor,
	XlRColor,
	NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.gridColor),
	sizeof(Pixel)
        },
	{
	XlNrefcursColor,
	XlCrefcursColor,
	XlDrefcursColor,
	XlRColor,
	NULL,
        NULL,
        XlRGrColor,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.refcursColor),
	sizeof(Pixel)
        },
	{
	XlNgraphAreaColor,
	XlCgraphAreaColor,
	XlDgraphAreaColor,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.graphAreaColor),
	sizeof(Pixel)
        },
	{
	XlNlegendFieldColor,
	XlClegendFieldColor,
	XlDlegendFieldColor,
	XlRColor,
        NULL,
        NULL,
	XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.legendColor),
	sizeof(Pixel)
	},
	{
	XlNmainFieldColor,
	XlCmainFieldColor,
	XlDmainFieldColor,
	XlRColor,
	NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.mainColor),
	sizeof(Pixel)
	},
	{
	XlNkeysColor,
	XlCkeysColor,
	XlDkeysColor,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.keysColor),
	sizeof(Pixel)
	},
	{
	XlNfontLegend,
	XlCfontLegend,
	XlDfontLegend,
	XlRFont,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.legendFont),
	sizeof(XFontStruct*),
	},
	{
	XlNtimeAxisFont,
	XlCtimeAxisFont,
	XlDtimeAxisFont,
	XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.timeAxisFont),
	sizeof(XFontStruct*),
	},
	{
	XlNvalueAxisFont,
	XlCvalueAxisFont,
	XlDvalueAxisFont,
	XlRFont,
        NULL,
        NULL,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.valueAxisFont),
	sizeof(XFontStruct*),
	},
        {
        XlNcurveDataSourceType,
        XlCcurveDataSourceType,
        XlDcurveDataSourceType,
        XlROption,
        NULL,
        XlOcurveDataSourceType,
        XlRGrIo,
        XtOffsetOf(XlCurveRec,curve.s_XLcurve.curveSourceType),
        sizeof(int)
        },
	{
	XlNcurveDataSource,
	XlCcurveDataSource,
	XlDcurveDataSource,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.curveSource),
	sizeof(char *),
        },
	{
	XlNvar1Info,
	XlCvar1Info,
	XlDvar1Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[0]),
	sizeof(char *),
        },
	{
	XlNvar2Info,
	XlCvar2Info,
	XlDvar2Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[1]),
	sizeof(char *),
        },
	{
	XlNvar3Info,
	XlCvar3Info,
	XlDvar3Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[2]),
	sizeof(char *),
        },
	{
	XlNvar4Info,
	XlCvar4Info,
	XlDvar4Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[3]),
	sizeof(char *),
        },
	{
	XlNvar5Info,
	XlCvar5Info,
	XlDvar5Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[4]),
	sizeof(char *),
        },
	{
	XlNvar6Info,
	XlCvar6Info,
	XlDvar6Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[5]),
	sizeof(char *),
        },
	{
	XlNvar7Info,
	XlCvar7Info,
	XlDvar7Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[6]),
	sizeof(char *),
        },
	{
	XlNvar8Info,
	XlCvar8Info,
	XlDvar8Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[7]),
	sizeof(char *),
        },
	{
	XlNvar9Info,
	XlCvar9Info,
	XlDvar9Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[8]),
	sizeof(char *),
        },
	{
	XlNvar10Info,
	XlCvar10Info,
	XlDvar10Info,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.varInfoStart[9]),
	sizeof(char *),
        },
	{
	XlNfactScaleValue,
	XlCfactScaleValue,
	XlDfactScaleValue,
	XlRFloat,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.factScaleValue),
	sizeof(float),
        },
	{
	XlNrangeTimeZoom,
	XlCrangeTimeZoom,
	XlDrangeTimeZoom,
	XlRText,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.rangeTimeZoom),
	sizeof(char *),
        },
	{
	XlNrangeTimeStart,
	XlCrangeTimeStart,
	XlDrangeTimeStart,
	XlRInt,
        NULL,
        NULL,
        XlRGrIo,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.rangeTimeStart),
	sizeof(int),
        },
	{
	XlNcurveColor1,
	XlCcurveColor1,
	XlDcurveColor1,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[0]),
	sizeof(Pixel),
	},
	{
	XlNcurveColor2,
	XlCcurveColor2,
	XlDcurveColor2,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[1]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor3,
	XlCcurveColor3,
	XlDcurveColor3,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[2]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor4,
	XlCcurveColor4,
	XlDcurveColor4,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[3]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor5,
	XlCcurveColor5,
	XlDcurveColor5,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[4]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor6,
	XlCcurveColor6,
	XlDcurveColor6,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[5]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor7,
	XlCcurveColor7,
	XlDcurveColor7,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[6]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor8,
	XlCcurveColor8,
	XlDcurveColor8,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[7]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor9,
	XlCcurveColor9,
	XlDcurveColor9,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[8]),
	sizeof(Pixel)
	},
	{
	XlNcurveColor10,
	XlCcurveColor10,
	XlDcurveColor10,
	XlRColor,
        NULL,
        NULL,
        XlRGrColor,
	XtOffsetOf(XlCurveRec,curve.s_XLcurve.col_trend[9]),
	sizeof(Pixel)
	},
};


/*
	Dichiarazioni funzioni varie
*/
static Boolean LoadFontTime();
static Boolean LoadFontValue();
static Boolean LoadFontLegend();
static void GetAllGCs();
static void GetGridGC();
static void GetRefCurGC();
static void GetLinesGC();
static void GetTrendGCs();
static void GetTrendFluctGCs();
static void add_translation();
static void SettaActionsFigli();
static void SettaResourceMmi();
static Boolean RecoveryVarInfo();
static Boolean LoadLegoCodStr();
static Boolean LoadDataF22();
static Boolean LoadIndVarSel();
static Boolean alloca_bufdati();
static Boolean alloca_punti();
static Boolean allocaLoadInfo(); 
static Boolean allocaLoadColor(); 
static Boolean allocaLegoCod(); 
static Boolean create_read_stipple();
static void AggDateTime();
static Boolean checkVarInfo();
static Boolean checkRangeZoom();


void prepare_draw();
void AggTimeAxis();
void DrawGridCurve();
Boolean DrawCurves();
void AggLegendValues();
void manageSeldata();
void ResetLegend();
Boolean convStringToPix();
char *convPixToString();
Boolean VerifStrFloat();
Boolean VerifStrInt();
void RefreshXlCurve();


/*
	Dichiarazione dei metodi (methods) 
*/
static void Initialize();
static void Destroy();
static void Resize();
static Boolean SetValues();
static Boolean Refresh();
static void Redisplay();
static Boolean Compile();
static XtGeometryResult GeometryManager();

#if defined XPRINTER_USED
static Boolean Print();
#endif



/*
	Actions del widget
*/
static void Seleziona();
static void AddSelez();
static void DoLayout();
static void PopupInterno();
static void StartRefCurs();
static void MoveRefCurs();
static void StopRefCurs();


#define POPUP_MENU_TRANS "#override <Btn2Down>: PopupInterno(%d)"


/*
	Translations
*/

static XtActionsRec actions[] = {
        {"Seleziona",Seleziona},
        {"AddSelez",AddSelez},
        {"PopupInterno",PopupInterno},
	{"StartRefCurs",StartRefCurs},
	{"MoveRefCurs",MoveRefCurs},
	{"StopRefCurs",StopRefCurs},
        };

static char defaultTranslations[]=
        "~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";


static char configTranslations[]="#replace\n\
        ~Shift<Btn1Down>:     Seleziona()   \n\
        Shift<Btn1Down>:       AddSelez()";

static char refcursTranslation[]="#replace\n\
        <Btn1Down>:     StartRefCurs()\n\
        <Btn1Motion>:   MoveRefCurs()\n\
        <Btn1Up>:       StopRefCurs()";


/*
	Inizializzazione del class record 
*/
XlCurveClassRec xlCurveClassRec = {
  { /* core fields */
    /* superclass               */      (WidgetClass) &xlManagerClassRec,
    /* class_name               */      "Curve",
    /* widget_size              */      sizeof(XlCurveRec),
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
    /* resize                   */      Resize, /* XtInheritResize,*/
    /* expose                   */      Redisplay, /* XtInheritExpose, */
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
    /* geometry_manager          */     GeometryManager, /*XtInheritGeometryManager, */
    /* change_managed            */     XtInheritChangeManaged,
    /* insert_child              */     XtInheritInsertChild,
    /* delete_child              */     XtInheritDeleteChild,
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
  { /* curve fields */
    /* empty                    */      0
  }
};



WidgetClass xlCurveWidgetClass = (WidgetClass) &xlCurveClassRec;




static XtGeometryResult GeometryManager(w, request, reply)
Widget w;
XtWidgetGeometry *request;
XtWidgetGeometry *reply;
{
if(request->request_mode & CWWidth)
        {
        XtResizeWidget(w,(request->width),(w->core.height),(w->core.border_width));
        }
if(request->request_mode & CWHeight)
        {
        XtResizeWidget(w,(w->core.width),(request->height),(w->core.border_width));
        }

return XtGeometryYes;
}




static void Resize(w)
Widget w;
{
XlCurveWidget newcw= (XlCurveWidget)w;
        Arg args[3];
        Cardinal lnargs,numChild;
        Dimension wform,hform;
	Position xform,yform;
        WidgetList childList;

printf("Resize: ENTRATO!!!\n");

        lnargs=0;
        XtSetArg(args[lnargs],XmNnumChildren,&numChild);lnargs++;
        XtSetArg(args[lnargs],XmNchildren,&childList);lnargs++;
        XtGetValues(newcw,args,lnargs);

        lnargs=0;
        wform=newcw->xlmanager.width0-10;
        hform= newcw->xlmanager.height0-10;
	xform=5;
        yform=5;
        XtSetArg(args[lnargs],XmNwidth,wform);lnargs++;
        XtSetArg(args[lnargs],XmNheight,hform);lnargs++;
	XtSetArg(args[lnargs],XmNx,xform);lnargs++;
        XtSetArg(args[lnargs],XmNy,yform);lnargs;
        XtSetValues(childList[0],args,lnargs);
}




static void Initialize(treq,tnew,args,num_args)
Widget treq,tnew;
ArgList args;
Cardinal *num_args;
{

char *argv[10];
Widget main_widget;
WidgetClass Classe;
XmFontList font_time, font_value;
char path_name[MAXPATHNAME],msg[500]; 
Boolean error_path=False;

XlCurveWidget newcw= (XlCurveWidget) tnew;

printf("Initialize: ENTRATO!!!\n");
printf("Initialize: newcw=%d\n",newcw);

/*
   Settaggio dei fonts per l'asse dei valori
*/
   if(!LoadFontValue(newcw,newcw->curve.s_XLcurve.valueAxisFont))
      if(!LoadFontValue(newcw,"fixed"))
         XlError("XlCurve","Initialize","Impossibile caricare default font");
   font_value=newcw->curve.s_XLcurve.font_list_value;
/*
   Settaggio dei fonts per l'asse dei tempi
*/
   if(!LoadFontTime(newcw,newcw->curve.s_XLcurve.timeAxisFont))
      if(!LoadFontTime(newcw,"fixed"))
         XlError("XlCurve","Initialize","Impossibile caricare default font");
   font_time=newcw->curve.s_XLcurve.font_list_time;
/*
   Settaggio dei fonts per la legendarea
*/
   if(newcw->curve.s_XLcurve.legend) {
      if(!LoadFontLegend(newcw,newcw->curve.s_XLcurve.legendFont))
         if(!LoadFontLegend(newcw,"fixed"))
            XlError("XlCurve","Initialize","Impossibile caricare default font");
   }
/*
   Controllo del settaggio della variabile d'ambiente LEGOCAD_ICO.
   Controllo dell'esistenza dei files bitmap per il flooding e l'hatching.
*/
   if(XlGetenv("LEGOCAD_ICO")==NULL) {
      strcpy(msg,"LEGOCAD_ICO not defined!!!");
      XlWarning("XlCurve","Initialize",msg);
      error_path=True;
   }
   else {
      strcpy(path_name,XlGetenv("LEGOCAD_ICO"));
      strcat(path_name,"/dimple1");
      if(access(path_name,F_OK)!=0) {
         sprintf(msg,"File %s not found",path_name);
         XlWarning("XlCurve","Initialize",msg);
         error_path=True;
      }
      else {
         strcpy(newcw->curve.s_XLcurve.flood_name,path_name);
         strcpy(path_name,XlGetenv("LEGOCAD_ICO"));
         strcat(path_name,"/hatch.bmp");
         if(access(path_name,F_OK)!=0) {
            sprintf(msg,"File %s not found",path_name);
            XlWarning("XlCurve","Initialize",msg);
            error_path=True;
         }
         else {
            strcpy(newcw->curve.s_XLcurve.hatch_name,path_name);
         }
      }
   } 
   if(error_path==False) {
/*
   Caricamento del pixmap da utilizzare per l'opzione fluctua.
*/
      if(create_read_stipple(newcw)==True) {
/*
   Impostazione dei GC che verranno utilizzati.
*/
         GetAllGCs(newcw);
/*
   Determinazione del numero di curve configurate a partire dall'indice
   specificato nella risorsa numCurvesInd.
*/
         newcw->curve.s_XLcurve.numCurves=newcw->curve.s_XLcurve.numCurvesInd+1;
/*
   Creazione del layout dell'oggetto XlCurve
*/
         main_widget=create_xlcurve(newcw);
         newcw->curve.contesto=(_UxCtopLevelCurve *)UxGetContext(main_widget);
         main_widget=create_legend(newcw);
         newcw->curve.contesto=(_UxCtopLevelCurve *)UxGetContext(main_widget); 

         SettaActionsFigli(newcw);

         if(newcw->xlmanager.config==False) {
            if(newcw->curve.s_XLcurve.numCurves!=0)
               SettaResourceMmi(newcw);
         }   
      }
   }
   else {
      newcw->curve.s_XLcurve.configur_error=True;
   }

printf("Initialize: USCITO!!!\n");

}

#define superclass (&xlManagerClassRec)



static void Seleziona(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
Widget w_par_lev2,w_par_lev3;


printf("Sono entrato in Seleziona\n");
/*
 Richiama la routine di select di XlCurve per la gestione della
 selezione
*/
if((strcmp(XtName(w),"legendarea")==0) || (strcmp(XtName(w),"graphicarea")==0) ||
	(strcmp(XtName(w),"configur")==0) || (strcmp(XtName(w),"save")==0) ||
	(strcmp(XtName(w),"savegr")==0) || (strcmp(XtName(w),"groups")==0) ) 
	{
	if(XlIsXlManager(XtParent(XtParent(w))))
          (*superclass->xlmanager_class.select) (XtParent(XtParent(w)),0);
        else
          printf("\n NON E' UN MANAGER 1\n");
        }
else  if((strcmp(XtName(w),"changetime")==0)||(strcmp(XtName(w),"changevalue")==0) ||
	(strcmp(XtName(w),"curvefield")==0) || (strcmp(XtName(w),"datetime")==0) ||
	(strcmp(XtName(w),"formvalue")==0) || (strcmp(XtName(w),"formtime")==0) ||
	(strcmp(XtName(w),"rowColDescr")==0) || (strcmp(XtName(w),"rowColLimLow")==0) ||
	(strcmp(XtName(w),"rowColVal")==0) || (strcmp(XtName(w),"rowColLimHigh")==0) ||
	(strcmp(XtName(w),"rowColUnit")==0) || (strcmp(XtName(w),"rowColCursor")==0) ||
	(strcmp(XtName(w),"rowColCod")==0) || (strcmp(XtName(w),"rowColBut")==0) )	
	{
	w_par_lev2=XtParent(w);
	if(XlIsXlManager(XtParent(XtParent(w_par_lev2))))
          (*superclass->xlmanager_class.select) (XtParent(XtParent(w_par_lev2)),0);
        else
          printf("\n NON E' UN MANAGER 2\n");
        }
else  if((strcmp(XtName(w),"timeleft")==0) || (strcmp(XtName(w),"seldata")==0) ||
	(strcmp(XtName(w),"timeright")==0) || (strcmp(XtName(w),"decreasetime")==0) ||
	(strcmp(XtName(w),"abstime")==0) || (strcmp(XtName(w),"flow")==0) ||
	(strcmp(XtName(w),"decreasevalue")==0) || (strcmp(XtName(w),"rangeup")==0) ||
	(strcmp(XtName(w),"defaultset")==0) || (strcmp(XtName(w),"rangedown")==0) ||
	(strcmp(XtName(w),"increasevalue")==0) )	
	{
	w_par_lev3=XtParent(XtParent(w));
	if(XlIsXlManager(XtParent(XtParent(w_par_lev3))))
          (*superclass->xlmanager_class.select) (XtParent(XtParent(w_par_lev3)),0);
        else
          printf("\n NON E' UN MANAGER 3\n");
        }
else if( strcmp(XtName(w),"contenitore1")==0)
        {
        if(XlIsXlManager(XtParent(w)))
                (*superclass->xlmanager_class.select) (XtParent(w),0);
        else
                printf("\n NON E' UN MANAGER 4\n");
        }
else
        {
        if(XlIsXlManager(w))
                (*superclass->xlmanager_class.select) (w,0);
        else
                printf("\n NON E' UN MANAGER 5 e sono %s \n",XtName(w));

        }

}



static void AddSelez(w,event,params,num_params)
Widget w;
XExposeEvent *event;
String *params;
Cardinal *num_params;
{
printf("Sono in AddSelez\n");
/* richiama il metodo di select */
/*if((strcmp(XtName(w),"draw0")==0)||(strcmp(XtName(w),"draw1")==0) ||
        (strcmp(XtName(w),"ord1")==0)||(strcmp(XtName(w),"mis1")==0) ||
        (strcmp(XtName(w),"tim1")==0) || (strcmp(XtName(w),"val1")==0) ||
        (strcmp(XtName(w),"tempo")==0) ||
        (strcmp(XtName(XtParent(w)),"contenitore")==0) )
        (*superclass->xlmanager_class.select) (XtParent(XtParent(w)),1);
else*/
        (*superclass->xlmanager_class.select) (XtParent(w),1);
}



static void PopupInterno(w,event,param,nparam)
Widget w;
XEvent *event;
String *param;
Cardinal *nparam;
{

}



static Boolean SetValues(current,request,new,args,num_args)
Widget current,request,new;
ArgList args;
Cardinal *num_args;
{
Arg comp_args[2];
XlCurveWidget curcw= (XlCurveWidget) current;
XlCurveWidget newcw= (XlCurveWidget) new;
char *app_char;
Boolean do_redisplay = False;

if(curcw->xlmanager.width0!=newcw->xlmanager.width0 ||
        curcw->xlmanager.height0!=newcw->xlmanager.height0)
        {
        Arg args[3];
        Cardinal lnargs,numChild;
        Dimension wform,hform;
        WidgetList childList;
        lnargs=0;
        XtSetArg(args[lnargs],XmNnumChildren,&numChild);lnargs++;
        XtSetArg(args[lnargs],XmNchildren,&childList);lnargs++;
        XtGetValues(newcw,args,lnargs);

        lnargs=0;
        wform=newcw->xlmanager.width0-10;
        hform= newcw->xlmanager.height0-10;
        XtSetArg(args[lnargs],XmNwidth,wform);lnargs++;
        XtSetArg(args[lnargs],XmNheight,hform);lnargs++;
        XtSetValues(childList[0],args,lnargs);
        do_redisplay = True;

        }

/*
        risorsa width
*/
if(curcw->core.width != newcw->core.width)
        {
        if(newcw->core.width<MIN_WIDTH)
                newcw->core.width=MIN_WIDTH;
        newcw->xlmanager.width0=newcw->core.width;
        do_redisplay = True;
        }

/*
        risorsa height
*/
if(curcw->core.height != newcw->core.height)
        {
        if(newcw->core.height<MIN_HEIGHT)
                newcw->core.height=MIN_HEIGHT;
        newcw->xlmanager.height0=newcw->core.height;
        do_redisplay = True;
        }

/*
        risorsa width0
*/
if(curcw->xlmanager.width0 != newcw->xlmanager.width0)
        {
        if(newcw->xlmanager.width0<MIN_WIDTH)
                newcw->xlmanager.width0=MIN_WIDTH;
        newcw->core.width=newcw->xlmanager.width0;
        do_redisplay = True;
        }

/*
        risorsa height0
*/
if(curcw->xlmanager.height0 != newcw->xlmanager.height0)
        {
        if(newcw->xlmanager.height0<MIN_HEIGHT)
                newcw->xlmanager.height0=MIN_HEIGHT;
        newcw->core.height=newcw->xlmanager.height0;
        do_redisplay = True;
        }

return(1);
}



static void Destroy(w)
Widget w;
{
int i;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw= (XlCurveWidget) w;

printf("Destroy: ENTRATO!!!\n");

        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
	if (cw->curve.s_XLcurve.grid_gc)
           XtReleaseGC(cw,cw->curve.s_XLcurve.grid_gc);
	if (cw->curve.s_XLcurve.lines_gc)
	   XtReleaseGC(cw,cw->curve.s_XLcurve.lines_gc);
	for(i=0;i<NUMMAXVAR;i++) {
	   if(cw->curve.s_XLcurve.trends_gc[i])
	      XtReleaseGC(cw,cw->curve.s_XLcurve.trends_gc[i]);
	   if(cw->curve.s_XLcurve.trends_fluct_gc[i])
	      XtReleaseGC(cw,cw->curve.s_XLcurve.trends_fluct_gc[i]);
	}
	if(cw->xlmanager.config==False) {
         if(cw->curve.s_XLcurve.first_refresh==False) {
           if(cw->curve.s_XLcurve.buffer_f22)
	      free(cw->curve.s_XLcurve.buffer_f22);
           if(cw->curve.s_XLcurve.name_desc_var)
	      free(cw->curve.s_XLcurve.name_desc_var);
/*
	   if(refcurs_gc)
	      XtReleaseGC(cw,refcurs_gc);
*/
	   for(i=0;i<NUMMAXVAR;i++) {
              if(cw->curve.s_XLcurve.points_to_draw[i])
	         free(cw->curve.s_XLcurve.points_to_draw[i]);
              if(cw->curve.s_XLcurve.points_fluctua[i])
	         free(cw->curve.s_XLcurve.points_fluctua[i]);
              if(cw->curve.s_XLcurve.varInfoChange[i])
                 free(cw->curve.s_XLcurve.varInfoChange[i]);
              if(cw->curve.s_XLcurve.var_lego_cod[i]) 
                 free(cw->curve.s_XLcurve.var_lego_cod[i]);
           }
	   if(cw->curve.s_XLcurve.legend) {	
              for(i=0;i<cw->curve.s_XLcurve.numCurves;i++)
                 if(cw->curve.s_XLcurve.colTrendString)
                    free(cw->curve.s_XLcurve.colTrendString[i]);
           }
         }
        }
        UxTopLevelCurveContext = UxSaveCtx;

printf("Destroy: USCITO!!!\n");

}



static Boolean Refresh(w)
Widget w;
{
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

/*
printf("Refresh: *****************************\n");
*/
   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);

   if(cw->curve.s_XLcurve.first_refresh==True) {
      if(cw->curve.s_XLcurve.configur_error==False) {
/*
   Configurazione variabili corretta.
*/
         if(cw->curve.s_XLcurve.curveSourceType==ARCH_CIRC_XLCURVE) {
/*
	Recupero delle dimensioni della drawing area 'curvefield' e caricamento
	in due variabili globali e calcolo della posizione di partenza
        della cursor line e reference line.
*/
            get_something(curvefield, XmNwidth, (void*) &width_curvefield);
            get_something(curvefield, XmNheight, (void*) &height_curvefield);
	    curs_pos_pixel=(int)width_curvefield*(cursor_line_pos/100.0);
	    refer_pos_pixel=(int)height_curvefield*(1.0-(ref_line_pos/100.0));

            cw->curve.s_XLcurve.rangeTimeDef=cw->curve.s_XLcurve.rangeTimeStart;
/*
printf("width_curvefield = %d\t\theight_curvefield = %d\n",
       width_curvefield,height_curvefield);
*/
/*
   Allocazione strutture dinamiche.
*/
            GetRefCurGC(w);
            if(alloca_punti(cw)==False)
               return(False);
	    if(allocaLoadInfo(cw)==False)
               return(False);
	    if(cw->curve.s_XLcurve.legend) {	
               if(allocaLoadColor(cw)==False)
                  return(False);
            }
            if(allocaLegoCod(cw)==False)
               return(False);
	    SettingVarInfo(cw);
/*
printf("Refresh: sono in ARCH_CIRC_XLCURVE e first_refresh!!!\n");
*/
	    if(LoadDataF22(cw,TUTTI_CAMP)==False)
               return(False);
	    cw->curve.s_XLcurve.first_refresh=False;
            RefreshXlCurve(w);
	 }
         else {
	    printf("Caso RUN_TIME non ancora gestito!!!\n");
            return(False);
         }
      }
      else {
         printf("Errore di configurazione!!!\n");
         return(False);
      }
   }
   else {
/*
   Refresh successivi!!!
*/
      if(static_status==False) {
         if(LoadDataF22(cw,AGGIORNA_CAMP)==False)
            return(False);
         RefreshXlCurve(w);
      }
   }
   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("Refresh: *****************************\n");
*/
return(True);
}



static Boolean Compile(Widget w)
{
int i,ind_var_err;
char msg[400];
Boolean errors=False;

XlCurveWidget cw= (XlCurveWidget)w;

if(!XtIsWidget(w))
        {
        XlWarning("XlManager","Compile",
                "il parametro in ingresso non e' un Widget");
        return(False);
        }
if(checkVarInfo(cw,&ind_var_err)==False)
        {
        sprintf(msg,"Variable%d. Resource variable info: NOT CORRECT!!!",(ind_var_err+1));
        XlErrComp(w,"Compile",msg,NULL);
        errors=True;
        return(False);
        }
if(checkRangeZoom(cw)==False)
	{
        strcpy(msg,"Resource Time Scales: NOT CORRECT!!!");
        XlErrComp(w,"Compile",msg,NULL);
        errors=True;
        return(False);
	}
if(cw->curve.s_XLcurve.factScaleValue<LIM_INF_SCA_VAL)
        {
        strcpy(msg,"Resource: Scale factor axis value less than 1.0!!!");
        XlErrComp(w,"Compile",msg,NULL);
        errors=True;
        return(False);
        } 
if(xlCurveClassRec.xlmanager_class.compilatore==NULL)
        {
        XlErrComp(w,"Compile","Compiler not defined",NULL);
        return(False);
        }
if(!(*superclass->xlmanager_class.compile) (w,
     xlCurveClassRec.xlmanager_class.compilatore))
        {
        XlErrComp(w,"Compile","Error in upper-class",NULL);
        errors=True;
        XlEvError(cw,True);
        return(False);
        }
return(True);
}

static void Redisplay(w, event)
Widget w;
XExposeEvent *event;
{
int i;
XlCurveWidget cw= (XlCurveWidget)w;
register int x,y;
unsigned int width,height;
XmDrawingAreaCallbackStruct str;
Arg args[3];
/*
printf("ENTRO NELLA REDISPLAY!!!!!!!!!!!!!\n");
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

(*superclass->core_class.expose) (w,event,NULL);


if(cw->xlmanager.config==False) {
   if(cw->curve.s_XLcurve.first_refresh==False) {
      if(cw->curve.s_XLcurve.seldata_OK==True) {
         manageSeldata(cw);
      }
      else {
         if(cw->curve.s_XLcurve.change_conf_OK!=CONF_NO_CHANGE) {
/*
   Sono stati modificati i parametri delle curve. Controllo se sono
   cambiate anche le curve.
*/
            SettingVarInfo(cw);
            SettingColorTrend(cw);
            if(cw->curve.s_XLcurve.change_conf_OK==CURVE_CHANGE) {
/*
   Libero il buffer dei valori letti perche' viene riallocato
   nella LoadDataF22() con parametro TUTTI_CAMP.
*/         
	       free(cw->curve.s_XLcurve.buffer_f22);
	       LoadDataF22(cw,TUTTI_CAMP);
            }
            cw->curve.s_XLcurve.change_conf_OK=CONF_NO_CHANGE;
            RefreshXlCurve(w);
         }
         else {
/* La clear area seguente serve per gestire il flooding e la curva invisibile
   svincolati dal refresh. Gestire diversamente!!!!!!!! 24-2-98.

            XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxcurveField),
                       0,0,0,0,False);
*/
            DrawGridCurve(cw);
            DrawCurves(cw);
         }
      }
   }
}
else {
   XClearArea(XtDisplay(cw),XtWindow(cw),0,0,0,0,False);
   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->Uxcontenitore1),0,0,0,0,False);
   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxgraphicArea),0,0,0,0,False);
   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxcurveField),0,0,0,0,False);
   if(cw->curve.s_XLcurve.timePeriodKeys) {
      if(cw->curve.contesto->UxchangeTime!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxchangeTime),0,0,0,0,True);
      if(cw->curve.contesto->UxincreaseTime!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxincreaseTime),0,0,0,0,True);
      if(cw->curve.contesto->UxtimeLeft!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxtimeLeft),0,0,0,0,True);
      if(cw->curve.contesto->UxselData!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxselData),0,0,0,0,True);
      if(cw->curve.contesto->UxtimeRight!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxtimeRight),0,0,0,0,True);
      if(cw->curve.contesto->UxdecreaseTime!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxdecreaseTime),0,0,0,0,True);
      if(cw->curve.contesto->UxabsTime!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxabsTime),0,0,0,0,True);
      if(cw->curve.contesto->Uxflow!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->Uxflow),0,0,0,0,True);
   }
if(cw->curve.s_XLcurve.valueRangeKeys) {
   if(cw->curve.contesto->UxchangeValue!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxchangeValue),0,0,0,0,True);
   if(cw->curve.contesto->UxdecreaseValue!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxdecreaseValue),0,0,0,0,True);
   if(cw->curve.contesto->UxrangeUp!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrangeUp),0,0,0,0,True);
   if(cw->curve.contesto->UxdefaultSet!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxdefaultSet),0,0,0,0,True);
   if(cw->curve.contesto->UxrangeDown!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrangeDown),0,0,0,0,True);
   if(cw->curve.contesto->UxincreaseValue!=NULL)
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxincreaseValue),0,0,0,0,True);
}

if(cw->curve.s_XLcurve.legend) {
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlegendArea),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColBut),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColCod),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColDescr),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColLowLim),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColVal),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColHighLim),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColUnit),0,0,0,0,True);
	XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxrowColCurs),0,0,0,0,True);
        for(i=0;i<cw->curve.s_XLcurve.numCurves;i++)
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxpushButList[i]),0,0,0,0,True);
        for(i=0;i<=cw->curve.s_XLcurve.numCurves;i++) {
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlabelCodList[i]),0,0,0,0,True);
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlabelDescrList[i]),0,0,0,0,True);
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlabelLowLimList[i]),0,0,0,0,True);
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlabelValueList[i]),0,0,0,0,True);
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlabelHighLimList[i]),0,0,0,0,True);
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlabelUnitList[i]),0,0,0,0,True);
           XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxlabelCursorList[i]),0,0,0,0,True);
        }
        if(cw->curve.s_XLcurve.configurationButton)
	   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->Uxconfigur),0,0,0,0,True);
        if(cw->curve.s_XLcurve.saveButton)
	   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->Uxsave),0,0,0,0,True);
        if(cw->curve.s_XLcurve.saveGroupsButton)
	   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxsaveGr),0,0,0,0,True);
        if(cw->curve.s_XLcurve.groupsButton)
	   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->Uxgroups),0,0,0,0,True);
}
}

/*

        Richiamo del metodo di disegno del bordo se l'oggetto
        e' selezionato
*/
(*superclass->xlmanager_class.drawBorderIfSelected) (cw);

/*
printf("ESCO DALLA REDISPLAY!!!!!!!!!!!!!\n");
*/
}


static void StartRefCurs(w,ev)
Widget w;
XEvent *ev;
{
int x_pos,y_pos;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;

printf("StartRefCurs: ENTRATO!!!\n");

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *)UxGetContext(w);
/*
   Inizio operazione di spostamento della cursor line e della reference line.
*/
   static_status_app=static_status;
   if(static_status==False)
      static_status=True;
   XtGrabPointer(w,False,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
	GrabModeAsync,GrabModeAsync,XtWindow(w),None,CurrentTime);
   x_pos=ev->xbutton.x;
   y_pos=ev->xbutton.y;
   curs_pos_pixel=ev->xbutton.x;
   refer_pos_pixel=ev->xbutton.y;
   start_point=True;

   UxTopLevelCurveContext = UxSaveCtx;
        
printf("StartRefCurs: xbutton_iniz=%d\tybutton_iniz=%d\n",x_pos,y_pos);
printf("StartRefCurs: USCITO!!!\n");

}


static void MoveRefCurs(w,ev)
Widget w;
XEvent *ev;
{
int x_pos_curr,y_pos_curr;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;

/*
printf("MoveRefCurs: ENTRATO!!!\n");
*/
        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);

	if(start_point==False) {
	   XDrawLine(XlDisplay(w),XtWindow(w),refcurs_gc,curs_pos_pixel,0,
                     curs_pos_pixel,height_curvefield);
           XDrawLine(XlDisplay(w),XtWindow(w),refcurs_gc,0,refer_pos_pixel,
                     width_curvefield,refer_pos_pixel);
	}
	else {
	   start_point=False;
	}
	x_pos_curr=ev->xmotion.x;
	y_pos_curr=ev->xmotion.y;
	XDrawLine(XlDisplay(w),XtWindow(w),refcurs_gc,x_pos_curr,0,
                  x_pos_curr,height_curvefield);
        XDrawLine(XlDisplay(w),XtWindow(w),refcurs_gc,0,y_pos_curr,
                  width_curvefield,y_pos_curr);
	curs_pos_pixel = x_pos_curr;
	refer_pos_pixel =y_pos_curr;
	XFlush(XlDisplay(w));

        UxTopLevelCurveContext = UxSaveCtx;
/*
printf("MoveRefCurs: USCITO!!!\n");
*/
}



static void StopRefCurs(w,ev)
Widget w;
XEvent *ev;
{
int	i,pos_grid,rem_add;
int	current_coord;
div_t	interval_wid,interval_height;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;

printf("StopRefCurs: ENTRATO!!!\n");

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
/*
   Verifico se la cursor line o la reference line cadono sulla grid;
   in tal caso vengono spostate.
*/
   interval_wid=div(width_curvefield,NUMDIVWIDTH);
   interval_height=div(height_curvefield,NUMDIVHEIGHT);
   rem_add=0;
   for(i=0;i<=NUMDIVWIDTH;i++) {
      if(interval_wid.rem!=0) {
         pos_grid=(interval_wid.quot*i)+i;
         interval_wid.rem--;
         rem_add=i;
      }
      else {
         if(((interval_wid.quot*i)+rem_add)>=width_curvefield)
              current_coord=(width_curvefield-1);
         else
            current_coord=((interval_wid.quot*i)+rem_add);
            pos_grid=current_coord;
      }
      if(pos_grid==ev->xbutton.x) {
printf("StopRefCurs: xbutton.x=%d\tpos_grid=%d\n",ev->xbutton.x,pos_grid);
         if(ev->xbutton.x<SKIP_GRID) {
            ev->xbutton.x=ev->xbutton.x+SKIP_GRID;
            curs_pos_pixel=curs_pos_pixel+SKIP_GRID;
         }
         else {
            ev->xbutton.x=ev->xbutton.x-SKIP_GRID;
            curs_pos_pixel=curs_pos_pixel-SKIP_GRID;
         }
      }
   }
   rem_add=0;
   for(i=0;i<=NUMDIVHEIGHT;i++) {
      if(interval_height.rem!=0) {
         pos_grid=(interval_height.quot*i)+i;
         interval_height.rem--;
         rem_add=i;
      }
      else {
         if(((interval_height.quot*i)+rem_add)>=height_curvefield)
              current_coord=(height_curvefield-1);
         else
            current_coord=((interval_height.quot*i)+rem_add);
         pos_grid=current_coord;
      }
      if(pos_grid==ev->xbutton.y) {
printf("StopRefCurs: xbutton.y=%d\tpos_grid=%d\n",ev->xbutton.y,pos_grid);
         if(ev->xbutton.y<SKIP_GRID) {
            ev->xbutton.y=ev->xbutton.y+SKIP_GRID;
            refer_pos_pixel=refer_pos_pixel+SKIP_GRID;
         }
         else {
            ev->xbutton.y=ev->xbutton.y-SKIP_GRID;
            refer_pos_pixel=refer_pos_pixel-SKIP_GRID;
         }
      }
   }
   cursor_line_pos=(ev->xbutton.x/width_curvefield)*100.0;
   ref_line_pos=(1.0-((float)ev->xbutton.y/(float)height_curvefield ))*100.0;

   RefreshXlCurve(top_curve);

   static_status=static_status_app;
   start_point=True;

   XtUngrabPointer(w,CurrentTime);

   UxTopLevelCurveContext = UxSaveCtx;

printf("StopRefCurs: USCITO!!!\n");

}


static Boolean LoadFontTime( w,fontname)
Widget w;
char fontname[];
{
Display *disp;
XlCurveWidget cw = (XlCurveWidget)w;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

/* Carica il font ottenendo la descrizione del font stesso */
   disp=XtDisplay(w);
   cw->curve.s_XLcurve.font_info_time=XlGetFont(disp,fontname);
   if(cw->curve.s_XLcurve.font_info_time==NULL)
        {
        XlWarning("XlCurve","LoadFontTime","Cannot open font");
        return(False);
        }
        else
        {
	cw->curve.s_XLcurve.font_list_time =
			XmFontListCreate ( cw->curve.s_XLcurve.font_info_time, charset);
	if (cw->curve.s_XLcurve.font_list_time == NULL)
                {
                XlWarning("XlCurve","LoadFontTime","FONTLIST NULL");
                return(False);
                }
        }

return(True);
}


static Boolean LoadFontValue(w,fontname)
Widget w;
char fontname[];
{
Display *disp;
XlCurveWidget cw = (XlCurveWidget)w;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

/* Carica il font ottenendo la descrizione del font stesso */
   disp=XtDisplay(w);
   cw->curve.s_XLcurve.font_info_value=XlGetFont(disp,fontname);
   if(cw->curve.s_XLcurve.font_info_value==NULL)
        {
        XlWarning("XlCurve","LoadFontValue","Cannot open font");
        return(False);
        }
        else
        {
        cw->curve.s_XLcurve.font_list_value =
                XmFontListCreate( cw->curve.s_XLcurve.font_info_value,charset);
        if (cw->curve.s_XLcurve.font_list_value == NULL)
                {
                XlWarning("XlCurve","LoadFontValue","FONTLIST NULL");
                return(False);
                }
        }

return(True);
}


static Boolean LoadFontLegend( w,fontname)
Widget w;
char fontname[];
{
Display *disp;
XlCurveWidget cw = (XlCurveWidget)w;
static XmStringCharSet  charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;

/* Carica il font ottenendo la descrizione del font stesso */
   disp=XtDisplay(w);
   cw->curve.s_XLcurve.font_info_legend=XlGetFont(disp,fontname);
   if(cw->curve.s_XLcurve.font_info_legend==NULL)
        {
        XlWarning("XlCurve","LoadFontLegend","Cannot open font");
        return(False);
        }
        else
        {
        cw->curve.s_XLcurve.font_list_legend =
                        XmFontListCreate ( cw->curve.s_XLcurve.font_info_legend, charset);
        if (cw->curve.s_XLcurve.font_list_legend == NULL)
                {
                XlWarning("XlCurve","LoadFontLegend","FONTLIST NULL");
                return(False);
                }
        }

return(True);
}


static void GetAllGCs(w)
Widget w;
{
/*
   Creazione dei graphic context per: tracciamento della griglia,
   tracciamento della cursor line e reference line in refresh,
   tracciamento della cursor line e reference line in spostamento,
   tracciamento delle curve, tracciamento del fluctua.
*/  
   GetGridGC(w);
   GetLinesGC(w);
/*   GetRefCurGC(w); */
   GetTrendGCs(w);
   GetTrendFluctGCs(w);
}



static void GetGridGC(w)
Widget w;
{

XGCValues values;
XlCurveWidget cw = (XlCurveWidget)w;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
			 | GCLineStyle;

/* GC per il disegno della grid */
   values.line_style = LineSolid;
   values.line_width = 0;
   values.background = cw->curve.s_XLcurve.curveFieldColor;
   values.foreground = cw->curve.s_XLcurve.gridColor;
   cw->curve.s_XLcurve.grid_gc=XtGetGC(cw, valuemask, &values);

}



static void GetLinesGC(w)
Widget w;
{
XGCValues values;
XlCurveWidget cw = (XlCurveWidget)w;
unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;

/* GC per il disegno della reference line e cursor line in refresh */
   values.line_style = LineSolid;
   values.line_width = 0;
   values.background = cw->curve.s_XLcurve.curveFieldColor;
   values.foreground = cw->curve.s_XLcurve.refcursColor;
   cw->curve.s_XLcurve.lines_gc=XtGetGC(cw, valuemask, &values);
}



static void GetRefCurGC(w)
Widget w;
{
XGCValues values;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;
unsigned long valuemask= GCForeground | GCLineWidth
                         | GCLineStyle | GCBackground | GCFunction;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);

/*
   GC per il disegno e lo spostamento della reference line e della
   cursor line
*/
   values.line_style = LineSolid;
   values.line_width = 0;
   values.background = cw->curve.s_XLcurve.curveFieldColor;
   values.foreground = cw->curve.s_XLcurve.refcursColor;
   values.function = GXxor;
   refcurs_gc=XtGetGC(cw, valuemask, &values);

   UxTopLevelCurveContext = UxSaveCtx;
}
	


static void GetTrendFluctGCs(w)
Widget w;
{
int i;
XGCValues values;
XlCurveWidget cw = (XlCurveWidget)w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
			 | GCLineStyle | GCFillStyle | GCStipple;

   values.line_style = LineSolid;
   values.line_width = 0;
   values.background = cw->curve.s_XLcurve.curveFieldColor;
   values.fill_style = FillStippled;
   values.stipple    = cw->curve.s_XLcurve.stip_fluctua;
   for (i=0;i<NUMMAXVAR;i++) {
      values.foreground = cw->curve.s_XLcurve.col_trend[i];
      cw->curve.s_XLcurve.trends_fluct_gc[i]=XtGetGC(w,valuemask,&values);
   }
}



static void GetTrendGCs(w)
Widget w;
{
int i;
XGCValues values;
XlCurveWidget cw = (XlCurveWidget)w;

unsigned long valuemask= GCForeground | GCBackground | GCLineWidth
                         | GCLineStyle;

   values.line_style = LineSolid;
   values.line_width = 2;
   values.background = cw->curve.s_XLcurve.curveFieldColor;
   for (i=0;i<NUMMAXVAR;i++) {
      values.foreground = cw->curve.s_XLcurve.col_trend[i];
      cw->curve.s_XLcurve.trends_gc[i]=XtGetGC(w,valuemask,&values);
   }
}


/*
        metodo di stampa
*/

static Boolean Print()
{
return(1);
}
                                                                


/*
	Definizione funzioni varie
*/
static void SettaActionsFigli(tnew)
Widget tnew;
{
int i;
XlCurveWidget new = (XlCurveWidget)tnew;

printf("SettaActionsFigli: ENTRATO!!!\n");

if(new->xlmanager.config==True) {
   add_translation(new->curve.contesto->Uxcontenitore1,configTranslations);
   add_translation(new->curve.contesto->UxgraphicArea,configTranslations);
   add_translation(new->curve.contesto->UxcurveField,configTranslations);
   if(new->curve.s_XLcurve.timePeriodKeys) {
      add_translation(new->curve.contesto->UxchangeTime,configTranslations);
      add_translation(new->curve.contesto->UxincreaseTime,configTranslations);
      add_translation(new->curve.contesto->UxtimeLeft,configTranslations);
      add_translation(new->curve.contesto->UxselData,configTranslations);
      add_translation(new->curve.contesto->UxtimeRight,configTranslations);
      add_translation(new->curve.contesto->UxdecreaseTime,configTranslations);
      add_translation(new->curve.contesto->UxabsTime,configTranslations);
      add_translation(new->curve.contesto->Uxflow,configTranslations);
   } 
   add_translation(new->curve.contesto->UxformValue,configTranslations);
   add_translation(new->curve.contesto->UxformTime,configTranslations);
   if(new->curve.s_XLcurve.valueRangeKeys) {
      add_translation(new->curve.contesto->UxchangeValue,configTranslations);
      add_translation(new->curve.contesto->UxdecreaseValue,configTranslations);
      add_translation(new->curve.contesto->UxrangeUp,configTranslations);
      add_translation(new->curve.contesto->UxdefaultSet,configTranslations);
      add_translation(new->curve.contesto->UxrangeDown,configTranslations);
      add_translation(new->curve.contesto->UxincreaseValue,configTranslations);
   }
   if(new->curve.s_XLcurve.legend) {
      add_translation(new->curve.contesto->UxlegendArea,configTranslations);
      add_translation(new->curve.contesto->UxrowColBut,configTranslations);
      add_translation(new->curve.contesto->UxrowColCod,configTranslations);
      add_translation(new->curve.contesto->UxrowColDescr,configTranslations);
      add_translation(new->curve.contesto->UxrowColLowLim,configTranslations);
      add_translation(new->curve.contesto->UxrowColVal,configTranslations);
      add_translation(new->curve.contesto->UxrowColHighLim,configTranslations);
      add_translation(new->curve.contesto->UxrowColUnit,configTranslations);
      add_translation(new->curve.contesto->UxrowColCurs,configTranslations);
      for(i=0;i<new->curve.s_XLcurve.numCurves;i++) 
         add_translation(new->curve.contesto->UxpushButList[i],configTranslations);
      for(i=0;i<=new->curve.s_XLcurve.numCurves;i++) { 
         add_translation(new->curve.contesto->UxlabelCodList[i],configTranslations);
         add_translation(new->curve.contesto->UxlabelDescrList[i],configTranslations);
         add_translation(new->curve.contesto->UxlabelLowLimList[i],configTranslations);
         add_translation(new->curve.contesto->UxlabelValueList[i],configTranslations);
         add_translation(new->curve.contesto->UxlabelHighLimList[i],configTranslations);
         add_translation(new->curve.contesto->UxlabelUnitList[i],configTranslations);
         add_translation(new->curve.contesto->UxlabelCursorList[i],configTranslations);
      }
      if(new->curve.s_XLcurve.configurationButton)
         add_translation(new->curve.contesto->Uxconfigur,configTranslations);
      if(new->curve.s_XLcurve.saveButton)
         add_translation(new->curve.contesto->Uxsave,configTranslations);
      if(new->curve.s_XLcurve.saveGroupsButton)
         add_translation(new->curve.contesto->UxsaveGr,configTranslations);
      if(new->curve.s_XLcurve.groupsButton)
         add_translation(new->curve.contesto->Uxgroups,configTranslations);
   }
}
else {
   add_translation(new->curve.contesto->UxcurveField,refcursTranslation);
}
	   

printf("SettaActionsFigli: USCITO!!!\n");

}



static void add_translation(Widget wid,char *stringa)
{
   XtTranslations tr;
   char *policy;


/* verifico la consistenza dei parametri */
   if (wid && stringa)
   {
/* converto la stringa in XtTranslation */
       tr = XtParseTranslationTable(stringa);
       policy = stringa;
       if (*policy == '#')
       {
           policy++;
           switch( *policy )
           {
               case 'r':
                 set_something_val(wid, XmNtranslations, (XtArgVal)tr);
               break;
               case 'a':
                  XtAugmentTranslations(wid, tr);
               break;
               case 'o':
               default:
                  XtOverrideTranslations(wid, tr);
               break;
           }
       }
       else
           XtOverrideTranslations(wid, tr);
   }
}


void SettaResourceMmi(tnew)
Widget tnew;
{

XmFontList font_time, font_value;
XlCurveWidget new = (XlCurveWidget)tnew;
char	f22_circ_path_name[MAXPATHNAME],path_name[MAXPATHNAME],msg[500];
int i;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;

printf("SettaResourceMmi: ENTRATO!!!\n");

        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(tnew);
/*
	Inizializzazione della variabile che segnala eventuali errori di configurazione.
*/
        new->curve.s_XLcurve.configur_error=False;
/*
	Inizializzazione della variabile che segnala se e' cambiata la configurazione.
*/
        new->curve.s_XLcurve.change_conf_OK=CONF_NO_CHANGE;
/*
	Inizializzazione dello stato dei toggles nell'interfaccia 'Manipular curva'.
*/
	for(i=0;i<NUMMAXVAR;i++) 
	   new->curve.s_XLcurve.toggle_state[i]=0;
	for(i=0;i<NUMMAXVAR;i++) 
	   new->curve.s_XLcurve.curve_inv_state[i]=0;
/*
	Inizializzazione della variabile che segnala se una scala fisica e' gia' 
	visualizzata(fis_sca_displ) e se ci sono piu' scale fisiche da visualizzare
	num_scale_act.
*/
	new->curve.s_XLcurve.fis_sca_displ=-1;
	new->curve.s_XLcurve.fis_sca_app=-1;
        num_scale_act=0;
/*
	Inizializzazione della posizione % della ref. line e della cursor line.
*/
        ref_line_pos=REF_LIN_DEFAULT;
        cursor_line_pos=CUR_LIN_DEFAULT;
/*
   Inizializzazione della variabile che da la posizione nel file
   f22 del pixel che giace sulla cursor line. Se -1 nessun pixel.
*/
        new->curve.s_XLcurve.cur_line_pos_f22=-1;
/*
   Inizializzazione della variabile che mi dice se e' stata attivata
   l'interfaccia seldata.
*/
        new->curve.s_XLcurve.seldata_OK=False;
        new->curve.s_XLcurve.hour_sel=0;
        new->curve.s_XLcurve.minute_sel=0;
/*
   Inizializzazione del bottone "Caudal" a pushed in e della variabile
   di stato corrispondente a False (Dynamic). 
*/
	if(new->curve.s_XLcurve.timePeriodKeys) {
	   PushedEffect(flow);
           PushedEffect(abstime);
           abs_time_flag=True;
	   static_status=False;
        }
/*
	Inizializzazione della struttura contenente i limiti di visualizzazione
	in percentuale che vengono modificati dalle operazioni sull'asse dei valori.
*/
        new->curve.s_XLcurve.min_max_perc.min=DEF_LL_DISPL;
        new->curve.s_XLcurve.min_max_perc.max=DEF_HL_DISPL; 
/*
	Setting nella graphic area della stringa che specifica la sorgente dei dati.
*/
	if(new->curve.s_XLcurve.curveSourceType==ARCH_CIRC_XLCURVE) {
	   /* La sorgente dei dati e' il file circolare */
	   strcpy(f22_circ_path_name,new->curve.s_XLcurve.curveSource);
	   if(access(f22_circ_path_name,F_OK)!=0) {
	      sprintf(msg,"File %s not found",f22_circ_path_name);
	      XlWarning("XlCurve","Initialize",msg);
	      XtVaSetValues(sourcedata,
			    RES_CONVERT(XmNlabelString, "Data source: Error archive file!!!"),NULL);
              new->curve.s_XLcurve.configur_error=True;
	   }
	   else {
	      sprintf(msg,"Data source: %s",f22_circ_path_name);
	      XtVaSetValues(sourcedata,
			    RES_CONVERT(XmNlabelString,msg),NULL); 
	   }
	} 
	else {
	   XtVaSetValues(sourcedata,
			 RES_CONVERT(XmNlabelString, "Data source: runtime"),NULL);
	}
/*
	Caricamento dei range necessari per gli zoom sull'asse dei tempi.
*/
	RecoveryRangeZoom(new);
/*
	Inizializzazione della variabile first_refresh=True.
*/
	new->curve.s_XLcurve.first_refresh=True;
 
        UxTopLevelCurveContext = UxSaveCtx;

printf("SettaResourceMmi: USCITO!!!\n");
	
}


/*******************************************************************/
/*                                                                 */
/*   Funzione che disegna la griglia nel 'curvefield'              */
/*                                                                 */
/*******************************************************************/
void DrawGridCurve(w)
Widget w;
{

int 	i,rem_add,x_current,y_current;
div_t	interval_wid,interval_height;	
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;
/*
printf("DrawGridCurve: ENTRATO!!!\n");
*/
        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
	interval_wid=div(width_curvefield,NUMDIVWIDTH);
	interval_height=div(height_curvefield,NUMDIVHEIGHT);
/*
printf("DrawGridCurve: width_quot=%d\twidth_rem=%d\n",interval_wid.quot,
        interval_wid.rem);
printf("DrawGridCurve: height_quot=%d\theight_rem=%d\n",interval_height.quot,
        interval_height.rem);
*/
        rem_add=0;
	for(i=0;i<=NUMDIVWIDTH;i++) {
           if(interval_wid.rem!=0) {
              XDrawLine(XlDisplay(curvefield),XtWindow(curvefield),
                 cw->curve.s_XLcurve.grid_gc,((interval_wid.quot*i)+i),0,
                 ((interval_wid.quot*i)+i),height_curvefield);
              interval_wid.rem--;
              rem_add=i;
           }
           else {
              if(((interval_wid.quot*i)+rem_add)>=width_curvefield)
                 x_current=(width_curvefield-1);
              else
                 x_current=((interval_wid.quot*i)+rem_add);
              XDrawLine(XlDisplay(curvefield),XtWindow(curvefield),
                 cw->curve.s_XLcurve.grid_gc,x_current,
                 0,x_current,height_curvefield);
           }
        }
        rem_add=0;
	for(i=0;i<=NUMDIVHEIGHT;i++) {
           if(interval_height.rem!=0) {
             XDrawLine(XlDisplay(curvefield),XtWindow(curvefield),
                cw->curve.s_XLcurve.grid_gc,0,((interval_height.quot*i)+i),
                width_curvefield,((interval_height.quot*i)+i));
             interval_height.rem--;
             rem_add=i;  
           }
           else {
              if(((interval_height.quot*i)+rem_add)>=height_curvefield)
                 y_current=(height_curvefield-1);
              else
                 y_current=((interval_height.quot*i)+rem_add);
              XDrawLine(XlDisplay(curvefield),XtWindow(curvefield),
                 cw->curve.s_XLcurve.grid_gc,0,y_current,
                 width_curvefield,y_current);
           }
        }

        UxTopLevelCurveContext = UxSaveCtx;
/*
printf("DrawGridCurve: USCITO!!!\n");
*/
}




/*******************************************************************/
/*                                                                 */
/*   Funzione che aggiorna le colonne 'value' e 'cursor' nella     */
/*   legenda con i valori correnti.
/*                                                                 */
/*******************************************************************/
void AggLegendValues(w)
Widget w;
{
int i;
char str_displ[200];
XmString compound;
XlCurveWidget cw = (XlCurveWidget)w;

_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
/*
printf("AggLegend: ENTRATO!!!\n");
*/
   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
/*
   Aggiornamento della colonna 'value' e della colonna 'cursor'
   nella legenda.
*/
   if(cw->curve.s_XLcurve.legend) {
      for(i=0;i<NUMMAXVAR;i++) {
         if(cw->curve.s_XLcurve.ind_var_sel[i]!=-1) {
            if(cw->curve.s_XLcurve.n_points[i]!=0)
               sprintf(str_displ,"%.2f",cw->curve.s_XLcurve.last_val_displ[i]);
            else
               sprintf(str_displ,"----.--");
            compound=XmStringCreateSimple(str_displ);
            XtVaSetValues(labelValueList[i+1],XmNlabelString,compound,NULL);
            XmStringFree(compound);
         }
      }
      for(i=0;i<NUMMAXVAR;i++) {
         if(cw->curve.s_XLcurve.ind_var_sel[i]!=-1) {
            if(cw->curve.s_XLcurve.n_points[i]!=0) {
               if(cw->curve.s_XLcurve.curs_pos_OK==True)
                  sprintf(str_displ,"%.2f",cw->curve.s_XLcurve.curs_val_displ[i]);
               else
                  sprintf(str_displ,"----.--");
            }
            else {
               sprintf(str_displ,"----.--");
            }
            compound=XmStringCreateSimple(str_displ);
            XtVaSetValues(labelCursorList[i+1],XmNlabelString,compound,NULL);
            XmStringFree(compound);
         }
      }
   }
/*
   Aggiornamento della data e del tempo.
*/
   AggDateTime(w);

   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("AggLegendValues: USCITO!!!\n");
*/
}


Boolean LoadIndVarSel(w)
Widget w;
{

int i,j,length,count;
Boolean found;
char str_app[CHAR_LEGO_COD];
char *str_app1;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

printf("LoadIndVarSel: ENTRATO!!!\n");

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
   for(i=0;i<cw->curve.s_XLcurve.numCurves;i++) {

printf("LoadIndVarSel:var_lego_cod[%d]=%s\n",i,cw->curve.s_XLcurve.var_lego_cod[i]);
      count=0;
      found=False;
      j=0;
      if(strcmp(cw->curve.s_XLcurve.var_lego_cod[i],"")!=0) {
         while((found==False)&&(j<cw->curve.s_XLcurve.num_var_f22)) {
/*
	Eliminazione dei blank dal nome della variabile presa dal file f22.
*/
            if((str_app1=strchr(cw->curve.s_XLcurve.name_desc_var[j].nomevar,' '))!=NULL) {
               length=strlen(cw->curve.s_XLcurve.name_desc_var[j].nomevar)-strlen(str_app1);
               strncpy(str_app,cw->curve.s_XLcurve.name_desc_var[j].nomevar,length);
               str_app[length]='\0';
            }
            else {
               strcpy(str_app,cw->curve.s_XLcurve.name_desc_var[j].nomevar);
            }
            if(strcmp(cw->curve.s_XLcurve.var_lego_cod[i],str_app)==0) {
            
printf("LoadIndVarSel: Trovata var_lego_cod[%d]=%s in f22\n",i,cw->curve.s_XLcurve.var_lego_cod[i]);
               cw->curve.s_XLcurve.ind_var_sel[i]=j;
               if(cw->curve.s_XLcurve.legend)
                  XtVaSetValues(pushButList[i],XmNsensitive,True,NULL);
               found=True;
	    }
	    else {
	       j++;
	    }
	 }
	 if(j==cw->curve.s_XLcurve.num_var_f22) {
	   /* La i-esima variabile configurata non e' presente nel file f22. */
printf("LoadIndVarSel: WARNING!! Variabile: %s non presente nel file f22!!!\n",cw->curve.s_XLcurve.var_lego_cod[i]);
	   cw->curve.s_XLcurve.ind_var_sel[i]=-1;
           count++;
           strcpy(cw->curve.s_XLcurve.varInfoChange[i],"");
           if(cw->curve.s_XLcurve.legend) {
              XtVaSetValues(pushButList[i],XmNsensitive,False,NULL);
              ResetLegend(cw,i);
           }
	 }
      }
      else {
         cw->curve.s_XLcurve.ind_var_sel[i]=-1;
         count++;
         strcpy(cw->curve.s_XLcurve.varInfoChange[i],"");
         if(cw->curve.s_XLcurve.legend) {
            XtVaSetValues(pushButList[i],XmNsensitive,False,NULL);
            ResetLegend(cw,i);
         }
      }
   }
   if(count==cw->curve.s_XLcurve.numCurves)
      return(False);
   for(i=cw->curve.s_XLcurve.numCurves;i<NUMMAXVAR;i++)
      cw->curve.s_XLcurve.ind_var_sel[i]=-1;
   UxTopLevelCurveContext = UxSaveCtx;

printf("LoadIndVarSel: USCITO!!!\n");

return(True);
}



void ResetLegend(w,i)
Widget w;
int i;
{

XmString compound;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
   compound=XmStringCreateSimple("");
   XtVaSetValues(labelCodList[i+1],XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple("");
   XtVaSetValues(labelDescrList[i+1],XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple("");
   XtVaSetValues(labelLowLimList[i+1],XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple("");
   XtVaSetValues(labelValueList[i+1],XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple("");
   XtVaSetValues(labelHighLimList[i+1],XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple("");
   XtVaSetValues(labelUnitList[i+1],XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple("");
   XtVaSetValues(labelCursorList[i+1],XmNlabelString,compound,NULL);
   XmStringFree(compound);
   XtVaSetValues(pushButList[i],XmNsensitive,False,NULL);
   UxTopLevelCurveContext = UxSaveCtx;

}



/*******************************************************************/
/*                                                                 */
/*   Funzione che carica nella struttura buffer_f22 i campioni     */
/*   attualmente presenti nel file f22circ.dat relativamente alle  */
/*   variabili configurate.                                        */
/*                                                                 */
/*******************************************************************/
Boolean LoadDataF22(w,flag)
Widget w;
int flag;
{
int ind_buffer_f22;
int num_camp_read,camp_visual;
int num_shift;
int i,j;
float tempo=0.0;
XlCurveWidget cw = (XlCurveWidget)w;
/*
printf("LoadDataF22: ******************************\n");
*/
   if(flag==TUTTI_CAMP) {
/*
   Primo refresh: lettura completa del file f22circ.
*/
      if(f22_open_file_new(cw->curve.s_XLcurve.curveSource, &(cw->curve.s_XLcurve.file_f22))==0) {
         XlError("XlCurve","LoadDataF22","open f22circ error!!!");
         return(False);
      }
/*
   Lettura della parte di header del file f22. 
*/
      if(f22_leggo_header(&cw->curve.s_XLcurve.header_f22,cw->curve.s_XLcurve.file_f22,1,&cw->curve.s_XLcurve.simpar)==0) {
         XlWarning("XlCurve","LoadDataF22","read header error!!!");
	 return(False);
      }
/*
   Lettura del nome e della descrizione delle variabili presenti nel file f22.
*/

      if(f22_leggo_nomi_var(cw->curve.s_XLcurve.file_f22,&cw->curve.s_XLcurve.name_desc_var,&cw->curve.s_XLcurve.num_var_f22)==0) {
         XlError("XlCurve","LoadDataF22","read name-descr var error!!!");
         return(False);
      }
/*
printf("LoadDataF22: num_var_f22=%d\n",cw->curve.s_XLcurve.num_var_f22);
*/
/*
   Ricerca dei nomi delle variabili configurate nella struttura
   contenente tutte quelle presenti nel file f22. Caricamento degli
   indici in un array.
*/ 
      if(LoadIndVarSel(cw)==False) {
/*
   Le variabili configurate non sono presenti nel file f22. Esco
   dall'applicazione.
*/
         XlWarning("XlCurve","Refresh",
                   "variables configured not present in the file f22circ");
         return(False);
      }
/*
   Allocazione della struttura che contiene i tempi e i valori relativi
   alle variabili configurate
*/
      if(alloca_bufdati(cw)==True) {
/*
   Lettura di tutti i campioni presenti nel file f22.
*/
         num_camp_read=f22_leggi_tutti_camp(cw->curve.s_XLcurve.file_f22,
                                            cw->curve.s_XLcurve.header_f22,
                                            cw->curve.s_XLcurve.buffer_f22,
                                            cw->curve.s_XLcurve.ind_var_sel);
         if(num_camp_read==0) {
/*
   Non ci sono campioni nel file f22circ corrente.
*/
           cw->curve.s_XLcurve.n_last_val=0; 
           cw->curve.s_XLcurve.t_start=-1.0;
           cw->curve.s_XLcurve.t_end=-1.0;
         }
         else {
	    cw->curve.s_XLcurve.n_last_val=num_camp_read-1;
	    cw->curve.s_XLcurve.t_start=cw->curve.s_XLcurve.buffer_f22[0].tempo.tempo;
	    cw->curve.s_XLcurve.t_end=cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.n_last_val].tempo.tempo;
         }
/*
printf("LoadDataF22: num_camp_read = %d\n",num_camp_read);
printf("LoadDataF22: tempo_iniz=%f\n",cw->curve.s_XLcurve.buffer_f22[0].tempo.tempo);
printf("LoadDataF22: tempo_fin=%f\n",cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.n_last_val].tempo.tempo);
*/
         f22_close_file(cw->curve.s_XLcurve.file_f22);
      }
      else {
         f22_close_file(cw->curve.s_XLcurve.file_f22);
         return(False);
      }
   }
   else {
/*
   Refresh successivi: aggiornamento.
*/
      if(f22_open_file_new(cw->curve.s_XLcurve.curveSource, &(cw->curve.s_XLcurve.file_f22))==0) {
         XlError("XlCurve","LoadDataF22","open f22circ error!!!");
         return(False);
      }
/*
   Lettura della parte di header del file f22. 
*/
      if(f22_leggo_header(&cw->curve.s_XLcurve.header_f22,cw->curve.s_XLcurve.file_f22,0,&cw->curve.s_XLcurve.simpar)==0) {
         XlWarning("XlCurve","LoadDataF22","read header error!!!");
	 return(False);
      }
      if(cw->curve.s_XLcurve.t_start==-1.0) {
         tempo=-1.0;
         ind_buffer_f22=0;
      }
      else {
         tempo=cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.n_last_val].tempo.tempo;
         ind_buffer_f22=cw->curve.s_XLcurve.n_last_val+1;
      }
      num_camp_read=f22_leggi_new(cw->curve.s_XLcurve.file_f22,
                                  cw->curve.s_XLcurve.header_f22,
				  &tempo,&cw->curve.s_XLcurve.dati_f22,
                                  cw->curve.s_XLcurve.ind_var_sel);
/*
printf("LoadDataF22: num_camp_read=%d\n",num_camp_read);
*/
      f22_close_file(cw->curve.s_XLcurve.file_f22);
      num_shift= ind_buffer_f22+num_camp_read-(cw->curve.s_XLcurve.header_f22.num_campioni+1);	
      if(num_shift>0) {
         if((ind_buffer_f22-num_shift)>0) {
	    memmove(&cw->curve.s_XLcurve.buffer_f22[0],
                    &cw->curve.s_XLcurve.buffer_f22[num_shift],
                    (ind_buffer_f22-num_shift)*sizeof(SCIRC_SEL_NEW));
	    ind_buffer_f22=ind_buffer_f22-num_shift;
	 }
	 else
	    ind_buffer_f22=0;
      }
      for(i=0;i<num_camp_read;i++) {
         for(j=0;j<NUMMAXVAR;j++) {
	    if(cw->curve.s_XLcurve.ind_var_sel[j]!=-1) {
	       cw->curve.s_XLcurve.buffer_f22[ind_buffer_f22].mis[j]=
               cw->curve.s_XLcurve.dati_f22[i].mis[j];
	       cw->curve.s_XLcurve.buffer_f22[ind_buffer_f22].tempo.tempo=
               cw->curve.s_XLcurve.dati_f22[i].tempo.tempo;
	    }
	 }
         ind_buffer_f22++;
      }

printf("LoadDataF22: num_camp_read = %d\n",num_camp_read);
printf("LoadDataF22: ind_buffer_f22 = %d\n",ind_buffer_f22);

      if((cw->curve.s_XLcurve.t_start!=cw->curve.s_XLcurve.t_end)||
         (num_camp_read!=0)) {
         cw->curve.s_XLcurve.n_last_val=ind_buffer_f22-1;
         cw->curve.s_XLcurve.t_start=cw->curve.s_XLcurve.buffer_f22[0].tempo.tempo;
         cw->curve.s_XLcurve.t_end=cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.n_last_val].tempo.tempo;
      }
      if(cw->curve.s_XLcurve.dati_f22)
         free(cw->curve.s_XLcurve.dati_f22);
   }
/*
	Determinazione della posizione del primo e dell'ultimo campione.
*/
   cw->curve.s_XLcurve.pos_end_visual=cw->curve.s_XLcurve.n_last_val;
   cw->curve.s_XLcurve.pos_start_visual=
                       calc_start_pos(cw->curve.s_XLcurve.buffer_f22,
			              cw->curve.s_XLcurve.rangeTimeStart,
                                      cw->curve.s_XLcurve.pos_end_visual);
/*
printf("LoadDataF22: pos_start_visual=%d\tpos_end_visual=%d\trangeTimeStart=%d\n",
        cw->curve.s_XLcurve.pos_start_visual,cw->curve.s_XLcurve.pos_end_visual,
        cw->curve.s_XLcurve.rangeTimeStart);
printf("LoadDataF22: ******************************\n");
*/
return(True);	
}




Boolean alloca_bufdati(w)
Widget w;
{
size_t size;
XlCurveWidget cw = (XlCurveWidget)w;

printf("alloca_bufdati: ENTRATO!!!\n");
/*
   Alloca bufdati della dimensione pari al numero di istanti di tempo
   presenti nel file circolare per le al piu' 10 variabili selezionate.
*/
	size=(size_t)(cw->curve.s_XLcurve.header_f22.num_campioni + 1);

printf("alloca_bufdati: size=%d\n",size); 

	cw->curve.s_XLcurve.buffer_f22=(SCIRC_SEL_NEW *)calloc(size,sizeof(SCIRC_SEL_NEW));
	if(cw->curve.s_XLcurve.buffer_f22==NULL) {
           XlError("XlCurve","alloca_bufdati","calloc error!!!");
	   return(False);
        }
	
printf("alloca_bufdati: USCITO!!!\n");

return(True);

}


Boolean alloca_punti(w)
Widget w;
{
int i;
_UxCtopLevelCurve      *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

printf("alloca_punti: ENTRATO!!!\n");

        UxSaveCtx = UxTopLevelCurveContext;
        UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);
/*
	Allocazione della struttura che contiene i punti da visualizzare per le 10 possibili
	curve
*/
	for(i=0;i<NUMMAXVAR;i++) {
	   cw->curve.s_XLcurve.points_to_draw[i]=(XPoint *)calloc((width_curvefield+1),sizeof(XPoint));
           cw->curve.s_XLcurve.n_points[i]=0;
	   if(cw->curve.s_XLcurve.points_to_draw[i]==NULL) {
              XlError("XlCurve","alloca_punti","calloc points_to_draw error!!!");
	      return(False);
           }
        }
/*
	Allocazione della struttura che contiene i punti da visualizzare per la 'fluctua curva'. 
*/
	for(i=0;i<NUMMAXVAR;i++) {
	   cw->curve.s_XLcurve.points_fluctua[i]=(XPoint *)calloc((width_curvefield+3),sizeof(XPoint));
	   if(cw->curve.s_XLcurve.points_fluctua[i]==NULL) {
              XlError("XlCurve","alloca_bufdati","calloc points_fluctua error!!!");
	      return(False);
           }
	}
        UxTopLevelCurveContext = UxSaveCtx;

printf("alloca_punti: USCITO!!!\n");

return(True);
}


Boolean allocaLoadInfo(w)
Widget w;
{

int i;
XlCurveWidget cw = (XlCurveWidget)w;

printf("allocaLoadInfo: ENTRATO!!!\n");

	for(i=0;i<NUMMAXVAR;i++) {
           cw->curve.s_XLcurve.varInfoChange[i]=(char *)calloc(MAXLENINFOVAR,sizeof(char));
	   if(cw->curve.s_XLcurve.varInfoChange[i]==NULL) {
                 XlError("XlCurve","allocaLoadInfo","calloc varInfoChange error!!!");
	         return(False);
	   }
        }
        for(i=0;i<cw->curve.s_XLcurve.numCurves;i++) {
              strcpy(cw->curve.s_XLcurve.varInfoChange[i],cw->curve.s_XLcurve.varInfoStart[i]);
              printf("allocaLoadInfo: varInfoChange[%d]=%s\n",i,cw->curve.s_XLcurve.varInfoChange[i]);
        }

printf("allocaLoadInfo: USCITO!!!\n");

}


Boolean allocaLoadColor(w)
Widget w;
{

int i;
XlCurveWidget cw = (XlCurveWidget)w;

printf("allocaLoadColor: ******************************\n");

   for(i=0;i<cw->curve.s_XLcurve.numCurves;i++) {
      cw->curve.s_XLcurve.colTrendString[i]=convPixToString(pushButList[i],XmNbackground);

printf("allocaLoadColor: colTrendString[%d]=%s\n",i,cw->curve.s_XLcurve.colTrendString[i]);

   }      

printf("allocaLoadColor: ******************************\n");

return(True);
}




Boolean allocaLegoCod(w)
Widget w;
{

int i;
XlCurveWidget cw = (XlCurveWidget)w;

printf("allocaLegoCod: ENTRATO!!!\n");

        for(i=0;i<NUMMAXVAR;i++) {
           cw->curve.s_XLcurve.var_lego_cod[i]=(char *)calloc(CHAR_LEGO_COD+1,sizeof(char));
           if(cw->curve.s_XLcurve.var_lego_cod[i]==NULL) {
                 XlError("XlCurve","allocaLegoCod","calloc var_lego_cod error!!!");
                 return(False);
           }
        }

printf("allocaLegoCod: USCITO!!!\n");

}




/*******************************************************************/
/*                                                                 */
/*   Funzione che esegue il tracciamento delle curve configurate   */
/*                                                                 */
/*******************************************************************/
Boolean DrawCurves(w)
Widget w;
{
int i,j;
int x_pos,y_pos;
Pixmap pixmap;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;
/*
printf("DrawCurves: entrato *******************************\n");
*/
   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *)UxGetContext(w);
/*
   Tracciamento della cursor e reference line.
*/
   XDrawLine(XlDisplay(curvefield),XtWindow(curvefield),
             cw->curve.s_XLcurve.lines_gc,curs_pos_pixel,0,
             curs_pos_pixel,height_curvefield);
   XDrawLine(XlDisplay(curvefield),XtWindow(curvefield),
             cw->curve.s_XLcurve.lines_gc,0,refer_pos_pixel,
             width_curvefield,refer_pos_pixel);
/*
printf("DrawCurves: Larghezza cf=%d\taltezza cf=%d\n",width_curvefield,height_curvefield);
*/
   prepare_draw(w);

/*
   Disegno delle curve configurate.
*/
   for(i=0;i<NUMMAXVAR;i++) {
      if((cw->curve.s_XLcurve.ind_var_sel[i]!=-1)&&
         (cw->curve.s_XLcurve.n_points[i]!=0)) {
/*
   Se la curva i-esima e' stata configurata e ci sono punti disponibili,
   viene tracciata.
*/
         switch (cw->curve.s_XLcurve.toggle_state[i]) {
	    case 0:
	       XDrawLines(XlDisplay(curvefield),XtWindow(curvefield),
                    cw->curve.s_XLcurve.trends_gc[i],
		    cw->curve.s_XLcurve.points_to_draw[i],
                    cw->curve.s_XLcurve.n_points[i],CoordModeOrigin);
               if(cw->curve.s_XLcurve.fis_sca_displ==i) {
                  CleanFisScale(w);
		  cw->curve.s_XLcurve.fis_sca_displ=-1;
                  num_scale_act--;
               }
               if(cw->curve.s_XLcurve.curve_inv_state[i]!=0) {
                  cw->curve.s_XLcurve.curve_inv_state[i]--;
                  XtVaSetValues(pushButList[i],
                         XmNlabelType,XmSTRING,
                         XmNbackground,cw->curve.s_XLcurve.col_trend[i],
                         NULL );
               }
               break;
            case 1:
               cw->curve.s_XLcurve.points_fluctua[i][0].x=cw->curve.s_XLcurve.x_pix_start;
               cw->curve.s_XLcurve.points_fluctua[i][0].y=height_curvefield;
               for(j=1;j<(cw->curve.s_XLcurve.n_points[i]+1);j++) {
		  cw->curve.s_XLcurve.points_fluctua[i][j].x=
                        cw->curve.s_XLcurve.points_to_draw[i][j-1].x;
		  cw->curve.s_XLcurve.points_fluctua[i][j].y=
                        cw->curve.s_XLcurve.points_to_draw[i][j-1].y;
               }
               cw->curve.s_XLcurve.points_fluctua[i][cw->curve.s_XLcurve.n_points[i]+1].x=width_curvefield;
               cw->curve.s_XLcurve.points_fluctua[i][cw->curve.s_XLcurve.n_points[i]+1].y=height_curvefield;

               XFillPolygon(XlDisplay(curvefield),XtWindow(curvefield),
                  cw->curve.s_XLcurve.trends_fluct_gc[i],cw->curve.s_XLcurve.points_fluctua[i],
                  cw->curve.s_XLcurve.n_points[i]+2,Complex,CoordModeOrigin);
               XDrawLines(XlDisplay(curvefield),XtWindow(curvefield),
                  cw->curve.s_XLcurve.trends_gc[i],cw->curve.s_XLcurve.points_to_draw[i],
                  cw->curve.s_XLcurve.n_points[i],CoordModeOrigin);
               if(cw->curve.s_XLcurve.fis_sca_displ==i) {
		  CleanFisScale(w);
		  cw->curve.s_XLcurve.fis_sca_displ=-1;
		  num_scale_act--;
               }
               if(cw->curve.s_XLcurve.curve_inv_state[i]!=0) {
                  cw->curve.s_XLcurve.curve_inv_state[i]--;
                  XtVaSetValues(pushButList[i],
                          XmNlabelType,XmSTRING,
                          XmNbackground,cw->curve.s_XLcurve.col_trend[i],
                          NULL );
               }
printf("DrawCurves: Curva numero: %d. Fluctua curva attivata!!!\n",i);
               break;
            case 2:
               XDrawLines(XlDisplay(curvefield),XtWindow(curvefield),
                          cw->curve.s_XLcurve.trends_gc[i],
                          cw->curve.s_XLcurve.points_to_draw[i],
                          cw->curve.s_XLcurve.n_points[i],CoordModeOrigin);
               if(num_scale_act>1) {
                  if(cw->curve.s_XLcurve.fis_sca_displ==i) {
		     cw->curve.s_XLcurve.toggle_state[i]=0;
                     num_scale_act--;
                  }
                  else {
                     ActFisScale(w,i);
                     num_scale_act--;
                     cw->curve.s_XLcurve.fis_sca_app=cw->curve.s_XLcurve.fis_sca_displ;
                     cw->curve.s_XLcurve.fis_sca_displ=i;
                  }
               }
               else {
                  if(cw->curve.s_XLcurve.fis_sca_app==i) {
                     cw->curve.s_XLcurve.toggle_state[i]=0;
                     cw->curve.s_XLcurve.fis_sca_app=-1;
                  }
                  else { 
                     ActFisScale(w,i);
                     cw->curve.s_XLcurve.fis_sca_displ=i;
                  }
               }	   
               if(cw->curve.s_XLcurve.curve_inv_state[i]!=0) {
                  cw->curve.s_XLcurve.curve_inv_state[i]--;
                  XtVaSetValues(pushButList[i],
                          XmNlabelType,XmSTRING,
                          XmNbackground,cw->curve.s_XLcurve.col_trend[i],
                          NULL );
               }
printf("DrawCurves: Curva numero: %d. Escala fisica visualizzata!!!\n",i);
               break;
            case 3:
               if(cw->curve.s_XLcurve.fis_sca_displ==i) {
                  CleanFisScale(w);
                  cw->curve.s_XLcurve.fis_sca_displ=-1;
                  num_scale_act--;
               }
               if(cw->curve.s_XLcurve.curve_inv_state[i]==0) { 
                  cw->curve.s_XLcurve.pix_hatch=XmGetPixmap(XtScreen(pushButList[i]),
                  cw->curve.s_XLcurve.hatch_name,
                  cw->curve.s_XLcurve.gridColor,
                  cw->curve.s_XLcurve.col_trend[i]);
                  XtVaSetValues(pushButList[i],
                               XmNlabelType,XmPIXMAP,
                               XmNlabelPixmap,cw->curve.s_XLcurve.pix_hatch,
                               XmNbackground,cw->curve.s_XLcurve.col_trend[i],
                               NULL );
                  cw->curve.s_XLcurve.curve_inv_state[i]++;
               }
printf("DrawCurves: Curva numero: %d. Invisible!!!\n",i);
               break;
         }
      }
      else {
/*
   Si controlla se l'indice corrente e' diventato -1 in seguito ad
   eliminazione runtime.
   In tal caso si resetta l'eventuale opzione attiva per la curva.
*/
         if(cw->curve.s_XLcurve.toggle_state[i]!=0) {
            cw->curve.s_XLcurve.toggle_state[i]=0;
            if(num_scale_act!=0) {
               num_scale_act=0;
               cw->curve.s_XLcurve.fis_sca_displ=-1;
               CleanFisScale(w);
            }
            if(cw->curve.s_XLcurve.curve_inv_state[i]!=0) {
               cw->curve.s_XLcurve.curve_inv_state[i]=0;
               XtVaSetValues(pushButList[i],
                        XmNlabelType,XmSTRING,
                        XmNbackground,cw->curve.s_XLcurve.col_trend[i],
                        NULL );
            }
         } 
      }
   }
   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("DrawCurves: uscito *******************************\n");
*/
}


/*******************************************************************/
/*                                                                 */
/*   Funzione che determina gli istanti di tempo che devono        */
/*   essere visualizzati sull'asse delle ascisse a partire dallo   */
/*   intervallo di visualizzazione corrente.                       */
/*                                                                 */
/*******************************************************************/
void AggTimeAxis(w)
Widget w;
{
int i;
int hh;
int mm;
int ss;
int resto;
int k1=3600;
int k2=60;
float t_iniz,t_curr,t_delta;
XmString compound;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;
/*
printf("AggTimeAxis: *******************************\n");
*/
   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);

   if(cw->curve.s_XLcurve.n_points[0]!=0) {
/*
   Esistono punti da rappresentare. Si prosegue.
*/
      t_delta=((float)cw->curve.s_XLcurve.rangeTimeStart/NUMDIVWIDTH);
      if(abs_time_flag==True) {
/*
printf("AggTimeAxis: Tempo assoluto!!!\n");
*/
         t_iniz=cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.pos_start_visual].tempo.tempo;
         t_curr=cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.pos_end_visual].tempo.tempo-
             cw->curve.s_XLcurve.rangeTimeStart;
      }
      else {
/*
printf("AggTimeAxis: Tempo relativo!!!\n");
*/
         t_iniz=0.0;
         t_curr=0.0;
      }
      for(i=0;i<=NUMDIVWIDTH;i++) {
         if(t_curr<t_iniz) {
            strcpy(cw->curve.s_XLcurve.str_time[i],"--:--");
         }
         else {
            hh = t_curr/k1;
            resto = (int)t_curr % k1;
            if( resto ) {
               mm = resto / k2;
               resto %= k2;
            }
            else
               mm = 0;
            ss = resto;
            sprintf(cw->curve.s_XLcurve.str_time[i],"%.2d:%.2d",hh,mm);
         }
/*
printf("AggTimeAxis: str_time[%d] = %s\n",i,cw->curve.s_XLcurve.str_time[i]);
*/
         t_curr+=t_delta;
      }
   }
   else {
      for(i=0;i<=NUMDIVWIDTH;i++) {
         strcpy(cw->curve.s_XLcurve.str_time[i],"--:--");
      }
   }
/*
   Aggiornamento degli istanti di tempo visualizzati sull'asse
   delle ascisse.
*/
   compound=XmStringCreateSimple(cw->curve.s_XLcurve.str_time[1]);
   XtVaSetValues(labtime1,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple(cw->curve.s_XLcurve.str_time[2]);
   XtVaSetValues(labtime2,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple(cw->curve.s_XLcurve.str_time[3]);
   XtVaSetValues(labtime3,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple(cw->curve.s_XLcurve.str_time[4]);
   XtVaSetValues(labtime4,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple(cw->curve.s_XLcurve.str_time[5]);
   XtVaSetValues(labtime5,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   compound=XmStringCreateSimple(cw->curve.s_XLcurve.str_time[6]);
   XtVaSetValues(labtime6,XmNlabelString,compound,NULL);
   XmStringFree(compound);

   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("AggTimeAxis: *******************************\n");
*/
}




/*******************************************************************/
/*                                                                 */
/*   Funzione che calcola la data corrente e trasforma il tempo di */
/*   simulazione corrente dal valore in secondi al formato: ore,   */
/*   minuti, secondi. Aggiorna l'interfaccia.                      */
/*                                                                 */
/*******************************************************************/
void AggDateTime(w)
Widget w;
{
int hh;
int mm;
int ss;
int resto;
int k1=3600;
int k2=60;
int giorno,mese,anno;
char datatempo[30];
char app[30];
XmString compound;
float tempoSim;
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;
/*
printf("AggDateTime: *******************************\n");
*/
   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);

   if(cw->curve.s_XLcurve.n_points[0]!=0) {
      tempoSim=cw->curve.s_XLcurve.t_end;
/*
printf("AggDateTime: tempoSim = %f\n",tempoSim);
*/
      hh = tempoSim/k1;
      resto = (int)tempoSim % k1;

      if( resto ) {
         mm = resto / k2;
         resto %= k2;
      }
      else
         mm = 0;
      ss = resto;
   
      sprintf(app,"%2d:%2d:%2d",hh,mm,ss);
   }
   else
      strcpy(app," 0: 0: 0");
   data(&giorno,&mese,&anno);
   sprintf(datatempo,"%2d.%2d.%2d ",giorno,mese,anno);
   strcat(datatempo,app);
/*
printf("AggDateTime: datatempo = %s\n",datatempo);
*/
   compound=XmStringCreateSimple(datatempo);
   XtVaSetValues(datetime,XmNlabelString,compound,NULL);
   XmStringFree(compound);
   UxTopLevelCurveContext = UxSaveCtx;
/*
printf("AggDateTime: *******************************\n");
*/
}




/*******************************************************************/
/*                                                                 */
/*   Funzione che prepara la struttura contenente i punti delle    */
/*   curve da tracciare.                                           */
/*                                                                 */
/*******************************************************************/
void prepare_draw(w)
Widget w;
{
float f_pix=0.0;
float pix_step,tempo_sim_curr;
float fatt_y[NUMMAXVAR],rapp,prod;
int x_pix=0,x_pixprec=0;      /* posizione in pixel del tempo sull-asse
                                 delle ascisse                          */
int y_pix;                    /* posizione in pixel delle ordinate      */
int i,ind_buf=0,j;
int draw_width_curr;
XPoint *pp[NUMMAXVAR];        /* percorrono i vettori points */
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;
/*
printf("prepare_draw: ******************************\n");
*/
   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext=UxContext=(_UxCtopLevelCurve *)UxGetContext(w);
/*
   Inizializzazione della variabile che dice se la cursor line
   interseca le curve.
*/
   cw->curve.s_XLcurve.curs_pos_OK=False;
/*
   Se il tempo iniziale e' uguale al tempo finale non disegna nessun punto.
*/
   if(cw->curve.s_XLcurve.t_start!=cw->curve.s_XLcurve.t_end) {
      pix_step=(float)width_curvefield/(float)cw->curve.s_XLcurve.rangeTimeStart;
      tempo_sim_curr=(cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.pos_end_visual].tempo.tempo-
                 cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.pos_start_visual].tempo.tempo);
      if(tempo_sim_curr>=(float)cw->curve.s_XLcurve.rangeTimeStart) 
         draw_width_curr=width_curvefield;
      else
         draw_width_curr=(int)(((float)width_curvefield*tempo_sim_curr)/
                               (float)cw->curve.s_XLcurve.rangeTimeStart);
      cw->curve.s_XLcurve.x_pix_start=width_curvefield-draw_width_curr; 
/*
printf("prepare_draw: x_pix_start = %d\n\tcurs_pos_pixel = %d\n",
       cw->curve.s_XLcurve.x_pix_start,curs_pos_pixel);
printf("prepare_draw: draw_width_curr = %d\n\twidth_curvefield = %d\n",
        draw_width_curr,width_curvefield);
*/
      ind_buf=cw->curve.s_XLcurve.pos_start_visual;
      for(i=0;i<NUMMAXVAR;i++) {
         if(cw->curve.s_XLcurve.ind_var_sel[i]!=-1) {
/*
   La variabile i-esima e' stata configurata. Inizializzazione.
*/
            if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERCENT)==0)
               fatt_y[i]=((float)height_curvefield/
                   (cw->curve.s_XLcurve.min_max_perc.max-cw->curve.s_XLcurve.min_max_perc.min));
            else if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0)
               fatt_y[i]=((float)height_curvefield/
                   ((cw->curve.s_XLcurve.min_max_perc.max-cw->curve.s_XLcurve.min_max_perc.min)/100.0));
            else
               fatt_y[i]=((float)height_curvefield/
                  (cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min));

	    pp[i]=cw->curve.s_XLcurve.points_to_draw[i];
            cw->curve.s_XLcurve.n_points[i]=0;
/*
   Per diversi tipi di unita' di misura si avranno scalamenti diversi
*/
            if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERCENT)==0)
               y_pix=(int)(fatt_y[i]*
                 (cw->curve.s_XLcurve.min_max_perc.max-cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
            else if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0)
               y_pix=(int)(fatt_y[i]*
                 ((cw->curve.s_XLcurve.min_max_perc.max/100.0)-cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
            else
               y_pix=(int)(fatt_y[i]*
                  (cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));

            if(cw->curve.s_XLcurve.x_pix_start==0) {
/*
   Lo spazio di visualizzazione e' pieno. Carico come primo punto quello
   con ascissa x=0. 
*/
               pp[i]->x=x_pix;
               pp[i]->y=y_pix;
               pp[i]++;
               cw->curve.s_XLcurve.n_points[i]++;
               if(x_pix==curs_pos_pixel) {
                  cw->curve.s_XLcurve.curs_pos_OK=True;

                  if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) {
                     rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/100.0;
                     cw->curve.s_XLcurve.curs_val_displ[i]= (cw->curve.s_XLcurve.min_max_oper[i].min + (rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                  }
                  else if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0) {  
                     rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/1.0;
                     cw->curve.s_XLcurve.curs_val_displ[i]= (cw->curve.s_XLcurve.min_max_oper[i].min + (rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                  }
                  else {
                     cw->curve.s_XLcurve.curs_val_displ[i]= cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i];
                  }
                  cw->curve.s_XLcurve.cur_line_pos_f22=ind_buf;
               }
            }
            else {
/*
   Spazio di visualizzazione non pieno. Carico come primo punto quello
   di ascissa x=x_pix_start.
*/
               pp[i]->x=cw->curve.s_XLcurve.x_pix_start;
               pp[i]->y=y_pix;
               pp[i]++;
               cw->curve.s_XLcurve.n_points[i]++;
               if(cw->curve.s_XLcurve.x_pix_start==curs_pos_pixel) {
                  cw->curve.s_XLcurve.curs_pos_OK=True;

                  if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) {
                     rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/100.0;
                     cw->curve.s_XLcurve.curs_val_displ[i]= (cw->curve.s_XLcurve.min_max_oper[i].min + (rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                  }
                  else if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0) {  
                     rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/1.0;
                     cw->curve.s_XLcurve.curs_val_displ[i]= (cw->curve.s_XLcurve.min_max_oper[i].min + (rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                  }
                  else {
                     cw->curve.s_XLcurve.curs_val_displ[i]= cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i];
                  }
                  cw->curve.s_XLcurve.cur_line_pos_f22=ind_buf;
               }
               else {
                  j=0;
                  while((j<cw->curve.s_XLcurve.x_pix_start)&&(j!=curs_pos_pixel))
                     j++;
                  if(cw->curve.s_XLcurve.curs_pos_OK==True)
                     cw->curve.s_XLcurve.curs_pos_OK=False;
                  cw->curve.s_XLcurve.cur_line_pos_f22=-1;
               }
	    }
	 } 
      }
      ind_buf++;
      x_pixprec=cw->curve.s_XLcurve.x_pix_start;
/*
   Caricamento del vettore dei punti da disegnare.
*/
      while(x_pix<width_curvefield && ind_buf<=cw->curve.s_XLcurve.pos_end_visual) {
         f_pix=pix_step*(cw->curve.s_XLcurve.buffer_f22[ind_buf].tempo.tempo -
                  cw->curve.s_XLcurve.buffer_f22[cw->curve.s_XLcurve.pos_start_visual].tempo.tempo);
         x_pix=cw->curve.s_XLcurve.x_pix_start+(int)f_pix;
	 if(x_pix==x_pixprec) {
	    ind_buf++;
	 }
	 else {
	    for(i=0;i<NUMMAXVAR;i++) {
	       if(cw->curve.s_XLcurve.ind_var_sel[i]!=-1) {
/*
   Si determina il valore dell'ordinata in pixel e l'ultimo valore
   visualizzato in unita' ingegneristiche. 
*/
                  if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) {
                     y_pix=(int)(fatt_y[i]*
                       (cw->curve.s_XLcurve.min_max_perc.max-cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                     rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/100.0;
                     prod=rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i];
                     cw->curve.s_XLcurve.last_val_displ[i]= (cw->curve.s_XLcurve.min_max_oper[i].min + prod);
                  }
                  else if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0) {
                     y_pix=(int)(fatt_y[i]*
                       ((cw->curve.s_XLcurve.min_max_perc.max/100.0)-cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                     rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/1.0;
                     prod=rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i];
                     cw->curve.s_XLcurve.last_val_displ[i]= (cw->curve.s_XLcurve.min_max_oper[i].min + prod);
                  }
                  else {
                     y_pix=(int)(fatt_y[i]*
                       (cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                     cw->curve.s_XLcurve.last_val_displ[i]=cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i];
                  }

                  pp[i]->x=x_pix;
                  pp[i]->y=y_pix;
                  pp[i]++;
                  cw->curve.s_XLcurve.n_points[i]++;

                  if((curs_pos_pixel>x_pixprec)&&(curs_pos_pixel<=x_pix)) {
                     cw->curve.s_XLcurve.curs_pos_OK=True;

                     if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERCENT)==0) {
                        rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/100.0;
                        cw->curve.s_XLcurve.curs_val_displ[i]=(cw->curve.s_XLcurve.min_max_oper[i].min + (rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                     }
                     else if(strcmp(cw->curve.s_XLcurve.tipo_unit[i],PERUNIT)==0) {  
                        rapp=(cw->curve.s_XLcurve.min_max_oper[i].max-cw->curve.s_XLcurve.min_max_oper[i].min)/1.0;
                        cw->curve.s_XLcurve.curs_val_displ[i]= (cw->curve.s_XLcurve.min_max_oper[i].min + (rapp*cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i]));
                     }
                     else {
                        cw->curve.s_XLcurve.curs_val_displ[i]= cw->curve.s_XLcurve.buffer_f22[ind_buf].mis[i];
                     }
                     cw->curve.s_XLcurve.cur_line_pos_f22=ind_buf;
                  }
               }
	    }	
	    ind_buf++;
            x_pixprec=x_pix;
         }
      }
/*
printf("prepare_draw: cur_line_pos_f22 = %d\n",cw->curve.s_XLcurve.cur_line_pos_f22);
*/
   }
   UxTopLevelCurveContext = UxSaveCtx; 
/*
printf("prepare_draw: ******************************\n");
*/
}



/*******************************************************************/
/*                                                                 */
/*   Funzione che crea il bitmap da utilizzare per il 'flooding'   */
/*   di una curva rappresentata.                                   */
/*                                                                 */
/*******************************************************************/
Boolean create_read_stipple(w)
Widget w;
{
unsigned int width,height;
int value;
int x_hot,y_hot;
XlCurveWidget cw = (XlCurveWidget)w;

printf("create_read_stipple: ******************************\n");

   value=XReadBitmapFile(XtDisplay(w),RootWindow(XtDisplay(w),DefaultScreen(XtDisplay(w))),
      cw->curve.s_XLcurve.flood_name,&width,&height,&cw->curve.s_XLcurve.stip_fluctua,&x_hot,&y_hot);

   if(value!=BitmapSuccess) {
      printf("create_read_stipple: XReadBitmapFile ERROR!!!\n");
      return(False);
   }

printf("create_read_stipple: ******************************\n");

return(True);
}



/*******************************************************************/
/*                                                                 */
/*   Funzione che impone come ultimo tempo da visualizzare quello  */
/*   impostato nell'interfaccia 'dateShell'.                       */
/*                                                                 */
/*******************************************************************/
void manageSeldata(w)
Widget w;
{

int i;
int time_sec;
XlCurveWidget cw = (XlCurveWidget)w;

printf("manageSeldata: ******************************\n");

   cw->curve.s_XLcurve.seldata_OK=False;
/*
	Conversione del tempo inserito da ore-minuti a secondi.
*/
   time_sec=((cw->curve.s_XLcurve.minute_sel*60)+(cw->curve.s_XLcurve.hour_sel*3600));

printf("manageSeldata: hour_sel=%d\tminute_sel=%d\ttime_sec=%d\n",
        cw->curve.s_XLcurve.hour_sel,cw->curve.s_XLcurve.minute_sel,time_sec);

/*
	Ricerca del tempo finale da visualizzare.
*/
   if(time_sec<=cw->curve.s_XLcurve.t_start) {
      cw->curve.s_XLcurve.pos_end_visual=0;
      cw->curve.s_XLcurve.pos_start_visual=0;
      if(static_status==False) {
         static_status=True;
         PushedEffect(flow);
      }
   }
   else {
      if(time_sec>=cw->curve.s_XLcurve.t_end) {
         cw->curve.s_XLcurve.pos_end_visual=cw->curve.s_XLcurve.n_last_val;
         if(static_status==True) {
            static_status=False;
            PushedEffect(flow);
         }
      }
      else {
         i=0;
         while(time_sec>cw->curve.s_XLcurve.buffer_f22[i].tempo.tempo)
            i++;
         cw->curve.s_XLcurve.pos_end_visual=i;
         if(static_status==False) {
            static_status=True;
            PushedEffect(flow);
         }

printf("manageSeldata: pos_end_visual=%d\n",i);

      }
      cw->curve.s_XLcurve.pos_start_visual=calc_start_pos(cw->curve.s_XLcurve.buffer_f22,
          cw->curve.s_XLcurve.rangeTimeStart,cw->curve.s_XLcurve.pos_end_visual);
   }
   RefreshXlCurve(cw);

printf("manageSeldata: ******************************\n");

}



/*******************************************************************/
/*                                                                 */
/*   Funzione che controlla la stringa 'VarInfo' contenente le     */
/*   informazioni relative ad una variabile: codice lego, codice   */
/*   KKS, descrizione, limite inf, limite sup, unita' di misura e  */
/*   e tipo di unita' di misura.                                   */
/*   Estrae le singole informazioni separate da '|' e verifica la  */
/*   loro correttezza.                                             */ 
/*                                                                 */
/*******************************************************************/
Boolean checkVarInfo(w,ind_var_err)
Widget w;
int *ind_var_err;
{

int i,j,k,length;
char str_app[MAXLENINFOVAR];
char *p,*str_end[NUMINFOVAR],*str_app1;
Boolean error=False;
XlCurveWidget cw = (XlCurveWidget)w;

printf("checkVarInfo: ENTRATO!!!\n");

   for(i=0;i<cw->curve.s_XLcurve.numCurves;i++) {
      if(strcmp(cw->curve.s_XLcurve.varInfoStart[i],"")!=0) {
         strcpy(str_app,cw->curve.s_XLcurve.varInfoStart[i]);

printf("checkVarInfo: str[%d]=%s\n",i,cw->curve.s_XLcurve.varInfoStart[i]);

         j=0;
         while ((str_app1=strchr(str_app, '|'))!=NULL) {
            length=strlen(str_app)-strlen(str_app1);
            if(length==0) {
               *ind_var_err=i;
               return(False);
            }
            strcpy(str_app,(str_app1+1));
            j++;
         }
         if((j==(NUMINFOVAR-1)) || (j==NUMINFOVAR)) {
/*
   La stringa con le informazioni della variabile corrente e' completa.
   Si controlla la correttezza delle singole informazioni. 
*/
            j=0;
            strcpy(str_app,cw->curve.s_XLcurve.varInfoStart[i]);
            while((str_app1=strchr(str_app, '|')) != NULL) {
               length=strlen(str_app)-strlen(str_app1);
	       p=NULL;
               p=(char *)calloc(length+1,sizeof(char));
               strncpy(p, str_app, length);
               p[length] = '\0';
               str_end[j]=p;
               j++;
               strcpy(str_app,(str_app1+1));
            }
            if((strlen(str_end[0])>CHAR_LEGO_COD))
               error=True;
            if((VerifStrFloat(str_end[3])==False)||(VerifStrFloat(str_end[4])==False))
               error=True;
            if(atof(str_end[4])<=atof(str_end[3]))
               error=True;
/*
   Si controlla il tipo di unita' di misura.
*/
            if(j==NUMINFOVAR) {
               if((strcmp(str_end[NUMINFOVAR-1],PERCENT)!=0) &&
                  (strcmp(str_end[NUMINFOVAR-1],PERUNIT)!=0) &&
                  (strcmp(str_end[NUMINFOVAR-1],ENGINEER)!=0)) {
                  printf("XlCurve: tipo unita' di misura ERRATO!!!\n");
                  error=True; 
               }
            }

            if(error==True) {
               for(k=0;k<j;k++)
                  free(str_end[k]);
               *ind_var_err=i;
               return(False);
            }
         }
         else {
/*
   Il numero di informazioni specificate e' minore di NUMINFOVAR-1
*/
            *ind_var_err=i;
            return(False);
         }
      }
   }
   *ind_var_err=-1;

printf("checkVarInfo: USCITO!!!\n");

return(True);
}



/*******************************************************************/
/*                                                                 */
/*   Funzione che verifica la correttezza della stringa che        */
/*   contiene gli intervalli di tempo per lo zoom.                 */
/*                                                                 */
/*******************************************************************/
Boolean checkRangeZoom(w)
Widget w;
{
int length,i;
int curr_range;
Boolean range_def_found=False;
char str_app[100],str_single_info[10],*str_app1;
XlCurveWidget cw = (XlCurveWidget)w;

   strcpy(str_app,cw->curve.s_XLcurve.rangeTimeZoom);
   i=0;
   if(strcmp(str_app,"")!=0) {   
      while((str_app1=strchr(str_app, '|')) != NULL) {
         length=strlen(str_app)-strlen(str_app1);
         if(length==0)
            return(False);
         strncpy(str_single_info, str_app, length);
         str_single_info[length] = '\0';
         if(VerifStrInt(str_single_info)==False)
            return(False);
         if(i==N_MAX_RANGE_Z)
            return(False);
         curr_range=atoi(str_single_info);
         if(curr_range==0)
            return(False);
         if(curr_range==cw->curve.s_XLcurve.rangeTimeStart)
            range_def_found=True;
         i++;
         strcpy(str_app,(str_app1+1));
      } 
      if(strlen(str_app)!=0)
         return(False);
      if(range_def_found==False)
         return(False);
   }
   else {
      return(False);
   }

return(True);

}


/***************************************************************
        Funzione che converte il valore di una stringa
        in un valore di pixel corrispondente;
***************************************************************/
Boolean convStringToPix(Widget ogget,char *Stringa,Pixel *Pix)
{
        unsigned long piani[1];
        Colormap colmap;
        XColor xcol;


        colmap = DefaultColormap ( XtDisplay(ogget),
                        DefaultScreen (XtDisplay(ogget)) );
        if ( !XParseColor (XtDisplay(ogget) , colmap , Stringa , &xcol) )
           {
           printf("Errore XParseColor: colore inesistente\n");
           return(False);
           }
        if(!XAllocColor(XtDisplay(ogget),colmap,&xcol))
           {
           printf("Errore XAllocColor: impossibile allocare un nuovo colore\n");
           return(False);
           }
        *Pix = xcol.pixel;
        return(True);
}



/***************************************************************
        Funzione che converte il valore del pixel della
        risorsa passata come parametro nella stringa
        contenente le componenti Red,Green,Blue
***************************************************************/
char *convPixToString(Widget wid,char *NomeRes)
{
        Arg args[3];
        XColor Colore;
        Colormap cmap;
        char *vstring;
        Display *Displ;

        /*  Ricavo il display dal widget passato come parametro */
        Displ = XtDisplay (wid);
        XtSetArg (args[0], NomeRes, &Colore.pixel);
        XtSetArg (args[1], XmNcolormap, &cmap);
        XtGetValues (wid, args, 2);
        XQueryColor (Displ, cmap, &Colore);
        vstring = (char *)XtCalloc (15, sizeof(char));
        sprintf (vstring,"#%04x%04x%04x",Colore.red,Colore.green,Colore.blue);
        return (vstring);
}



/************************************************
   Controlla se la stringa in input e' un float
************************************************/
Boolean VerifStrFloat(char *Stringa)
{
int i,lunghezza,Ritorno1,Ritorno2;
float Valore;

lunghezza = strlen (Stringa);
for (i=0;i<lunghezza;i++)
 if ( (!isdigit(Stringa[i])) && (Stringa[i] != '.') && (Stringa[i] != '-') &&
        (Stringa[i] != 'e') && (Stringa[i] != 'E') && (Stringa[i] != '+') )
   return (False);
Ritorno1 = sscanf (Stringa,"%e",&Valore);
Ritorno2 = sscanf (Stringa,"%f",&Valore);
if ( (Ritorno1==0) || (Ritorno2==0) )
  return (False);
return (True);
}



/*******************************************************************/
/*                                                                 */
/*   Controlla se la stringa in input e' un int                    */
/*                                                                 */
/*******************************************************************/
Boolean VerifStrInt(char *Stringa)
{
int i,lunghezza;

lunghezza = strlen (Stringa);
for (i=0;i<lunghezza;i++)
 if (!isdigit(Stringa[i])) return (False);
return (True);
}




/*******************************************************************/
/*                                                                 */
/*   Funzione che attiva le funzioni per il refresh dell'oggetto   */
/*   XlCurve.                                                      */
/*                                                                 */
/*******************************************************************/
void RefreshXlCurve(w)
Widget w;
{
_UxCtopLevelCurve  *UxSaveCtx, *UxContext;
XlCurveWidget cw = (XlCurveWidget)w;

   UxSaveCtx = UxTopLevelCurveContext;
   UxTopLevelCurveContext = UxContext = (_UxCtopLevelCurve *) UxGetContext(w);

   XClearArea(XtDisplay(cw),XtWindow(cw->curve.contesto->UxcurveField),0,0,0,0,True);
   DrawGridCurve(cw);
   DrawCurves(cw);
   AggTimeAxis(cw);
   AggLegendValues(cw);

   UxTopLevelCurveContext = UxSaveCtx;

}
