/******************************************************************************************

    Mini-Tools and Mini-Games for the Command Line.
    One page of code each or your money back.
    by Thomas Gruetzmacher (tomaes.32x.de)


    While most of these mini programs are mere exercises, some are
    a bit more clever and provide some level of entertainment and utility.

    Licence: http://creativecommons.org/licenses/by/3.0/deed.en



    latest update: 07/2012

   1 -> NBR_GAME : Three lines of obfuscated code for this mini game that
                   might amuse a first grader for up to twenty seconds.

   2 -> MM_GAME  : A version of the old board / deduction game 'Mastermind'
                   but with numbers rather than colors. Read about it in
                   an old C64 book.

   3 -> PWDGEN   : A simple password / token generator that takes
                   a formated string to spill out password suggestions.

   4 -> MEDT     : A small Hex-editor / Memory viewer / File dumper tool
                   Big enough to have its own help screen.

   5 -> PICROSS  : Picross / Nonogram based game. Unfinished yet.

   6 -> ANSIART  : Minimalist ANSI art (*.ans files) viewer. Very unfinished.

   7 -> EST_GAME : Estimation / Observation test game.

   8 -> FMERGE   : File stitching / Filter / Conversion tool
                   Some esoteric features in this one.

   9 -> PERM     : Permutation / Shuffle / Anagram tool

  10 -> SBFED    : File Encoding / Decoding tool

  11 -> PRIME    : Prime number evaluator.

  12 -> AJOKE    : "Gentlemen, The results speak for themselves."

*******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define xxxNBR_GAME
#define xxxMM_GAME
#define xxxPICROSS_GAME
#define xxxPWDGEN
#define xxxMEDT
#define xxxANSIART
#define xxxEST_GAME
#define xxxFMERGE
#define xxxPERM
#define xxxSBFED
#define xxxPRIME
#define AJOKE

int main( int argc, char **argv )
{

/******************************************************************************************

  Random number guessing game in a couple of lines. "Not pretty, but fairly short" version.

*******************************************************************************************/

#ifdef NBR_GAME
#define P printf
 int r, a=4, i=0, A=1, Z=10; srand(time(0));

 for( P("%d..%d\n",A,Z),r=(rand()%(Z-A+1))+A; r^i&&(a--?P("(try %d/4)?:",4-a): !P("Comp wins! (was:%d)",r)); )
 {
    scanf("%d",&i); P("%s\n",r>i?"Mine's >":r<i?"Mine's <":"Got it.\nYou win!");
 }
#endif




/******************************************************************************************

    A version of Mastermind, but based on numbers, not colors
    (http://en.wikipedia.org/wiki/Mastermind_(game))

*******************************************************************************************/

#ifdef MM_GAME

#define DIGITS 3

 char inp[10] = {0}, nr[DIGITS+1] = {0};
 int i, guess = 0, rplace = 0, wplace = 0;

 srand(time(NULL));

 for( i = 0; i < DIGITS; i++ ) nr[i] = '0' + rand() % 10;


 while( rplace != DIGITS )
 {
     printf("\n Enter number (%d digits): ", DIGITS);
     scanf("%s3", &inp);

     rplace = wplace = 0;
     guess++;

     for( i = 0; i < DIGITS; i++ )
     {
        if (inp[i] == nr[i]) rplace++; else if (strchr(nr,inp[i])) wplace++;
     }

      printf("\n Right place: %d, Wrong place: %d, attempts: %d", rplace, wplace, guess );
 }

 puts("\n Got it!");


#endif



/******************************************************************************************

  Minimal passwort / token generator, based on a formated input string.
  Example: 5000x0xxx

           first digit (in HEX): generate 5 results
           each '0': replace with random number
           each 'x': replace with random character

*******************************************************************************************/

#ifdef PWDGEN
#define MAXLEN 20
 int i, r;
 char inp[MAXLEN] = {0}, outp[MAXLEN] = {0};

 printf("\nInput:");
 scanf("%s20", &inp);

 if ( *inp >='a' && *inp <='f' ) r = *inp-'a'+10; else
 if ( *inp >='0' && *inp <='9' ) r = *inp-'0';

 srand( time(NULL) );

 while(r--)
 {
     for(i = 1; i < strlen(inp); i++)
         outp[i-1] = (inp[i]=='0') ? outp[i-1]='0' + rand() % 10:
                     (inp[i]=='x') ? outp[i-1]='a' + rand() % 26: inp[i];

     printf("\n (%2.d) pwd: %s", r + 1, outp);
 }
