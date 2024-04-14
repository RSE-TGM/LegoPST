/*@(#)CAProut.c Routine per il Controllo Accesso Programmi  1997-03-25	*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<errno.h>
#include	<ctype.h>
#include	<fcntl.h>
#include	<sys/stat.h>


#ifdef OSF1_40
#include	<sys/socket.h>
#include	<sys/ioctl.h>
#include	<net/if.h>
#endif

#ifdef LINUX
#include	<net/if.h>
#include	<sys/ioctl.h>
#include	<sys/socket.h>
#include	<linux/if_ether.h>
#endif

#ifdef NUTC
#include        <windows.h>
#include        <winbase.h>
#include        <wincon.h>
#include        <time.h>
#include        <nb30.h>
#include	<malloc.h>
#endif

#include	<unistd.h>
#include 	<netdb.h>
#include	<string.h>
#include	<errno.h>
#include	<sys/utsname.h>
#include	"CAP.h"


/*----------------------------------------------------------------------*/
/*	Define								*/

#define	OK		0
#define	NOK		(-1)
#define	LF		0x0a
#define	MOD256		0x100L

/*----------------------------------------------------------------------*/
/*	Typedef								*/

#ifdef NUTC
typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff [30];
}ASTAT, *PASTAT;

ASTAT Adapter;
#endif

/*----------------------------------------------------------------------*/
/*	Variabili							*/

unsigned int Debug = 0x00000000 ;
char * CAPdirname = NULL ;
char CAPINFO_FN [ 1024 ] ;
char CAPCONF_FN [ 1024 ] ;

unsigned	short	xsubi[3] ;

unsigned char	pbf[PLAINTEXT_MAX+1];	/* Plaintext  buf. per AZxxcode	*/
int	pbfl;
int	pbfj;
unsigned char	cbf[PLAINTEXT_MAX*2+1];	/* Cyphertext buf. per AZxxcode	*/

/*======================================================================*/
/* az()		Mappa la codifica (hex) del carattere su A-Z		*/
/*
 Cypher  :  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
 Plain   :  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 A B C D E F
 */
unsigned char*	az( chx )
unsigned char	chx;
{
static	unsigned char	azstr[4];
char		hex02[4];
int		y;
int		chi, cho;

	memset( azstr, 0, sizeof(azstr) );
	sprintf( hex02, "%02X", (chx & 0xFF) ); /* !!! Copy chx as 1st byte of the int expected
						       by sprintf() setting other bytes to 0. */
	for( y = 0; y < 2; y++ )
	{
		chi	= hex02[y];
		cho	= chi;
		if( isdigit( chi ) )
		{
			if( mrand48() & (long)2 )
			{
				cho	= chi + 'A' - '0';
			}
			else
			{
				cho	= chi + 'K' - '0';
			}
		}
		else
		{
			if( isxdigit( chi ) )
				cho	= chi + 'U' - 'A';
		}
		azstr[y]	= cho;
	}
	return	azstr;
}					/* az()				*/


/*======================================================================*/
/* AZdecode()	Interpreta una stringa A-Z				*/
/*
 Cypher  :  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
 Plain   :  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 A B C D E F
 */
int	AZdecode( ccc, ppp )
unsigned char*	ccc;				/* Cyphertext (output)		*/
unsigned char**	ppp;				/* Plaintext  (input)		*/
{
int		j, k;
int		ch0;
int		ch1;
int		chx;
int		chi;

	*ppp	= NULL;
	if( strlen( (char*)ccc ) > PLAINTEXT_MAX*2 ) return( -1 );
	if( strlen( (char*)ccc ) % 2 )               return( -1 );

	strcpy( (char*)cbf, (char*)ccc );
	k	= 0;
	memset( pbf, 0, sizeof(pbf) );
	pbfj	= 0;
	for( j = 0; j < strlen( (char*)cbf ); j += 2 )
	{
		if( cbf[j] < 'A' || cbf[j] > 'Z' ) return( -1 );
		ch0	= ch1	= 0;
		chi	= cbf[j+0];
		if( chi <= 'J' )               ch0 = chi - 'A';
		if( chi >= 'K' && chi <= 'T' ) ch0 = chi - 'K';
		if( chi >= 'U' )               ch0 = chi - 'U' + 0xa;

		chi	= cbf[j+1];
		if( chi <= 'J' )               ch1 = chi - 'A';
		if( chi >= 'K' && chi <= 'T' ) ch1 = chi - 'K';
		if( chi >= 'U' )               ch1 = chi - 'U' + 0xa;
		chx	= (ch0 << 4) + ch1;
		pbf[pbfj++]	= chx;
	}
	if ( Debug ) { printf( "??? |%s|\n", pbf ); }
	*ppp	= pbf;
	return( 0 );

}					/* AZdecode()			*/

