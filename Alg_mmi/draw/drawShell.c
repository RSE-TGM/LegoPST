
/*******************************************************************************
       drawShell.c
       (Generated from interface file drawShell.i)
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeBG.h>
#include <Xm/CascadeB.h>
#include <Xm/ToggleB.h>
#include <Xm/Separator.h>
#include <Xm/PushB.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/DrawnB.h>
#include <Xm/Frame.h>
#include <Xm/DrawingA.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
        Inizio sezione per SCCS
*/
/*
   modulo drawShell.i
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)drawShell.i	5.1
*/
/*
        Variabile per identificazione della versione
*/
/*
        Fine sezione per SCCS
*/
#include <draw.h>
#ifndef DESIGN_TIME
#include <math.h>
#endif
#include "res_edit.h"
extern Boolean saving;
extern swidget drawFsBD;
extern swidget fileSelectionBox;
extern swidget drawExitD;
extern swidget create_drawFsBD();
extern swidget create_drawExitD();
extern swidget create_SaveSelection();
extern char *nome_file_draw;
extern Widget FinestraDraw;

extern void reset_drawing_background(Widget );

int curr_line_style;
int curr_line_width;
int curr_step=1;
int grid_on=0;
Window win_step; /* window su cui effettuare lo step */
#ifndef ON_PAGEDIT
Widget FinestraDraw;
#endif


/*******************************************************************************
       The definition of the context structure:
       If you create multiple copies of your interface, the context
       structure ensures that your callbacks use the variables for the
       correct copy.

       For each swidget in the interface, each argument to the Interface
       function, and each variable in the Interface Specific section of the
       Declarations Editor, there is an entry in the context structure.
       and a #define.  The #define makes the variable name refer to the
       corresponding entry in the context structure.
*******************************************************************************/

typedef	struct
{
	Widget	UxmainWindow;
	Widget	UxworkAreaMgr;
	Widget	UxworkFrame;
	Widget	UxworkDrArea;
	Widget	Uxdrawline_bt;
	Widget	Uxdrawrect_bt;
	Widget	Uxdrawcircle_bt;
	Widget	Uxdrawpoli_bt;
	Widget	Uxdrawfree_bt;
	Widget	Uxframe1;
	Widget	UxdrawingArea1;
	Widget	UxsdrawnButton1;
	Widget	UxsdrawnButton2;
	Widget	UxsdrawnButton3;
	Widget	UxsdrawnButton4;
	Widget	UxsdrawnButton5;
	Widget	UxsdrawnButton6;
	Widget	UxsdrawnButton7;
	Widget	UxsdrawnButton8;
	Widget	UxsdrawnButton9;
	Widget	UxsdrawnButton10;
	Widget	UxsdrawnButton11;
	Widget	UxsdrawnButton12;
	Widget	UxsdrawnButton13;
	Widget	UxsdrawnButton14;
	Widget	UxsdrawnButton15;
	Widget	UxsdrawnButton16;
	Widget	Uxlabel2;
	Widget	Uxframe2;
	Widget	UxdrawingArea2;
	Widget	UxdrawnButton1;
	Widget	UxdrawnButton2;
	Widget	UxdrawnButton3;
	Widget	UxdrawnButton4;
	Widget	UxdrawnButton5;
	Widget	UxdrawnButton6;
	Widget	UxdrawnButton7;
	Widget	UxdrawnButton8;
	Widget	UxdrawnButton9;
	Widget	UxdrawnButton10;
	Widget	UxdrawnButton11;
	Widget	UxdrawnButton12;
	Widget	UxdrawnButton13;
	Widget	UxdrawnButton14;
	Widget	UxdrawnButton15;
	Widget	UxdrawnButton16;
	Widget	Uxlabel1;
	Widget	Uxarrow_bt;
	Widget	Uxdrawarc_bt;
	Widget	Uxzoom_bt;
	Widget	UxDrawDemoMainMenu;
	Widget	UxfilePane;
	Widget	UxsaveButton;
	Widget	UxsaveAsButton;
	Widget	UxfilePane_b8;
	Widget	UxprtButton;
	Widget	UxprtSetupButton;
	Widget	UxfilePane_b7;
	Widget	UxexitButton;
	Widget	UxfileCascade;
	Widget	UxeditPane;
	Widget	UxundoButton;
	Widget	UxdupButton;
	Widget	UxcutButton;
	Widget	UxcopyButton;
	Widget	UxpasteButton;
	Widget	UxdeleteButton;
	Widget	UxeditPane_b12;
	Widget	UxtopButton;
	Widget	UxbottomButton;
	Widget	UxeditPane_b13;
	Widget	UxgroupButton;
	Widget	UxungroupButton;
	Widget	UxeditCascade;
	Widget	UxviewPane;
	Widget	Uxview_normal;
	Widget	UxviewPane_b20;
	Widget	Uxview_200;
	Widget	Uxview_400;
	Widget	Uxview_800;
	Widget	UxviewPane_b6;
	Widget	Uxgrid_pb;
	Widget	UxviewCascade;
	Widget	UxdrawPane;
	Widget	Uxfilled_bt;
	Widget	UxdrawPane_b9;
	Widget	UxlineStylePane;
	Widget	Uxsolid_bt;
	Widget	Uxdashed_bt;
	Widget	Uxlinedot_bt;
	Widget	Uxline_bt;
	Widget	UxlineWidthPane;
	Widget	Uxthiny_bt;
	Widget	Uxpix_1_bt;
	Widget	Uxpix_2_bt;
	Widget	Uxpix_3_bt;
	Widget	Uxpix_6_bt;
	Widget	Uxwidth_other;
	Widget	Uxline_width_bt;
	Widget	UxdrawPane_b12;
	Widget	Uxsnap_Pane;
	Widget	Uxsn5;
	Widget	Uxsn10;
	Widget	Uxsn15;
	Widget	Uxsnother;
	Widget	Uxsn1;
	Widget	Uxsnap_casc;
	Widget	UxdrawPane_b15;
	Widget	UxrotatePane;
	Widget	UxDrawDemoMainMenu_p14_b1;
	Widget	UxDrawDemoMainMenu_p14_b2;
	Widget	UxrotatePane_b5;
	Widget	UxDrawDemoMainMenu_p14_b3;
	Widget	UxDrawDemoMainMenu_p14_b4;
	Widget	UxdrawPane_b16;
	Widget	UxpullDownMenu_top_b1;
	Widget	UxhelpPane;
	Widget	UxonContextButton;
	Widget	UxonWindowButton;
	Widget	UxonKeysButton;
	Widget	UxonHelpButton;
	Widget	UxhelpPane_b8;
	Widget	UxindexButton;
	Widget	UxtutorialButton;
	Widget	UxhelpPane_b9;
	Widget	UxonVersionButton;
	Widget	UxhelpCascade;
	Widget	UxcolorPane;
	Widget	Uxedit_colors_bt;
	Widget	UxDrawDemoMainMenu_top_b4;
	swidget	Uxdraw_wid;
	unsigned char	Uxstr_title[150];
	XrmDatabase	UxRisDbColori;
	swidget	Uxvett_colori[32];
	swidget	*Uxvett_fill;
	swidget	*Uxvett_border;
	swidget	Uxvett_draw[9];
	swidget	Uxpassed_wid;
	unsigned char	*Uxnome;
} _UxCdrawShell;

