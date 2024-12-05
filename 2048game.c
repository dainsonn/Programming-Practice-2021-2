#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#define SIZE 4

void ready(unsigned char board[SIZE][SIZE]);
void addnum(unsigned char board[SIZE][SIZE]);
void draw(unsigned char board[SIZE][SIZE]);
void setBufferedInput(bool enable);
void moveleft(unsigned char board[SIZE][SIZE]);
void moveright(unsigned char board[SIZE][SIZE]);
void moveup(unsigned char board[SIZE][SIZE]);
void movedown(unsigned char board[SIZE][SIZE]);
void gameover(unsigned char board[SIZE][SIZE]);

unsigned int score; 
unsigned int move; 

int main(){
  unsigned char board[SIZE][SIZE];
  char key;

  printf("\033[?25l\033[2J");

  ready(board);
  setBufferedInput(false);

  while(1){
    key = getchar();
    move = 0;
    switch(key){
      case 97:
      case 68:
        moveleft(board); break;
      case 100:
      case 67:
        moveright(board); break;
      case 119:
      case 65:
        moveup(board); break;
      case 115:
      case 66:
        movedown(board); break;
    }

    for(int x=0; x<SIZE; x++){
      for(int y=0; y<SIZE; y++){
        if(board[x][y]%2 == 1){
          board[x][y] -=1;
        }
      }
    }

    if(move > 0){
      draw(board);
      usleep(150000);
      addnum(board);
      draw(board);
      gameover(board);
    }
  }
  setBufferedInput(true);
  printf("\033[?25h\033[m");
	return EXIT_SUCCESS;
}

void addnum(unsigned char board[SIZE][SIZE]){
  unsigned char x, y;
  unsigned char list[SIZE*SIZE][2], cnt = 0;

  for(x=0; x<SIZE; x++){
    for(y=0; y<SIZE; y++){
      if(board[x][y] == 0){
        list[cnt][0] = x;
        list[cnt][1] = y;
        cnt++;
      }
    }
  }

  srand(time(NULL));
  if(cnt > 0){
    unsigned char r = rand()%cnt;
    x = list[r][0];
    y = list[r][1];
    unsigned char n = (rand()%10)<8?2:4;
    board[x][y] = n;
  }
}

void draw(unsigned char board[SIZE][SIZE]){
  printf("\033[H");
  printf("2048.c %16d pts\n\n",score);

  for(int x=0; x<SIZE; x++){
    for(int y=0; y<SIZE; y++){
      if(board[x][y] == 0){
        printf("    .");
      }
      else{
        printf("%5d", board[x][y]);
      }
    }
    printf("\n");
  }

  printf("\n");
	printf(" ←, ↑, →, ↓ or A, W, D, S \n");
}

void ready(unsigned char board[SIZE][SIZE]){
  for(int x=0; x<SIZE; x++){
    for(int y=0; y<SIZE; y++){
      board[x][y] = 0;
    }
  }
  addnum(board);
  addnum(board);
  draw(board);
  score = 0;
}

void setBufferedInput(bool enable){
	static bool enabled = true;
	static struct termios old;
	struct termios new;

	if (enable && !enabled){
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		enabled = true;
	}
  else if (!enable && enabled){
		tcgetattr(STDIN_FILENO,&new);
		old = new;
		new.c_lflag &=(~ICANON & ~ECHO);
		tcsetattr(STDIN_FILENO,TCSANOW,&new);
		enabled = false;
	}
}

void moveleft(unsigned char board[SIZE][SIZE]){
  for(int x=0; x<SIZE; x++){
    for(int y=1; y<SIZE; y++){
      for(int z=y; z>0; z--){
        if(board[x][z] == 0 || board[x][z]%2 == 1){
          break;
        }
        if(board[x][z-1] != 0 && board[x][z-1] != board[x][z]){
          break;
        }
        if(board[x][z-1] == 0){
          board[x][z-1] = board[x][z];
        }
        else if(board[x][z-1] == board[x][z]){
          board[x][z-1] *= 2;
          board[x][z-1] += 1;
          score += 2*(board[x][z]);
        }
        board[x][z] = 0;
        move++;
      }
    }
  }
}

void moveright(unsigned char board[SIZE][SIZE]){
  for(int x=0; x<SIZE; x++){
    for(int y=2; y>=0; y--){
      for(int z=y; z<3; z++){
        if(board[x][z]==0 || board[x][z]%2 == 1){
          break;
        }
        if(board[x][z+1] != 0 && board[x][z+1] != board[x][z]){
          break;
        }                    
        if(board[x][z+1]==0){
          board[x][z+1] = board[x][z];
        }
        else if(board[x][z] == board[x][z+1]){
          board[x][z+1] *= 2;
          board[x][z+1] += 1;
          score += 2*(board[x][z]);
        }
        board[x][z]=0;                            
        move++;
      }
    }
  }
}

void moveup(unsigned char board[SIZE][SIZE]){
  for(int y=0; y<SIZE; y++){
    for(int x=1; x<SIZE; x++){
      for(int z=x; z>0; z--){
        if(board[z][y] == 0 || board[z][y]%2 == 1){
          break;
        }
        if(board[z-1][y] != 0 && board[z-1][y] != board[z][y]){
          break;
        }
        if(board[z-1][y] == 0) {
          board[z-1][y] = board[z][y];
        }
        else if(board[z][y] == board[z-1][y]){
          board[z-1][y] *= 2;
          board[z-1][y] += 1;
          score += 2*(board[z][y]);
        }
        board[z][y] = 0;
        move++;
      }
    }
  }
}

void movedown(unsigned char board[SIZE][SIZE]){
  for(int y=0; y<4; y++){
    for(int x=2; x>=0; x--){
      for(int z=x; z<3; z++){
        if(board[z][y] == 0 || board[z][y]%2 == 1){
          break;
        }
        if(board[z+1][y] != 0 && board[z+1][y] != board[z][y]){
          break;
        }
        if(board[z+1][y] == 0){
          board[z+1][y] = board[z][y];
        }
        else if(board[z][y] == board[z+1][y]){
          board[z+1][y] *= 2;
          board[z+1][y] += 1;
          score += 2*(board[z][y]);
        }
        board[z][y]=0;
        move++;
      }
    }
  }
}

void gameover(unsigned char board[SIZE][SIZE]){
  for(int x=0; x<4; x++){
    for(int y=0; y<4; y++){
      if(board[x][y]==0){
        return;
      }
    }
  }  
  for(int x=0; x<3; x++){
    for(int y=0; y<3; y++){
      if(board[x][y] == board[x+1][y] || board[x][y] == board[x][y+1]){
      return;
      }
    }
  }    
  for(int x=0; x<3; x++){
    if(board[x][3] == board[x+1][3]){
      return;
    }
  }     
  for(int y=0; y<3; y++){
    if(board[3][y] == board[3][y+1]){
      return; 
    }
  }
  printf("         GAME OVER          \n");
  exit(0);
}