#endif

/******************************************************************************************

  Minimal ANSI art viewer. Work in progress.

*******************************************************************************************/

#ifdef ANSIART

#define ESC 0x1B
#define LEFTBRACKET '['

 char * filename = "ab-onion.ans";
 char * out[100][80];
 char inp, c, a, b;
 int i, count = 0;
 int x, y, sx, sy;

 for( y = 0; y < 100;  y++ )
 for( x = 0; x <  80;  x++ )
 {
     out[y][x] = 0x20;
 }

 FILE *f = fopen(filename, "r");

 if(f == NULL)
 {
    printf("\nError opening file <%s>\n", filename);
    return -1;
 }
 else
 {
     printf("\nReading file <%s>", filename);

     /* cache file first */
     char in[200*80] = {'\0'};

     i = 0;
     while( (in[i++] = fgetc(f)) != EOF && i < 100*80 );

     fclose(f);

     printf("\nDone reading file (%d bytes).\n", i);
     system("PAUSE");

     x = y = sx = sy = 0;
     i = -1;

     while( in[++i] )
     {

        if (in[i] != ESC)
        {
            //printf("\nline: %d, x: %d -> %c", y,x,in[i]);
            out[y][x] = in[i];
            if (in[i] == '\n') y++;
            x++;
        }
        else
        if ( in[i] == ESC && in[i+1] == LEFTBRACKET )
        {
            /* interpret sequence */
            /* crs up        -> CSI n A */
            /* crs down      -> CSI n B */
            /* crs forw      -> CSI n C */
            /* crs back      -> CSI n D */
            /* crs next line -> CSI n E */
            /* crs prev line -> CSI n F */
            /* crs horz absol-> CSI n G */

            /* save    position -> CSI s */
            /* restore position -> CSI r */

            char cmds[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 's', 'r' };

            /* TODO: look for trailing command first */

            i += 2;
            char param   = in[i];
            char command = in[i+1];

            if(param == 's') { sx =  x;  sy =  y; }
            if(param == 'r') { x  = sx;  y  = sy; }

            switch(command)
            {
                case 'A': y -= param; break;
                case 'B': y += param; break;
                case 'C': x += param; break;
                case 'D': x -= param; break;
                case 'E': x  = 0; y += param; break;
                case 'F': x  = 0; y -= param; break;
                case 'G': x  = param; break;
                case ';': a = in[i+2];
                          b = in[i+3];
                          if (b =='f' || b == 'H') { x = a; y = param; }
                          break;
            }

        }
        else
        {
            /* ... */
        }

     }

    printf("\n Buffer contents: \n");

     for( y = 0; y <  80;  y++ )
     for( x = 0; x < 100;  x++ )
     {
         printf( "%c", out[y][x] );
     }

 }


#endif


/******************************************************************************************

  Tiny memory editor / viewer / file dumper utility.
  Enough features to have its own little help screen. :)

*******************************************************************************************/