static _UxCdrawShell           *UxDrawShellContext;
#define mainWindow              UxDrawShellContext->UxmainWindow
#define workAreaMgr             UxDrawShellContext->UxworkAreaMgr
#define workFrame               UxDrawShellContext->UxworkFrame
#define workDrArea              UxDrawShellContext->UxworkDrArea
#define drawline_bt             UxDrawShellContext->Uxdrawline_bt
#define drawrect_bt             UxDrawShellContext->Uxdrawrect_bt
#define drawcircle_bt           UxDrawShellContext->Uxdrawcircle_bt
#define drawpoli_bt             UxDrawShellContext->Uxdrawpoli_bt
#define drawfree_bt             UxDrawShellContext->Uxdrawfree_bt
#define frame1                  UxDrawShellContext->Uxframe1
#define drawingArea1            UxDrawShellContext->UxdrawingArea1
#define sdrawnButton1           UxDrawShellContext->UxsdrawnButton1
#define sdrawnButton2           UxDrawShellContext->UxsdrawnButton2
#define sdrawnButton3           UxDrawShellContext->UxsdrawnButton3
#define sdrawnButton4           UxDrawShellContext->UxsdrawnButton4
#define sdrawnButton5           UxDrawShellContext->UxsdrawnButton5
#define sdrawnButton6           UxDrawShellContext->UxsdrawnButton6
#define sdrawnButton7           UxDrawShellContext->UxsdrawnButton7
#define sdrawnButton8           UxDrawShellContext->UxsdrawnButton8
#define sdrawnButton9           UxDrawShellContext->UxsdrawnButton9
#define sdrawnButton10          UxDrawShellContext->UxsdrawnButton10
#define sdrawnButton11          UxDrawShellContext->UxsdrawnButton11
#define sdrawnButton12          UxDrawShellContext->UxsdrawnButton12
#define sdrawnButton13          UxDrawShellContext->UxsdrawnButton13
#define sdrawnButton14          UxDrawShellContext->UxsdrawnButton14
#define sdrawnButton15          UxDrawShellContext->UxsdrawnButton15
#define sdrawnButton16          UxDrawShellContext->UxsdrawnButton16
#define label2                  UxDrawShellContext->Uxlabel2
#define frame2                  UxDrawShellContext->Uxframe2
#define drawingArea2            UxDrawShellContext->UxdrawingArea2
#define drawnButton1            UxDrawShellContext->UxdrawnButton1
#define drawnButton2            UxDrawShellContext->UxdrawnButton2
#define drawnButton3            UxDrawShellContext->UxdrawnButton3
#define drawnButton4            UxDrawShellContext->UxdrawnButton4
#define drawnButton5            UxDrawShellContext->UxdrawnButton5
#define drawnButton6            UxDrawShellContext->UxdrawnButton6
#define drawnButton7            UxDrawShellContext->UxdrawnButton7
#define drawnButton8            UxDrawShellContext->UxdrawnButton8
#define drawnButton9            UxDrawShellContext->UxdrawnButton9
#define drawnButton10           UxDrawShellContext->UxdrawnButton10
#define drawnButton11           UxDrawShellContext->UxdrawnButton11
#define drawnButton12           UxDrawShellContext->UxdrawnButton12
#define drawnButton13           UxDrawShellContext->UxdrawnButton13
#define drawnButton14           UxDrawShellContext->UxdrawnButton14
#define drawnButton15           UxDrawShellContext->UxdrawnButton15
#define drawnButton16           UxDrawShellContext->UxdrawnButton16
#define label1                  UxDrawShellContext->Uxlabel1
#define arrow_bt                UxDrawShellContext->Uxarrow_bt
#define drawarc_bt              UxDrawShellContext->Uxdrawarc_bt
#define zoom_bt                 UxDrawShellContext->Uxzoom_bt
#define DrawDemoMainMenu        UxDrawShellContext->UxDrawDemoMainMenu
#define filePane                UxDrawShellContext->UxfilePane
#define saveButton              UxDrawShellContext->UxsaveButton
#define saveAsButton            UxDrawShellContext->UxsaveAsButton
#define filePane_b8             UxDrawShellContext->UxfilePane_b8
#define prtButton               UxDrawShellContext->UxprtButton
#define prtSetupButton          UxDrawShellContext->UxprtSetupButton
#define filePane_b7             UxDrawShellContext->UxfilePane_b7
#define exitButton              UxDrawShellContext->UxexitButton
#define fileCascade             UxDrawShellContext->UxfileCascade
#define editPane                UxDrawShellContext->UxeditPane
#define undoButton              UxDrawShellContext->UxundoButton
#define dupButton               UxDrawShellContext->UxdupButton
#define cutButton               UxDrawShellContext->UxcutButton
#define copyButton              UxDrawShellContext->UxcopyButton
#define pasteButton             UxDrawShellContext->UxpasteButton
#define deleteButton            UxDrawShellContext->UxdeleteButton
#define editPane_b12            UxDrawShellContext->UxeditPane_b12
#define topButton               UxDrawShellContext->UxtopButton
#define bottomButton            UxDrawShellContext->UxbottomButton
#define editPane_b13            UxDrawShellContext->UxeditPane_b13
#define groupButton             UxDrawShellContext->UxgroupButton
#define ungroupButton           UxDrawShellContext->UxungroupButton
#define editCascade             UxDrawShellContext->UxeditCascade
#define viewPane                UxDrawShellContext->UxviewPane
#define view_normal             UxDrawShellContext->Uxview_normal
#define viewPane_b20            UxDrawShellContext->UxviewPane_b20
#define view_200                UxDrawShellContext->Uxview_200
#define view_400                UxDrawShellContext->Uxview_400
#define view_800                UxDrawShellContext->Uxview_800
#define viewPane_b6             UxDrawShellContext->UxviewPane_b6
#define grid_pb                 UxDrawShellContext->Uxgrid_pb
#define viewCascade             UxDrawShellContext->UxviewCascade
#define drawPane                UxDrawShellContext->UxdrawPane
#define filled_bt               UxDrawShellContext->Uxfilled_bt
#define drawPane_b9             UxDrawShellContext->UxdrawPane_b9
#define lineStylePane           UxDrawShellContext->UxlineStylePane
#define solid_bt                UxDrawShellContext->Uxsolid_bt
#define dashed_bt               UxDrawShellContext->Uxdashed_bt
#define linedot_bt              UxDrawShellContext->Uxlinedot_bt
#define line_bt                 UxDrawShellContext->Uxline_bt
#define lineWidthPane           UxDrawShellContext->UxlineWidthPane
#define thiny_bt                UxDrawShellContext->Uxthiny_bt
#define pix_1_bt                UxDrawShellContext->Uxpix_1_bt
#define pix_2_bt                UxDrawShellContext->Uxpix_2_bt
#define pix_3_bt                UxDrawShellContext->Uxpix_3_bt
#define pix_6_bt                UxDrawShellContext->Uxpix_6_bt
#define width_other             UxDrawShellContext->Uxwidth_other
#define line_width_bt           UxDrawShellContext->Uxline_width_bt
#define drawPane_b12            UxDrawShellContext->UxdrawPane_b12
#define snap_Pane               UxDrawShellContext->Uxsnap_Pane
#define sn5                     UxDrawShellContext->Uxsn5
#define sn10                    UxDrawShellContext->Uxsn10
#define sn15                    UxDrawShellContext->Uxsn15
#define snother                 UxDrawShellContext->Uxsnother
#define sn1                     UxDrawShellContext->Uxsn1
#define snap_casc               UxDrawShellContext->Uxsnap_casc
#define drawPane_b15            UxDrawShellContext->UxdrawPane_b15
#define rotatePane              UxDrawShellContext->UxrotatePane
#define DrawDemoMainMenu_p14_b1 UxDrawShellContext->UxDrawDemoMainMenu_p14_b1
#define DrawDemoMainMenu_p14_b2 UxDrawShellContext->UxDrawDemoMainMenu_p14_b2
#define rotatePane_b5           UxDrawShellContext->UxrotatePane_b5
#define DrawDemoMainMenu_p14_b3 UxDrawShellContext->UxDrawDemoMainMenu_p14_b3
#define DrawDemoMainMenu_p14_b4 UxDrawShellContext->UxDrawDemoMainMenu_p14_b4
#define drawPane_b16            UxDrawShellContext->UxdrawPane_b16
#define pullDownMenu_top_b1     UxDrawShellContext->UxpullDownMenu_top_b1
#define helpPane                UxDrawShellContext->UxhelpPane
#define onContextButton         UxDrawShellContext->UxonContextButton
#define onWindowButton          UxDrawShellContext->UxonWindowButton
#define onKeysButton            UxDrawShellContext->UxonKeysButton
#define onHelpButton            UxDrawShellContext->UxonHelpButton
#define helpPane_b8             UxDrawShellContext->UxhelpPane_b8
#define indexButton             UxDrawShellContext->UxindexButton
#define tutorialButton          UxDrawShellContext->UxtutorialButton
#define helpPane_b9             UxDrawShellContext->UxhelpPane_b9
#define onVersionButton         UxDrawShellContext->UxonVersionButton
#define helpCascade             UxDrawShellContext->UxhelpCascade
#define colorPane               UxDrawShellContext->UxcolorPane
#define edit_colors_bt          UxDrawShellContext->Uxedit_colors_bt
#define DrawDemoMainMenu_top_b4 UxDrawShellContext->UxDrawDemoMainMenu_top_b4
#define draw_wid                UxDrawShellContext->Uxdraw_wid
#define str_title               UxDrawShellContext->Uxstr_title
#define RisDbColori             UxDrawShellContext->UxRisDbColori
#define vett_colori             UxDrawShellContext->Uxvett_colori
#define vett_fill               UxDrawShellContext->Uxvett_fill
#define vett_border             UxDrawShellContext->Uxvett_border
#define vett_draw               UxDrawShellContext->Uxvett_draw
#define passed_wid              UxDrawShellContext->Uxpassed_wid
#define nome                    UxDrawShellContext->Uxnome


Widget	drawShell;

/*******************************************************************************
       The following are translation tables.
*******************************************************************************/

static char	*lineTable = "#replace\n\
<Btn1Down>:first_point_line()\n\
<Btn1Motion>:draw_draget()\n\
<Btn1Up>:end_draw_draget()\n\
<Key>F5:draw_delete()\n\
<Key>Left:move_tasti()\n\
<Key>Right:move_tasti()\n\
<Key>Up:move_tasti()\n\
<Key>Down:move_tasti()\n";

static char	*rectTable = "#replace\n\
<Btn1Down>:first_point_rect()\n\
<Btn1Motion>:draw_draget()\n\
<Btn1Up>:end_draw_draget()\n\
<Key>F5:draw_delete()\n\
<Key>Left:move_tasti()\n\
<Key>Right:move_tasti()\n\
<Key>Up:move_tasti()\n\
<Key>Down:move_tasti()\n";

static char	*circleTable = "#replace\n\
<Btn1Down>:first_point_circle()\n\
<Btn1Motion>:draw_draget()\n\
<Btn1Up>:end_draw_draget()\n\
<Key>F5:draw_delete()\n\
<Key>Left:move_tasti()\n\
<Key>Right:move_tasti()\n\
<Key>Up:move_tasti()\n\
<Key>Down:move_tasti()\n";

static char	*arcTable = "#replace\n\
<Btn1Down>:first_point_arc()\n\
<Btn1Motion>:draw_draget()\n\
<Btn1Up>:end_draw_draget()\n\
<Key>F5:draw_delete()\n\
<Key>Left:move_tasti()\n\
<Key>Right:move_tasti()\n\
<Key>Up:move_tasti()\n\
<Key>Down:move_tasti()\n";

static char	*poliTable = "#replace\n\
<Btn1Down>:first_point_poli()\n\
<Btn2Down>:end_draw_draget()\n\
<Key>F5:draw_delete()\n\
<Motion>:draw_draget()\n\
<Key>Left:move_tasti()\n\
<Key>Right:move_tasti()\n\
<Key>Up:move_tasti()\n\
<Key>Down:move_tasti()\n\
<Btn3Down>:end_draw_draget()\n";

static char	*textTable = "#replace\n\
<Btn1Down>:nothing()\n\
<Btn1Motion>:nothing()\n\
<Btn1Up>:first_text()\n\
<Key>Return:next_line()\n\
<Key>:draw_text()\n";

static char	*pickTable = "#replace\n\
<Key>F5:draw_delete()\n\
~Shift<Btn1Down>:start_pick()\n\
<Btn1Motion>:move_pick()\n\
<Btn1Up>:end_pick()\n\
Shift<Btn1Down>:ext_pick()\n\
~Shift<Btn2Down>:start_pick()\n\
<Btn2Motion>:move_pick()\n\
<Btn2Up>:end_pick()\n\
<Key>Left:move_tasti()\n\
<Key>Right:move_tasti()\n\
<Key>Up:move_tasti()\n\
<Key>Down:move_tasti()\n";

static char	*freeTable = "#replace\n\
<Btn1Down>:first_point_poli()\n\
<Btn2Down>:end_draw_draget()\n\
<Key>F5:draw_delete()\n\
<Btn1Motion>:first_point_poli()\n\
<Motion>:draw_draget()\n\
<Key>Left:move_tasti()\n\
<Key>Right:move_tasti()\n\
<Key>Up:move_tasti()\n\
<Key>Down:move_tasti()\n";

/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget	create_drawShell();
static void load_vett_colors(void);

/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void load_vett_colors(void)
{
vett_border[0]=drawnButton1;
vett_border[1]=drawnButton2;
vett_border[2]=drawnButton3;
vett_border[3]=drawnButton4;
vett_border[4]=drawnButton5;
vett_border[5]=drawnButton6;
vett_border[6]=drawnButton7;
vett_border[7]=drawnButton8;
vett_border[8]=drawnButton9;
vett_border[9]=drawnButton10;
vett_border[10]=drawnButton11;
vett_border[11]=drawnButton12;
vett_border[12]=drawnButton13;
vett_border[13]=drawnButton14;
vett_border[14]=drawnButton15;
vett_border[15]=drawnButton16;

vett_fill[0]=sdrawnButton1;
vett_fill[1]=sdrawnButton2;
vett_fill[2]=sdrawnButton3;
vett_fill[3]=sdrawnButton4;
vett_fill[4]=sdrawnButton5;
vett_fill[5]=sdrawnButton6;
vett_fill[6]=sdrawnButton7;
vett_fill[7]=sdrawnButton8;
vett_fill[8]=sdrawnButton9;
vett_fill[9]=sdrawnButton10;
vett_fill[10]=sdrawnButton11;
vett_fill[11]=sdrawnButton12;
vett_fill[12]=sdrawnButton13;
vett_fill[13]=sdrawnButton14;
vett_fill[14]=sdrawnButton15;
vett_fill[15]=sdrawnButton16;

}

