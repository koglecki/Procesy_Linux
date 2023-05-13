# Signals_Linux

Aby uruchomić program w konsoli systemowej Linux, po pobraniu należy skompilować go komendą:
  gcc signalsLinux.c -o <nazwa pliku wykonywalnego>
  
A następnie uruchomić:
  ./<nazwa pliku wykonywalnego> -w 5 -m 5
  
Liczba przy przełączniku -w oznacza czas (w sekundach) co jaki będzie się uruchamiał kolejny proces potomny.
Liczba przy przełączniku -m oznacza maksymalną długość życia procesu potomnego w sekundach
  
Działający program można zatrzymać kombinacją klawiszy Ctrl-C. Wtedy program poczeka na wszystkie rozpoczęte a jeszcze niezakończone
procesy potomne i zakończy swoje działanie.
