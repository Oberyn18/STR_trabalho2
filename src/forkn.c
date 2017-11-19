/* forkb.c - For n subprocesses and stress cpu.

   Copyright (c) 2016, Monaco F. J. - <moanco@icmc.usp.br>

   This file is part of Real-Time Sand Box - RTSB

   RTSB is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include "utils.h"

/* Customizations */

#define SCHEDULER_POLICY SCHED_OTHER
#define SCHEDULER_PRIORITY 0

/* Main */

int main (int argc, char **argv)
{
  struct sched_param param;
  int pid, maxchilds, i, status, rs;
  
  if (argc<2)
    {
      fprintf (stderr, "Usage: forkn <number-of-childs>\n");
      exit (EXIT_FAILURE);
    }

  /*param.sched_priority = SCHEDULER_PRIORITY;
  rs = sched_setscheduler(0, SCHEDULER_POLICY, &param);*/

  param.sched_priority = sched_get_priority_max(2);
  rs = sched_setscheduler(0, SCHED_RR, &param);


  sysfatal (rs);


  maxchilds = atoi (argv[1]);

  printf ("Forking %d childs. Press Ctr-C to stop.\n", maxchilds);

  i = 0;
  pid = getpid ();
  while ((pid > 0) && (i++<maxchilds))
    {
      pid = fork ();
      sysfatal (pid<0);
    }

  if (pid>0)
    {
      while (1)
	wait (&status);
    }
  else
    {
      while (1);
    }
    

  return EXIT_SUCCESS;
}
