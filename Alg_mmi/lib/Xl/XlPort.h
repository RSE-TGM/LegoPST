/**********************************************************************
*
*       C Header:               XlPort.h
*       Subsystem:              1
*       Description:
*       %created_by:    lomgr %
*       %date_created:  Wed May 28 10:41:52 1997 %
*
**********************************************************************/
/*
   modulo XlPort.h
   tipo 
   release 5.1
   data 11/13/95
   reserved @(#)XlPort.h	5.1
*/
/*
 * XlPort.h - file di include pubblico per l'utilizzo del widget
 *             port
 */
#ifndef _XlPort_h
#define _XlPort_h

/*
 nomi di risorse utilizzate internamente all'oggetto
 */
#define XlNtipoPort "tipoPort"
#define XlCTipoPort "TipoPort"
#define XlDtipoPort "Port type"
#define XlOtipoPort "Analog input,Analog output,Digital input,Digital output"

#define XlNinterfacePort "interfacePort"
#define XlCInterfacePort "InterfacePort"
#define XlDinterfacePort "Interface port"
#define XlOinterfacePort "No,Yes"

#define XlNlayoutPort "layoutPort"
#define XlCLayoutPort "LayoutPort"
#define XlDlayoutPort "Port layout"
#define XlOlayoutPort "Arrow,Square"

#define XlNportName "portName"
#define XlCPortName "PortName"
#define XlDportName "Port name"

#define XlNportNameConnect "portNameConnect"
#define XlCPortNameConnect "PortNameConnect"
#define XlDportNameConnect "Connect port name"

#define XlNlabelForegroundColor "labelForegroundColor"
#define XlCLabelForegroundColor "LabelForegroundColor"
#define XlDlabelForegroundColor "Label foreground color"

#define XlNlabelBackgroundColor "labelBackgroundColor"
#define XlCLabelBackgroundColor "LabelBackgroundColor"
#define XlDlabelBackgroundColor "Label background color"

#define XlNconnectMode "connectMode"
#define XlCConnectMode "ConnectMode"
#define XlDconnectMode "Connect mode"
#define XlOconnectMode "No,Yes"

#define XlNvisibleMode "visibleMode"
#define XlCVisibleMode "VisibleMode"
#define XlDvisibleMode "Visible mode"
#define XlOvisibleMode "No,Yes"

#define XlNdigitalPortColor "digitalPortColor"
#define XlCDigitalPortColor "DigitalPortColor"
#define XlDdigitalPortColor "Digital Port Color"

#define XlNanalogPortColor "analogPortColor"
#define XlCAnalogPortColor "AnalogPortColor"
#define XlDanalogPortColor "Analog Port Color"

#define XlNPortColorStartConnection "PortColorStartConnection"
#define XlCPortColorStartConnection "portColorStartConnection"

#define XlNPortColorConnected "PortColorConnected"
#define XlCPortColorConnected "portColorConnected"

#define XlDefaultColorDigitalPort "blue"
#define XlDefaultColorAnalogPort "red"
#define XlDefaultColorStartConnection "green"
#define XlDefaultColorConnected "black"

#define XlNpositionTag "positionTag"
#define XlCPositionTag "PositionTag"
#define XlDpositionTag "Tag position"
#define XlOpositionTag "Center,Top,Bottom"


/*
	modi di visualizzazione delle porte e delle porte di interfaccia
*/
#define PORT_UNMANAGED	0
#define PORT_MANAGED    1
#define PORT_TAG	2

#define INTERFACE_PORT_UNMANAGED	PORT_UNMANAGED
#define INTERFACE_PORT_MANAGED    PORT_MANAGED
#define INTERFACE_PORT_TAG	PORT_TAG

#define NOME_LABEL_PORTE_INTERFACCIA  "wLabel"
#define INPUT_PORT_A  0
#define OUTPUT_PORT_A  1
#define INPUT_PORT_D  2
#define OUTPUT_PORT_D  3
#define PORT_DEFAULT  INPUT_PORT_A
#define _PORT_MAX     3

#define NOT_INTERFACE_PORT	0
#define INTERFACE_PORT		1
#define DEFAULT_INTERFACE_PORT  NOT_INTERFACE_PORT

#define PORT_ARROW	0
#define PORT_SQUARE	1
#define DEFAULT_LAYOUT_PORT	PORT_ARROW

#define CONNESSIONE_PORTE_NON_ATTIVA	0
#define CONNESSIONE_PORTE_ATTIVA	1
#define CONNESSIONE_PORTE_DEFAULT       CONNESSIONE_PORTE_NON_ATTIVA

#define VISIBLE_PORT_OFF		0
#define VISIBLE_PORT_ON			1
#define VISIBLE_PORT_STATE_DEFAULT      VISIBLE_PORT_ON

#define VISIBLE_PORT_NAME_OFF		0
#define VISIBLE_PORT_NAME_ON			1
#define VISIBLE_PORT_NAME_STATE_DEFAULT      VISIBLE_PORT_NAME_OFF

#define TAG_POSITION_CENTER	0
#define TAG_POSITION_TOP	1
#define TAG_POSITION_BOTTOM	2
#define TAG_POSITION_DEFAULT 	TAG_POSITION_CENTER

#define ICONREG_1 1

/* declare the class constant */
extern WidgetClass xlPortWidgetClass;

/* dichiarazione tipi per Classe e Instanziazione di Port */
typedef struct _XlPortClassRec *XlPortWidgetClass;
typedef struct _XlPortRec *XlPortWidget;

/* dichiarazioni di funzioni utilizzabili dall'esterno */
Boolean XlIsInterfacePort(Widget);
Boolean XlIsPort(Widget);
void XlPortResetConnection();
Boolean XlInterfacePortIsConnected(Widget );
Boolean XlPortIsConnected(Widget );
Widget XlPortGetConnectedPort(Widget );/* solo per le porte non di interf. */
Boolean XlCheckPort(Widget );
Boolean XlPortIsInput(Widget );
Boolean XlPortIsOutput(Widget );
void XlPortPosizionaLabel();
/* chi usa le porte (config) puo' farsi passare i rifiuti di PortSelect,
   che altrimenti finiscono solo su stderr: livello 0 informazione,
   1 rifiuto, 2 errore */
typedef void (*XlPortMsgProc)(Widget, int, char *);
void XlPortSetMsgHandler(XlPortMsgProc);

#endif /* _XlPort_h */

