#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#define width 20
#define height 20
#define initial_speed 400
#define arr_size 3

struct index{
	char q[20];
	char a[20];
}library [arr_size]={
					 {"* oooOoooo","mars"},     //put {question,answer}, increase arr_size by 1!!
					 {"1+1","2"},
					 {"2b || !2b","hamlet"},
		 			 };

typedef struct index index;

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
				buffer[x]='x';
			}
			else if(is_quest(x,y,qsize)){
				int i=0;
				for(i=0; i<qsize; i++){
					buffer[x]=quest[i];
					x++;
				}
			}
			else if(is_answer(x,y,asize)){
				int i=0;
				for(i=0; i<asize; i++){
					buffer[x]='*';
					x++;
				}
			}
		    else{
				 buffer[x]=' ';
	    	}
	        if (x==0 && y==height-1){
				buffer[x]='l';
				x++;
				buffer[x]='v';
				x++;
				buffer[x]='.';
				x++;
		     	int l=level+1;
				while (l != 0) {
				   l= l % 10;
       		  	   buffer[x]= '0'+l;
       		  	   l=l/10;
				   x++;
   		        }
   			  //x--;
			}
		}
		buffer[width]='\0';
        printf("%s\n",buffer);
	}
}

void print_level(int level){
	printf("lv.%d",level);
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

void play(){
	int level=0;
	int result;
	do{
	   current cur=currentlevel(cur,level);
		do{
			print(cur.len_q,cur.len_a,cur.index.q,level);
			printf("\n    ESC to exit");
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
			}
			input[i]='\0';
			result =strcmp(input,cur.index.a);
			if(result==0){
				level++;
				break;
			}
            Gotoxy(0,0);
        }while(result!=0);
        Gotoxy(0,0);
    }while(level<arr_size);
}

/*void play(){
	char ans[arr_size][20] = {"mars","2","hamlet"};
	char quest[arr_size][20]={"* oooOoooo","1+1","2b || !2b"};
	int q_size[arr_size];
	int a_size[arr_size];
	int i=0;
	for(i=0; i<arr_size; i++){
		a_size[i]=strlen(ans[i]);
		q_size[i]=strlen(quest[i]);
	}
	int level=0;
	int result;
	do{
		char cur_quest[q_size[level]];
		strcpy(cur_quest,quest[level]);
		char cur[a_size[level]];
		strcpy(cur,ans[level]);
		do{
			print(q_size[level],a_size[level],cur_quest,level);
			printf("\n    ESC to exit");
			result=1;
			int i=0;
			char input [a_size[level]];
			int x=width/2 - a_size[level]/2;
			int y=12;
			Gotoxy(x,y);
			int pos=x;
			char c;
			while(i<a_size[level]+1){
				c=getch();
		
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
				
				
			}
			input[i]='\0';
			result =strcmp(input,cur);
			if(result==0){
				level++;
				break;
			}
            Gotoxy(0,0);
        }while(result!=0);
        Gotoxy(0,0);
    }while(level<arr_size);
}*/

int main(){
	play();
	return 0;
}
