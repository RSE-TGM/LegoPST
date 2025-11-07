wm maxsize . 680 630 
wm minsize . 680 630
wm title . "H2O Physical Properties"
frame .radio
frame .radio.uno
frame .radio.due
frame .radio.tre
frame .radio.qua
frame .radio.cin
pack .radio
set tabfun 1
set icount 1
foreach i {uno due tre qua cin} {
 pack .radio.$i -side left -expand 1 -anchor n -pady .3c -padx .3c
}
foreach i {t(p,s) t(p,h) tsat(p) s(p,h) s(p,t,y) s*\ \s**} {
 set a($i) [expr $icount]
 incr icount 
 set maiu [string toupper $i] 
 radiobutton .radio.$a($i) -text "$maiu" -variable tabfun \
            -relief flat -value $a($i) -command {setta $tabfun}
 pack .radio.$a($i) -side top -pady 2 -anchor w -in .radio.uno
}
foreach i {h(p,s) h(p,t,y) h*\ \h** ro(p,s) ro(p,h) ro(p,t,y) } {
 set a($i) [expr $icount]
 incr icount
 set maiu [string toupper $i] 
 radiobutton .radio.$a($i) -text "$maiu" -variable tabfun \
            -relief flat -value $a($i) -command {setta $tabfun}
 pack .radio.$a($i) -side top -pady 2 -anchor w -in .radio.due
}
foreach i {RO*RO** [dRO/dS]p(P,S) [dRO/dS]p(P,H) [dRO/dP]s(P,S) [dRO/dP]s(P,H) Y(P,S) } {
 set a($i) [expr $icount]
 incr icount
 set maiu [string trim $i] 
 radiobutton .radio.$a($i) -text "$maiu" -variable tabfun \
            -relief flat -value $a($i) -command {setta $tabfun}
 pack .radio.$a($i) -side top -pady 2 -anchor w -in .radio.tre
}
foreach i {Y(P,H) Cp(P,S) Cp(P,H) Cp(P,T,Y) MU(P,S) MU(P,H) } {
 set a($i) [expr $icount]
 incr icount
 set maiu [string trim $i] 
 radiobutton .radio.$a($i) -text "$maiu" -variable tabfun \
            -relief flat -value $a($i) -command {setta $tabfun}
 pack .radio.$a($i) -side top -pady 2 -anchor w -in .radio.qua
}
foreach i { MU(P,T,Y) K(P,S) K(P,H) K(P,T,Y) } {
 set a($i) [expr $icount]
 incr icount
 set maiu [string toupper $i] 
 radiobutton .radio.$a($i) -text "$maiu" -variable tabfun \
            -relief flat -value $a($i) -command {setta $tabfun}
 pack .radio.$a($i) -side top -pady 1 -anchor w -in .radio.cin
}
frame .sepa1 -relief ridge -bd 1 -height 2
pack .sepa1 -side top -fill x -pady .3c
frame .inputval
frame .m
pack .inputval -side top -expand 1 -anchor n -pady .3c
pack .m -side top
frame .m.inputval1
frame .m.inputval2
frame .m.inputval3
frame .m.inputval4
pack .m.inputval1 -side left -anchor nw -pady 1m -padx .5c
pack .m.inputval2 -side left -anchor nw -pady 1m -padx .5c
pack .m.inputval4 -side left -anchor w -pady 1m -padx 2c
pack .m.inputval3 -side right -anchor se -pady 1m
set inplabel {Calculation of temperature [�C]}
set enlab1 {Pressure [Bar]}
set enlab2 {Entropy [kJ/kg/�C]}
set enlab3 { }
set outlab1 {Temperature [�C]}
set outlab2 { }
set par1 0; set par2 0; set par3 0
set res1 0; set res2 0
label .inputval.lab -text $inplabel -font {Arial 10 bold}
pack .inputval.lab -side top -anchor n -fill x
label .m.inputval1.lab1 -text $enlab1
label .m.inputval1.lab2 -text $enlab2
label .m.inputval1.lab3 -text $enlab3
pack .m.inputval1.lab1 -side top -anchor w
pack .m.inputval1.lab2 -side top -anchor w -pady .6c
pack .m.inputval1.lab3 -side top -anchor w
entry .m.inputval2.en1 -relief sunken -width 10 -textvariable par1
entry .m.inputval2.en2 -relief sunken -width 10 -textvariable par2
pack .m.inputval2.en1 -side top -anchor w
pack .m.inputval2.en2 -side top -anchor w -pady .5c
button .m.inputval3.bu1 -text "Clear" -command pulientry
button .m.inputval3.bu2 -text "y=f(x)" -command {calcola $tabfun $par1 $par2 $par3}
pack .m.inputval3.bu2 -side right -padx .5c
pack .m.inputval3.bu1 -side right
frame .sepa2 -relief ridge -bd 1 -height 2
pack .sepa2 -side top -fill x -pady .5c
frame .b
frame .c
frame .d
pack .b -side top
frame .b.outval1
frame .b.outval2
frame .b.outval3
pack .b.outval1 -side left -anchor nw -pady .3c -padx .5c
pack .b.outval2 -side left -anchor nw -pady .3c
pack .b.outval3 -side left -padx 5c -pady .3c
pack .c -side top
pack .d -side right
label .b.outval1.lab1 -text $outlab1
label .b.outval1.lab2 -text $outlab2
label .b.outval2.lab1 -textvariable res1 -background #fff -relief sunken -bd 2
label .c.mex -text " " -font {Arial 8 bold}
button .d.quit -text "Quit" -command {destroy .}
pack .b.outval1.lab1 -side top -anchor w 
pack .b.outval1.lab2 -side top -anchor w -pady .3c
pack .b.outval2.lab1 -side top -anchor w
pack .c.mex -side top
pack .d.quit -side right -anchor se -padx 1c -pady .2c
set numen 2
set numpre 2
set numus 1
set numuspre 1

