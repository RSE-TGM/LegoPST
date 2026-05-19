# this is the subroutine to create the popup menu
#
# Return-Value is the name of the popup menu

proc create_popup {w {choice 0}} { set p $w.popup; catch {destroy $p} ; menu $p
# general functions
$p add command -label Happy -command "$w insert end {i'm happy to help you\n}"  
$p add command -label Copy -command "tk_textCopy $w" 
$p add command -label Paste -command "tk_textPaste $w" 
$p add command -label Cut -command "tk_textCut $w"

# special functions
switch $choice { 
1 {$p add command -label IP  -command "$w insert end {245.245.123.23\n}"}
2 {$p add command -label Phone  -command "$w insert end {0800 - 845674\n}"}
3 {$p add command -label Mail  -command "$w insert end {bas6@cs.waikato.ac.nz\n}"}
4 {} 5 {$p add command -label home -command "$w insert end {www.news.com\n}"}
default {} 
} 

return $p 
}

# this is the main programm
# 1. create a text-widget (.t)
# 2. put a bind command in the text-widget
# 2.1 the subroutine create_popup creates the commands you want
# 2.2 the tk_popup function creates the popup menu on the screen
# 3. Create five text-widgets (.1 - .5) with one special bind functions

text .t -width 40 -height 10
pack .t -side bottom 
bind .t <ButtonRelease-3> {tk_popup [create_popup %W] %X %Y}

for {set i 1} {$i<=5} {incr i} { 
text .$i -width 40 -height 3
bind .$i <ButtonRelease-3> "tk_popup [create_popup .$i $i] %X %Y"
pack .$i -side top 

}
