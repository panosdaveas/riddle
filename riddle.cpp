#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#define width 20
#define height 20
#define MAX_LEN 100
#define RED 4
#define BLACK 0

void SetColorAndBackground(int ForgC, int BackC) {
  WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
  return;
}

void Gotoxy(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct index {
  char *q;
  char a[MAX_LEN];
  char h[MAX_LEN];
  int size;
  int max;
  int count;
};

typedef struct index index;
index *library;

void remove_newline(char str[]) {
  int i = 0;
  while (str[i] != '\0') {
    if (str[i] == '\n') {
      str[i] = '\0';
    }
    i++;
  }
}

void open_file() {
  FILE *fp;
  char file[20] = "library.txt";
  fp = fopen(file, "r");
  if (fp == NULL) {
    exit(1);
  }
  int i = 0;
  int size = 1;
  int sizet = 1;
  char buffer[MAX_LEN];
  while (!feof(fp)) {
    index stream;
    int j = 0;
    library = (index *)realloc(library, sizeof(index) * size);
    char **temp = (char **)malloc(sizeof(char *));
    fgets(buffer, MAX_LEN, fp);
    if (buffer[0] == '\n') {
      fgets(buffer, MAX_LEN, fp);
      while (buffer[0] != '\n') {
        temp = (char **)realloc(temp, sizeof(char *) * sizet);
        temp[j] = (char *)malloc(MAX_LEN * sizeof(char));
        strcpy(temp[j], buffer);
        fgets(buffer, MAX_LEN, fp);
        sizet++;
        j++;
      }
      int k = 0;
      int s = 0;
      int start = 0;
      int end = 0;
      int max = 0;
      stream.q = (char *)malloc(sizeof(char) * j * MAX_LEN);
      for (k = 0; k < j; k++) {
        start += end;
        end += strlen(temp[k]);
        if (strlen(temp[k]) > max) {
          max = strlen(temp[k]);
        }
        int c = 0;
        for (s = start; s <= end; s++) {
          stream.q[s] = temp[k][c++];
        }
      }
      library[i].max = max;
      library[i].count = j;
    }
    library[i].q = (char *)malloc(sizeof(char) * j * MAX_LEN);
    strcpy(library[i].q, stream.q);
    fgets(library[i].a, MAX_LEN, fp);
    fgets(library[i].h, MAX_LEN, fp);
    remove_newline(library[i].a);
    remove_newline(library[i].h);
    library->size = size;
    i++;
    size++;
    free(stream.q);
    free(temp);
  }
  fclose(fp);
}

int is_border(int x, int y) {
  return (x == 0 || x == width - 1 || y == 0 || y == height - 1);
}

int is_answer(int x, int y, int asize) {
  if (x == (width / 2 - asize / 2 - asize % 2) && y == 12) {
    return 1;
  }
  return 0;
}

void print_quest(int r) {
  int i = 0;
  int j = 0;
  int count = 0;
  char split[library[r].count][MAX_LEN];
  int size[library[r].count];
  for (i = 0; i <= (strlen(library[r].q)); i++) {
    if (library[r].q[i] == '\n' || library[r].q[i] == '\0') {
      split[count][j] = '\0';
      count++;
      j = 0;
    } else {
      split[count][j] = library[r].q[i];
      j++;
    }
  }
  int y = height / 3;
  for (int s = 0; s < count; s++) {
    size[s] = strlen(split[s]);
    Gotoxy(width / 2 - size[s] / 2 - size[s] % 2, y);
    printf("%s", split[s]);
    y++;
  }
}

void print(int qsize, int asize, char *quest, int level, int curmax) {
  char buffer[width + 1];
  int y = 0;
  for (y = 0; y < height; y++) {
    int x = 0;
    for (x = 0; x < width; x++) {
      if (is_border(x, y)) {
        if (y == 0 && x == 0) {
          buffer[x] = 201;
        } else if (y == height - 1 && x == 0) {
          buffer[x] = 200;
        } else if (y == height - 1 && x == width - 1) {
          buffer[x] = 188;
        } else if (y == 0 && x == width - 1) {
          buffer[x] = 187;
        } else if (y == height - 1 || y == 0) {
          buffer[x] = 205;
        } else {
          buffer[x] = 186;
        }
      } else if (is_answer(x, y, asize)) {
        int i = 0;
        for (i = 0; i < asize; i++) {
          buffer[x] = '*';
          x++;
        }
      } else {
        buffer[x] = ' ';
      }
      if (x == 1 && y == height - 1) {
        buffer[x] = 'l';
        x++;
        buffer[x] = 'v';
        x++;
        buffer[x] = '.';
        x++;
        level++;
        if (level < 10) {
          buffer[x] = '0' + level;
        } else if (level >= 10) {
          buffer[x] = '0' + level / 10;
          x++;
          buffer[x] = '0' + level % 10;
        }
      }
    }
    buffer[width] = '\0';
    printf("%s\n", buffer);
  }
  print_quest(level - 1);
  Gotoxy(0, height);
}

struct current {
  struct index index;
  int len_q;
  int len_a;
  int max;
};
typedef struct current current;

current currentlevel(current cur, int level) {
  cur.index = library[level];
  cur.len_a = strlen(library[level].a);
  cur.len_q = strlen(library[level].q);
  cur.max = library[level].max;
  return cur;
}

void print_hint(char *hint, int i) {
  Gotoxy(width / 2 + 5, height + 3);
  hint = library[i].h;
  printf(":%s", hint);
}

char caps_lock_trick(char c) {
  if (c >= 'A' && c <= 'Z') {
    c += 'a' - 'A';
  }
  return c;
}

void win_message() {
  char c[MAX_LEN] = "You Win!";
  system("cls");
  Gotoxy(width / 2 - strlen(c) / 2, height / 2);
  printf("%s", c);
  Gotoxy(0, height);
  system("pause");
}

void play() {
  int level = 0;
  int result;
  do {
    current cur = currentlevel(cur, level);
    do {
      print(cur.len_q, cur.len_a, cur.index.q, level, cur.max);
      printf("\n    Enter to solve");
      printf("\n     ESC to exit");
      SetColorAndBackground(4, 0);
      printf("\n     ! for HINT");
      SetColorAndBackground(7, 0);
      result = 1;
      int i = 0;
      char input[cur.len_a];
      int x = width / 2 - cur.len_a / 2 - cur.len_a % 2;
      int y = 12;
      Gotoxy(x, y);
      int pos = x;
      char c;
      while (i < cur.len_a + 1) {
        c = getch();
        c = caps_lock_trick(c);
        if (c == '\r' || pos == x - 1) {
          break;
        } else if (c == '\b' && pos == x) {
          break;
        } else if (c != '\b' && c != '\r' && c != '\n') {
          input[i] = c;
          printf("%c", input[i]);
          pos++;
          i++;
        } else if (c == '\b') {
          i--;
          input[i] = '*';
          pos--;
          Gotoxy(pos, y);
          printf("%c", input[i]);
          Gotoxy(pos, y);
          input[i] = '\0';
        }
        if (c == 27) {
          system("cls");
          return;
        }
        if (c == 33) {
          print_hint(library[i].a, level);
          break;
        }
      }
      input[i] = '\0';
      result = strcmp(input, cur.index.a);
      if (result == 0) {
        level++;
        break;
      }
      Gotoxy(0, 0);
    } while (result != 0);
    system("cls");
    Gotoxy(0, 0);
  } while (level < library->size - 1);
  if (level == library->size - 1) {
    win_message();
  }
}

void welcome(){
	char wel[11]="Welcome...";
	int size=strlen(wel);
	Gotoxy(width/2-size/2,height/3);
	printf("%s",wel);
	int i=size-1;
	int x=width/2+size/2-1;
	int y=height/3;
	Sleep(900);
	for (i=size; i>0; i--){
		Sleep(300);
		Gotoxy(x,y);
		printf(" ");
		x--;
	}
}

int main() {
  system("cls");
  welcome();
  system("cls");
  open_file();
  play();
  free(library);
  return 0;
}