proc setta {tabfun} {
  global numen numpre enlab1 enlab2 enlab3 outlab1 outlab2
  global numus numuspre w
  global res1 res2
  set numpre $numen 
  set numuspre $numus
#  pulientry
  switch $tabfun \
 1 {set inplabel {Calculation of temperature [�C]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Entropy [kJ/kg/�C]}; set enlab3 { }; \
    set outlab1 {Temperature [�C]}; set outlab2 { }} \
 2 {set inplabel {Calculation of temperature [�C]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Enthalpy [kJ/kg]}; set enlab3 { }; \
    set outlab1 {Temperature [�C]}; set outlab2 { }} \
 3 {set inplabel {Calculation of saturation temperature [�C]}; set numen 1; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 { }; set enlab3 { }; \
    set outlab1 {Temperature [�C]}; set outlab2 { }} \
 4 {set inplabel {Calculation of entropy [kJ/kg/�C]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Enthalpy [kJ kg-1]}; set enlab3 { }; \
    set outlab1 {Entropy [kJ/kg/�C]}; set outlab2 { }} \
 5 {set inplabel {Calculation of entropy [kJ/kg/�C]}; set numen 3; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Temperature [�C]}; set enlab3 {Steam ratio [p.u.]}; \
    set outlab1 {Entropy [kJ/kg/�C]}; set outlab2 { }} \
 6 {set inplabel {Calculation of entropy of water and satured steam [kJ/kg/�C]}; set numen 1; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 { }; set enlab3 { }; \
    set outlab1 {Entropy of satured water [kJ/kg/�C]}; set outlab2 {Entropy of satured steam [kJ/kg/�C]}} \
 7 {set inplabel {Calculation of enthalpy [kJ/kg]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Entropy [kJ/kg/�C]}; set enlab3 { }; \
    set outlab1 {Enthalpy [kJ/kg]}; set outlab2 { }} \
 8 {set inplabel {Calculation of enthalpy [kJ kg-1]}; set numen 3; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Temperature [�C]}; set enlab3 {Steam ratio [p.u.]}; \
    set outlab1 {Enthalpy [kJ/kg]}; set outlab2 { }} \
 9 {set inplabel {Calculation of enthalpy of water and satured steam [kJ/kg]}; set numen 1; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 { }; set enlab3 { }; \
    set outlab1 {Entalpy of satured water [kJ/kg]}; set outlab2 {Entalpy of satured steam [kJ/kg]}} \
