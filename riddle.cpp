#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#define width 20
#define height 20
#define  MAX_LEN  100
#define RED	4
#define BLACK	0

void SetColorAndBackground(int ForgC, int BackC){
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

struct index{
	char q[MAX_LEN];
	char a[MAX_LEN];
    char h[MAX_LEN];
	int size;
};

typedef struct index index;
struct index *library;

void remove_newline(char str[]){
	int i=0;
	while(str[i]!='\0'){
		if(str[i]=='\n'){
			str[i]='\0';
		}
	   i++;
	 }
}

void open_file(){
	FILE *fp;
	char file[20]="library.txt";
	fp=fopen(file,"r");
    if (fp==NULL) {
      exit(1);
    }
    int i=0;
    int size=1;
    while(!feof(fp) ){
    	index stream;
    	library=(index*) realloc(library,sizeof(index) * size);
    	fgets(stream.q,MAX_LEN,fp);
    	fgets(stream.a,MAX_LEN,fp);
        fgets(stream.h,MAX_LEN,fp);
    	remove_newline(stream.q);
    	remove_newline(stream.a);
    	remove_newline(stream.h);
    	library[i]=stream;
    	library->size=size;
    	i++;
    	size++;
    }
    fclose(fp);
}

void Gotoxy(int x, int y) {
  COORD coord; coord.X = x; coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);}

int is_border(int x,int y){
	return (x==0 || x==width-1 || y==0 || y==height-1);
}

int is_quest(int x, int y,int qsize){
	if (x==(width/2 -qsize/2) && y==height/3){
		return 1;
	}
 	return 0;
}

int is_answer(int x, int y,int asize){
	if (x==(width/2 -asize/2) && y==12){
		return 1;
	}
	return 0;
}

void print(int qsize,int asize, char *quest,int level){
	char buffer[width+1];
	int y =0;
	for (y=0; y<height; y++){
		int x=0;
		for(x=0; x<width; x++){
			if(is_border(x,y)){
				if(y==0 && x==0) {
					    	buffer[x]=201;
					    }
                    	else if(y==height-1 && x==0){
                    		buffer[x]=200;
                    	}
                    	else if(y==height-1 && x==width-1){
                    		buffer[x]=188;
                    	}
                    	else if(y==0 && x==width-1){
                    		buffer[x]=187;
                    	}
                    	else if(y==height-1 || y==0){
                    		buffer[x]=205;
                    	}
                         else {
                         	buffer[x]=186;
                   		}
			}
        //works if quest is 1 printed in one row!    
		//	else if(is_quest(x,y,qsize)){
		//		int i=0;
		//		for(i=0; i<qsize; i++){
		//			buffer[x]=quest[i];
		//			x++;
		//		}
		//	}
			else if (is_answer(x,y,asize)){
				int i=0;
				for(i=0; i<asize; i++){
					buffer[x]='*';
					x++;
				}
			}
		    else{
				 buffer[x]=' ';
	    	}
	        if (x==1 && y==height-1){
				buffer[x]='l';
				x++;
				buffer[x]='v';
				x++;
				buffer[x]='.';
				x++;
                level++;
                if (level<10) {
                   buffer[x]='0'+level; 
                }
                else if (level>=10){
                    buffer[x]='0'+level/10;
                    x++;
                    buffer[x]='0'+level%10;
                }
            }
        }
        buffer[width]='\0';
        printf("%s\n",buffer);
	}
    //works for up to 2 rows ofalse,f quest input 
    //in library.txt separate the lines with key character %
    //ig hello%world will print:
    //hello
    //world
    //
    //code starts here:
    char up[MAX_LEN];
    char down[MAX_LEN];
    int i=0;
    int c='%';
    char *loc;
    int pos=0;
    int flag=0;
    for(i=0; i<qsize; i++){
        if(quest[i]=='%'){
            pos=i;
            flag=1;
        }
    }
    if(flag==0){
       for(i=0; i<qsize; i++){
        up[i]=quest[i];
        down[i]='\0';
       } 
       up[i]='\0';
       int ul=strlen(up);
       Gotoxy(width/2-ul/2,height/3);
       printf("%s",up);
    }
    else{
        for(i=0; i<pos; i++){
            up[i]=quest[i];
        }
        up[i]='\0';
        int ul=strlen(up);
        Gotoxy(width/2-ul/2,height/3);
        printf("%s",up);
        int j=0;
        for(i=pos+1; i<qsize; i++){
            down[j]=quest[i];
            j++;
        }
        down[j]='\0';
        int dl=strlen(down);
        Gotoxy(width/2-dl/2,height/3+2);
        printf("%s",down);
    } //:ends here
    Gotoxy(0,height);
}

struct current{
	struct index index;
	int len_q;
	int len_a;
};
typedef struct current current;

current currentlevel(current cur,int level){
	 cur.index=library[level];
	 cur.len_q=strlen(library[level].q);
	 cur.len_a=strlen(library[level].a);
	 return cur;
}

void print_hint(char *hint,int i){
	Gotoxy(width/2+5,height+3);
	hint = library[i].h;
	printf(":%s",hint);
}

char caps_lock_trick(char c){
    if(c>='A' && c<='Z'){
        c+='a'-'A';
    }
    return c;
}

void win_message(){
    char c[MAX_LEN]="You Win!";
    system("cls");
    Gotoxy(width/2 - strlen(c)/2, height/2);
    printf("%s",c);
    Gotoxy(0, height);
    system("pause");
}

void play(){
	int level=0;
	int result;
	do{
	    current cur=currentlevel(cur,level);
		do{
			print(cur.len_q,cur.len_a,cur.index.q,level);
			printf("\n    Enter to solve");
			printf("\n     ESC to exit");
			SetColorAndBackground(4,0);
			printf("\n     ! for HINT");
			SetColorAndBackground(7,0);
			result=1;
			int i=0;
			char input [cur.len_a];
			int x=width/2 - cur.len_a/2;
			int y=12;
			Gotoxy(x,y);
			int pos=x;
			char c;
			while(i<cur.len_a+1){
				c=getch();
                c=caps_lock_trick(c);
				if(c=='\r' || pos==x-1 ){
					break;
				}
				else if(c=='\b' && pos==x){
					break;
				}
				else if(c!='\b' && c!='\r' && c!='\n'){
					input[i]=c;
					printf("%c",input[i]);
					pos++;
						i++;
				}
				else if (c == '\b'){
					i--;
					input[i]='*';
					pos--;
					Gotoxy(pos,y);
					printf("%c",input[i]);
	                Gotoxy(pos,y);
					input[i]='\0';
				}
				if(c==27){
					system("cls");
					return;
				}
				if(c==33){
					print_hint(library[i].a,level);
					break;
				}
			}
			input[i]='\0';
			result =strcmp(input,cur.index.a);
			if(result==0){
				level++;
				break;
			}
            Gotoxy(0,0);
        }while(result!=0);
        system("cls");
        Gotoxy(0,0);
    }while(level<library->size-1);
    if (level==library->size-1) {
        win_message();
    }
}

int main(){
    system("cls");
	open_file();
	play();
	free(library);
	return 0;
}