#ifdef MEDT

 #define KEY_ADR   'i'
 #define KEY_ASCII 'a'
 #define KEY_HEX   'h'
 #define KEY_DUMP  'd'
 #define KEY_FILL  'f'
 #define KEY_REPL  'r'
 #define KEY_HELP  '?'
 #define KEY_QUIT  'q'

 #define CHAR_DUMMY '.'

 char *adr;
 char inp[40] = {0};
 char *mstart = (char*)inp;
 char *mend   = mstart + 0xFF;

 int cont = 1;

 printf("\nMEDT - 1.0a; %c = help", KEY_HELP);

 while (cont)
 {
    printf("\n\n[adr: %X-%X]>:", mstart, mend);

    scanf("%s40", inp);

    if (*inp == KEY_HELP)
    {
        printf("\n %c - This help screen", KEY_HELP);
        printf("\n %c - Input memory adress range", KEY_ADR);
        printf("\n %c - Display range as ASCII chars", KEY_ASCII);
        printf("\n %c - Display range as HEX values", KEY_HEX);
        printf("\n %c - Save range as a file", KEY_DUMP);
        printf("\n %c - Fill range with byte", KEY_FILL);
        printf("\n %c - Search and replace byte in range with byte", KEY_REPL);
        printf("\n %c - Quit\n", KEY_QUIT);
    }
    else if (*inp == KEY_ADR)
    {
        int adr1, adr2;

        printf("\nAdresses (HEX) [start, end]:");
        scanf("%X, %X", &adr1, &adr2);

        mstart = (char*)adr1;
        mend   = (char*)adr2;
    }
    else if (*inp == KEY_FILL)
    {
        int val;

        printf("\nFill with byte (HEX) [value]: ");
        scanf("%X", val);

        for( adr = mstart; adr < mend; adr++ ) *adr = val & 0xFF;
    }
    else if (*inp == KEY_REPL)
    {
        int val1, val2;
        int count = 0;

        printf("\nSearch and replace byte x with value y (HEX) [x, y]: ");
        scanf("%X,%X", &val1, &val2 );

        for( adr = mstart; adr < mend; adr++ )
        {
            if (*adr == (val1 & 0xFF)) { *adr = val2 & 0xFF; count++; }
        }

        printf("\n%d bytes found and replaced.\n", count);
    }
    else if (*inp == KEY_ASCII)
    {
        printf("\nMemory at (ASCII display mode): %X - %X\n\n", mstart, mend);

        /* printable 7Bit ASCII chars only, DUMMY_CHAR for the rest, 60 chars each line */
        for( adr = mstart; adr < mend; adr++ )
        {
            printf("%c%s", (*adr >= 0x20 && *adr <= 0x7E) ? *adr : CHAR_DUMMY, !((adr - mstart + 1) % 60) ? "\n" : "" );
        }
    }
    else if (*inp == KEY_HEX)
    {
        printf("\nMemory at (HEX display mode): %X - %X\n\n", mstart, mend);

        /* separated hex values, 20 per line. leading 0, if needed */
        for( adr = mstart; adr < mend; adr++ )
        {
            char val = *adr & 0x0F;
            printf("-%s%X%s", (val < 0x10) ? "0" : "", val, !((adr - mstart + 1) % 20) ? "\n" : "" );
        }
    }
    else if (*inp == KEY_DUMP)
    {
        char filename[80] = "";

        printf("\nPath/filename: ");
        scanf("%s80", filename);

        printf("\nWriting file to disk... ");

        FILE *f = fopen(filename,"wb");

        if (f != NULL)
        {
            for( adr = mstart; adr < mend; adr++ ) fputc(*adr & 0xFF, f);
            fclose(f);
            printf("done.\n");
        }
        else printf("Error!\n");
    }

    if ( !strcmp(inp,"exit") || *inp == KEY_QUIT ) cont = 0;
 }

#endif



/******************************************************************************************

    A version of Picross.

    A Logic / Picture Puzzle Game. Unfinished.
    (http://en.wikipedia.org/wiki/Picross)

*******************************************************************************************/

#ifdef PICROSS_GAME

 #define F_SIZE      10
 #define PIC_COUNT    2
 #define BASE_CHAR 0x22
 #define CODE_CHAR   97

 int i,i2, x,y, cx,cy,cn;

 char f[F_SIZE*F_SIZE] = {0};
 char n[F_SIZE]        = {0};

 char nh[F_SIZE][F_SIZE]; /* hor: line, row */
 char nv[F_SIZE][F_SIZE]; /* ver: line, row */

 memset( nh, 0, sizeof(nh) );
 memset( nv, 0, sizeof(nv) );


 char pics[PIC_COUNT][F_SIZE*F_SIZE];

 strcpy( pics[0], "-------------oooo-----o-o--o---o---o--o-o-----oooo-o---o--o---o-o--o-----oooo-----------------------" );
 strcpy( pics[1], "-------------ooo------o--o-----o---oooooo--------oo--------o-o---ooooo--o--o-------ooo--------------" );
 strcpy( pics[2], "" );
/* => 110, 101, 102, 98, 98, 98, 99, ..

 - 97 + 13
 o 97 +  4
 - 97 +  5
 o 97 +  1
 - 97 +  1
 o 97 +  1
 - 97 +  2
 ...
*/

/* lv1                      lv2                     lv3
  - - - - - - - - - -       - - - - - - - - - -     o o o o o o o o - o
  - - - o o o o - - -       - - - o o o - - - -     - - - o - - - o - o
  - - o - o - - o - -       - - o - - o - - - -     o - o o - o o o - -
  - o - - - o - - o -       - o - - - o o o o o     o - - - - - - o - o
  o - - - - - o o o o       o - - - - - - - - o     o o o - - - - o - o
  - o - - - o - - o -       o - - - - - - - - o     - o - - o o o o - o
  - - o - o - - o - -       - o - - - o o o o o     - o - - - - o - - -
  - - - o o o o - - -       - - o - - o - - - -     o o o o o - o o - o
  - - - - - - - - - -       - - - o o o - - - -     o - - - - - - - - o
  - - - - - - - - - -       - - - - - - - - - -     o o o o o o o o o o
*/