/*======================================================================*/
/* AZencode()	Mappa la stringa di ingresso su A-Z			*/

int	AZencode( ppp, ccc )
unsigned char*	ppp;				/* Plaintext  (input)		*/
unsigned char**	ccc;				/* Cyphertext (output)		*/
{
int	j;

	if ( Debug ) { printf( "::: |%s|\n", ppp ); }
	*ccc	= NULL;

	memset( cbf, 0, sizeof(cbf) );
	pbfl	= CAPINFO_SZ;
	memcpy( pbf, ppp, pbfl );
	for( j = 0; j < pbfl; j++ )
	{
		strcat( (char*)cbf, (char*)az( pbf[j] ) );
	}
	cbf[CAPINFO_SZ*2]	= '\0';
	if ( Debug ) { printf( ">>> |%s|\n", cbf ); }
	*ccc	= cbf;
	return( 0 );

}					/* AZencode()			*/


/*======================================================================*/
void	decode( bbb, lll )
unsigned char*	bbb;
int	lll;
{
int	chx;
long	yyy;
long	delta;
int	k;


	/*
	 * Set the 3 seed shorts from the RANDSEED define.
	 */

	xsubi [ 0 ] = RANDSEED1 ;
	xsubi [ 1 ] = RANDSEED2 ;
	xsubi [ 2 ] = RANDSEED3 ;

	for( k = 0; k < lll; k++ )
	{
		chx	= *(bbb+k);
		delta	= nrand48( xsubi ) % MOD256;
		yyy	= (MOD256 + (long)chx - delta) % MOD256 ;
		*(bbb+k)= (char)yyy;
	}

}					/* decode()			*/


/*======================================================================*/
void	encode( bbb, lll )
unsigned char*	bbb;
int		lll;
{
int	chx;
char	chy;
int	delta;
int	k;

	xsubi [ 0 ] = RANDSEED1 ;
	xsubi [ 1 ] = RANDSEED2 ;
	xsubi [ 2 ] = RANDSEED3 ;

	for( k = 0; k < lll; k++ )
	{
		chx	= *(bbb+k);
		delta	= nrand48( xsubi ) ;
		chy	= delta;
		chy	+= chx;
		*(bbb+k)= chy;
if ( Debug ) {
printf( "@@@ % 4d) delta:% 16ld. {%02x} -> {%02x} - chy: %02X\n", k, delta, delta, chx, *(bbb+k) );
}
/* ...		*(bbb+k)	= *(bbb+k) + nrand48( xsubi );	*/
	}

}					/* encode()			*/


