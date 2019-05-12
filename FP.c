#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


pthread_t tid[10];
char command[100];
int buffer[10];

struct cron_data{
  int tm_sec;
  int tm_min;   
  int tm_hour; 
  int tm_mday; 
  int tm_mon; 
  int tm_wday; 
	int buffer[10];
  char command[100];
};

int cariline(){
		int n;
    FILE *fp = fopen ( "/home/dani/FP/crontab.data", "r");
    char line[1024];
    char ch = getc ( fp );
    int index = 0;
    while ( ch != EOF ) {
        if ( ch != '\n'){
            
        }else {
            line[index] = '\0';
            n+=1;
        }
        ch = getc ( fp );
    }
    fclose ( fp );
    return n;
}

void* coba(void *args){
  int i=1;
  char command[1000];
  struct cron_data* datanextjob =(struct cron_data*)args; 
  time_t now=time(NULL); 
  struct tm waktu=*localtime(&now);
  time_t nextjob=time(NULL); 
  struct tm waktunextjob=*localtime(&nextjob);
  while(1){
    waktunextjob.tm_sec=0;
    if(datanextjob->tm_min!= -1){
      waktunextjob.tm_min=datanextjob->tm_min;
    }
    if(datanextjob->tm_hour!= -1){
      waktunextjob.tm_hour=datanextjob->tm_hour;
    }
    if(datanextjob->tm_mday!= -1){
      waktunextjob.tm_mday=datanextjob->tm_mday;
    }
    if(datanextjob->tm_mon!= -1){
      waktunextjob.tm_mon=datanextjob->tm_mon;
    }
    if(datanextjob->tm_wday!= -1){
      waktunextjob.tm_wday=datanextjob->tm_wday;
    }

    if(waktu.tm_sec==waktunextjob.tm_sec && waktu.tm_min==waktunextjob.tm_min && waktu.tm_hour==waktunextjob.tm_hour && waktu.tm_mday==waktunextjob.tm_mday && waktu.tm_mon==waktunextjob.tm_mon && waktu.tm_wday==waktunextjob.tm_wday){
      sprintf(command,"touch ~/FP/Coba%d.txt",i);
      system(command);
      i++;
    }
    sleep(1);
    time_t now=time(NULL); 
    waktu=*localtime(&now);
    time_t nextjob=time(NULL); 
    waktunextjob=*localtime(&nextjob); 
  }
  
}


//===================================================================================================================================================================================================================================//
int main() {
  pid_t pid = fork(), sid;
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  umask(0);
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }
  if ((chdir("/home/dani/FP")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  int linecrontab = cariline();
  int n;
  while(1){
    if(n<linecrontab){
      
      struct cron_data cronjob;
      char ch;
      int count;
      FILE *fp = fopen("crontab.data", "r");
      fseek(fp, 0L, SEEK_END);
          count = ftell(fp);
          fseek(fp, 0L, SEEK_SET);
      int i = 0;	
      int dir = 0;
      while(count--){
        ch = fgetc(fp);
        if(i<9){
          if(ch!='*'){
            buffer[i]= ch-'0';
          }else{
            buffer[i]= -1;
          }
            
        }
        if(i>=10){
          command[dir]=ch;
          dir++;
        }
        i++;
      }


      if (buffer[0]== -1 && buffer[2]== -1 && buffer[4]== -1 && buffer[6]== -1 && buffer[8]== -1)
      {
          cronjob.tm_sec= 0;
      }
      if (buffer[0]== -1)
      {
        cronjob.tm_min= -1;
      }else{
        cronjob.tm_min=buffer[0];
      }

       if (buffer[2]== -1)
      {
        cronjob.tm_hour= -1;
      }else{
        cronjob.tm_hour=buffer[2];
      }

       if (buffer[4]== -1)
      {
        cronjob.tm_mday= -1;
      }else{
        cronjob.tm_mday=buffer[4];
      }

       if (buffer[6]== -1)
      {
        cronjob.tm_mon=-1;
      }else{
        cronjob.tm_mon=buffer[6];
      }

       if (buffer[8]== -1)
      {
        cronjob.tm_wday= -1;
      }else{
        cronjob.tm_wday=buffer[8];
      }

      pthread_create(&(tid[n]), NULL,&coba,(void *)&cronjob);
      pthread_join(tid[n], NULL);
      n++;
    }
  }

  exit(EXIT_SUCCESS);
}
