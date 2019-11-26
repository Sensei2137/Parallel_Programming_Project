#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
//struktury do kolejek komunikatowt
struct miejsebuf
	{
		long type;
		int miejsce;
	}msg,msg2,subs;
struct kanalbuf
	{
		long type;;
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
	}potw,potw1,potw2;
struct chatstruct
	{
	  long type;
	  char text[20];
	}chat,odbiur;


//Rozmiary poszczegolnych struktur	
int potw_size	= sizeof(char)*10;
int login_size 	= sizeof(char)*20;
int msg_size	= sizeof(char)*10;
int logout_size = sizeof(int);

//Deklaracja zmiennych
char str[100];
int typ_subskrypcji;
int index; 
int typ; 
int input;
int try,traj;
int pid;

//Typ struktur do kolejek komunikatow
msg.type = 2;
msg2.type = 2;
logout.type = 3;
login.type  = 1;
kan.type = 2;
kan.kanal = -1;

// Kolejki komunikatow
int id	= msgget(9001,0644|IPC_CREAT); // Glowna kolejka
int kolejka_kanal = msgget(9002,0644|IPC_CREAT); //Kolejka do przyjmowania wiadomosci
int kolejka_ser = msgget(9003,0644|IPC_CREAT);  //Kolejka do rozsylania wiadomosci synch
int kolejka_synch = msgget(9004,0644|IPC_CREAT); // Kolejka do rozsylania wiadomosci asynch


//Poczatek programu
printf("podaj login\n");//Wybor loginu
scanf("%s",str);
cof:
printf("podaj id <0,99)\n");//Wybor id
scanf("%d", &index);
if((index<0)||(index>99)){
printf("prosze podac prawidlowy index\n");
goto cof;
}
cof1:
printf("wybierz typ subskrypcji ||  1=synch 0 = asynch\n");//Wybor typu subskrypcji
scanf("%d",&typ_subskrypcji);
if((typ_subskrypcji!=1)&&(typ_subskrypcji!=0)){
printf("prosze podac prawidlowy typ subskrypcji\n");
goto cof1;
}
//Przeslanie informacji o kliencie do servera
strcpy(login.login,str);
login.index = index;
printf("Wysyla informajce logowania...\n ");
try = msgsnd(id,&login,login_size,0);
printf("Wyslano\n\n");
printf("Oczekiwanie na odpowiedz...\n\n");
try = msgrcv(id,&potw1,potw_size,9,0);
//Odpowiedz od servera
if(try>0)
{
	printf("Otrzymano wiadomosc: \n\n");
	if(potw1.text==0){
	  printf("Istnieje proces o takim id\n");//Program konczy sie, gdy sprobujemy nadpisac innego klienta
	  exit(0);
	}
	printf("Witaj w usludze \n\n"); //Wszystko poszlo pomyslnie
}
else
{
	printf(" error. \n\n");
}
if(typ_subskrypcji==0)
  subs.miejsce=0;
  else
    subs.miejsce=1;
  
//Glowna petla
while(1){
  
printf("OPCJE 1-wyslij wiadomosc, 2 - wybierz kanal, 3 - logout, 4 - odbierz wiadomosc\n\n");

scanf("%d",&input);

switch(input)
	{
		//Wysylanie wiadomosci
		case 1:
		      if(kan.kanal==-1){
			printf("proces nie jest zajerstrowany\n");
			break;
		      }
			//Klient wysyla pierwszy komunikat o checi nadawania, nastepnie wysyla wlasciwa wiadomosc
			potw.type=20;
			potw.text=kan.kanal;
			msgsnd(kolejka_kanal,&potw,sizeof(int),0);
			chat.type=kan.kanal;
			scanf("%s",str);
			strcpy(chat.text,str);
			msgsnd(kolejka_kanal,&chat,sizeof(char[20]),0);
			break;
		//Ustalenie kanalu (tematu)
		case 2:
			msg.miejsce=index;
			printf("wybierz kanal | numery 0-4\n");
			scanf("%d",&kan.kanal);
			if((kan.kanal>4) ||(kan.kanal<0)) {
			printf("Wybrano nieprawidlowy kanal\n");
			kan.kanal=-1;
			break;
		      }
		      //Utworzenie procesu odczytujacego wiadomosci asynchornicznie
		      if(typ_subskrypcji == 0){
			  if(fork()==0)
			  {
			    pid = getpid();
			    potw.type=20;
			    while(1){
			    traj = msgrcv(kolejka_synch,&odbiur,sizeof(char[20]),kan.kanal,0);
			    if(traj>0)
			      printf("%s\n",odbiur.text);//Wypisanie wiadomosci
			    usleep(5000);
			    }
			  }
			}
			//Typ struktry potwierdzajace odbior
			switch(kan.kanal){
			  case 4:
			    subs.type = 24;
			    break;
			  case 3:
			    subs.type = 23;
			    break;
			  case 2:
			    subs.type = 22;
			    break;
			  case 1:
			    subs.type = 21;
			    break;
			  case 0:
			    subs.type = 20;
			    break;
			  
			}
			//wyslanie informacji serverowi
			msg2.miejsce=typ_subskrypcji;
			msgsnd(id,&msg,sizeof(int),0); //Index
			msgsnd(id,&kan,sizeof(int),0); //Nr kanalu - tematu
			msgsnd(id,&msg2,sizeof(int),0);//Typ subskrypcji
			printf("Wyslano wiadomosc\n");
			msgrcv(id,&potw,potw_size,9,0);
			if(potw.text==0)
			printf("Dolaczono do istniejacego kanalu\n");
			else
			printf("Utworzono nowy kanal\n");
			break;
		//Logout
		case 3:
			logout.esc = index;
			kan1.kanal=kan.kanal;
			kan1.type=10;
			msgsnd(id,&logout,logout_size,0);
			msgsnd(id,&kan1,sizeof(int),0);
			printf("Disconnect\n");
			kill(pid, 9); // Zabicie procesu nasluchujacego
			exit(0);
			break;
		//Nasluch wiadomosci
		case 4:
		      if(kan.kanal==-1){
			printf("proces nie jest zajerstrowany\n");
			break;
		      }
			//W trybie asynchornicznym nasluch jest uniemozliwiony
			if(typ_subskrypcji==0){
			printf("Opcja niedostepna w tym trybie subskrypcji\n");
			break;
		      }
			//Otrzymanie wiadomosci
			msgrcv(kolejka_ser,&odbiur,sizeof(char[20]),kan.kanal,0);
			//Wyslanie potwierdznia
			msgsnd(kolejka_kanal,&subs,sizeof(int),0);
			//Wypisanie wiadomosci
			printf("%s\n",odbiur.text);
			break;
		default:
			break;
	}

}
return 0;
}