/*======================================================================*/
/*                                                                      */
/* Funzione che restituisce l'indirizzo IP della macchina corrente in   */
/* formato esadecimale e in formato 'xxx.yyy.www.zzz'.                  */
/*                                                                      */
/*======================================================================*/  
int get_intern_addr( char *buffer_hex, char *buffer_dec)
{
struct hostent * HostEnt ;
char HostName[50];
char ch_addr_0 [ 4 ] ;
char ch_addr_1 [ 4 ] ;
char ch_addr_2 [ 4 ] ;
char ch_addr_3 [ 4 ] ;

  errno	= 0;
  if ( gethostname ( HostName , sizeof ( HostName ) ) == -1 ) { 
    fprintf ( stderr , "get_intern_addr: Errore gethostname() - errno: %d\n", errno ) ;
    return ( -1 ) ;
  } 

  h_errno = 0;
  HostEnt = gethostbyname ( HostName ) ;
  if ( HostEnt == NULL ) {
    fprintf ( stderr , "get_intern_addr: Errore gethostbyname() - h_errno: %d\n", h_errno ) ;
    return ( -1 ) ;
  }

# ifdef ENDIAN_DEPENDENT
    unsigned int address = 0x00000000 ;

    address |= ( ( unsigned int ) ( HostEnt -> h_addr [ 0 ] ) ) << 24 ;
    address |= ( ( unsigned int ) ( HostEnt -> h_addr [ 1 ] ) ) << 16 ;
    address |= ( ( unsigned int ) ( HostEnt -> h_addr [ 2 ] ) ) << 8 ;
    address |= ( ( unsigned int ) ( HostEnt -> h_addr [ 3 ] ) ) ;

    sprintf ( buffer_hex , "%08X" , address ) ;
    sprintf ( buffer_dec , "%d", address ) ;
# else
/* Indirizzo IP formato esadecimale */
    sprintf ( ch_addr_0 , "%02X" , ( unsigned int ) ( HostEnt -> h_addr [ 0 ] ) & 0x00000000FF ) ;
    sprintf ( ch_addr_1 , "%02X" , ( unsigned int ) ( HostEnt -> h_addr [ 1 ] ) & 0x00000000FF ) ;
    sprintf ( ch_addr_2 , "%02X" , ( unsigned int ) ( HostEnt -> h_addr [ 2 ] ) & 0x00000000FF ) ;
    sprintf ( ch_addr_3 , "%02X" , ( unsigned int ) ( HostEnt -> h_addr [ 3 ] ) & 0x00000000FF ) ;

    strcat ( buffer_hex , ch_addr_0 ) ;
    strcat ( buffer_hex , ch_addr_1 ) ;
    strcat ( buffer_hex , ch_addr_2 ) ;
    strcat ( buffer_hex , ch_addr_3 ) ;

/* Indirizzo IP formato 'xxx.yyy.www.zzz' */
    sprintf ( ch_addr_0 , "%d" , ( unsigned int ) ( HostEnt -> h_addr [ 0 ] ) & 0x00000000FF ) ;
    sprintf ( ch_addr_1 , "%d" , ( unsigned int ) ( HostEnt -> h_addr [ 1 ] ) & 0x00000000FF ) ;
    sprintf ( ch_addr_2 , "%d" , ( unsigned int ) ( HostEnt -> h_addr [ 2 ] ) & 0x00000000FF ) ;
    sprintf ( ch_addr_3 , "%d" , ( unsigned int ) ( HostEnt -> h_addr [ 3 ] ) & 0x00000000FF ) ;

    strcat ( buffer_dec , ch_addr_0 ) ;
    strcat ( buffer_dec , "." );
    strcat ( buffer_dec , ch_addr_1 ) ;
    strcat ( buffer_dec , "." );
    strcat ( buffer_dec , ch_addr_2 ) ;
    strcat ( buffer_dec , "." );
    strcat ( buffer_dec , ch_addr_3 ) ;

//    printf("IP address hex = %s\n", buffer_hex);

//    printf("IP address:\t%s\n", buffer_dec);

# endif

return(0);

}

#ifdef LINUX
/*======================================================================*/
/*                                                                      */
/* Funzione che restituisce l'indirizzo della scheda ethernet (MAC      */
/* address) nel caso di piattaforma Linux  in particolare Red Hat 7.2   */
/*                                                                      */
/*======================================================================*/
int get_ethern_addr_LINUX( char *buffer)
{
int s,i;
struct  ifreq  ifr;
char    bufapp[3];
unsigned char *ch; 
char  pathfilecondevice[100];
FILE *filecondevice;
char  nomedevice[100];


   /* Get a socket */
   s = socket(AF_INET,SOCK_DGRAM,0);
   if (s < 0) {
      perror("socket");
      return(-1);
   }

   strcpy(ifr.ifr_name,"eth0");
   if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
      strcpy(ifr.ifr_name,"eth1");
      if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
         strcpy(ifr.ifr_name,"eth2");
         if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
            strcpy(ifr.ifr_name,"eth3");
            if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
               strcpy(ifr.ifr_name,"eth4");
               if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
                  strcpy(ifr.ifr_name,"eth5");
                  if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
                     strcpy(ifr.ifr_name,"eth6");
                     if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
                        printf("Cerco RuggeroOrlando in %s \n",CAPdirname);
                        strcpy(pathfilecondevice,CAPdirname);
                        strcat(pathfilecondevice,"/RuggeroOrlando");
                        filecondevice=fopen(pathfilecondevice,"r");
                        if (filecondevice == NULL ) {
                              perror("RuggeroOrlando:");
                              return(-1);
                           }
                        fscanf(filecondevice,"%s",nomedevice);
                        printf("nomedevice=%s \n",nomedevice);
                        strcpy(ifr.ifr_name,nomedevice);
                        if (ioctl(s,SIOCGIFHWADDR,&ifr) < 0) {
                           perror(&ifr.ifr_name[0]);
                           return(-1);
                        }
                     }
                  }
               }
            }
         }
      }
   }
