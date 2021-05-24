#include "serial_com.h"

// Autorzy: Julianna Lachowicz, Jaroslaw Affek

char menu, prev_menu;   						// zmienne zapamietujace wybor uzytkownika obecny i poprzedni
float timer = 0, previous = 0;			// zmienne przechowujace mierzony czas
unsigned short int state = 0;				// zmienna okreslajaca aktualny stan stopera (zatrzymany lub w trakcie pomiaru)

void main (void)
{
  setup_UART();											// konfiguracja transmisji UART
	send_string("STOPER: Julianna Lachowicz, Jaroslaw Affek\n\n");
	send_string("Wlacz stoper:      w\nZatrzymaj stoper:  s\nZresetuj stoper:   r\n\n");
	setup_counter();									// konfiguracja licznika T1 odpowiadajacego za pomiar czasu w stoperze
	while(1)
	{
		menu = receive_char();					// wczytanie znaku od uzytkownika
		switch(menu)										// switch odpowiadajacy za interakcje z uzytkownikiem
		{
			case 'w': 										// uruchomienie licznika, gdy juz uruchomiony - brak reakcji (kontynuacja pomiaru)
				state = 1;
				if(prev_menu == 'w')
					previous = 0;
				timer = count(timer,previous);
				previous = timer;
				break;
			case 's':											// zatrzymanie licznika i zapis poprzedniego czasu z momentu zatrzymania
				state = 0;
				if(previous != 0)
					text(timer,previous);
				break;
			case 'r': 										// reset licznika do 000, bez wplywu na zatrzymanie/wlaczanie pomiaru czasu, powoduje zapis stanu sprzed wcisniecia r
				timer = 0;
				if(state == 1)
					timer = count(timer,previous);
				else
					if(previous != 0)
						text(timer,previous);
				previous = timer;
				break;
			default:											// pozostale znaki nie wplywaja na dzialanie stopera
				if(previous != 0 && state == 1)
				{
					previous = 0;
					timer = count(timer,previous);
					previous = timer;
				}
		}
		prev_menu = menu;
	}
	
}