void load_vett_draw()
{
int i;
Pixmap vett_pix[8];

vett_draw[0]= arrow_bt;
vett_draw[1]= zoom_bt;
vett_draw[2]= drawline_bt;
vett_draw[3]= drawrect_bt;
vett_draw[4]= drawcircle_bt;
vett_draw[5]= drawarc_bt;
vett_draw[6]= drawpoli_bt;
vett_draw[7]= drawfree_bt;
LoadDrawPixmap(vett_pix,vett_draw[0]);
for(i=0; i<8 ; i++)
	set_something(vett_draw[i],XmNlabelPixmap,(void*) vett_pix[i]);
}


void set_fill_colors()
{
Pixel pixel;
int i;

for(i=0; i< 12; i++)
	{
	get_something(vett_border[i],XmNbackground, (void*) &pixel);
	set_something(vett_fill[i],XmNbackground, (void*) pixel);
	get_something(vett_border[i],XmNtopShadowColor, (void*) &pixel);
	set_something(vett_fill[i],XmNtopShadowColor, (void*) pixel);
	get_something(vett_border[i],XmNbottomShadowColor, (void*) &pixel);
	set_something(vett_fill[i],XmNbottomShadowColor, (void*) pixel);
	
	}
}

void selez_bott_fill(indice)
int indice;
{
wselez_bott_fill(vett_fill[indice]);
set_current_gcs(draw_wid); 
}

void selez_bott_border(indice)
int indice;
{
wselez_bott_border(vett_border[indice]);
set_current_gcs(draw_wid); 
}

void selez_bott_draw(indice)
int indice;
{
int i;
for(i=0; i< 8; i++)
	{
	set_something( vett_draw[i], XmNshadowType, (void*) XmSHADOW_OUT);
	}
set_something( vett_draw[indice], XmNshadowType, (void*) XmSHADOW_IN);
}

void wselez_bott_fill(wid)
swidget wid;
{
int i;
for(i=0; i< 16; i++)
	{
	set_something( vett_fill[i], XmNshadowThickness, (void*) 2);
	set_something( vett_fill[i], XmNshadowType, (void*) XmSHADOW_ETCHED_IN);
	}
set_something( wid , XmNshadowType, (void*) XmSHADOW_IN);
set_something( wid , XmNshadowThickness, (void*) 4);
/* bott_fill_selected=wid; */
set_bott_fill(draw_wid,wid);
}

void wselez_bott_border(wid)
swidget wid;
{
int i;
for(i=0; i< 16; i++)
	{
	set_something( vett_border[i], XmNshadowThickness, (void*) 2);
	set_something( vett_border[i], XmNshadowType, (void*) XmSHADOW_ETCHED_IN);
	}
set_something( wid, XmNshadowThickness, (void*) 4);
set_something( wid, XmNshadowType, (void*) XmSHADOW_IN);
/* bott_border_selected = wid; */
set_bott_border(draw_wid,wid);
}

void reset_line_def(line_width,line_style,wid)
int line_width,line_style;
Widget wid;
{
#ifndef DESIGN_TIME
_UxCdrawShell           *UxSaveCtx, *UxContext;
UxSaveCtx = UxDrawShellContext;
UxDrawShellContext = UxContext =
                        (_UxCdrawShell *) UxGetContext( wid );
#endif
switch (line_width)
	{
	case 0:
		set_something(thiny_bt, XmNset, (void*) True);
		set_something(pix_1_bt, XmNset, (void*) False);
		set_something(pix_2_bt, XmNset, (void*) False);
		set_something(pix_3_bt, XmNset, (void*) False);
		set_something(pix_6_bt, XmNset, (void*) False);
	break;
	case 1:
		set_something(thiny_bt, XmNset, (void*) False);
		set_something(pix_1_bt, XmNset, (void*) True);
		set_something(pix_2_bt, XmNset, (void*) False);
		set_something(pix_3_bt, XmNset, (void*) False);
		set_something(pix_6_bt, XmNset, (void*) False);
	break;
	case 2:
		set_something(thiny_bt, XmNset, (void*) False);
		set_something(pix_1_bt, XmNset, (void*) False);
		set_something(pix_2_bt, XmNset, (void*) True);
		set_something(pix_3_bt, XmNset, (void*) False);
		set_something(pix_6_bt, XmNset, (void*) False);
	break;
	case 3:
		set_something(thiny_bt, XmNset, (void*) False);
		set_something(pix_1_bt, XmNset, (void*) False);
		set_something(pix_2_bt, XmNset, (void*) False);
		set_something(pix_3_bt, XmNset, (void*) True);
		set_something(pix_6_bt, XmNset, (void*) False);
	break;
	case 4:
		set_something(thiny_bt, XmNset, (void*) False);
		set_something(pix_1_bt, XmNset, (void*) False);
		set_something(pix_2_bt, XmNset, (void*) False);
		set_something(pix_3_bt, XmNset, (void*) False);
		set_something(pix_6_bt, XmNset, (void*) True);
	break;
	}

switch (line_style)
	{
	case LineSolid:
		set_something(solid_bt, XmNset, (void*) True);
		set_something(dashed_bt, XmNset, (void*) False);
	break;
	case LineOnOffDash:
		set_something(solid_bt, XmNset, (void*) False);
		set_something(dashed_bt, XmNset, (void*) True);
	break;
	}
#ifndef DESIGN_TIME
UxDrawShellContext = UxSaveCtx;
#endif
}


void set_draw_translations(wid)
Widget wid;  /* widget della drawing area */
{
#ifndef DESIGN_TIME
        add_def_translation(wid, pickTable);
#endif
}