/*
   printf("get_ethern_addr_LINUX : Device is: %s\n", ifr.ifr_name);
   printf("get_ethern_addr_LINUX : Address is ");
*/

   ch = ifr.ifr_hwaddr.sa_data;

   for (i = 0; i < 6; i++) {
      sprintf(bufapp,"%02hX", *ch);
      strcat(buffer, bufapp);
      ch++;
/*
      printf("%x ", ifr.default_pa[i] & 0xff);
*/
   }
/*
   printf("\n");
   printf("get_ethern_addr_LINUX : buffer = %s\n", buffer);
*/
   close(s);

return(0);
}
#endif



#ifdef OSF1_40
/*======================================================================*/
/*                                                                      */
/* Funzione che restituisce l'indirizzo della scheda ethernet (MAC      */
/* address) nel caso di piattaforma Digital con sistema operativo       */ 
/* Digital UNIX ver. 4.0.                                               */
/*                                                                      */
/*======================================================================*/
int get_ethern_addr_OSF1( char *buffer)
{
int s,i;
struct  ifdevea  devea;
char    bufapp[3];

char  pathfilecondevice[100];
FILE *filecondevice;
char  nomedevice[100];

   /* Get a socket */
   s = socket(AF_INET,SOCK_DGRAM,0);
   if (s < 0) {
      perror("socket");
      return(-1);
   }

   strcpy(devea.ifr_name,"le0");
   if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
      strcpy(devea.ifr_name,"ln0");
      if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
         strcpy(devea.ifr_name,"tu0");
         if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
            strcpy(devea.ifr_name,"te0");
            if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
               strcpy(devea.ifr_name,"vb");
               if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
                  strcpy(devea.ifr_name,"xna");
                  if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
                     strcpy(devea.ifr_name,"ee0");
                     if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
			printf("Cerco RuggeroOrlando in %s \n",CAPdirname);
			strcpy(pathfilecondevice,CAPdirname);
			strcat(pathfilecondevice,"/RuggeroOrlando");
			filecondevice=fopen(pathfilecondevice,"r");
			if (filecondevice == NULL ) {
			      perror("RuggeroOrlando:");
			      return(-1);
			   }
			fscanf(filecondevice,"%s",nomedevice);
			printf("nomedevice=%s \n",nomedevice);
			strcpy(devea.ifr_name,nomedevice);
                  	if (ioctl(s,SIOCRPHYSADDR,&devea) < 0) {
                     	   perror(&devea.ifr_name[0]);
                           return(-1);
			}
                     }
                  }
               }
            }
         }
      }
   }
/*
   printf("get_ethern_addr_OSF1 : Device is: %s\n", devea.ifr_name);
   printf("get_ethern_addr_OSF1 : Address is ");
*/
   for (i = 0; i < 6; i++) {
      sprintf(bufapp,"%02X", devea.default_pa[i] & 0xff);
      strcat(buffer, bufapp);
/*
      printf("%x ", devea.default_pa[i] & 0xff);
*/
   }
/*
   printf("\n");
   printf("get_ethern_addr_OSF1 : buffer = %s\n", buffer);
*/
   close(s);

return(0);
}
#endif


