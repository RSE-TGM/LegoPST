
/**************************************************************************
 * Port to Linux IA32 / Alpha / x86_64 with gcc 3.x 
 **************************************************************************
 *
 * Contact Information: Loic Domaigne <loicWorks@gmx.net> 
 * -------------------
 *
 * Change Log
 * ----------
 *
 * Loic- 2003.05.03
 *  include <stdlib.h> to fix the warning "implicit declaration of 
 *  function 'exit'"
 *
 * Loic- 2003.05.08
 *  Fixed portability issue on Alpha (the variable ecode in chk_dce_err() is 
 *  an unsigned int on Alpha)
 * 
 * Loic- 2004.12.10
 *  Port to general LP64 architecture (including among others x86_64)
 * 
 **************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
void 
chk_dce_err(ecode, where, why, fatal)
     error_status_t ecode;
     char * where;
     char * why;
     unsigned int fatal;
{

  dce_error_string_t errstr;
  int error_status;                           
  
  if (ecode != error_status_ok)
    {
       dce_error_inq_text(ecode, errstr, &error_status); 
       if (error_status == error_status_ok)
	 /* make sure we print things right on LP64*/
#if defined(__LP64)
	 printf("ERROR.  where = <%s> why = <%s> error code = 0x%x"
		"reason = <%s>\n",
	      where, why, ecode, errstr);
#else
	 printf("ERROR.  where = <%s> why = <%s> error code = 0x%lx"
		"reason = <%s>\n",
	      where, why, ecode, errstr);
#endif
       else
	 /* make sure we print things right on LP64 */
#if defined(__LP64)
	 printf("ERROR.  where = <%s> why = <%s> error code = 0x%x\n",
		where, why, ecode);
#else
	 printf("ERROR.  where = <%s> why = <%s> error code = 0x%lx\n",
	      where, why, ecode);
#endif       
       if (fatal) exit(1);
    }
}
