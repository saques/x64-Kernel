#include <c_syscall.h>
#include <clib.h>
#include <printf.h>

#define P_TIMER 5
int64_t fread(uint64_t fd, uint8_t * buf, int64_t len);

/* prints character through standard output */
void putchar(uint8_t c){
  write(STDOUT, &c, 1);
}
/* prints c characters through standard output */
void putchars(uint8_t * str, uint64_t c) {
  write(STDOUT, str, c);
}

/* prints escaped character, for debug purposes*/
void raw_putchar(uint8_t c) {
  switch(c) {
    case '\b':
      putchar('\\');
      putchar('b');
      break;
    case '\n':
      putchar('\\');
      putchar('n');
      break;
    case ' ':
      putchar('\\');
      putchar('s');
      break;
    default:
      putchar(c);
      break;
  }
}

void raw_putchars(uint8_t * str, uint64_t c) {
  for (int i = 0; i < c; i++) {
    raw_putchar(str[i]);
  }
}
static uint8_t buf[BUFSIZ];
static int64_t n = 0;
static uint8_t *bufp = buf;

/* getchar implementation as found in K&R */
int64_t getchar(void) {

  if (n == 0) {
    n = fread(STDIN, buf, sizeof (buf) - 1);
    bufp = buf;
  }
  return (--n >= 0) ? (uint8_t) *bufp++ : -1;
}

/* shifts input buffer and inserts char at the beginning*/
int ungetc(uint8_t c) {
  for (int64_t j = n-1; j > 0; j--){
    bufp[j] = bufp[j-1];
  }
  bufp[0] = c;
  n++;
  return 0;
}

/* reads from input until \n. also prints cursor */
int64_t fread(uint64_t fd, uint8_t * buf, int64_t len) {
  int64_t ret = 0;
  int64_t count = 0;
  int64_t j = 0;
  uint8_t bufaux [BUFSIZ];
  uint8_t * bufp = bufaux;
  uint8_t ent_bool = 0;
  uint64_t pulse = 0;
  uint8_t pchar = ' ';

  len = len - 1;
  // ncPrintDec(len);
  while (!ent_bool) {
    if (0 < (ret = read(fd, bufp, len - j))) {
      for (int i = 0; i < ret; i++) {
        if (bufp[i] == '\n') {
          buf[j] = '\n';
          putchar('\n');
          ent_bool = 1;
          j++;
        }
        else if (bufp[i] == '\b') {
          if (j > 0) {
            buf[j] = ' ';
            putchar(' ');
            putchar('\b');
            putchar('\b');
            putchar(' ');
            putchar('\b');
            j--;
          }
        }
        else if (bufp[i] == '\t') {
          for (int a = 0; a < 4 && j < len - 1; a++, j++) {
            buf[j] = ' ';
            putchar(' ');
          }
        }
        else {
          if (j < len - 1) {
            buf[j] = bufp[i];
            putchar(buf[j]);
            j++;
          }
        }
      }
      bufp = bufaux;
    }
    else {
      sleep(0);
      if (pulse > P_TIMER) {
        pchar = (pchar == '|') ? ' ': '|';
        putchar(pchar);
        putchar('\b');
        pulse = 0;
      }
      else {
        pulse++;
      }
    }
  }
  buf[j] = '\0';
  return j;
}

/* prints current time */
void print_time(uint8_t * buf) {
  int64_t i;
  TIME * t = time();
  printf("%d/%d/%d %d:%d:%d\n", t->day, t->month, t->year, t->hour, t->min, t->sec);
  free(t);
}

/* returns "random" number. */
int64_t rand() {
  return 42;
}

/* srand "implementation". */
void srand (uint64_t seed) {
  return;
}

/* clears standard output*/
void clear_screen() {
  clear();
}

void wkexit(){
	kill(getPpid(),1);
	exit();
}