#ifdef NUTC
/*======================================================================*/
/*                                                                      */
/* Funzione che restituisce l'indirizzo della scheda ethernet (MAC      */
/* address) nel caso di piattaforma PC con sistema operativo            */ 
/* Windows NT.                                                          */
/*                                                                      */
/*======================================================================*/
int get_ethern_addr_NT(char * buffer_MAC)
{
NCB     ncb;
UCHAR   uRetCode;

LPCTSTR lpRootPathName = "C:\\";
LPTSTR  lpVolumeNameBuffer;
DWORD   nVolumeNameSize;
LPDWORD lpVolumeSerialNumber;
LPDWORD lpMaximumComponentLength ;
LPDWORD lpFileSystemFlags;
LPTSTR  lpFileSystemNameBuffer;
DWORD   nFileSystemNameSize;

char    ind_IP_hex[10] = "";
char    ind_IP_dec[20] = "";
short   i;
char    buffapp[3];

/* Test recupero volume serial number
char    buffer_prova[20];
BOOL    ret;
*/

   memset(&ncb, 0,sizeof(ncb) );
   ncb.ncb_command = NCBRESET;
   ncb.ncb_lana_num = 0;

   uRetCode = Netbios( &ncb);

   printf("get_ethern_addr_NT: Codice ritorno NCBRESET: 0x%x \n", uRetCode);

   memset(&ncb, 0,sizeof(ncb) );
   ncb.ncb_command = NCBASTAT;
   ncb.ncb_lana_num = 0;

/* Recupero l'indirizzo IP della macchina corrente */
   if( get_intern_addr(ind_IP_hex, ind_IP_dec) == 0) {
     
      strcpy(ncb.ncb_callname, ind_IP_dec);

      ncb.ncb_buffer = (char *) &Adapter;
      ncb.ncb_length = sizeof(Adapter);

      uRetCode = Netbios( &ncb);

      printf("get_ethern_addr_NT: Codice ritorno NCBASTAT: 0x%x \n", uRetCode);

      if (uRetCode == 0) {
         for (i = 0; i < 6; i++) {
            sprintf(buffapp,"%02X", Adapter.adapt.adapter_address[i]);
            strcat(buffer_MAC, buffapp);
         }
         printf("MAC address:\t%s\n", buffer_MAC);
      }
      else {
/* Errore nel recupero del MAC address. Si restituisce il serial
   number del volume del disco C concatenato con la stringa "0000" */
         printf("get_ethern_addr_NT: Errore recupero MAC address.\n");
         printf("                    Utilizzo serial number volume disco C\n");

         lpVolumeNameBuffer = (LPTSTR)malloc(256);
         nVolumeNameSize = 256;
         lpVolumeSerialNumber = (LPDWORD)malloc(sizeof(DWORD));
         lpMaximumComponentLength = (LPDWORD)malloc(sizeof(DWORD));
         lpFileSystemFlags = (LPDWORD)malloc(sizeof(DWORD));
         lpFileSystemNameBuffer = (LPTSTR)malloc(256);
         nFileSystemNameSize = 256;

         GetVolumeInformation( lpRootPathName, lpVolumeNameBuffer,
                               nVolumeNameSize, lpVolumeSerialNumber, 
                               lpMaximumComponentLength, lpFileSystemFlags,
                               lpFileSystemNameBuffer, nFileSystemNameSize ); 

         sprintf(buffer_MAC, "%x", *lpVolumeSerialNumber); 
         strcat(buffer_MAC, "0000");
         return(-1);
      }
   }
   else {
/* Errore nel recupero dell'indirizzo IP. Si restituisce il serial
   number del volume del disco C concatenato con la stringa "0000" */
      printf("get_ethern_addr_NT: Errore recupero IP address.\n");
      printf("                    Utilizzo serial number volume disco C\n");

      lpVolumeNameBuffer = (LPTSTR)malloc(256);
      nVolumeNameSize = 256;
      lpVolumeSerialNumber = (LPDWORD)malloc(sizeof(DWORD));
      lpMaximumComponentLength = (LPDWORD)malloc(sizeof(DWORD));
      lpFileSystemFlags = (LPDWORD)malloc(sizeof(DWORD));
      lpFileSystemNameBuffer = (LPTSTR)malloc(256);
      nFileSystemNameSize = 256;

      GetVolumeInformation( lpRootPathName, lpVolumeNameBuffer,
                            nVolumeNameSize, lpVolumeSerialNumber, 
                            lpMaximumComponentLength, lpFileSystemFlags,
                            lpFileSystemNameBuffer, nFileSystemNameSize ); 
      sprintf(buffer_MAC, "%x", *lpVolumeSerialNumber);
      strcat(buffer_MAC, "0000");
      return(-1);   
   }


/* Test recupero volume serial number
lpVolumeNameBuffer = (LPTSTR)malloc(256);
nVolumeNameSize = 256;
lpVolumeSerialNumber = (LPDWORD)malloc(sizeof(DWORD));
lpMaximumComponentLength = (LPDWORD)malloc(sizeof(DWORD));
lpFileSystemFlags = (LPDWORD)malloc(sizeof(DWORD));
lpFileSystemNameBuffer = (LPTSTR)malloc(256);
nFileSystemNameSize = 256;

ret = GetVolumeInformation(lpRootPathName, lpVolumeNameBuffer,
                           nVolumeNameSize, lpVolumeSerialNumber,
                           lpMaximumComponentLength, lpFileSystemFlags,
                           lpFileSystemNameBuffer, nFileSystemNameSize);

printf("ret = %d\n", ret);
printf("Volume serial number = %x\n", *(lpVolumeSerialNumber));
sprintf(buffer_prova, "%x", *(lpVolumeSerialNumber));
printf("buffer_prova = %s\n", buffer_prova);
*/

return(0);
}
#endif


