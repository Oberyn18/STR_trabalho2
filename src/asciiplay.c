/* asciiplay.c - A very simple ascii movie player

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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <ncurses.h>

#include "utils.h"

#include <asciiplay.h>

/* Customizations */

#define SCHEDULER_POLICY SCHED_OTHER
#define SCHEDULER_PRIORITY 0

/* Change only if needed. */

#define LOGNAME  "log.txt"	

#define BLANK ' '

int delay;
float skew;
int goon;

int filecount = 0;
int logging = 0;

FILE *logfile;

struct timeval now, before, elapsed;
float av;
int min, max;
long long int count; 


/* SIGINT handler. The variable goon controls the main loop. */

void quit (int signum)
{
  goon=0;
}


/* Clear the scene vector. */

void clearscene (char scene[NSCENES][NROWS][NCOLS])
{
  int i, j, k;

  /* Initilize image */
  
  for (k=0; k<NSCENES; k++)
    for (i=0; i<NROWS; i++)
      for (j=0; j<NCOLS; j++)
	scene[k][i][j] = BLANK;
  
 }

/* Load all scenes. */

void readscenes (char *dir, char scene[NSCENES][NROWS][NCOLS])
{
  int i, j, k;
  FILE *file;
  char scenefile[1024], c;
  
  /* Read scnenes. */
  
  i=0;
  for (k=0; k<NSCENES; k++)
    {
      sprintf (scenefile, "%s/scenes/scene-%d.txt", 
         dir ? dir : "..", k);
      printf("%s", scenefile);
      file = fopen (scenefile, "r");
      sysfatal (!file);
      
      printf ("Reading %s\n", scenefile); 
 
      for (i=0; i<NROWS; i++)
	{

	  for (j=0; j<NCOLS; j++)
	    scene[k][i][j] = (char) fgetc (file);
	  
	  while (((c = fgetc(file)) != '\n') && (c != EOF))
	    printf ("%d\n", c);
	  
	}

      fclose (file);
     
    }
  
  
}

/* Draw ascii scene. */
  
void showscene (char scene[NSCENES][NROWS][NCOLS], int number)
{
  double fps;

  number = number % NSCENES;


  draw (scene, number);


  memcpy (&before, &now, sizeof (struct timeval));
  gettimeofday (&now, NULL);

  timeval_subtract (&elapsed, &now, &before);

  count++;

  if (count > 10)
    {
      if (elapsed.tv_usec < min)
	min = elapsed.tv_usec;
      if (elapsed.tv_usec > max)
	max = elapsed.tv_usec;

      fps = 1 / (elapsed.tv_sec + (elapsed.tv_usec * 1E-6));

      printf ("Frame %llu. Elapsed: %d %d, (%d) delay= %d skew= %5.2f, fps=%5.2f %s\n", count,
	      (int) elapsed.tv_sec, 
	      (int) elapsed.tv_usec, max-min, delay, skew, fps, logging ? "logging" : "");

      /* Plot: count, elapsed, min, max, delta, fps */

      if (logging)
	fprintf (logfile, "%llu %d %d %d %d %5.2f\n", count, (int) elapsed.tv_usec,
		 min, max, max-min, fps);
    }
  else
    printf ("Frame %llu. Elapsed: %d %d .... (calibrating)\n", 
	    count, (int) elapsed.tv_sec, 
	    (int) elapsed.tv_usec);
  
  
}
    




/* Process user input. This function runs in a separate thread. */

void * userinput (void *args)
{
  int c;
  while (1)
    {
      c = getchar();
      switch (c)
      {
      case '+':			/* Increase FPS. */
	delay = delay * (0.9);
	break;
      case '-':			/* Decrease FPS. */
	delay = delay * (1.1);
	break;
      case 'q':
	kill (0, SIGINT);	/* Quit. */
	break;
      case 'l':			/* Start logging. */
	if (!logging){
	  logfile = fopen (LOGNAME, "w");
	  sysfatal (!logfile);
	  logging = 1;
	}
	break;
      case 's':			/* Stop logging. */
	if (logging)
	  {
	    fclose (logfile);
	    logging=0;
	  }
	break;
      case 'd':			/* Increase FPS. */
	skew = skew * (0.9);
	break;
      case 'u':			/* Decrease FPS. */
	skew = skew * (1.1);
	break;
      default:
	break;
      }

    }
  
}

/* Flush log file. This function runs in a separate thread. */

void * flushlog (void *args)
{
  while (1)
    {
      if (logging)
	{
	  fflush (logfile);
	}
      sleep (1);
    }
  return NULL;
}


int main (int argc, char **argv)
{
  struct sigaction act;
  int rs;
  pthread_t pthread, pthreadlog;
  char *scenedir;

  char scene[NSCENES][NROWS][NCOLS];

  /* Process arguments. */


  scenedir = NULL;
  if (argc>1)
    scenedir = argv[1];
  

  /* Handle SIGNINT. */
  
  memset (&act, 0, sizeof (struct sigaction));
  act.sa_handler = quit;
  sigaction(SIGINT, &act, NULL);
  

  /* Ncurses initialization. */

  initscr();
  noecho();
  curs_set(FALSE);
  cbreak();

  /* Default values. */

  delay = 1E6 / 4;		/* Frame time. */
  skew = 1;			/* Slow down factor. */

  min = 1000000;
  max = 0;
  count =0;


  select_scheduler ();

  clearscene(scene);
  
  readscenes (scenedir, scene);
  
  /* showscene (scene,0); */

  gettimeofday (&now, NULL);

  rs = pthread_create (&pthread, NULL, userinput, NULL);
  sysfatal (rs);

  rs = pthread_create (&pthreadlog, NULL, flushlog, NULL);
  sysfatal (rs);

  goon=1;

  playmovie (scene);

  endwin();

  if (logging)
    fclose (logfile);

  return EXIT_SUCCESS;
}
