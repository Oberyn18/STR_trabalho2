
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <sched.h>
#include <asciiplay.h>
#include "src/utils.h"

/* This function implements the program's main loop. It cycles
   through the scenes and plays the animation. */

void playmovie (char scene[NSCENES][NROWS][NCOLS])
{

  int k = 0;
  struct timespec how_long;
  how_long.tv_sec = 0;
  while (goon)
    {
      clear ();
      refresh ();
      showscene (scene, k);
      k = (k + 1) % NSCENES;
      how_long.tv_nsec = (delay+skew) * 1e3;
      nanosleep (&how_long, NULL);
    }

}

/* Select scheduler and priority. */

void select_scheduler (void)
{
  struct sched_param param;
  int rs;

  param.sched_priority = 0;
  rs = sched_setscheduler(0, SCHED_OTHER, &param);
  sysfatal (rs);

}

/* Draw a the given scene on the screen. Currently, this iterates through the
   scene matrix outputig each caracter by means of indivudal puchar calls. One
   may want to try a different approach which favour performance. For instance,
   issuing a single 'write' call for each line. Would this yield any significant
   performance improvement? */

void draw (char scene[NSCENES][NROWS][NCOLS], int number)
{
  int i, j;
  for (i=0; i<NROWS; i++)
    {
      for (j=0; j<NCOLS; j++)
	{
	  putchar (scene[number][i][j]);
	}
      putchar ('\n');
      putchar ('\r');
    }
}
