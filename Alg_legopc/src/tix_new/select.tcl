
source $env(LG_TIX)/checkopen.tcl


#proc ShowNames { c tipo } {
#         set nelem 0
##         set ms 1000
#         if { $tipo == 1} {
#		catch [$c delete infoitemname]
#	 return
#	 }       
#         foreach item [$c find withtag module] {
#		set lc [$c bbox $item] 
#		set x [lindex $lc 2]
#		set y [lindex $lc 1]
##tk_messageBox -icon warning -message $lc -type ok
#        	if { $tipo == 2 } { set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.name]]] }
#		if { $tipo == 3 } { set pisqu [file rootname [lindex [$c gettags $item] [lsearch [$c gettags $item] *.cls]]] }
#
#         	set tid [$c create text $x $y -text $pisqu -tags {infoitemname}]
#		set lc [$c bbox $tid] 
#		set x1 [lindex $lc 0]
#		set y1 [lindex $lc 1]
#		set x2 [lindex $lc 2]
#		set y2 [lindex $lc 3]
#		$c create rectangle $x1 $y1 $x2 $y2 -fill olivedrab1 -tags {infoitemname}
#		$c raise $tid
#		incr nelem
#         } 
##after $ms $c delete infoitemname
#}



proc showVars {c x y} {
#tk_messageBox -message "contiene: $c"
	set myTags [$c gettags current]
      if {[file rootname [lindex $myTags [lsearch $myTags *.ori]]] == ""} {
	 set modName [string range [lindex $myTags 3] 0 3]
      } else {
       set modName [string range [lindex $myTags 5] 0 3]
	}
	catch	{$c delete modOpt}
	showIt $modName
	$c delete modOpt
}

proc pix2c {v} {
#	global convConst
#	return [expr ($v/$convConst)*1.9]
        return [expr ($v/$::dpimon)*2.54]
	   }


proc showIt {mod} {
	
package require Tix 

	global curFileName campo contarighe
	global numVars nomeVars tipoVars tipVarMod

	if {[loadVariables $mod]} return
              
	set w1 .varwin
	catch {destroy $w1}
	toplevel $w1
	wm title $w1 "Block $mod variables"
	wm maxsize $w1 620 440

	set but $w1.buttons
	frame $but
	pack $but -side bottom -pady 10
	button $but.quit -text "Quit" -command "destroy $w1" 
      pack $but.quit -side left -expand yes -ipadx 35 -padx 50

	set w2 $w1.sfondo
      tixScrolledWindow $w2 -scrollbar "auto -x"
	pack $w2
	set w3 [$w2 subwidget window]

	frame $w3.middle
	pack $w3.middle -side top -padx 10 -pady 10

	set tit $w3.middle.titolo
	frame $tit
	pack $tit -side top

	set campo(0) "Var.Name"
	set campo(1) "  "

	button $tit.nome -textvariable campo(0) -width 8 -relief raised \
		-font titleFont -state disabled -background green \
		-disabledforeground black
	button $tit.tipo -textvariable campo(1) -width 2 -relief raised \
		-font titleFont -state disabled -background green
	pack $tit.nome $tit.tipo -side left -anchor w

      set contarighe 0
      set index 0
      set fileid [checkopen "proc/f14.dat" r]
      gets $fileid linea
      while { $linea != "*LG*EOF" } {
       set righef14($contarighe) $linea
	 incr contarighe
	 gets $fileid  linea
                                 
       if { $linea == "*LG*CONDIZIONI INIZIALI VARIABILI DI INGRESSO  " } {
        while {$linea != "*LG*DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI" } {
            incr index
            set righef14($contarighe) $linea
	      incr contarighe
	      gets $fileid linea
	     
             if { $linea != "*LG*DATI FISICI E GEOMETRICI DEL SISTEMA SUDDIVISI A BLOCCHI" } {
                set nome [string range $linea 4 11]
		    set rig $w3.middle.riga$index
		    frame $rig
	          pack $rig -side top
                set nome1 [ string range $linea 8 11 ]
                if { $mod == $nome1 } { 
		       set nomeVars($index) $nome
                   set tipovar $tipVarMod($nome)
        	       button $rig.nome -textvariable nomeVars($index) -width 8 -relief groove \
		      	-font entryFont -background gray80
			 bind $rig.nome <1> "setSlot $nomeVars($index)"
		       button $rig.tipo -textvariable tipoVars($index) -width 2 -relief groove \
		      	-font entryFont -state disabled -background yellow \
		      	-disabledforeground black
		       pack $rig.nome $rig.tipo -side left
		               }
                      }	
                 }
           }
      }
}     