/*----------------------------------------------------------------------------------*/

 /* select random pic, or a randomly generated pic */
 if ( rand()%(PIC_COUNT+1) < PIC_COUNT )
 {
    int cell = 0;
    char set = 0;

     /* decode pic data, if needed */
     /*
     for(i  = 0; i < strlen(pics[lv]); i++ )
     {
        int inrow = pics[lv][i] - CODE_CHAR;
        for(i2 = 0; i2 < inrow; i2++ ) f[cell++] = set;
        set = !set;
     }
     */

     /* no decoding */
    for(i = 0; i < sizeof(f); i++)
        f[i] = (pic[lv][i] == 'o') ? 1 : 0;
 }
 else
 {
    srand( time(NULL) );

    for(i = 0; i < sizeof(f); i++)
        f[i] = rand() % 5 ? 0 : 1;
 }


 /*----------------------------------------------------------------------------------*/


 int inpx = 0, inpy = 0, cont = 1, turns = 0;

 /* main input loop; */
 while( cont )
 {

    /* scan field -> horizontal [TODO: vertical; outter: x, inner: y] */
    for(y = 0, cy = 0; y < F_SIZE; y++)
    {
        /* start with no row results */
        memset(n, 0, F_SIZE);

        for( x = 0, cx = 0, cn = 0; x < F_SIZE; x++ )
        {
            /* count stones in a row */
            if ( f[x + F_SIZE*y] ) cx++;
            /* gap or corner stone? -> add results to n[] */
            if (cx && (!f[x+F_SIZE*y] || x == F_SIZE-1) ) { n[cn++] = cx + '0'; cx = 0; }
        }
        /* nh = string together all horizontal results, line breaks after each line */
        strcat(n, strlen(n) ? "": "none");
        strcat(nh[y], n);
    }

    /* show field + right hints per line */
    for( i = 1; i <= sizeof(f); i++ )
    {
        printf( "%c", f[i-1] + BASE_CHAR );
        if ( !(i % F_SIZE) ) printf(" %s\n", nh[(i-1)/F_SIZE] );
    }

    /* bottom hints; test with HOR. hint numbers; TODO for vert. numbers */
    for( i = 0; i < F_SIZE; i++ )
    {
        int validln = 0;
        char row[F_SIZE+1] = {0};

        for(i2 = 0; i2 < F_SIZE; i2++)
            if ((row[i2] = nh[i2][i] ? nh[i2][i] : ' ') != ' ') validln = 1;

        if (validln) printf("%s\n", row);
    }

    /*----------------------------------------------------------------------------------*/

    printf("\nTurn: %d, Enter x,y (0..9, 0..9, diff. coords to quit):", ++turns);
    scanf("%i,%i", &inpx, &inpy);

    /* check for exit */
    cont = ( (inpx*inpy) >= 0) && ((inpx*inpy) < sizeof(f) );

    if (cont)
    {
      printf("%d, %d -> %s", inpx, inpy, f[F_SIZE * inpy + inpx] ? "Hit!" : "Miss!" );
      f[F_SIZE*inpy + inpx] = 0;

      /* no stones left to uncover? level complete */
      char fclear = 1;
      for( i = 0; i < F_SIZE && (fclear = !f[i]); i++ )

      if(fclear)
      {
          printf("\Pic uncovered (in %d turns)! Congratulation", turns);
          cont = 0;
      }

    }

 }

 system("PAUSE");

#endif



/******************************************************************************************

    Estimation Game
    Guess what number is the most common in a random number field.

*******************************************************************************************/

#ifdef EST_GAME

#define F_SIZE 14
#define DIFFICULTY 5

 int  i, a, nr, higher = 0, ce[DIFFICULTY] = {0};
 char f[ F_SIZE * F_SIZE ] = {0};
 time_t t = time(NULL);

 srand(t); puts("");

 for(i = 0; i < F_SIZE * F_SIZE; i++)
 {
     printf(" %d%s", nr = rand() % DIFFICULTY, ! ((i+1)%F_SIZE) ? "\n" : "" );
     ce[nr]++;
 }

 printf("\n Most common? :");
 scanf("%10d", &a);

 for(i = 0; i < DIFFICULTY; i++)
   if (a != i && ce[i] > ce[a]) higher++;

 if ( !higher ) printf(" Correct! (in: %d seconds)", time(NULL)-t); else
                printf(" Wrong! (%d other number(s) are more common)", higher);

 printf("\n Counts: ");
 for(i = 0; i < DIFFICULTY; i++)
     printf("%d = %d%s ", i, ce[i], (i != DIFFICULTY-1) ? "," : "." );

 puts("\n");
 system("PAUSE");

