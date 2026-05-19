proc checkopen { filename modo } {

    if {[catch {open $filename $modo} fileid]} {
	tk_messageBox -message "Can't open file $filename." -type ok -icon error
	exit 1
    } else { return $fileid }
}
