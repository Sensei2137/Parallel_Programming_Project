#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>


int main(int argc, char* argv[])
{
//struktury do kolejek komunikatow
struct miejsce
	{
		long type;
		//char text[10];
		int miejsce;
	}msg,msg2,subs;
struct kanalbuf
	{
		long type;
		//char text[10];
		int kanal;
	}kan,kan1;
struct loginbuf
	{
		long type;
		char login[10];
		int index;
	}login;
struct logoutbuf
	{
		long type;
		int esc;
	}logout;
struct potwbuf
	{
		long type;
		int text;
	}potw,potw1,potw2,potw3;
struct chatstruct
	{
	  long type;
	  char text[20];
	}chat;
//Rozmiary poszczegolnych struktur
int potw_size	= sizeof(char)*10;
int login_size 	= sizeof(char)*20;
int msg_size	= sizeof(char)*10;
int logout_size = sizeof(int);

//Tablice
int tab_log[100]; //ogolna tablica userow
//Tablice na poszczegolnych kanalach (tematach)
int tab_kanal0[15];
int tab_kanal1[15];
int tab_kanal2[15];
int tab_kanal3[15];
int tab_kanal4[15];
//Tablice na poszczegolnych kanalach do wiadomosci asynchronicznych (tematach)
int tab_kanal4synch[15];
int tab_kanal3synch[15];
int tab_kanal2synch[15];
int tab_kanal1synch[15];
int tab_kanal0synch[15];
// Kolejki komunikatow
int id	= msgget(9001,0644|IPC_CREAT); // Glowna kolejka
int kolejka_kanal = msgget(9002,0644|IPC_CREAT); //Kolejka do przyjmowania wiadomosci
int kolejka_ser = msgget(9003,0644|IPC_CREAT);  //Kolejka do rozsylania wiadomosci synch
int kolejka_synch = msgget(9004,0644|IPC_CREAT); // Kolejka do rozsylania wiadomosci asynch

//Zmienne pomocnicze
int a = -1;
int b = -1;
int c = -1;
int d = -1;
int n = 0;
int m = 0;
int z = 0;
//Zmienne iterowane
int tab0 = 0; 
int tab1 = 0; 
int tab2 = 0; 
int tab3 = 0;
int tab4 = 0;
int cht0, cht1, cht2, cht3, cht4;
int i;
//pidy procesow rozsylajacych wiadomosci
int pid4,pid3,pid2,pid1,pid0;

//Typ struktur do kolejek komunikatow
potw.type = 9;
potw2.type = 9;
potw3.type = 9;

//Wypelnienie tablic wartoscia -1
for(i=0;i<15;i++){
  tab_kanal0[i]=-1;
  tab_kanal1[i]=-1;
  tab_kanal2[i]=-1;
  tab_kanal3[i]=-1;
  tab_kanal4[i]=-1;
  tab_kanal4synch[i]=-1;
  tab_kanal3synch[i]=-1;
  tab_kanal2synch[i]=-1;
  tab_kanal1synch[i]=-1;
  tab_kanal0synch[i]=-1;
}

for(i=0;i<100;i++)
  tab_log[i]=-1;
  


while(1)
	{
		a = msgrcv(id,&login,login_size,1,IPC_NOWAIT); //oczekiwanie na login
		b = msgrcv(id,&msg,sizeof(int),2,IPC_NOWAIT); //oczekiwanie na rejestracje tematu
		c = msgrcv(id,&logout,logout_size,3,IPC_NOWAIT); //oczekiwanie na logout
 		d = msgrcv(kolejka_kanal,&potw1,sizeof(int),20,IPC_NOWAIT); // oczekiwanie wiadomosci do rozdysponowania

		if(a>0)
		{	
			//Sprawdzenie, czy proces o takim id jest juz w bazie
			n=login.index;
			for(i=0; i<100; i++)  
			{  
			 if(tab_log[i]==n){
			   potw2.text=0;
			   msgsnd(id,&potw2,potw_size,0);
			   goto end;
			   break;
			  }
			  else
			  potw2.text=1;
			}
			
			tab_log[n]=n;
			printf("\nOtrzymano typ_1\n");
			printf("login: %s \n",login.login);
			printf("index: %d \n\n",tab_log[n]);
			
			//wyslij potwierdzenie odbioru
		
			msgsnd(id,&potw2,potw_size,0);
		

		}
		if(b>0)
		{	
			//Informacje o temacie oraz typie subskrypcji
			msgrcv(id,&kan,sizeof(int),2,IPC_NOWAIT);
			msgrcv(id,&subs,sizeof(int),2,IPC_NOWAIT);
			printf("Otrzymano informacje\n");
		//Przyporzadkowanie danych klienta	
		switch(kan.kanal){
		
		  case 0:
			   tab0=0;
			   while(tab_kanal0[tab0]>=0)
			   tab0++;
			   tab_kanal0[tab0]=msg.miejsce;
			   tab0=0;
			   while(tab_kanal0synch[tab0]>=0)
			   tab0++;
			   tab_kanal0synch[tab0]=subs.miejsce;
			   potw3.text=0;
			   break;
		  case 1:
			   tab1=0;
			   while(tab_kanal1[tab1]>=0)
			   tab1++;
			   tab_kanal1[tab1]=msg.miejsce;
			   tab1=0;
			   while(tab_kanal1synch[tab1]>=0)
			   tab4++;
			   tab_kanal1synch[tab1]=subs.miejsce;
			   potw3.text=0;
			   break;
		  case 2:
			   tab2=0;
			   while(tab_kanal2[tab2]>=0)
			   tab2++;
			   tab_kanal2[tab2]=msg.miejsce;
			   tab2=0;
			   while(tab_kanal2synch[tab2]>=0)
			   tab2++;
			   tab_kanal2synch[tab2]=subs.miejsce;
			   potw3.text=0;
			   break;
		  case 3:
			   tab3=0;
			   while(tab_kanal3[tab3]>=0)
			   tab3++;
			   tab_kanal3[tab3]=msg.miejsce;
			   tab3=0;
			   while(tab_kanal3synch[tab3]>=0)
			   tab3++;
			   tab_kanal3synch[tab4]=subs.miejsce;
			   potw3.text=0;
			   break;
		  case 4:
			   tab4=0;
			   while(tab_kanal4[tab4]>=0)
			   tab4++;
			   tab_kanal4[tab4]=msg.miejsce;
			   tab4=0;
			   while(tab_kanal4synch[tab4]>=0)
			   tab4++;
			   tab_kanal4synch[tab4]=subs.miejsce;
			   potw3.text=0;
			   break;
		  default:
			  potw3.text=1;
			  break;
			   
			}
			msgsnd(id,&potw3,potw_size,0);
			
			
		}
		//Logout
		if(c>0)
		{
		  z=0;
		msgrcv(id,&kan1,sizeof(int),10,0);  
		tab_log[logout.esc]=-1;
		
		//Sprawdzenie ilu klientow zostalo w systemie
		for(i=0;i<100;i++){
		  if(tab_log[i]>=0)
		    z++;
		  
		}
		//Jesli ostatni klient wylogowal sie z serwera - serwer konczy prace
		if(z==0)
		{
		   msgctl(9001,IPC_RMID,0);
		   msgctl(9002,IPC_RMID,0);
		   msgctl(9003,IPC_RMID,0);
		   msgctl(9004,IPC_RMID,0);
		   exit(0);
		}
		//Usuniecie klienta z tablic
		switch(kan1.kanal){
		
		  case 0:
			   for(tab0=0;tab0<15;tab0++)
			   {
			    if(tab_kanal0[tab0]==logout.esc){
			      tab_kanal0[tab0]=-1;
			      break;
			    }
			   }
			   break;
		  case 1:
			   for(tab1=0;tab1<15;tab1++)
			   {
			    if(tab_kanal1[tab1]==logout.esc){
			      tab_kanal1[tab1]=-1;
			      break;
			    }
			   }
			   break;
		  case 2:
			   for(tab2=0;tab2<15;tab2++)
			   {
			    if(tab_kanal2[tab2]==logout.esc){
			      tab_kanal2[tab2]=-1;
			      break;
			    }
			   }
			   break;
		  case 3:
			   for(tab3=0;tab3<15;tab3++)
			   {
			    if(tab_kanal3[tab3]==logout.esc){
			      tab_kanal3[tab3]=-1;
			      break;
			    }
			   }
			   break;
		  case 4:
			   for(tab0=0;tab4<15;tab4++)
			   {
			    if(tab_kanal4[tab4]==logout.esc){
			      tab_kanal4[tab4]=-1;
			      break;
			    }
			   }
			   break;
		  default:
			  break;
		}
		}
		//Rozsylanie wiadomosci
		if(d>0){
		  //Wiadomosc do przekazania
		  msgrcv(kolejka_kanal,&chat,sizeof(char[20]),potw1.text,0);
		  //Utworzenie procesow, ktore rozsylaja wiadomosci do odpowiednich tematow
		  if(fork()==0){
		      
		   pid4 = getpid();
		   chat.type=potw1.text;
		   for(cht4=0;cht4<15;cht4++){
			  if(tab_kanal4[cht4]>=0){
			    if(tab_kanal4synch[cht4]==0)
			      msgsnd(kolejka_synch,&chat,sizeof(char[20]),0);
			    else
			    {
			      msgsnd(kolejka_ser,&chat,sizeof(char[20]),0);
			      msgrcv(kolejka_kanal,&potw,sizeof(int),24,0);
			    }
			  }
			    
			  }
			  
		  //Zabicie procesu, aby nie tkwil w systemie 
		  kill(pid4,9);
		   
		  }
		  if(fork()==0){
		   pid3 = getpid();
		   chat.type=potw1.text;
		   for(cht3=0;cht3<15;cht3++){
			if(tab_kanal3[cht3]>=0){
			    if(tab_kanal3synch[cht3]==0)
			      msgsnd(kolejka_synch,&chat,sizeof(char[20]),0);
			    else
			    {
			      msgsnd(kolejka_ser,&chat,sizeof(char[20]),0);
			      msgrcv(kolejka_kanal,&potw,sizeof(int),24,0);
			    }
			  }
			  } 
		  //Zabicie procesu, aby nie tkwil w systemie   
		  kill(pid3,9);
		   
		  }
		    if(fork()==0){
		   pid2 = getpid();
		   chat.type=potw1.text;
		   for(cht2=0;cht2<15;cht2++){
			if(tab_kanal2[cht2]>=0){
			    if(tab_kanal2synch[cht2]==0)
			      msgsnd(kolejka_synch,&chat,sizeof(char[20]),0);
			    else
			    {
			      msgsnd(kolejka_ser,&chat,sizeof(char[20]),0);
			      msgrcv(kolejka_kanal,&potw,sizeof(int),24,0);
			    }
			  }
			  } 
		  //Zabicie procesu, aby nie tkwil w systemie  
		  kill(pid2,9);
		   
		  }
		    if(fork()==0){
		   pid1 = getpid();
		   chat.type=potw1.text;
		   for(cht1=0;cht1<15;cht1++){
			if(tab_kanal1[cht1]>=0){
			    if(tab_kanal1synch[cht1]==0)
			      msgsnd(kolejka_synch,&chat,sizeof(char[20]),0);
			    else
			    {
			      msgsnd(kolejka_ser,&chat,sizeof(char[20]),0);
			      msgrcv(kolejka_kanal,&potw,sizeof(int),24,0);
			    }
			  }
			  } 
		  //Zabicie procesu, aby nie tkwil w systemie   
		  kill(pid1,9);
		   
		  }
		    if(fork()==0){
		   pid0 = getpid();
		   chat.type=potw1.text;
		   for(cht0=0;cht0<15;cht0++){
			if(tab_kanal0[cht0]>=0){
			    if(tab_kanal0synch[cht0]==0)
			      msgsnd(kolejka_synch,&chat,sizeof(char[20]),0);
			    else
			    {
			      msgsnd(kolejka_ser,&chat,sizeof(char[20]),0);
			      msgrcv(kolejka_kanal,&potw,sizeof(int),24,0);
			    }
			  }
			  } 
		  //Zabicie procesu, aby nie tkwil w systemie   
		  kill(pid0,9);
		   
		  }
		
		}
		//Petla servera
		end:
		a = -1;
		b = -1;
		c = -1;
		d = -1;
		usleep(5000);
		
	}

return 0;
}
