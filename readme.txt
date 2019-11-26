W folderze znajduja sie dwa pliki:
	-127273_server
	-127273_klient
zawierajce programy wyszczegolnione w nazwie
Obydwa programy nalezy skompliowac i uruchomic w nastepujacy sposob:
	-serwer w osobnym terminalu
	-kazdy klient w osobnym terminalu
Nie jest wazna kolejnosc uruchamiania, jezeli serwer zostanie uruchomiony jako pierwszy - bedzie czekal na klientow
Jesli klient zostanie uruchomiony jako pierwszy - bedzie czekal na uruchomienie serwera
Serwer ma limit 100 uzytkownikow
Wyrozniamy 5 kanalow (tematow), kazdy kanal ma 15 miejsc
W celu unikniecia bledow zalecanie jest zakonczenie pracy klienta przez logout(funkcja programu), ostatni klient zamyka rowniez serwer

Skrocony opis dzialania:

Serwer: 
	Oczekuje na procesy w petli, zapisuje je w bazie wraz z ich danymi(id, typ subskrypcji, wybrany kanal), rozsyla wiadomosci na odpowiednie kanaly, usuwa proces z bazy po otrzymaniu informacji "logout"
Klient:
	Wysyla swoje dane do serwera, po wybraniu kanalu(tematu), w petli pozwala na nadawanie i odbieranie wiadomosci (lub odbiera wiadomosci w tle w zaleznosci od typu subskrypcji), posiada rowniez funkcje logoutu
W przypadku ewentualnych bugow zamieszczam rowniez program czyszczacy kolejki komunikatow