10 {set inplabel {Calculation of density RO [kg/m3]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Entropy [kJ/kg/�C]}; set enlab3 { }; \
    set outlab1 {Density RO [kg/m3]}; set outlab2 { }} \
11 {set inplabel {Calculation of density RO [kg/m3]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Enthalpy [kJ/kg]}; set enlab3 { }; \
    set outlab1 {Density RO [kg/m3]}; set outlab2 { }} \
12 {set inplabel {Calculation of density RO [kg/m3]}; set numen 3; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Temperature [�C]}; set enlab3 {Steam ratio [p.u.]}; \
    set outlab1 {Density RO [kg/m3]}; set outlab2 { }} \
13 {set inplabel {Calculation of density of water and satured steam [kg/m3]}; set numen 1; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 { }; set enlab3 { }; \
    set outlab1 {Density of satured water [kg/m3]}; set outlab2 {Density of satured steam [kg/m3]}} \
14 {set inplabel {Calculation of [dRO/dS]p [kg�K/m�J]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {entropy [kJ/kg/�C]}; set enlab3 { };
    set outlab1 {[dRO/dS]p [kg�K/m�J]}; set outlab2 { }} \
15 {set inplabel {Calculation of [dRO/ds]p [kg�K/m�J]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {enthalpy [kJ/kg]}; set enlab3 { };
    set outlab1 {[dRO/dS]p [kg�K/m�J]}; set outlab2 { }} \
16 {set inplabel {Calculation of [dRO/dP]s [s�/m�]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {entropy [kJ/kg/�C]}; set enlab3 { };
    set outlab1 {[dRO/dP]s [s�/m�]}; set outlab2 { }} \
17 {set inplabel {Calculation of [dRO/dP]s [s�/m�]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {enthalpy [kJ/kg]}; set enlab3 { };
    set outlab1 {[dRO/dP]s [s�/m�]}; set outlab2 { }} \
18 {set inplabel {Calculation of steam ratio [p.u.]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Entropy [kJ/kg/�C]}; set enlab3 { };
    set outlab1 {Steam ratio [p.u.]}; set outlab2 { }} \
19 {set inplabel {Calculation of steam ratio [p.u.]}; set numen 2; set numus 1; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Enthalpy [kJ/kg]}; set enlab3 { };
    set outlab1 {Steam ratio [p.u.]}; set outlab2 { }} \
20 {set inplabel {Calculation of specific heat CP [J/kg/�C]}; set numen 2; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Entropy [kJ/kg/�C]}; set enlab3 { };
    set outlab1 {Specific heat Cp [J/kg/�C]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam}} \
21 {set inplabel {Calculation of specific heat Cp [J/kg/�C]}; set numen 2; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Enthalpy [kJ/kg]}; set enlab3 { };
    set outlab1 {Specific heat Cp [J/kg/�C]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam}} \
22 {set inplabel {Calculation of specific heat Cp [J/kg/�C]}; set numen 3; set numus 2; \
     set enlab1 {Pressure [Bar]}; set enlab2 {Temperature [�C]}; set enlab3 {Steam ratio [p.u.]}; \
     set outlab1 {Specific heat CP [J/kg/�C]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam }} \
23 {set inplabel {Calculation of Dynamic Viscosity MU [kg/s m]}; set numen 2; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Entropy [kJ/kg/�C]}; set enlab3 { };
    set outlab1 {Dynamic Viscosity MU [kg/s m]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam}} \
24 {set inplabel {Calculation of Dynamic Viscosity MU [kg/s m]}; set numen 2; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Enthalpy [kJ/kg]}; set enlab3 { };
    set outlab1 {Dynamic Viscosity MU [kg/s m]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam }} \
25 {set inplabel {Calculation of Dynamic Viscosity MU [kg/s m]}; set numen 3; set numus 2; \
     set enlab1 {Pressure [Bar]}; set enlab2 {Temperature [�C]}; set enlab3 {Steam ratio [p.u.]}; \
     set outlab1 {Dynamic Viscosity MU [kg/s m]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam }} \
26 {set inplabel {Calculation of Thermal Conductivity K [W/m/K]}; set numen 2; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Entropy [kJ/kg/�C]}; set enlab3 { };
    set outlab1 {Thermal Conductivity K [W/m/K]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam }} \