#endif



/******************************************************************************************

    (Text-)file appending / copy / filter tool

    - can add EOF text markers
    - filter out extended ASCII
    - convert Umlauts and Esszett
    - apply 1990s style l33t sp34k ;)

*******************************************************************************************/

#ifdef FMERGE

#define MAX_FILE_COUNT 99

/* TODO: currently only works with markers + one add. flag */

  char HELP_TEXT[] =
    "\nUsage: fmerge [parameters]\n"
    "\nFirst parameter not recognized as an option  = output filename."
    "\nAll other unrecognized parameters following  = input filename(s)."
    "\n\nOptions:"
    "\n --m  : Insert EOF markers."
    "\n --d  : Convert Umlaut and Esszett characters."
    "\n --7  : 7 Bit ASCII only. Extented ASCII chars will be replaced with a dash."
    "\n --l  : L33t Sp43k filter.\n"
    "\nNote that --d, --7 and --l cannot be mixed.";

 char infilenames[ MAX_FILE_COUNT ][250];
 char outfilename[250];

 FILE *infile, *outfile;

 char norm_chars[] = "abcdefghijklmnopqrstuvwxyz" "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 char leet_chars[] = "4bcd3fgh1jklmn0pqr5+uvwxyz" "48CD3FGH1JK7MNOPQR$+UVWXYZ";

 char de_chars[] = "äöüÄÖÜß";

 char flag_markers = 0,
      flag_deconv  = 0,
      flag_7bit    = 0,
      flag_leet    = 0;

 char fcount = 0, c;
 int i, pos;

 /* cmd line checking ..
    - first unrecognized option/parameter      = output filename
    - all other unrecognized parameters after  = input filenames
 */
 if (argc > 2)
 {
     for( i = 1; i < argc; i++ )
     {
         if      (!strcmp(argv[i], "--m")) flag_markers = 1;
         else if (!strcmp(argv[i], "--d")) flag_deconv  = 1;
         else if (!strcmp(argv[i], "--7")) flag_7bit    = 1;
         else if (!strcmp(argv[i], "--l")) flag_leet    = 1;
         else
         {
            if (fcount > 0)
                strncpy( infilenames[fcount-1], argv[i], 250 );
            else
                strncpy( outfilename, argv[i], 250 );

            fcount++;
         }
     }
 }

 if (fcount < 2)
 {
     puts("Error: At least one input and one output file required.");
     puts(HELP_TEXT);
     return -1;
 }

 if ( (flag_deconv + flag_7bit + flag_leet) > 1)
 {
    puts("Error: Too many parameters."
         "Only one of the following parameters permitted per run: --f, --7, --l");

    puts(HELP_TEXT);
    return -1;
 }

 outfile = fopen( outfilename, "wb" );

 printf("\Appending %d file(s) and save as <%s>\n", fcount-1, outfilename);
 printf( "\nFlags Overview:"
         "\n - 7bit only    : %s"
         "\n - DE-Char Conv : %s"
         "\n - L33T sp34k   : %s"
         "\n - EOF-Markers  : %s\n",
         flag_7bit    ? "yes" : "no",
         flag_deconv  ? "yes" : "no",
         flag_leet    ? "yes" : "no",
         flag_markers ? "yes" : "no" );


 for( i = 0; i < fcount-1; i++ )
 {
    infile = fopen(infilenames[i], "rb");

    if (infile)
    {
        if (flag_leet)
        {
            while ( (c = fgetc(infile)) != EOF)
            {
              if ( (pos = (strchr(norm_chars, c) - norm_chars)) >= 0 )
                fputc( leet_chars[pos], outfile);
              else
                fputc(c, outfile);
            }
        }
        else if (flag_7bit)
        {
            while ( (c = fgetc(infile)) != EOF)
               if (c >= 0) fputc(c, outfile); else fputc('-', outfile);
        }
        else if (flag_deconv)
        {
            char oldc;

            while ( (c   = fgetc(infile)) != EOF )
               if ( (pos = (strchr(de_chars, c) - de_chars)) >= 0  )
               {
                    if (pos == 0) fputs("ae", outfile);
                    if (pos == 1) fputs("oe", outfile);
                    if (pos == 2) fputs("ue", outfile);
                    if (pos == 3) fputs("AE", outfile);
                    if (pos == 4) fputs("OE", outfile);
                    if (pos == 5) fputs("UE", outfile);

                    /* since there's no capital Esszett, we need some context for conversion */
                    if (pos == 6) { fputs( (oldc >= 0x61) ? "ss" : "SS", outfile); }
               }
               else fputc(oldc = c, outfile);
        }

        if (flag_markers)
        {
            char marker[200] = "\r\n---- end of file < ";

            strncat( marker, infilenames[i], 160 );
            strcat(  marker, " > ----\r\n" );

            fputs( marker, outfile );
        }

        printf( "\n<%s> Ok.", infilenames[i] );
    }
    else
        printf( "\n<%s> Error opening file", infilenames[i] );

    fclose( infile );

 }

 fclose( outfile );
 puts("");


