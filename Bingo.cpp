#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>

pthread_mutex_t mutex;
sem_t wrote,note,note2;


int board1[5][5];
int board2[5][5];
int buff1[26]={0};
int buff2[26]={0};
int speakerbuff[26]={0};
int cnum;
pthread_t pid1,pid2,pid3;

void initialize(){
    pthread_mutex_init(&mutex,NULL);
    sem_init(&wrote,0,2);
    sem_init(&note,0,0);
    }
void fillBoard1(int i,int j){
       int n1=rand()%25+1;
    
    if(buff1[n1]==0){
        board1[i][j]=n1;
        buff1[n1]++;    
    }
    else fillBoard1(i,j);
   
}
void fillBoard2(int i,int j){
     int    n2=rand()%25+1;
      if(buff2[n2]==0){
        board2[i][j]=n2;
        buff2[n2]++;    
    }
    else fillBoard2(i,j);
}
callNumber(){
 int    n3=rand()%25+1;
      if(speakerbuff[n3]==0){
        cnum=n3;
        speakerbuff[n3]++;    
    }
    else callNumber();
}
void print1(){
      int i,j,k;
    printf("\nPlayer 1:-\n");
for(k=0;k<5;k++){
        printf("|----");
    }
printf("|\n");    
for(i=0;i<5;i++){
    printf("|");
    for(j=0;j<5;j++){
    if(board1[i][j]==-1) {
        
    printf("  X ");    
    printf("|");
    }
    else
    printf(" %2d |", board1[i][j]);
        
    }
    printf("\n|");
    for(k=0;k<5;k++){
        printf("----|");
    }
    printf("\n");
   }
}

void print2(){
     int i,j,k;
printf("\nPlayer 2:-\n");
    for(k=0;k<5;k++){
            printf("|----");
        }
        printf("|\n");    
    for(i=0;i<5;i++){
        printf("|");
        for(j=0;j<5;j++){
        if(board2[i][j]==-1) printf("  X |");
        else    
            printf(" %2d |", board2[i][j]);
        }
        printf("\n|");
    for(k=0;k<5;k++){
        printf("----|");
    }
    printf("\n");
   }
}

void * speaker(void* arg){
    int i;
    for(i=0;i<25;i++){

    sleep(5);
    srand(time(0));
    sem_wait(&wrote);
    sem_wait(&wrote);
    
    pthread_mutex_lock(&mutex);
    callNumber();
    printf("\nCurrent Number:%d\n",cnum);
    pthread_mutex_unlock(&mutex);
    sem_post(&note);
    sem_post(&note);
       }    
}

int checkRow(int board[][5], int flag)
{
 int count=0,i,j;
 for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            if(board[i][j]==-1){
                count++;
            if(count==5) flag++;
            }
         }
        count=0;
}
  return flag;
}
int checkCol(int board[][5],int flag)
{
 int count=0,i,j;
 for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            if(board[j][i]==-1){
                count++;
            if(count==5) flag++;
            }
        }
        count=0;
    }
 return flag;
 }
int checkDiagNear(int board[][5],int flag)
{
 int count=0,i,j;
 for(i=0;i<5;i++){
     if(board[i][i]==-1){
                count++;
        }
  }
    if(count==5) flag++; 
return flag;
   }
int checkDiagFar(int board[][5],int flag)
{
     int count=0,i,j;
 for(i=4;i>=0;i--){
    
            if(board[i][4-i]==-1){
                count++;
            }
       }
    if(count==5) flag++;
 return flag;
}

void* player1(void* arg){    
    int i;
    while(1){
    
    sleep(7);
    int i,j,flag=0,flagt=0;
    sem_wait(&note);
    pthread_mutex_lock(&mutex);
    for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            if(cnum==board1[i][j])
                    board1[i][j]=-1;
             }
    }
    print1();
    
    flag+=checkRow(board1,flagt);
    flagt=0;
    flag+=checkCol(board1,flagt);
    flagt=0;
    flag+=checkDiagNear(board1,flagt);
    flagt=0;
    flag+=checkDiagFar(board1,flagt);
    
    if(flag>=5) {
        print1();
        print2();
        printf("The winner is player 1\n");
        exit(0);
    }
    printf("Player 1 score: %d\n", flag);
    pthread_mutex_unlock(&mutex);
    sem_post(&wrote);
  }
}
void * player2(void* arg){
     int i;
     while(1){
      sleep(7);
     int i,j,flag=0,flagt=0;
     sem_wait(&note);
    pthread_mutex_lock(&mutex);
    for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            if(cnum==board2[i][j])
                    board2[i][j]=-1;
            }
    }
    print2();
     flag+=checkRow(board2,flagt);
    flagt=0;
    flag+=checkCol(board2,flagt);
    flagt=0;
    flag+=checkDiagNear(board2,flagt);
    flagt=0;
    flag+=checkDiagFar(board2,flagt);
    
    if(flag>=5) {
        print1();
        print2();
        printf("The winner is player 2\n");
        exit(0);
    }
    printf("Player 2 score: %d\n", flag);
    pthread_mutex_unlock(&mutex);
    sem_post(&wrote);
 }
}

int main()
{
srand(time(0));
initialize();
int i,j,k,l;

for(i=0;i<5;i++){
    for(j=0;j<5;j++){
        fillBoard1(i,j);
        fillBoard2(i,j);
    }
}
print1();
print2();
 
void *status;

pthread_create(&pid1,NULL,speaker,NULL);
pthread_create(&pid2,NULL,player1,NULL);
pthread_create(&pid3,NULL,player2,NULL);

pthread_join(pid1,&status);
pthread_join(pid2,&status);
pthread_join(pid3,&status);
return 0;
}