proc setSlot {nomepert} {
      global evar1 rvar
	global env
	set evar1{rvar} $nomepert	   
      set comando $env(LG_BIN)/LegoSim/LgSincroAccShM.exe 
      set argomenti $nomepert
      exec  $comando $argomenti
	}
global env envir
global wsBackg wsWidth wsBackg wsWidth wsHeight 
global wsScrWidth wsScrHeight wsXsiz wsYsiz wsXmin wsYmin 
global progNumb portw curTool progName palId jshift porth
global numBlo

set envir Select

set modelname [file tail [pwd]]
set curFileName "$env(LG_MODELS)/$modelname/$modelname"
wm title . "Graphics Vars Selection - $curFileName.tom"
set c .frame.c

menu .menu -tearoff 0
set m .menu.file
menu $m -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "File" -menu $m -underline 0

$m add command -label "Quit" -command "destroy ."

set m .menu.opt
menu $m -tearoff 0 -activebackground darkblue -activeforeground white
.menu add cascade -label "View" -menu $m -underline 0

set showon 2
$m add radio -label "off" -variable showon -value 1 -command "ShowNames $c 1"
$m add radio -label "Show Names" -variable showon -value 2 -command " ShowNames $c 2"
$m add radio -label "Show Classes" -variable showon -value 3 -command "ShowNames $c 3"
# aggiungo menu per la ricerca dei blocchi
set ::canv1 $c
set ::modalita 1
$m add command -label "Find name" -command trova_win 
##############


. configure -menu .menu

set  ::dpimon [winfo fpixels . 1i ]

frame .frame
pack .frame -side top -fill both -expand yes

set wsBackg gray90
set wsWidth 512
set wsHeight 384
set wsScrWidth 800
set wsScrHeight 600
set wsXsiz $wsScrWidth
set wsYsiz $wsScrHeight
set wsXmin $wsWidth
set wsYmin $wsHeight

set progNumb 0
set portw 12
set curTool none
set progName "name"
set palId 0
set jshift .5
set porth 12
set numBlo 0
#set convConst 72.

canvas $c -bg $wsBackg -scrollregion [list 0 0 $wsScrWidth $wsScrHeight] \
	-width $wsWidth -height $wsHeight -xscrollcommand ".frame.hscroll set" \
	-yscrollcommand ".frame.vscroll set"
scrollbar .frame.vscroll -command "$c yview"
scrollbar .frame.hscroll -orient horiz -command "$c xview"

grid $c -in .frame -row 0 -column 0 -rowspan 1 -columnspan 1 -sticky news
grid .frame.vscroll -row 0 -column 1 -rowspan 1 -columnspan 1 -sticky news
grid .frame.hscroll -row 1 -column 0 -rowspan 1 -columnspan 1 -sticky news
grid rowconfig .frame 0 -weight 1 -minsize 0
grid columnconfig .frame 0 -weight 1 -minsize 0

#images
set pixmapPath "$env(LG_PIXMAPS)"
set olddir [pwd]
cd $pixmapPath
foreach singleconn [glob -nocomplain {????_[news].ppm}] {
	image create photo [file rootname $singleconn] -file [file join $pixmapPath $singleconn]
}
image create photo portActive -file [file join $pixmapPath actconn.ppm]
cd $olddir

#help files
set helpPath "$env(LG_HELP)"
catch {font create helpFont -family Helvetica -size 9}
catch {font create entryFont -family Courier -size 12}
catch {font create titleFont -family Courier -size 12 -weight bold -slant italic}
catch {font create entryBig -family Courier -size 14}
catch {font create titleBig -family Courier -size 14 -weight bold}

$c bind module <1> "showVars $c %x %y"

source $env(LG_TIX)/read_con.tcl
source $env(LG_TIX)/read_f01.tcl
source $env(LG_TIX)/fileio.tcl
source $env(LG_TIX)/itemjoin.tcl
source $env(LG_TIX)/read_f14.tcl
source $env(LG_TIX)/viewmgr.tcl

topRead $c $curFileName
loadF01 $c no
catch { ShowNames $c 2}
