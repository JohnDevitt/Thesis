#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* cinterf.h is necessary for the XSB API, as well as the path manipulation routines */
#include "cinterf.h"

static FILE *ml_fp_interf = NULL;

int main(int argc, char *argv[])
{ 
  char init_string[MAXPATHLEN];
  int rc;
  XSB_StrDefine(return_string);
  int rc_first;
  char *str;
  char str1[1024];

  /* clean XSB compiled code */
  remove("featuresdef.P");
  remove("featlstn.xwam");
  remove("featuresall.xwam");

  ml_fp_interf = fopen ("features.FT", "w");

  str = getenv ("ML_ST_FEAT_TOOL");
  if (!str)
  {
    fprintf(stderr, "ML-FEAT-PROC Error: Environment variable ML_ST_FEAT_TOOL is not defined!..\n");
    exit(1);
  }
  
  sprintf(str1, "cp -f %s .", str);
  system(str1);

  /* 
   *   xsb_init_string relies on the calling program to pass the absolute or relative
   *   path name of the XSB installation directory.
   *   The absolute path of the XSB installation directory is provided by the environment variable XSB_DIR. 
   */

  str = getenv ("XSB_DIR");
  if (!str)
  {
    fprintf(stderr, "ML-FEAT-PROC Error: Environment variable XSB_DIR is not defined!..\n");
    exit(1);
  }

  strcpy(init_string,str);

  if (xsb_init_string(init_string)) 
  {
    fprintf(stderr,"%s initializing XSB: %s\n",xsb_get_init_error_type(),
	    xsb_get_init_error_message());
    exit(XSB_ERROR);
  }

  if (xsb_command_string(CTXTc "consult('featlstn.P').") == XSB_ERROR)
  {
    fprintf(stderr,"ML-FEAT-PROC Error (1): Error consulting featlst.P: %s/%s\n",xsb_get_error_type(CTXT),
          xsb_get_error_message(CTXT));
  }

  system("head -36 featlstn.P | sed 's/%//g' > featuresdef.P");
  system("cat features.P featuresdef.P > featuresall.P");

  /* Create command to consult the file featuresall.P, and send it. */
  if (xsb_command_string(CTXTc "consult('featuresall.P').") == XSB_ERROR)
  {
    fprintf(stderr,"ML-FEAT-PROC Error (2): Error consulting featuresall.P: %s/%s\n",xsb_get_error_type(CTXT),
          xsb_get_error_message(CTXT));
  }

  rc = xsb_query_string_string(CTXTc "ft(N,M).",&return_string,"=");
  rc_first = 1;
  while (rc == XSB_SUCCESS) 
  {
    if (!rc_first)
      fprintf(ml_fp_interf, ",");
    if (rc_first)
      rc_first = 0;
    fprintf(ml_fp_interf, " %s",(return_string.string));
    rc = xsb_next_string(CTXTc &return_string,"=");
  }
  fprintf(ml_fp_interf, "\n");

  if (rc == XSB_ERROR) 
   fprintf(stderr,"ML-FEAT-PROC Error: Query Error: %s/%s\n",xsb_get_error_type(CTXT),xsb_get_error_message(CTXT));

  xsb_close(CTXT);      /* Close connection */

  fclose(ml_fp_interf);

  return(0);
}