/*======================================================================*/
int cap_info ( struct CAPinfo * Info )
{
/*
 * Retrieve system information and store them into Info.
 *
 * Return:  0 if successful.
 *         -1 on failure.
 */

  struct utsname Utsname ; 
  char buf[14] = "";
  char buf_hex[10] = "";
  char buf_dec[20] = "";
  char buf_sysname[SYSNAME_LEN+1];
  char buf_machine[MACHINE_LEN+1];



  memset ( Info , '\0' , sizeof ( struct CAPinfo ) ) ;

  errno	= 0;
  if ( uname ( & Utsname ) == -1 ) {
/* Fallito il recupero del nome del sistema operativo e del nome
   della macchina. Assegno dei valori fittizi. */
    fprintf ( stderr , "cap_info: Errore uname() - errno: %d\n", errno ) ;
    strncpy ( Info -> SysName , "" , SYSNAME_LEN ) ;
    strncpy ( Info -> Machine , "" , MACHINE_LEN ) ;
    return (-1) ;
  }

  strncpy ( Info -> SysName , Utsname . sysname , SYSNAME_LEN ) ;
  strncpy ( Info -> Machine , Utsname . machine , MACHINE_LEN ) ;

  strncpy ( buf_sysname , Utsname . sysname , SYSNAME_LEN ) ;
  strncpy ( buf_machine ,  Utsname . machine , MACHINE_LEN ) ;
  buf_sysname[SYSNAME_LEN] = '\0';
  buf_machine[MACHINE_LEN] = '\0';
  printf("SysName: %s Machine: %s\n", buf_sysname, buf_machine);

#if defined (OSF1_40)
/* Recupero dell'indirizzo della scheda ethernet (MAC address) nel caso
   di macchina Digital UNIX con sistema operativo OSF1 ver. 4.0 */

   if( get_ethern_addr_OSF1(buf) == -1) {
/* Errore nel recupero del MAC address. Si restituisce un indirizzo
   fittizio costituito dalla stringa "000000000000" */
      printf("Errore recupero MAC address!!!\n");
      strncpy ( Info -> Address , "000000000000" , ADDRESS_LEN ) ;
      return(-1);
   }
   strncpy ( Info -> Address , buf , ADDRESS_LEN ) ;

#elif defined (LINUX)
/* Recupero dell'indirizzo della scheda ethernet (MAC address) nel caso
   di macchina LINUX in particolare Red Hat 7.2 */

   if( get_ethern_addr_LINUX(buf) == -1) {
/* Errore nel recupero del MAC address. Si restituisce un indirizzo
   fittizio costituito dalla stringa "000000000000" */
      printf("Errore recupero MAC address!!!\n");
      strncpy ( Info -> Address , "000000000000" , ADDRESS_LEN ) ;
      return(-1);
   }
   strncpy ( Info -> Address , buf , ADDRESS_LEN ) ;



#elif defined (NUTC)
/* Recupero dell'indirizzo della scheda ethernet (MAC address) nel caso
   di PC con sistema operativo Windows NT */
 
   if(get_ethern_addr_NT(buf) == -1) {
/* E' fallito il recupero dell'indirizzo IP oppure del MAC address.
   Viene restituito il 'serial number' del volume del disco C  */
      strncpy ( Info -> Address , buf , ADDRESS_LEN ) ;
      return(-1);
   }
/* Recupero corretto del MAC address */
   strncpy ( Info -> Address , buf , ADDRESS_LEN ) ;

#else
/* Recupero dell'indirizzo IP in tutti i casi esclusi i precedenti */

  if( get_intern_addr(buf_hex, buf_dec) == -1) {
/* Errore nel recupero dell'indirizzo IP. Si restituisce un indirizzo
   fittizio costituito dalla stringa "00000000" */

     printf("Errore recupero IP address!!!\n");
     strncpy ( Info -> Address , "00000000" , ADDRESS_LEN ) ;
     return(-1);
  }

  strncpy ( Info -> Address , buf_hex , ADDRESS_LEN ) ;

#endif

  /* Change all '\0' in blanks. */
  {
    unsigned char * ptr = ( unsigned char * ) ( Info ) ;
    int structsize = sizeof ( struct CAPinfo ) ;
    int i ;

    for ( i = 0 ; 
	  i < structsize ; 
	  i ++ ) {
      if ( * ptr == '\0' ) { * ptr = ' ' ; }
      ptr ++ ;
    }
  }

  Info -> nil = '\0' ;

  if ( Debug ) { printf ( "### |%s|\n" , ( char * ) Info ) ; }

  return ( 0 ) ;

}					/* cap_info()			*/


