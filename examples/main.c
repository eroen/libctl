/* libctl: flexible Guile-based control files for scientific software 
 * Copyright (C) 1998 Steven G. Johnson
 *
 * This file may be used without restriction.  It is in the public
 * domain, and is NOT restricted by the terms of any GNU license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * Steven G. Johnson can be contacted at stevenj@alum.mit.edu.
 */

/**************************************************************************/

/* main program for a simulation that uses libctl.

   You should not need to modify this file.

   The resulting program will run any Scheme files that are passed
   as parameters on the command line.  It will automatically load
   ctl.scm and the specification file if the CTL_SCM and SPEC_SCM
   preprocessor symbols are defined to the corresponding filenames
   (e.g. see the accompanying Makefile).
*/

/**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <guile/gh.h>

#include "ctl-io.h"

#ifndef HAVE_GUILE_1_3
#define gh_load gh_eval_file
#endif

/**************************************************************************/

/* Main program.  Start up Guile, declare functions, load any
   scripts passed on the command-line, and drop into interactive
   mode if read-input-vars was never called. */

void main_entry(int argc, char *argv[])
{
  int i;

  /* Notify Guile of functions that we are making callable from Scheme.
     These are defined in the specifications file, from which the
     export_external_functions routine is automatically generated. */
  export_external_functions();

  /* Also export the read_input_vars and write_output_vars routines
     that are automatically generated from the specifications file: */
  gh_new_procedure ("read-input-vars", read_input_vars, 0, 0, 0);
  gh_new_procedure ("write-output-vars", write_output_vars, 0, 0, 0);

  /* load ctl.scm if it was given at compile time */
#ifdef CTL_SCM
  gh_load(CTL_SCM);
#endif

  /* load the specification file if it was given at compile time */
#ifdef SPEC_SCM
  gh_load(SPEC_SCM);
#endif

  /* load any scheme files specified on the command-line: */
  for (i = 1; i < argc; ++i)
    gh_load(argv[i]);

  /* if read-input-vars was never called, drop into interactive mode: */
  /* (the num_read_input_vars count is kept by ctl-io in read_input_vars) */
  if (num_read_input_vars == 0)
    gh_repl(argc, argv);
}

int main (int argc, char *argv[])
{
  gh_enter (argc, argv, main_entry);
  return EXIT_SUCCESS;
}
