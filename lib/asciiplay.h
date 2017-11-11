
#ifndef ASCIIPLAY_H
#define ASCIIPLAY_H

#define NSCENES 8		
#define NCOLS 80
#define NROWS 25

void showscene (char scene[NSCENES][NROWS][NCOLS], int number);

void playmovie (char scene[NSCENES][NROWS][NCOLS]);

void draw (char scene[NSCENES][NROWS][NCOLS], int number);

void select_scheduler (void);

extern int delay;
extern float skew;
extern int goon;


#endif	/* ASCIIPLAY_H */