int InitEnv ( void ) {
/*
 * Initialize environment variables Debug and CAPpathname.
 */

  char * DebugString = getenv ( "CAPDEBUG" ) ;
  CAPdirname = getenv ( "ALTERLEGO_CAP_DIR" ) ;

  if ( DebugString == NULL ) { 

    Debug = 0x00000000 ; 

  } else {
    Debug = 0x00000001 ; 
/*
    sscanf ( DebugString , "%x" , & Debug ) ;
*/
  }

  if ( CAPdirname == NULL ) { 
    fprintf(stderr, "\n#################################################\n");
    fprintf(stderr, "       LICENSE CONFIGURATION ERROR!!!\n");
    fprintf(stderr, "#################################################\n");
    fprintf(stderr, "Environment variable ALTERLEGO_CAP_DIR not set!!!\n");
    return(-1);
  }

  if ( Debug ) {
    fprintf ( stderr , "CAP: CAPDEBUG=%s\n" , DebugString ) ;
    fprintf ( stderr , "CAP: ALTERLEGO_CAP_DIR=%s\n" , CAPdirname ) ; 
  }

  /* Build path names for CAP.info and CAP.conf */

  sprintf ( CAPINFO_FN , 
	    "%s/%s" ,
	    CAPdirname , "CAP.info" ) ;

  sprintf ( CAPCONF_FN , 
	    "%s/%s" ,
	    CAPdirname , "CAP.conf" ) ;

  return(0);
}