#endif


/******************************************************************************************

    Fisher-Yates / Durstenfeld-Shuffle with presets
    (useful for searching for anagrams or just trying to come up with project names ;)

*******************************************************************************************/

#ifdef PERM
#define LOOPS 20

 int i, j, r;
 char inp[40], tmp;

 srand(time(NULL));

 printf("String to shuffle, [shortcuts: a-z, 0-9, dice]: ");
 scanf("%s40", &inp);

 if ( !strcmp(inp, "a-z" ) ) strcpy(inp, "abcdefghijklmnopqrstuvwxyz");
 if ( !strcmp(inp, "0-9" ) ) strcpy(inp, "0123456789");
 if ( !strcmp(inp, "dice") ) strcpy(inp, "123456");

 for( j = LOOPS; j-- > 0; )
 {
     for( i = strlen(inp); i-- > 1; )
     {
        r = rand() % i;

        tmp    = inp[i];
        inp[i] = inp[r];
        inp[r] = tmp;
     }

     printf("<%s>\n", inp);
 }

 system("PAUSE");

#endif

/******************************************************************************************

 SBFED = Super Bad File Encoding & Decoding

 (shortest and possibly worst mini file encrypter/decrypter possible. Don't use this
  for your billion dollar trade secrets. ;)

*******************************************************************************************/
#ifdef SBFED

 if (argc != 3) { puts("Error: 2 params for in/out files needed."); return -1; };

 FILE *sf = fopen( argv[1], "rb");
 FILE *df = fopen( argv[2], "wb");

 char c;
 while( (c = fgetc(sf)) != EOF ) fputc( (((c^-1) & 0xF0) >> 4) | (((c^-1) & 0x0F) << 4), df );

 fclose(sf);
 fclose(df);

#endif

/******************************************************************************************

    Prime Number Evaluation. Short, simple & fast enough. AKS this is not, though.

*******************************************************************************************/
#ifdef PRIME

 /* Beware: C99 stuff. ANSI/C89-only compilant compilers might argue this. */
 long long int i, p, isprime = 1;

 printf("\nNumber:");
 scanf("%lld", &p);

 if (p == 2) isprime = 1;
 else
 if (p < 2 || !(p % 2)) isprime = 0;
 else
 for(i = 3; ( i <= sqrt(p)) && (isprime = p % i); i += 2);

 printf("\nPrime: %s\n", isprime ? "Yes" : "No");

#endif


/******************************************************************************************

    (ANTI-)JOKE GENERATOR. NOTHING IS SACRED!
    "Why did the charming bulldozer run over the research facility?"
    "Because the bulldozer could not love a(n) upset aroma therapist!"

*******************************************************************************************/
#ifdef AJOKE

 srand(time(NULL));

 char *a[] = { "chicken", "astronaut", "bulldozer", "aroma therapist", "pole dancer" };
 char *b[] = { "cross", "run over", "walk over", "talk to", "love" };
 char *c[] = { "street", "bridge", "chicken farm", "rumba instructor meeting hall", "research facility" };
 char *d[] = { "bossy", "charming", "single", "blushing", "upset" };

 printf("\nWhy did the %s %s %s the %s?", d[rand()%5], a[rand()%5], b[rand()%5], c[rand()%5] );
 printf("\nBecause the %s could not %s a(n) %s %s!\n", a[rand()%5], b[rand()%5], d[rand()%5], a[rand()%5] );

#endif

 return 0;

}