27 {set inplabel {Calculation of Thermal Conductivity K [W/m/K]}; set numen 2; set numus 2; \
    set enlab1 {Pressure [Bar]}; set enlab2 {Enthalpy [kJ/kg]}; set enlab3 { };
    set outlab1 {Thermal Conductivity K [W/m/K]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam }} \
28 {set inplabel {Calculation of Thermal Conductivity K [W/m/K]}; set numen 3; set numus 2; \
     set enlab1 {Pressure [Bar]}; set enlab2 {Temperature [�C]}; set enlab3 {Steam ratio [p.u.]}; \
     set outlab1 {Thermal Conductivity K [W/m/K]}; set outlab2 {Fluid state flag:
  0 -> sub-cooled liquid
  1 -> saturated liquid (Y < 0.57)
  2 -> saturated steam (Y > 0.57)
  3 -> superheated steam } \
}
destroy .inputval.lab
destroy .m.inputval1.lab1 .m.inputval2.en1 .m.inputval1.lab2 .m.inputval1.lab3
destroy .b.outval1.lab1 .b.outval1.lab2 .b.outval2.lab1
if {$numpre >= 2} {
 destroy .m.inputval2.en2
}
if {$numpre == 3} {
 destroy .m.inputval2.en3
}
label .inputval.lab -text $inplabel -font {Arial 10 bold}
pack .inputval.lab -side top -anchor n -fill x
label .m.inputval1.lab1 -text $enlab1
label .m.inputval1.lab2 -text $enlab2
label .m.inputval1.lab3 -text $enlab3
entry .m.inputval2.en1 -relief sunken -width 10 -textvariable par1
pack .m.inputval1.lab1 -side top -anchor w
pack .m.inputval1.lab2 -side top -anchor w -pady .6c
pack .m.inputval1.lab3 -side top -anchor w
pack .m.inputval2.en1 -side top -anchor w
if {$numen >= 2} {
 entry .m.inputval2.en2 -relief sunken -width 10 -textvariable par2
 pack .m.inputval2.en2 -side top -anchor w -pady .5c
}
if {$numen == 3} {
 entry .m.inputval2.en3 -relief sunken -width 10 -textvariable par3
 pack .m.inputval2.en3 -side top -anchor w
}
if {$numuspre == 2} {
 destroy .b.outval2.lab2
}
label .b.outval1.lab1 -text $outlab1
label .b.outval1.lab2 -text $outlab2 -justify left
label .b.outval2.lab1 -textvariable res1 -background #fff -relief sunken -bd 2
pack .b.outval1.lab1 -side top -anchor w 
pack .b.outval1.lab2 -side left -anchor w -pady .3c
pack .b.outval2.lab1 -side top -anchor w
if {$numus == 2} {
 label .b.outval2.lab2 -textvariable res2 -background #fff -relief sunken -bd 2
 pack .b.outval2.lab2 -side top -anchor w -pady .3c
}
}

proc pulientry {} {
global par1 par2 par3 res1 res2 w
set par1 0
set par2 0
set par3 0
set res1 0
set res2 0
destroy .c.mex
label .c.mex -text " " -font {Arial 8 bold}
pack .c.mex -side top -expand 1
}

proc calcola {tabfun par1 par2 par3} {
global res1 res2 w env
global numus numuspre
set numuspre $numus
destroy .c.mex

set inptab [open $env(LG_TABLES)/tables.inp w]
puts $inptab $tabfun
puts $inptab $par1
puts $inptab $par2
puts $inptab $par3
close $inptab
set oldir [pwd]
cd $env(LG_TABLES)
exec H2O_tab
cd $oldir
set filex [file exists $env(LG_TABLES)/tables.out]
if {$filex == 1} {
 set fil [open $env(LG_TABLES)/tables.out r]
 gets $fil res1
 gets $fil res2
 close $fil
 file delete $env(LG_TABLES)/tables.out -force
 file delete $env(LG_TABLES)/tables.inp -force
} else {
 set res1 0
 set res2 0
 label .c.mex -text {RESULTS OUT OF WATER / STEAM TABLES} -font {Arial 8 bold}
 pack .c.mex -side top -expand 1 
}  
}
