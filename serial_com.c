#include <stdio.h>
#include <ADUC834.H>

// konfiguracja uk³adu UART - taktowanie licznikiem T3
void setup_UART(void)
{
	T3CON = 0x82;							// konfiguracja licznika dla taktowania domyslnego 1,572864 MHz oraz UART baud rate 9600
	T3FD = 0x12;
	SCON = 0x52;							// tryb 1 - transmisja asynchroniczna
}

// wys³anie znaku
//   c - znak do wys³ania
void send_char(char c)
{
	while(!TI);
	TI=0;
	SBUF = c;
}

// wys³anie ci¹gu znaków (zakoñczonego 0x00) 
//   *str - wskaŸnik na pierwszy znak z ci¹gu
void send_string(char *str)
{
   unsigned char c; 
   while((c = *str++)) 
     send_char(c); 
}

// odbieranie znaku
char receive_char()
{
	char c;
	while(!RI);
	RI=0;
	c = SBUF;
	return c;
}

// konfiguracja licznika T1
void setup_counter()
{
	TMOD = 0x10;						// licznik T1 w trybie 1 (16 bitowa pojemnosc zliczania taktow procesora)
	TL1 = 0xCC;							// dla taktowania 1,572864 MHz nie bylo mozliwe ustawienie przepelnienia licznika co 1s (nawet dla 16 bitowego trybu), wiec rejestr licznika przepelnia sie co 0,1 s
	TH1 = 0xCC;
	TCON = 0x40;						// wlaczenie licznika T1
}

// implementacja zamiany int na string zamiast funkcji np. sprintf() z uwagi na ograniczenia wersji testowej oprogramowania
void tostring(char str[], int num)
{
    int i, rem;			// rem - reszta
    for (i = 0; i < 3; i++)
    {
        rem = num % 10;
        num = num / 10;
				str[3 - (i + 1)] = rem + '0';
    }
    str[3] = '\0';
}

// wypisywanie czasu
void text(float time, float previous)
{
	char result[3];
	send_string("\r");
	tostring(result, (int)time);
	send_string("czas: ");
	send_string(result);
	send_string(" s");
	if(previous != 0)							// wyswietlanie pomiaru poprzedniego tylko kiedy taki istnieje
	{
		send_string(" # poprzedni: ");
		tostring(result, (int)previous);
		send_string(result);
		send_string(" s");
	}
}

// zliczanie czasu
float count(float timer, float previous)
{
	unsigned int pomocniczka = 0;					//przepelnienie licznika nastepuje po 0,1s, zmienna pomocnicza umozliwia wyswietlanie zmian stopera co 1 s
	while(!RI)														//licznik dziala do momentu wczytania dowolnego znaku - o ewentualnej akcji lub kontynuacji pomiaru decyduje switch w pliku main
	{
		if(TF1 == 1)												// TF1 informuje o przepelnieniu licznika (odmierzeniu 0,1 s)
		{
			TF1 = 0;													// zerowanie flagi informaujacej o przepelnieniu
			TL1 = 0xCC;												// nadawanie poczatkowych wartosci rejestrowi licznika po kazdym przepelnieniu
			TH1 = 0xCC;
			if(pomocniczka == 10)							// 0,1 s * 10 = 1 s , co dziesiec przepelnien stoper jest aktualizowany
			{
				text(timer, previous);
				pomocniczka = 0;
			}
			if(timer != 999)
				timer = timer + 0.1;							// inkrementacja zmiennej przechowujacej wartosc zliczanego czasu jesli wartosc licznik <=999 s
			pomocniczka++;
		}
	}

	return timer;
}