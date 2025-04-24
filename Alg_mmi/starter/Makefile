# ******* Telelogic expanded section *******

# make_macros from project "Alg_mmi-2007A1_RHE4_lomgr
GUI_BUILD=/usr/bin/aic
OS=LINUX
SQLITE_LIB=-L$(LEGOROOT_LIB)/sqlite_lib -lsqlite3
THREAD_LIB=-L$(LEGOROOT_LIB)/dcethreads_lib -ldcethreads -ldl
X_LIB=-L/usr/X11R6/lib -lMrm -lXm -lXt -lX11
X_INCLUDE=-I. -I/usr/local/include -I/usr/lib/gcc-lib/i386-redhat-linux/2.96/include -I/usr/include -I/usr/include/uil -I/usr/include/Xt -I/usr/include/lib 
C_FLAGS=-g  -DLINUX  -DXT_CODE -DXOPEN_CATALOG -DUNIX -Dmmap=_mmap_32_ -DXPRINTER_USED -DXLIB_ILLEGAL_ACCESS -I$(LEGOROOT_LIB)/dcethreads_include -I$(LEGOROOT_LIB)/sqlite_include  -I/usr/include
VERSIONE=-DBANCO_MANOVRA -DSCADA -DBACKTRACK -DF22_APPEND -DSNAP_PIAC -DPIACENZA -DREPLAY -DMFFR -DSAVEPERT
LINKER_OPTIONS=
UXCGEN=run_uxcgen12.sh $@ $<
#   modulo Makefile
#   tipo 
#   release 1.2
#   data 96/03/18
#   reserved @(#)Makefile	1.2
#
DEPEND=$(LEGOROOT)/install/makedepend
CFLAGS = -DSCADA_MMI -I$(LEGORT_INCLUDE) -I$(LEGOMMI_INCLUDE) -D$(OS) $(C_FLAGS) $(VERSIONE)
LIBUTIL =  $(LEGOROOT_LIB)/libutil.a

SORGENTI = starter.c
OGGETTI  = starter.o


all:  $(LEGOMMI_BIN)/starter

makefile:Makefile
	make inc

$(LEGOMMI_BIN)/starter: $(OGGETTI) $(LIBUTIL)
	cc -o $(LEGOMMI_BIN)/starter $(LINKER_OPTIONS) $(OGGETTI) \
        $(LIBUTIL) -lm
inc: 
	$(DEPEND) -s "# DO NOT DELETE" -- $(CFLAGS) -- $(SORGENTI)
	make

#
# Parte dedicata alla gestione con SCCS
#
INCLUDE =
FFILES =
CFILES = $(SORGENTI) $(INCLUDE)
UILFILES =
COMFILES = make.com
ALLFILES = $(CFILES) $(UILFILES) $(FFILES) Makefile make.com
GET=sccs get
REL=

# Da eseguire solo la prima volta !!
# appende l'header ai sorgenti e li inserisce in SCCS
first:
	mkdir SCCS
	chmod 777 SCCS
	ins_header $(LEGOROOT)/install/header.tpl $(CFILES)
	ins_header $(LEGOROOT)/install/header_uil.tpl $(UILFILES)
	ins_header $(LEGOROOT)/install/header_for.tpl $(FFILES)
	ins_header $(LEGOROOT)/install/header_com.tpl $(COMFILES)
	chmod a-w $(ALLFILES)
	sccs create $(ALLFILES)
what:
	sccs what $(ALLFILES)
get: $(ALLFILES)
$(ALLFILES):
	sccs get $(REL) $(ALLFILES)
edit:
	sccs get -e $(ALLFILES)
delta:
	sccs delta $(REL) $(ALLFILES)
force:
	sccs clean
	sccs get $(REL) Makefile
	make all REL=$(REL)
newversion:
	sccs check
	sccs clean
	$(GET) -e $(REL) $(ALLFILES)
	sccs delta $(ALLFILES)

              
vms:
	sccs get $(REL) $(ALLFILES)
	dcp -c $(ALLFILES) $(MACCHINA_VMS)::"$(DISCO_VMS):[$(PATH_VMS)`path_vms`]"