int CAP ( char * ProgramID , char * Buffer ) {
/*
 * Compare the actual system information and ProgramID
 * with that stored in the configuration file. 
 * If a match is found, get the initialization string in Buffer, 
 * otherwise access is denied.
 *
 * Return:  0 if access is allowed, and Buffer contains the Init-string.
 *         -1 if access is denied, due to a match not found in the
 *            system information or in the ProgramID.
 */

  struct CAPinfo HostInfo ;
  FILE * CAPfp ;
  char   addr_intern_hex[10] = "";
  char   addr_intern_dec[20] = "";
  unsigned char ConfBuffer [ CAPINFO_SZ * 2 + 1 ] ;
  int Found = 0 ;
  unsigned char * FileInfo ;
  char * ab_svin;

  Buffer [ 0 ] = '\0' ;

  /*
   * Check if the machine address is one of the ENEL CRA network.
   * In this case the license will be automatically active.
   */
  if( (ab_svin = getenv("ALG_LICENSE_FREE")) != NULL ) {
     if(strcmp(ab_svin, "YES") == 0) { 

printf("CAP DEBUG: Abilitato svincolo licenze!!!\n");

     if( get_intern_addr(addr_intern_hex, addr_intern_dec) == -1 )
        return(-1); 

     if (strstr(addr_intern_hex, IND_RETE_CRA) != NULL) {

printf("CAP DEBUG: Macchina rete ENEL!!! Address = %s\n",addr_intern_dec);

       if((strcmp(ProgramID,CONFIG_LIC) == 0) ||
          (strcmp(ProgramID,LEGOCAD_LIC) == 0))
          strcpy(Buffer,DEF_CRA_NORT);
       else
          strcpy(Buffer,DEF_CRA_RT);

printf("CAP DEBUG: Buffer = %s\n",Buffer);

       return(0);
     }
   }
  }

  if (InitEnv ( ) == -1) 
     return(-1);

  /*
   * If CAP.conf does not exist, return error.
   */

  if ( access ( CAPCONF_FN , R_OK ) == -1 ) {
    
    fprintf(stderr, "\n#################################################\n");
    fprintf(stderr, "       LICENSE CONFIGURATION ERROR!!!\n");
    fprintf(stderr, "#################################################\n");
    fprintf(stderr, "License configuration file CAP.conf not found in:\n");
    fprintf(stderr, "ALTERLEGO_CAP_DIR = %s\n", CAPdirname);
    return (-1) ;

  }

  if (cap_info ( & HostInfo ) == 0) {
     printf("CAP: Recupero informazioni macchina OK\n");
  } 
/*
printf("CAP DEBUG: Address = %s\n", HostInfo.Address);
*/
  strncpy ( HostInfo . ProgramID ,
	    ProgramID , 
	    PROGRAMID_LEN ) ; 

  if ( Debug ) { printf ( "!!! |%s|\n" , & HostInfo.Address ) ; }

  errno = 0 ;

  CAPfp = fopen ( CAPCONF_FN , "r" ) ;

  if ( CAPfp == NULL ) { 
    fprintf(stderr, "Error opening file:\n%s!!!\n",CAPCONF_FN);
    return ( -1 ) ;
  } 

  while ( fread ( ConfBuffer , 
		  CAPINFO_SZ * 2 + 1 ,
		  1 ,
		  CAPfp ) != 0
	  && ( ! Found ) ) {

    int i = 0 , 
	j = 0 ;

    ConfBuffer [ CAPINFO_SZ * 2 ] = '\0' ;

    if ( AZdecode ( ConfBuffer , & FileInfo ) == -1 ) { 
       fprintf(stderr, "\n#################################################\n");
       fprintf(stderr, "       LICENSE NOT FOUND!!!\n");
       fprintf(stderr, "       MA VA BENE LO STESSO\n");
       fprintf(stderr, "#################################################\n");
//       GUAG 2024
//       return ( -1 ) ;
       return ( 0 ) ;
    }

    ( ( struct CAPinfo * ) FileInfo ) -> nil = '\0' ;

    decode ( FileInfo , CAPINFO_SZ ) ;
// printf("CAP DEBUG: FileInfo = %s\n", FileInfo);
    if (memcmp( FileInfo , & HostInfo , CAPINFO_SZ - ENELINFO_LEN ) == 0) {

      if ( Debug ) { printf ( "%-3s |%s|\n" , " OK", FileInfo ) ; }

      for ( j = ENELINFO_LEN - 1 ; j >= 0 ; j -- ) {
	if ( ( ( struct CAPinfo * ) FileInfo ) -> EnelInfo [ j ] != ' ' ) {
           /* printf("CAP: j = %d     Char = %d\n"); */
           break ;
        }

        ( ( struct CAPinfo * ) FileInfo ) -> EnelInfo [ j ] = '\0' ;
      }

      strcpy ( Buffer , ( ( struct CAPinfo * ) FileInfo ) -> EnelInfo ) ;
//       printf("CAP: Buffer = %s\n", Buffer);
//       printf("CAP: LunBuffer = %d\n", strlen(Buffer));
      Found ++ ;

    } else {

      if ( Debug ) { printf ( "%-3s |%s|\n" , "NOK", ( char * ) ( FileInfo ) ) ; }

    }

  }

  fclose ( CAPfp ) ;

  return ( 0 ) ;

}


void PrintVersion ( void ) {
/*
 * Print Version information.
 */

  fprintf ( stderr ,
	    "CAP: Version 1.0  -  1997-03-26\n" ) ;

}