void set_arrow_mode(wid)
Widget wid;
{
Widget wbottone;
#ifndef DESIGN_TIME
_UxCdrawShell           *UxSaveCtx, *UxContext;
#endif
/*
        Riporta Draw nella modalita' puntatore per selezione
*/
set_draw_translations(wid);

#ifdef ON_PAGEDIT
        aggiungi_pagedit_translation(wid);
#endif

/*
 seleziona il bottone con la freccia
 (attenzione !! la routine puo' essere stata chiamata da un'altra
 interfaccia)
*/
/*
 1. ricava l'indice di widget del bottone
*/
wbottone =  get_bott_arrow(wid);
/*
 2. esegue uno switch del context settando il context
    di appartenenza del bottone-freccia
*/
#ifndef DESIGN_TIME
UxSaveCtx = UxDrawShellContext;
UxDrawShellContext = UxContext =
                        (_UxCdrawShell *) UxGetContext( wbottone );
#endif

/*
 3. seleziona graficamente il bottone
 */
void selez_bott_draw();
/*
 4. ripristina il context
*/
#ifndef DESIGN_TIME
UxDrawShellContext = UxSaveCtx;
#endif
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static	void	activateCB_drawline_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	add_def_translation(draw_wid,  lineTable);
#ifdef ON_PAGEDIT
	aggiungi_pagedit_translation(draw_wid);
#endif
	selez_bott_draw(2);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawrect_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	add_def_translation(draw_wid, rectTable);
#ifdef ON_PAGEDIT
	aggiungi_pagedit_translation(draw_wid);
#endif
	selez_bott_draw(3);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawcircle_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	add_def_translation(draw_wid, circleTable);
#ifdef ON_PAGEDIT
	aggiungi_pagedit_translation(draw_wid);
#endif
	selez_bott_draw(4);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawpoli_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	add_def_translation(draw_wid, poliTable);
#ifdef ON_PAGEDIT
	aggiungi_pagedit_translation(draw_wid);
#endif
	selez_bott_draw(6);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawfree_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	add_def_translation(draw_wid, freeTable);
#ifdef ON_PAGEDIT
	aggiungi_pagedit_translation(draw_wid);
#endif
	selez_bott_draw(7);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(0);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(1);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(2);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(3);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(4);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(5);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(6);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton8( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(7);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(8);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(9);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton11( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(10);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton12( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(11);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton13( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(12);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton14( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(13);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton15( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(14);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_sdrawnButton16( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_fill(15);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(0);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton2( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(1);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton3( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(2);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton4( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(3);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(4);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton6( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(5);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton7( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(6);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton8( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(7);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton9( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(8);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(9);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton11( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(10);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton12( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(11);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton13( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(12);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton14( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(13);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton15( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(14);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawnButton16( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_border(15);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_arrow_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	 add_def_translation(draw_wid, pickTable);
#ifdef ON_PAGEDIT
	aggiungi_pagedit_translation(draw_wid);
#endif
	 selez_bott_draw(0);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_drawarc_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	add_def_translation(draw_wid, arcTable);
#ifdef ON_PAGEDIT
	aggiungi_pagedit_translation(draw_wid);
#endif
	selez_bott_draw(5);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_zoom_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	selez_bott_draw(1);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_saveButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
#ifndef ON_PAGEDIT
	WriteBackground(draw_wid,nome_file_draw);
#endif
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_saveAsButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	extern swidget drawShell;
#ifndef ON_PAGEDIT
	UxPopupInterface ( create_SaveSelection(drawShell), no_grab);
#endif
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_prtButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
#if !defined ON_PAGEDIT && defined XPRINTER_USED
	PrintPag(draw_wid);
#endif
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_prtSetupButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
#if !defined ON_PAGEDIT && defined XPRINTER_USED
	PrintSetup(draw_wid);
#endif
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_exitButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	DeselectAllDraget(draw_wid,True);
	UxDestroyInterface(mainWindow);
#ifndef ON_PAGEDIT
	exit(0);
#endif
#ifdef ON_PAGEDIT
	set_draw_translations(draw_wid);
	aggiungi_pagedit_translation(draw_wid);
	/*
	 resetta il flag che indica che sto disegnando sul
	 background
	*/
	reset_drawing_background(draw_wid);
#endif
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_undoButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	/* chiama la funzione di undo 
	*/
	
	extern int undo_draget_paste();
	
	undo_draget_paste( draw_wid );
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_dupButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	int dx,dy;
	extern int curr_step;
	if(curr_step > 1)
		dx=dy=curr_step;
	else
		dx=dy=5;
	
	draget_duplicate(draw_wid,dx,dy);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_cutButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	  printf("Cut!\n");
	draget_cut(draw_wid);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_copyButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	  printf("Copy!\n");
	  draget_copy(draw_wid);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_pasteButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	  printf("Paste!\n");
	  draget_paste(draw_wid);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_deleteButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{ DrawDelete(draw_wid, NULL); }
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_topButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	DrawPutTop(draw_wid);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_bottomButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	DrawPutBottom(draw_wid);
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_groupButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	DrawGroup(draw_wid);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_ungroupButton( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	DrawUngroup(draw_wid);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_view_normal( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
		DrawSetZoom(draw_wid,1.0);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_view_200( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
		DrawSetZoom(draw_wid,2.0);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_view_400( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
		DrawSetZoom(draw_wid,4.0); 
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_view_800( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
		DrawSetZoom(draw_wid,8.0);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_grid_pb( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	DrawSetGrid(draw_wid);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_filled_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	set_current_filled(draw_wid,pcall->set);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_solid_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
		{
		set_line_style(draw_wid,LineSolid);
		set_current_gcs(draw_wid);
		}
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_dashed_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 {
	 set_line_style(draw_wid,LineOnOffDash);
	 set_current_gcs(draw_wid);
	 }
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_linedot_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 {
	 set_line_style(draw_wid,LineOnOffDash);
	 set_current_gcs(draw_wid);
	 }
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_thiny_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 {
	 set_line_width(draw_wid,0);
	 set_current_gcs(draw_wid);
	 }
	
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_pix_1_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 {
	 set_line_width(draw_wid,1);
	 set_current_gcs(draw_wid);
	 }
	
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_pix_2_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 {
	 set_line_width(draw_wid,2);
	 set_current_gcs(draw_wid);
	 }
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_pix_3_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 {
	 set_line_width(draw_wid,3);
	 set_current_gcs(draw_wid);
	 }
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_pix_6_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 {
	 set_line_width(draw_wid,6);
	 set_current_gcs(draw_wid);
	 }
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_width_other( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	extern swidget create_line_width_dialog();
	static Widget wdialog=NULL;
	
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (wdialog == NULL)
		wdialog= (Widget)create_line_width_dialog(draw_wid);
	
	if(pcall->set)
		UxPopupInterface(wdialog, no_grab);
	
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_sn5( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 DrawSetSnap(draw_wid,5);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_sn10( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 DrawSetSnap(draw_wid,10);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_sn15( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 DrawSetSnap(draw_wid,15);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_snother( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	extern swidget create_snap_width_dialog();
	static Widget wdialog=NULL;
	
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	
	if (wdialog == NULL)
		wdialog= (Widget)create_snap_width_dialog(draw_wid);
	
	if(pcall->set)
		UxPopupInterface(wdialog, no_grab);
	
	
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	valueChangedCB_sn1( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmToggleButtonCallbackStruct *pcall;
	pcall=(XmToggleButtonCallbackStruct *)UxCallbackArg;
	if (pcall->set)
	 DrawSetSnap(draw_wid,1);
	}
	UxDrawShellContext = UxSaveCtx;
}

static	void	activateCB_edit_colors_bt( UxWidget, UxClientData, UxCallbackArg )
	Widget		UxWidget;
	XtPointer	UxClientData, UxCallbackArg;

{
	_UxCdrawShell           *UxSaveCtx, *UxContext;

	UxSaveCtx = UxDrawShellContext;
	UxDrawShellContext = UxContext =
			(_UxCdrawShell *) UxGetContext( UxWidget );
	{
	XmString xm_str;
	xm_str=XmStringCreateSimple(" "); 
	UxPopupInterface(create_ColorViewer(DRAW_VER,"#ffffffffffff",xm_str,
				UxDisplay->db,
				vett_colori,
				vett_border[0]), no_grab);
	
	XmStringFree(xm_str);
	}
	UxDrawShellContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget	_Uxbuild_drawShell()
{
	Widget		_UxParent;
	Widget		filePane_shell;
	Widget		editPane_shell;
	Widget		viewPane_shell;
	Widget		drawPane_shell;
	Widget		lineStylePane_shell;
	Widget		lineWidthPane_shell;
	Widget		snap_Pane_shell;
	Widget		rotatePane_shell;
	Widget		helpPane_shell;
	Widget		colorPane_shell;


	/* Creation of drawShell */
	_UxParent = passed_wid;
	if ( _UxParent == NULL )
	{
		_UxParent = UxTopLevel;
	}

	drawShell = XtVaCreatePopupShell( "drawShell",
			topLevelShellWidgetClass,
			_UxParent,
			XmNx, 0,
			XmNy, 169,
			XmNwidth, 606,
			XmNheight, 190,
			XmNallowShellResize, FALSE,
			NULL );
	UxPutContext( drawShell, (char *) UxDrawShellContext );


	/* Creation of mainWindow */
	mainWindow = XtVaCreateManagedWidget( "mainWindow",
			xmMainWindowWidgetClass,
			drawShell,
			XmNheight, 190,
			XmNwidth, 606,
			NULL );
	UxPutContext( mainWindow, (char *) UxDrawShellContext );


	/* Creation of workAreaMgr */
	workAreaMgr = XtVaCreateManagedWidget( "workAreaMgr",
			xmDrawingAreaWidgetClass,
			mainWindow,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			NULL );
	UxPutContext( workAreaMgr, (char *) UxDrawShellContext );


	/* Creation of workFrame */
	workFrame = XtVaCreateManagedWidget( "workFrame",
			xmFrameWidgetClass,
			workAreaMgr,
			XmNx, 0,
			XmNy, 0,
			XmNwidth, 600,
			XmNheight, 496,
			NULL );
	UxPutContext( workFrame, (char *) UxDrawShellContext );


	/* Creation of workDrArea */
	workDrArea = XtVaCreateManagedWidget( "workDrArea",
			xmDrawingAreaWidgetClass,
			workFrame,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 588,
			XmNheight, 148,
			XmNmarginHeight, 0,
			XmNmarginWidth, 0,
			XmNresizePolicy, XmRESIZE_NONE,
			NULL );
	UxPutContext( workDrArea, (char *) UxDrawShellContext );


	/* Creation of drawline_bt */
	drawline_bt = XtVaCreateManagedWidget( "drawline_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 110,
			XmNy, 100,
			XmNwidth, 60,
			XmNheight, 50,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( drawline_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawline_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawline_bt, (char *) UxDrawShellContext );


	/* Creation of drawrect_bt */
	drawrect_bt = XtVaCreateManagedWidget( "drawrect_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 160,
			XmNy, 100,
			XmNwidth, 76,
			XmNheight, 76,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( drawrect_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawrect_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawrect_bt, (char *) UxDrawShellContext );


	/* Creation of drawcircle_bt */
	drawcircle_bt = XtVaCreateManagedWidget( "drawcircle_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 210,
			XmNy, 100,
			XmNwidth, 76,
			XmNheight, 76,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( drawcircle_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawcircle_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawcircle_bt, (char *) UxDrawShellContext );


	/* Creation of drawpoli_bt */
	drawpoli_bt = XtVaCreateManagedWidget( "drawpoli_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 310,
			XmNy, 100,
			XmNwidth, 76,
			XmNheight, 76,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( drawpoli_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawpoli_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawpoli_bt, (char *) UxDrawShellContext );


	/* Creation of drawfree_bt */
	drawfree_bt = XtVaCreateManagedWidget( "drawfree_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 360,
			XmNy, 100,
			XmNwidth, 76,
			XmNheight, 76,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( drawfree_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawfree_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawfree_bt, (char *) UxDrawShellContext );


	/* Creation of frame1 */
	frame1 = XtVaCreateManagedWidget( "frame1",
			xmFrameWidgetClass,
			workDrArea,
			XmNx, 8,
			XmNy, 51,
			XmNwidth, 562,
			XmNheight, 41,
			XmNshadowType, XmSHADOW_ETCHED_IN,
			NULL );
	UxPutContext( frame1, (char *) UxDrawShellContext );


	/* Creation of drawingArea1 */
	drawingArea1 = XtVaCreateManagedWidget( "drawingArea1",
			xmDrawingAreaWidgetClass,
			frame1,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 558,
			XmNheight, 38,
			XmNmarginHeight, 2,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( drawingArea1, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton1 */
	sdrawnButton1 = XtVaCreateManagedWidget( "sdrawnButton1",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 76,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNshadowThickness, 2,
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton1, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton1,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton1, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton2 */
	sdrawnButton2 = XtVaCreateManagedWidget( "sdrawnButton2",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 106,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton2, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton2,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton2, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton3 */
	sdrawnButton3 = XtVaCreateManagedWidget( "sdrawnButton3",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 136,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton3, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton3,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton3, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton4 */
	sdrawnButton4 = XtVaCreateManagedWidget( "sdrawnButton4",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 166,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton4, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton4,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton4, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton5 */
	sdrawnButton5 = XtVaCreateManagedWidget( "sdrawnButton5",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 196,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton5, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton5,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton5, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton6 */
	sdrawnButton6 = XtVaCreateManagedWidget( "sdrawnButton6",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 226,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton6, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton6,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton6, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton7 */
	sdrawnButton7 = XtVaCreateManagedWidget( "sdrawnButton7",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 256,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton7, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton7,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton7, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton8 */
	sdrawnButton8 = XtVaCreateManagedWidget( "sdrawnButton8",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 286,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton8, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton8,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton8, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton9 */
	sdrawnButton9 = XtVaCreateManagedWidget( "sdrawnButton9",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 316,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton9, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton9,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton9, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton10 */
	sdrawnButton10 = XtVaCreateManagedWidget( "sdrawnButton10",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 346,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton10, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton10,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton10, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton11 */
	sdrawnButton11 = XtVaCreateManagedWidget( "sdrawnButton11",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 376,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton11, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton11,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton11, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton12 */
	sdrawnButton12 = XtVaCreateManagedWidget( "sdrawnButton12",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 406,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton12, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton12,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton12, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton13 */
	sdrawnButton13 = XtVaCreateManagedWidget( "sdrawnButton13",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 434,
			XmNy, 0,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#dddddd" ),
			RES_CONVERT( XmNlabelString, "1" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton13, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton13,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton13, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton14 */
	sdrawnButton14 = XtVaCreateManagedWidget( "sdrawnButton14",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 464,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#bbbbbb" ),
			RES_CONVERT( XmNlabelString, "2" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton14, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton14,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton14, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton15 */
	sdrawnButton15 = XtVaCreateManagedWidget( "sdrawnButton15",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 494,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#999999" ),
			RES_CONVERT( XmNlabelString, "3" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton15, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton15,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton15, (char *) UxDrawShellContext );


	/* Creation of sdrawnButton16 */
	sdrawnButton16 = XtVaCreateManagedWidget( "sdrawnButton16",
			xmDrawnButtonWidgetClass,
			drawingArea1,
			XmNx, 524,
			XmNy, 2,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#777777" ),
			RES_CONVERT( XmNlabelString, "4" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( sdrawnButton16, XmNactivateCallback,
		(XtCallbackProc) activateCB_sdrawnButton16,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sdrawnButton16, (char *) UxDrawShellContext );


	/* Creation of label2 */
	label2 = XtVaCreateManagedWidget( "label2",
			xmLabelWidgetClass,
			drawingArea1,
			XmNx, 7,
			XmNy, 4,
			XmNwidth, 62,
			XmNheight, 24,
			RES_CONVERT( XmNlabelString, "Fill Color" ),
			NULL );
	UxPutContext( label2, (char *) UxDrawShellContext );


	/* Creation of frame2 */
	frame2 = XtVaCreateManagedWidget( "frame2",
			xmFrameWidgetClass,
			workDrArea,
			XmNx, 7,
			XmNy, 10,
			XmNwidth, 562,
			XmNheight, 40,
			NULL );
	UxPutContext( frame2, (char *) UxDrawShellContext );


	/* Creation of drawingArea2 */
	drawingArea2 = XtVaCreateManagedWidget( "drawingArea2",
			xmDrawingAreaWidgetClass,
			frame2,
			XmNresizePolicy, XmRESIZE_NONE,
			XmNx, 2,
			XmNy, 2,
			XmNwidth, 558,
			XmNheight, 38,
			XmNmarginHeight, 2,
			XmNmarginWidth, 2,
			NULL );
	UxPutContext( drawingArea2, (char *) UxDrawShellContext );


	/* Creation of drawnButton1 */
	drawnButton1 = XtVaCreateManagedWidget( "drawnButton1",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 74,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton1, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton1,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton1, (char *) UxDrawShellContext );


	/* Creation of drawnButton2 */
	drawnButton2 = XtVaCreateManagedWidget( "drawnButton2",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 104,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton2, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton2,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton2, (char *) UxDrawShellContext );


	/* Creation of drawnButton3 */
	drawnButton3 = XtVaCreateManagedWidget( "drawnButton3",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 134,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton3, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton3,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton3, (char *) UxDrawShellContext );


	/* Creation of drawnButton4 */
	drawnButton4 = XtVaCreateManagedWidget( "drawnButton4",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 164,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton4, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton4,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton4, (char *) UxDrawShellContext );


	/* Creation of drawnButton5 */
	drawnButton5 = XtVaCreateManagedWidget( "drawnButton5",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 194,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton5, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton5,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton5, (char *) UxDrawShellContext );


	/* Creation of drawnButton6 */
	drawnButton6 = XtVaCreateManagedWidget( "drawnButton6",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 224,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton6, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton6,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton6, (char *) UxDrawShellContext );


	/* Creation of drawnButton7 */
	drawnButton7 = XtVaCreateManagedWidget( "drawnButton7",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 254,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton7, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton7,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton7, (char *) UxDrawShellContext );


	/* Creation of drawnButton8 */
	drawnButton8 = XtVaCreateManagedWidget( "drawnButton8",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 280,
			XmNy, 2,
			XmNwidth, 34,
			XmNheight, 28,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton8, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton8,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton8, (char *) UxDrawShellContext );


	/* Creation of drawnButton9 */
	drawnButton9 = XtVaCreateManagedWidget( "drawnButton9",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 314,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton9, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton9,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton9, (char *) UxDrawShellContext );


	/* Creation of drawnButton10 */
	drawnButton10 = XtVaCreateManagedWidget( "drawnButton10",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 344,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton10, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton10,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton10, (char *) UxDrawShellContext );


	/* Creation of drawnButton11 */
	drawnButton11 = XtVaCreateManagedWidget( "drawnButton11",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 374,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton11, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton11,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton11, (char *) UxDrawShellContext );


	/* Creation of drawnButton12 */
	drawnButton12 = XtVaCreateManagedWidget( "drawnButton12",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 404,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNlabelString, " " ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton12, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton12,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton12, (char *) UxDrawShellContext );


	/* Creation of drawnButton13 */
	drawnButton13 = XtVaCreateManagedWidget( "drawnButton13",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 434,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#dddddd" ),
			RES_CONVERT( XmNlabelString, "1" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton13, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton13,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton13, (char *) UxDrawShellContext );


	/* Creation of drawnButton14 */
	drawnButton14 = XtVaCreateManagedWidget( "drawnButton14",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 464,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#bbbbbb" ),
			RES_CONVERT( XmNlabelString, "2" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton14, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton14,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton14, (char *) UxDrawShellContext );


	/* Creation of drawnButton15 */
	drawnButton15 = XtVaCreateManagedWidget( "drawnButton15",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 494,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#999999" ),
			RES_CONVERT( XmNlabelString, "3" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton15, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton15,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton15, (char *) UxDrawShellContext );


	/* Creation of drawnButton16 */
	drawnButton16 = XtVaCreateManagedWidget( "drawnButton16",
			xmDrawnButtonWidgetClass,
			drawingArea2,
			XmNx, 524,
			XmNy, 1,
			XmNwidth, 30,
			XmNheight, 30,
			RES_CONVERT( XmNbackground, "#777777" ),
			RES_CONVERT( XmNlabelString, "4" ),
			XmNrecomputeSize, FALSE,
			NULL );
	XtAddCallback( drawnButton16, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawnButton16,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawnButton16, (char *) UxDrawShellContext );


	/* Creation of label1 */
	label1 = XtVaCreateManagedWidget( "label1",
			xmLabelWidgetClass,
			drawingArea2,
			XmNx, 7,
			XmNy, 5,
			XmNwidth, 62,
			XmNheight, 24,
			RES_CONVERT( XmNlabelString, "Line Color" ),
			NULL );
	UxPutContext( label1, (char *) UxDrawShellContext );


	/* Creation of arrow_bt */
	arrow_bt = XtVaCreateManagedWidget( "arrow_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 10,
			XmNy, 100,
			XmNwidth, 60,
			XmNheight, 50,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( arrow_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_arrow_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( arrow_bt, (char *) UxDrawShellContext );


	/* Creation of drawarc_bt */
	drawarc_bt = XtVaCreateManagedWidget( "drawarc_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 260,
			XmNy, 100,
			XmNwidth, 76,
			XmNheight, 76,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( drawarc_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_drawarc_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( drawarc_bt, (char *) UxDrawShellContext );


	/* Creation of zoom_bt */
	zoom_bt = XtVaCreateManagedWidget( "zoom_bt",
			xmDrawnButtonWidgetClass,
			workDrArea,
			XmNx, 60,
			XmNy, 100,
			XmNwidth, 60,
			XmNheight, 50,
			XmNlabelType, XmPIXMAP,
			XmNshadowThickness, 2,
			XmNshadowType, XmSHADOW_OUT,
			XmNrecomputeSize, TRUE,
			NULL );
	XtAddCallback( zoom_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_zoom_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( zoom_bt, (char *) UxDrawShellContext );


	/* Creation of DrawDemoMainMenu */
	DrawDemoMainMenu = XtVaCreateManagedWidget( "DrawDemoMainMenu",
			xmRowColumnWidgetClass,
			mainWindow,
			XmNborderWidth, 0,
			XmNrowColumnType, XmMENU_BAR,
			XmNmenuAccelerator, "<KeyUp>F10",
			NULL );
	UxPutContext( DrawDemoMainMenu, (char *) UxDrawShellContext );


	/* Creation of filePane */
	filePane_shell = XtVaCreatePopupShell ("filePane_shell",
			xmMenuShellWidgetClass, DrawDemoMainMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	filePane = XtVaCreateWidget( "filePane",
			xmRowColumnWidgetClass,
			filePane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( filePane, (char *) UxDrawShellContext );


	/* Creation of saveButton */
	saveButton = XtVaCreateManagedWidget( "saveButton",
			xmPushButtonGadgetClass,
			filePane,
			RES_CONVERT( XmNlabelString, "Save" ),
			RES_CONVERT( XmNmnemonic, "S" ),
			NULL );
	XtAddCallback( saveButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_saveButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( saveButton, (char *) UxDrawShellContext );


	/* Creation of saveAsButton */
	saveAsButton = XtVaCreateManagedWidget( "saveAsButton",
			xmPushButtonGadgetClass,
			filePane,
			RES_CONVERT( XmNlabelString, "Save As ..." ),
			RES_CONVERT( XmNmnemonic, "A" ),
			NULL );
	XtAddCallback( saveAsButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_saveAsButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( saveAsButton, (char *) UxDrawShellContext );


	/* Creation of filePane_b8 */
	filePane_b8 = XtVaCreateManagedWidget( "filePane_b8",
			xmSeparatorGadgetClass,
			filePane,
			NULL );
	UxPutContext( filePane_b8, (char *) UxDrawShellContext );


	/* Creation of prtButton */
	prtButton = XtVaCreateManagedWidget( "prtButton",
			xmPushButtonWidgetClass,
			filePane,
			RES_CONVERT( XmNlabelString, "Print" ),
			NULL );
	XtAddCallback( prtButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_prtButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( prtButton, (char *) UxDrawShellContext );


	/* Creation of prtSetupButton */
	prtSetupButton = XtVaCreateManagedWidget( "prtSetupButton",
			xmPushButtonWidgetClass,
			filePane,
			RES_CONVERT( XmNlabelString, "Setup Printer..." ),
			NULL );
	XtAddCallback( prtSetupButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_prtSetupButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( prtSetupButton, (char *) UxDrawShellContext );


	/* Creation of filePane_b7 */
	filePane_b7 = XtVaCreateManagedWidget( "filePane_b7",
			xmSeparatorWidgetClass,
			filePane,
			NULL );
	UxPutContext( filePane_b7, (char *) UxDrawShellContext );


	/* Creation of exitButton */
	exitButton = XtVaCreateManagedWidget( "exitButton",
			xmPushButtonGadgetClass,
			filePane,
			RES_CONVERT( XmNlabelString, "Exit" ),
			RES_CONVERT( XmNmnemonic, "E" ),
			NULL );
	XtAddCallback( exitButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_exitButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( exitButton, (char *) UxDrawShellContext );


	/* Creation of fileCascade */
	fileCascade = XtVaCreateManagedWidget( "fileCascade",
			xmCascadeButtonWidgetClass,
			DrawDemoMainMenu,
			RES_CONVERT( XmNlabelString, "File" ),
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNsubMenuId, filePane,
			NULL );
	UxPutContext( fileCascade, (char *) UxDrawShellContext );


	/* Creation of editPane */
	editPane_shell = XtVaCreatePopupShell ("editPane_shell",
			xmMenuShellWidgetClass, DrawDemoMainMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	editPane = XtVaCreateWidget( "editPane",
			xmRowColumnWidgetClass,
			editPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( editPane, (char *) UxDrawShellContext );


	/* Creation of undoButton */
	undoButton = XtVaCreateManagedWidget( "undoButton",
			xmPushButtonWidgetClass,
			editPane,
			RES_CONVERT( XmNlabelString, "Undo" ),
			RES_CONVERT( XmNmnemonic, "U" ),
			NULL );
	XtAddCallback( undoButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_undoButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( undoButton, (char *) UxDrawShellContext );


	/* Creation of dupButton */
	dupButton = XtVaCreateManagedWidget( "dupButton",
			xmPushButtonWidgetClass,
			editPane,
			RES_CONVERT( XmNlabelString, "Duplicate" ),
			RES_CONVERT( XmNmnemonic, "D" ),
			NULL );
	XtAddCallback( dupButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_dupButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( dupButton, (char *) UxDrawShellContext );


	/* Creation of cutButton */
	cutButton = XtVaCreateManagedWidget( "cutButton",
			xmPushButtonGadgetClass,
			editPane,
			XmNaccelerator, "Shift <Key>DeleteChar",
			RES_CONVERT( XmNacceleratorText, "Shift+Del" ),
			RES_CONVERT( XmNlabelString, "Cut" ),
			RES_CONVERT( XmNmnemonic, "t" ),
			NULL );
	XtAddCallback( cutButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_cutButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( cutButton, (char *) UxDrawShellContext );


	/* Creation of copyButton */
	copyButton = XtVaCreateManagedWidget( "copyButton",
			xmPushButtonGadgetClass,
			editPane,
			XmNaccelerator, "Ctrl <Key>InsertChar",
			RES_CONVERT( XmNacceleratorText, "Ctrl+Ins" ),
			RES_CONVERT( XmNlabelString, "Copy" ),
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	XtAddCallback( copyButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_copyButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( copyButton, (char *) UxDrawShellContext );


	/* Creation of pasteButton */
	pasteButton = XtVaCreateManagedWidget( "pasteButton",
			xmPushButtonGadgetClass,
			editPane,
			XmNaccelerator, "Shift <Key>InsertChar",
			RES_CONVERT( XmNacceleratorText, "Shift+Ins" ),
			RES_CONVERT( XmNlabelString, "Paste" ),
			RES_CONVERT( XmNmnemonic, "P" ),
			NULL );
	XtAddCallback( pasteButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_pasteButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( pasteButton, (char *) UxDrawShellContext );


	/* Creation of deleteButton */
	deleteButton = XtVaCreateManagedWidget( "deleteButton",
			xmPushButtonGadgetClass,
			editPane,
			RES_CONVERT( XmNlabelString, "Delete" ),
			RES_CONVERT( XmNmnemonic, "D" ),
			RES_CONVERT( XmNacceleratorText, "BackSpace" ),
			XmNaccelerator, "<Key>BackSpace",
			NULL );
	XtAddCallback( deleteButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_deleteButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( deleteButton, (char *) UxDrawShellContext );


	/* Creation of editPane_b12 */
	editPane_b12 = XtVaCreateManagedWidget( "editPane_b12",
			xmSeparatorWidgetClass,
			editPane,
			NULL );
	UxPutContext( editPane_b12, (char *) UxDrawShellContext );


	/* Creation of topButton */
	topButton = XtVaCreateManagedWidget( "topButton",
			xmPushButtonWidgetClass,
			editPane,
			RES_CONVERT( XmNlabelString, "Put Top" ),
			NULL );
	XtAddCallback( topButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_topButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( topButton, (char *) UxDrawShellContext );


	/* Creation of bottomButton */
	bottomButton = XtVaCreateManagedWidget( "bottomButton",
			xmPushButtonWidgetClass,
			editPane,
			RES_CONVERT( XmNlabelString, "Put Bottom" ),
			NULL );
	XtAddCallback( bottomButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_bottomButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( bottomButton, (char *) UxDrawShellContext );


	/* Creation of editPane_b13 */
	editPane_b13 = XtVaCreateManagedWidget( "editPane_b13",
			xmSeparatorWidgetClass,
			editPane,
			NULL );
	UxPutContext( editPane_b13, (char *) UxDrawShellContext );


	/* Creation of groupButton */
	groupButton = XtVaCreateManagedWidget( "groupButton",
			xmPushButtonWidgetClass,
			editPane,
			RES_CONVERT( XmNlabelString, "Group" ),
			RES_CONVERT( XmNmnemonic, "G" ),
			NULL );
	XtAddCallback( groupButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_groupButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( groupButton, (char *) UxDrawShellContext );


	/* Creation of ungroupButton */
	ungroupButton = XtVaCreateManagedWidget( "ungroupButton",
			xmPushButtonWidgetClass,
			editPane,
			RES_CONVERT( XmNlabelString, "Ungroup" ),
			RES_CONVERT( XmNmnemonic, "U" ),
			NULL );
	XtAddCallback( ungroupButton, XmNactivateCallback,
		(XtCallbackProc) activateCB_ungroupButton,
		(XtPointer) UxDrawShellContext );

	UxPutContext( ungroupButton, (char *) UxDrawShellContext );


	/* Creation of editCascade */
	editCascade = XtVaCreateManagedWidget( "editCascade",
			xmCascadeButtonWidgetClass,
			DrawDemoMainMenu,
			RES_CONVERT( XmNlabelString, "Edit" ),
			RES_CONVERT( XmNmnemonic, "E" ),
			XmNsubMenuId, editPane,
			NULL );
	UxPutContext( editCascade, (char *) UxDrawShellContext );


	/* Creation of viewPane */
	viewPane_shell = XtVaCreatePopupShell ("viewPane_shell",
			xmMenuShellWidgetClass, DrawDemoMainMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	viewPane = XtVaCreateWidget( "viewPane",
			xmRowColumnWidgetClass,
			viewPane_shell,
			XmNradioBehavior, TRUE,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNentryClass, xmToggleButtonWidgetClass,
			NULL );
	UxPutContext( viewPane, (char *) UxDrawShellContext );


	/* Creation of view_normal */
	view_normal = XtVaCreateManagedWidget( "view_normal",
			xmToggleButtonWidgetClass,
			viewPane,
			RES_CONVERT( XmNlabelString, "Normal view" ),
			RES_CONVERT( XmNmnemonic, "N" ),
			XmNset, TRUE,
			NULL );
	XtAddCallback( view_normal, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_view_normal,
		(XtPointer) UxDrawShellContext );

	UxPutContext( view_normal, (char *) UxDrawShellContext );


	/* Creation of viewPane_b20 */
	viewPane_b20 = XtVaCreateManagedWidget( "viewPane_b20",
			xmSeparatorWidgetClass,
			viewPane,
			NULL );
	UxPutContext( viewPane_b20, (char *) UxDrawShellContext );


	/* Creation of view_200 */
	view_200 = XtVaCreateManagedWidget( "view_200",
			xmToggleButtonWidgetClass,
			viewPane,
			RES_CONVERT( XmNlabelString, "200%" ),
			RES_CONVERT( XmNmnemonic, "0" ),
			NULL );
	XtAddCallback( view_200, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_view_200,
		(XtPointer) UxDrawShellContext );

	UxPutContext( view_200, (char *) UxDrawShellContext );


	/* Creation of view_400 */
	view_400 = XtVaCreateManagedWidget( "view_400",
			xmToggleButtonWidgetClass,
			viewPane,
			RES_CONVERT( XmNlabelString, "400%" ),
			RES_CONVERT( XmNmnemonic, "4" ),
			NULL );
	XtAddCallback( view_400, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_view_400,
		(XtPointer) UxDrawShellContext );

	UxPutContext( view_400, (char *) UxDrawShellContext );


	/* Creation of view_800 */
	view_800 = XtVaCreateManagedWidget( "view_800",
			xmToggleButtonWidgetClass,
			viewPane,
			RES_CONVERT( XmNlabelString, "800%" ),
			RES_CONVERT( XmNmnemonic, "8" ),
			NULL );
	XtAddCallback( view_800, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_view_800,
		(XtPointer) UxDrawShellContext );

	UxPutContext( view_800, (char *) UxDrawShellContext );


	/* Creation of viewPane_b6 */
	viewPane_b6 = XtVaCreateManagedWidget( "viewPane_b6",
			xmSeparatorWidgetClass,
			viewPane,
			NULL );
	UxPutContext( viewPane_b6, (char *) UxDrawShellContext );


	/* Creation of grid_pb */
	grid_pb = XtVaCreateManagedWidget( "grid_pb",
			xmPushButtonWidgetClass,
			viewPane,
			RES_CONVERT( XmNlabelString, "Grid" ),
			RES_CONVERT( XmNmnemonic, "G" ),
			NULL );
	XtAddCallback( grid_pb, XmNactivateCallback,
		(XtCallbackProc) activateCB_grid_pb,
		(XtPointer) UxDrawShellContext );

	UxPutContext( grid_pb, (char *) UxDrawShellContext );


	/* Creation of viewCascade */
	viewCascade = XtVaCreateManagedWidget( "viewCascade",
			xmCascadeButtonWidgetClass,
			DrawDemoMainMenu,
			RES_CONVERT( XmNlabelString, "View" ),
			RES_CONVERT( XmNmnemonic, "V" ),
			XmNsubMenuId, viewPane,
			NULL );
	UxPutContext( viewCascade, (char *) UxDrawShellContext );


	/* Creation of drawPane */
	drawPane_shell = XtVaCreatePopupShell ("drawPane_shell",
			xmMenuShellWidgetClass, DrawDemoMainMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	drawPane = XtVaCreateWidget( "drawPane",
			xmRowColumnWidgetClass,
			drawPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( drawPane, (char *) UxDrawShellContext );


	/* Creation of filled_bt */
	filled_bt = XtVaCreateManagedWidget( "filled_bt",
			xmToggleButtonWidgetClass,
			drawPane,
			RES_CONVERT( XmNlabelString, "Filled" ),
			RES_CONVERT( XmNmnemonic, "F" ),
			XmNset, TRUE,
			XmNindicatorOn, FALSE,
			NULL );
	XtAddCallback( filled_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_filled_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( filled_bt, (char *) UxDrawShellContext );


	/* Creation of drawPane_b9 */
	drawPane_b9 = XtVaCreateManagedWidget( "drawPane_b9",
			xmSeparatorWidgetClass,
			drawPane,
			NULL );
	UxPutContext( drawPane_b9, (char *) UxDrawShellContext );


	/* Creation of lineStylePane */
	lineStylePane_shell = XtVaCreatePopupShell ("lineStylePane_shell",
			xmMenuShellWidgetClass, drawPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	lineStylePane = XtVaCreateWidget( "lineStylePane",
			xmRowColumnWidgetClass,
			lineStylePane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNradioBehavior, TRUE,
			NULL );
	UxPutContext( lineStylePane, (char *) UxDrawShellContext );


	/* Creation of solid_bt */
	solid_bt = XtVaCreateManagedWidget( "solid_bt",
			xmToggleButtonWidgetClass,
			lineStylePane,
			RES_CONVERT( XmNlabelString, "Solid" ),
			XmNset, TRUE,
			NULL );
	XtAddCallback( solid_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_solid_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( solid_bt, (char *) UxDrawShellContext );


	/* Creation of dashed_bt */
	dashed_bt = XtVaCreateManagedWidget( "dashed_bt",
			xmToggleButtonWidgetClass,
			lineStylePane,
			RES_CONVERT( XmNlabelString, "Dashed" ),
			RES_CONVERT( XmNmnemonic, "D" ),
			NULL );
	XtAddCallback( dashed_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_dashed_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( dashed_bt, (char *) UxDrawShellContext );


	/* Creation of linedot_bt */
	linedot_bt = XtVaCreateManagedWidget( "linedot_bt",
			xmToggleButtonWidgetClass,
			lineStylePane,
			RES_CONVERT( XmNlabelString, "Line - dot" ),
			RES_CONVERT( XmNmnemonic, "L" ),
			NULL );
	XtAddCallback( linedot_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_linedot_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( linedot_bt, (char *) UxDrawShellContext );


	/* Creation of line_bt */
	line_bt = XtVaCreateManagedWidget( "line_bt",
			xmCascadeButtonWidgetClass,
			drawPane,
			RES_CONVERT( XmNlabelString, "Line Style" ),
			RES_CONVERT( XmNmnemonic, "L" ),
			XmNsubMenuId, lineStylePane,
			NULL );
	UxPutContext( line_bt, (char *) UxDrawShellContext );


	/* Creation of lineWidthPane */
	lineWidthPane_shell = XtVaCreatePopupShell ("lineWidthPane_shell",
			xmMenuShellWidgetClass, drawPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	lineWidthPane = XtVaCreateWidget( "lineWidthPane",
			xmRowColumnWidgetClass,
			lineWidthPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNradioBehavior, TRUE,
			NULL );
	UxPutContext( lineWidthPane, (char *) UxDrawShellContext );


	/* Creation of thiny_bt */
	thiny_bt = XtVaCreateManagedWidget( "thiny_bt",
			xmToggleButtonWidgetClass,
			lineWidthPane,
			RES_CONVERT( XmNlabelString, "Thin" ),
			RES_CONVERT( XmNmnemonic, "T" ),
			XmNset, TRUE,
			NULL );
	XtAddCallback( thiny_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_thiny_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( thiny_bt, (char *) UxDrawShellContext );


	/* Creation of pix_1_bt */
	pix_1_bt = XtVaCreateManagedWidget( "pix_1_bt",
			xmToggleButtonWidgetClass,
			lineWidthPane,
			RES_CONVERT( XmNlabelString, "1 pixel" ),
			RES_CONVERT( XmNmnemonic, "1" ),
			NULL );
	XtAddCallback( pix_1_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_pix_1_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( pix_1_bt, (char *) UxDrawShellContext );


	/* Creation of pix_2_bt */
	pix_2_bt = XtVaCreateManagedWidget( "pix_2_bt",
			xmToggleButtonWidgetClass,
			lineWidthPane,
			RES_CONVERT( XmNlabelString, "2 pixel" ),
			RES_CONVERT( XmNmnemonic, "2" ),
			NULL );
	XtAddCallback( pix_2_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_pix_2_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( pix_2_bt, (char *) UxDrawShellContext );


	/* Creation of pix_3_bt */
	pix_3_bt = XtVaCreateManagedWidget( "pix_3_bt",
			xmToggleButtonWidgetClass,
			lineWidthPane,
			RES_CONVERT( XmNlabelString, "3 pixel" ),
			RES_CONVERT( XmNmnemonic, "3" ),
			NULL );
	XtAddCallback( pix_3_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_pix_3_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( pix_3_bt, (char *) UxDrawShellContext );


	/* Creation of pix_6_bt */
	pix_6_bt = XtVaCreateManagedWidget( "pix_6_bt",
			xmToggleButtonWidgetClass,
			lineWidthPane,
			RES_CONVERT( XmNlabelString, "6 pixel" ),
			RES_CONVERT( XmNmnemonic, "6" ),
			NULL );
	XtAddCallback( pix_6_bt, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_pix_6_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( pix_6_bt, (char *) UxDrawShellContext );


	/* Creation of width_other */
	width_other = XtVaCreateManagedWidget( "width_other",
			xmToggleButtonWidgetClass,
			lineWidthPane,
			RES_CONVERT( XmNlabelString, "other ..." ),
			RES_CONVERT( XmNmnemonic, "o" ),
			NULL );
	XtAddCallback( width_other, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_width_other,
		(XtPointer) UxDrawShellContext );

	UxPutContext( width_other, (char *) UxDrawShellContext );


	/* Creation of line_width_bt */
	line_width_bt = XtVaCreateManagedWidget( "line_width_bt",
			xmCascadeButtonWidgetClass,
			drawPane,
			RES_CONVERT( XmNlabelString, "Line Width" ),
			RES_CONVERT( XmNmnemonic, "W" ),
			XmNsubMenuId, lineWidthPane,
			NULL );
	UxPutContext( line_width_bt, (char *) UxDrawShellContext );


	/* Creation of drawPane_b12 */
	drawPane_b12 = XtVaCreateManagedWidget( "drawPane_b12",
			xmSeparatorWidgetClass,
			drawPane,
			NULL );
	UxPutContext( drawPane_b12, (char *) UxDrawShellContext );


	/* Creation of snap_Pane */
	snap_Pane_shell = XtVaCreatePopupShell ("snap_Pane_shell",
			xmMenuShellWidgetClass, drawPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	snap_Pane = XtVaCreateWidget( "snap_Pane",
			xmRowColumnWidgetClass,
			snap_Pane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			XmNradioBehavior, TRUE,
			NULL );
	UxPutContext( snap_Pane, (char *) UxDrawShellContext );


	/* Creation of sn5 */
	sn5 = XtVaCreateManagedWidget( "sn5",
			xmToggleButtonWidgetClass,
			snap_Pane,
			RES_CONVERT( XmNlabelString, "5 pixel" ),
			RES_CONVERT( XmNmnemonic, "5" ),
			NULL );
	XtAddCallback( sn5, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_sn5,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sn5, (char *) UxDrawShellContext );


	/* Creation of sn10 */
	sn10 = XtVaCreateManagedWidget( "sn10",
			xmToggleButtonWidgetClass,
			snap_Pane,
			RES_CONVERT( XmNlabelString, "10 pixel" ),
			RES_CONVERT( XmNmnemonic, "1" ),
			NULL );
	XtAddCallback( sn10, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_sn10,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sn10, (char *) UxDrawShellContext );


	/* Creation of sn15 */
	sn15 = XtVaCreateManagedWidget( "sn15",
			xmToggleButtonWidgetClass,
			snap_Pane,
			RES_CONVERT( XmNlabelString, "15 pixel" ),
			RES_CONVERT( XmNmnemonic, "1" ),
			NULL );
	XtAddCallback( sn15, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_sn15,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sn15, (char *) UxDrawShellContext );


	/* Creation of snother */
	snother = XtVaCreateManagedWidget( "snother",
			xmToggleButtonWidgetClass,
			snap_Pane,
			RES_CONVERT( XmNlabelString, "other ..." ),
			RES_CONVERT( XmNmnemonic, "o" ),
			NULL );
	XtAddCallback( snother, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_snother,
		(XtPointer) UxDrawShellContext );

	UxPutContext( snother, (char *) UxDrawShellContext );


	/* Creation of sn1 */
	sn1 = XtVaCreateManagedWidget( "sn1",
			xmToggleButtonWidgetClass,
			snap_Pane,
			RES_CONVERT( XmNlabelString, "Snap Off" ),
			RES_CONVERT( XmNmnemonic, "O" ),
			NULL );
	XtAddCallback( sn1, XmNvalueChangedCallback,
		(XtCallbackProc) valueChangedCB_sn1,
		(XtPointer) UxDrawShellContext );

	UxPutContext( sn1, (char *) UxDrawShellContext );


	/* Creation of snap_casc */
	snap_casc = XtVaCreateManagedWidget( "snap_casc",
			xmCascadeButtonWidgetClass,
			drawPane,
			RES_CONVERT( XmNlabelString, "Snap" ),
			RES_CONVERT( XmNmnemonic, "S" ),
			XmNsubMenuId, snap_Pane,
			NULL );
	UxPutContext( snap_casc, (char *) UxDrawShellContext );


	/* Creation of drawPane_b15 */
	drawPane_b15 = XtVaCreateManagedWidget( "drawPane_b15",
			xmSeparatorWidgetClass,
			drawPane,
			NULL );
	UxPutContext( drawPane_b15, (char *) UxDrawShellContext );


	/* Creation of rotatePane */
	rotatePane_shell = XtVaCreatePopupShell ("rotatePane_shell",
			xmMenuShellWidgetClass, drawPane,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	rotatePane = XtVaCreateWidget( "rotatePane",
			xmRowColumnWidgetClass,
			rotatePane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( rotatePane, (char *) UxDrawShellContext );


	/* Creation of DrawDemoMainMenu_p14_b1 */
	DrawDemoMainMenu_p14_b1 = XtVaCreateManagedWidget( "DrawDemoMainMenu_p14_b1",
			xmPushButtonWidgetClass,
			rotatePane,
			RES_CONVERT( XmNlabelString, "Rotate Right" ),
			NULL );
	UxPutContext( DrawDemoMainMenu_p14_b1, (char *) UxDrawShellContext );


	/* Creation of DrawDemoMainMenu_p14_b2 */
	DrawDemoMainMenu_p14_b2 = XtVaCreateManagedWidget( "DrawDemoMainMenu_p14_b2",
			xmPushButtonWidgetClass,
			rotatePane,
			RES_CONVERT( XmNlabelString, "Rotate Left" ),
			NULL );
	UxPutContext( DrawDemoMainMenu_p14_b2, (char *) UxDrawShellContext );


	/* Creation of rotatePane_b5 */
	rotatePane_b5 = XtVaCreateManagedWidget( "rotatePane_b5",
			xmSeparatorWidgetClass,
			rotatePane,
			NULL );
	UxPutContext( rotatePane_b5, (char *) UxDrawShellContext );


	/* Creation of DrawDemoMainMenu_p14_b3 */
	DrawDemoMainMenu_p14_b3 = XtVaCreateManagedWidget( "DrawDemoMainMenu_p14_b3",
			xmPushButtonWidgetClass,
			rotatePane,
			RES_CONVERT( XmNlabelString, "Rotate Vertical" ),
			NULL );
	UxPutContext( DrawDemoMainMenu_p14_b3, (char *) UxDrawShellContext );


	/* Creation of DrawDemoMainMenu_p14_b4 */
	DrawDemoMainMenu_p14_b4 = XtVaCreateManagedWidget( "DrawDemoMainMenu_p14_b4",
			xmPushButtonWidgetClass,
			rotatePane,
			RES_CONVERT( XmNlabelString, "Rotate Horizontal" ),
			NULL );
	UxPutContext( DrawDemoMainMenu_p14_b4, (char *) UxDrawShellContext );


	/* Creation of drawPane_b16 */
	drawPane_b16 = XtVaCreateManagedWidget( "drawPane_b16",
			xmCascadeButtonGadgetClass,
			drawPane,
			RES_CONVERT( XmNlabelString, "Rotate" ),
			XmNsubMenuId, rotatePane,
			NULL );
	UxPutContext( drawPane_b16, (char *) UxDrawShellContext );


	/* Creation of pullDownMenu_top_b1 */
	pullDownMenu_top_b1 = XtVaCreateManagedWidget( "pullDownMenu_top_b1",
			xmCascadeButtonWidgetClass,
			DrawDemoMainMenu,
			RES_CONVERT( XmNlabelString, "Draw" ),
			XmNsubMenuId, drawPane,
			RES_CONVERT( XmNmnemonic, "d" ),
			NULL );
	UxPutContext( pullDownMenu_top_b1, (char *) UxDrawShellContext );


	/* Creation of helpPane */
	helpPane_shell = XtVaCreatePopupShell ("helpPane_shell",
			xmMenuShellWidgetClass, DrawDemoMainMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	helpPane = XtVaCreateWidget( "helpPane",
			xmRowColumnWidgetClass,
			helpPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( helpPane, (char *) UxDrawShellContext );


	/* Creation of onContextButton */
	onContextButton = XtVaCreateManagedWidget( "onContextButton",
			xmPushButtonGadgetClass,
			helpPane,
			RES_CONVERT( XmNlabelString, "On Context ..." ),
			RES_CONVERT( XmNmnemonic, "C" ),
			NULL );
	UxPutContext( onContextButton, (char *) UxDrawShellContext );


	/* Creation of onWindowButton */
	onWindowButton = XtVaCreateManagedWidget( "onWindowButton",
			xmPushButtonGadgetClass,
			helpPane,
			RES_CONVERT( XmNlabelString, "On Window ..." ),
			RES_CONVERT( XmNmnemonic, "W" ),
			NULL );
	UxPutContext( onWindowButton, (char *) UxDrawShellContext );


	/* Creation of onKeysButton */
	onKeysButton = XtVaCreateManagedWidget( "onKeysButton",
			xmPushButtonGadgetClass,
			helpPane,
			RES_CONVERT( XmNlabelString, "On Keys ..." ),
			RES_CONVERT( XmNmnemonic, "K" ),
			NULL );
	UxPutContext( onKeysButton, (char *) UxDrawShellContext );


	/* Creation of onHelpButton */
	onHelpButton = XtVaCreateManagedWidget( "onHelpButton",
			xmPushButtonGadgetClass,
			helpPane,
			RES_CONVERT( XmNlabelString, "On Help ..." ),
			RES_CONVERT( XmNmnemonic, "H" ),
			NULL );
	UxPutContext( onHelpButton, (char *) UxDrawShellContext );


	/* Creation of helpPane_b8 */
	helpPane_b8 = XtVaCreateManagedWidget( "helpPane_b8",
			xmSeparatorGadgetClass,
			helpPane,
			NULL );
	UxPutContext( helpPane_b8, (char *) UxDrawShellContext );


	/* Creation of indexButton */
	indexButton = XtVaCreateManagedWidget( "indexButton",
			xmPushButtonGadgetClass,
			helpPane,
			RES_CONVERT( XmNlabelString, "Index" ),
			RES_CONVERT( XmNmnemonic, "I" ),
			NULL );
	UxPutContext( indexButton, (char *) UxDrawShellContext );


	/* Creation of tutorialButton */
	tutorialButton = XtVaCreateManagedWidget( "tutorialButton",
			xmPushButtonGadgetClass,
			helpPane,
			RES_CONVERT( XmNlabelString, "Tutorial" ),
			RES_CONVERT( XmNmnemonic, "T" ),
			NULL );
	UxPutContext( tutorialButton, (char *) UxDrawShellContext );


	/* Creation of helpPane_b9 */
	helpPane_b9 = XtVaCreateManagedWidget( "helpPane_b9",
			xmSeparatorGadgetClass,
			helpPane,
			NULL );
	UxPutContext( helpPane_b9, (char *) UxDrawShellContext );


	/* Creation of onVersionButton */
	onVersionButton = XtVaCreateManagedWidget( "onVersionButton",
			xmPushButtonGadgetClass,
			helpPane,
			RES_CONVERT( XmNlabelString, "On Version ..." ),
			RES_CONVERT( XmNmnemonic, "V" ),
			NULL );
	UxPutContext( onVersionButton, (char *) UxDrawShellContext );


	/* Creation of helpCascade */
	helpCascade = XtVaCreateManagedWidget( "helpCascade",
			xmCascadeButtonWidgetClass,
			DrawDemoMainMenu,
			RES_CONVERT( XmNlabelString, "Help" ),
			RES_CONVERT( XmNmnemonic, "H" ),
			XmNsubMenuId, helpPane,
			NULL );
	UxPutContext( helpCascade, (char *) UxDrawShellContext );


	/* Creation of colorPane */
	colorPane_shell = XtVaCreatePopupShell ("colorPane_shell",
			xmMenuShellWidgetClass, DrawDemoMainMenu,
			XmNwidth, 1,
			XmNheight, 1,
			XmNallowShellResize, TRUE,
			XmNoverrideRedirect, TRUE,
			NULL );

	colorPane = XtVaCreateWidget( "colorPane",
			xmRowColumnWidgetClass,
			colorPane_shell,
			XmNrowColumnType, XmMENU_PULLDOWN,
			NULL );
	UxPutContext( colorPane, (char *) UxDrawShellContext );


	/* Creation of edit_colors_bt */
	edit_colors_bt = XtVaCreateManagedWidget( "edit_colors_bt",
			xmPushButtonWidgetClass,
			colorPane,
			RES_CONVERT( XmNlabelString, "Edit Colors..." ),
			NULL );
	XtAddCallback( edit_colors_bt, XmNactivateCallback,
		(XtCallbackProc) activateCB_edit_colors_bt,
		(XtPointer) UxDrawShellContext );

	UxPutContext( edit_colors_bt, (char *) UxDrawShellContext );


	/* Creation of DrawDemoMainMenu_top_b4 */
	DrawDemoMainMenu_top_b4 = XtVaCreateManagedWidget( "DrawDemoMainMenu_top_b4",
			xmCascadeButtonWidgetClass,
			DrawDemoMainMenu,
			RES_CONVERT( XmNlabelString, "Colors" ),
			XmNsubMenuId, colorPane,
			NULL );
	UxPutContext( DrawDemoMainMenu_top_b4, (char *) UxDrawShellContext );

	XtVaSetValues(DrawDemoMainMenu,
			XmNmenuHelpWidget, helpCascade,
			NULL );


	XtAddCallback( drawShell, XmNdestroyCallback,
		(XtCallbackProc) UxDestroyContextCB,
		(XtPointer) UxDrawShellContext);

	XmMainWindowSetAreas( mainWindow, DrawDemoMainMenu, (Widget) NULL,
			(Widget) NULL, (Widget) NULL, workAreaMgr );

	return ( drawShell );
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget	create_drawShell( _Uxpassed_wid, _Uxnome )
	swidget	_Uxpassed_wid;
	unsigned char	*_Uxnome;
{
	Widget                  rtrn;
	_UxCdrawShell           *UxContext;

	UxDrawShellContext = UxContext =
		(_UxCdrawShell *) UxNewContext( sizeof(_UxCdrawShell), False );

	passed_wid = _Uxpassed_wid;
	nome = _Uxnome;

	{
		draw_wid=passed_wid;
#ifndef ON_PAGEDIT
		FinestraDraw = draw_wid;
#endif
		win_step=XtWindow(draw_wid);
		curr_line_style= LineSolid;
		curr_line_width= 0;
#ifdef ON_PAGEDIT
		printf("\n deselez draget\n");
		DeselectAllDraget(draw_wid,True);
#else
		append_redraw(draw_wid);
#endif
		RisDbColori = XrmGetFileDatabase("FileColoriDefault");
		UxDisplay->db = RisDbColori;
		rtrn = _Uxbuild_drawShell();

		vett_fill=&vett_colori[16];
		vett_border=&vett_colori[0];
		load_vett_draw();
		load_vett_colors();
		set_fill_colors();
		/*
		 memorizza nella struttura di draw wid i bottoni in selezione
		 corrente per i colori ed il bottone di arrow
		*/
		set_bott_border(draw_wid,vett_border[0]);
		set_bott_fill(draw_wid,vett_fill[0]);
		set_bott_arrow(draw_wid,vett_draw[0]);
		set_def_bott_border(draw_wid,vett_border[0]);
		set_def_bott_fill(draw_wid,vett_border[1]);
		/*
		 seleziona i bottoni graficamente
		*/
		selez_bott_border(0);
		selez_bott_fill(1);
		set_arrow_mode(draw_wid);
		set_current_gcs(draw_wid);
#ifndef ON_PAGEDIT
		strcpy(str_title,"DRAW - on animated icon: ");
		strcat(str_title,nome);
#else
		/*
		 Caso di utilizzo del Draw all'interno del PAGEDIT
		 - il titolo specifica su quale pagina si sta lavorando
		 - vengono disabilitati i menu di File Edit e View che sono
		   in parte riportati direttamente sulla pagina di pagedit
		*/
		strcpy(str_title,"DRAW - on page: ");
		strcat(str_title,nome);
		set_something(saveButton,XmNsensitive, (void*) False);
		set_something(saveAsButton, XmNsensitive, (void*) False);
		set_something(editCascade,XmNsensitive, (void*) False);
		set_something(viewCascade,XmNsensitive, (void*) False);
#endif
#if defined ON_PAGEDIT || !defined XPRINTER_USED
		set_something(prtButton,XmNsensitive, (void*) False);
		set_something(prtSetupButton, XmNsensitive, (void*) False);
#endif
		set_something(rtrn,XmNtitle,(void*) str_title);
		return(rtrn);
	}
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/

