
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

int firstTime = 1;
void draw (char scene[NSCENES][NROWS][NCOLS], int number)
{
  int i, j, s;
  /* Each line will have this length */
  int totalLength = (NROWS*(NCOLS+2) + 1);
  /* Array of strings */
  char strings[NSCENES+1][totalLength];
  int position;
  if(firstTime != 0) {
    /* Go through all the scenes */
    for (s = 0; s < NSCENES; s++) {
      position = 0;
      for (i = 0; i < NROWS; i++) {
        for (j = 0; j < NCOLS; j++) {
          strings[s][position++] = scene[s][i][j];
        }
        /* Add special characteres */
        strings[s][position++] = '\n';
        strings[s][position++] = '\r';
      }
      /*  End of string */
      strings[s][position++] = '\0';
    }
    /* Print the required string  */
    printf("%s", strings[number]);
    firstTime = 0;
  } else {
    /* This scene presents problems when being displayed using just printf, so for this case, i'll do the normal way */
    if (number == 7) {
      for (i = 0; i < NROWS; i++) {
          for (j = 0; j < NCOLS; j++) {
            putchar (scene[number][i][j]);
          }
          /* Add special characteres */
          putchar ('\n');
          putchar ('\r'); 
      }
    } else {
      /* Print the required string  */
      printf("%s", strings[number]);
    }
  } 
}
