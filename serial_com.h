// modu� komunikacji szeregowej dla mikrokontrolera ADuC834

#ifndef serial_com_h
#define serial_com_h

// konfiguracja uk�adu UART
void setup_UART(void);

// wys�anie znaku
//   c - znak do wys�ania
void send_char(char c);

// wys�anie ci�gu znak�w (zako�czonego 0x00) 
//   *str - wska�nik na pierwszy znak z ci�gu
void send_string(char *str);

// odbieranie znaku
char receive_char();

// konfiguracja licznika T1
void setup_counter();

// zamiana int na string
// str - string do wyswietlenia jako wartosc na stoperz, num - wartosc int, ktora ma byc zamieniona na string
void tostring(char str[], int num);

// wypisywanie czasu
// time - zmierzony czas do wyswietlenia w oknie terminala
void text(float time, float previous);

// zliczanie czasu
// timer - mierzony czas, previous - poprzednio zmierzony czas
// funkcja zwraca aktualna wartosc czasu na stoperze
float count(float timer, float previous);

#endif