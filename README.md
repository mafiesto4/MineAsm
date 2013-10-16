MineAsm
=======

MineAsm - kompilator assemblera dla komputerów w Minecraft

##### Argumenty wejciowe:
###### -f [file]     - Ustawia plik wejściowy do skompilowania
###### -o [file]     - Ustawia plik wyjściowy do wygenerowania
###### -p [platform] - Wybiera platforme docelową(Możliwe: mc)
###### -b [adress]   - Ustawia adres początku programu w pamięci(standardowo wynosi 0)
###### -v            - Kompilator wypisze do konsoli sekwencję działań

#### Link do skompilowanej aplikacji pod Win32:
[Download](http://mafiesto4.pl/pliki/MineAsm.rar "Download")

=======
##### Przykładowy program 'licznik'

###### ; Rejestr numer 0 czyli mnemonic: Reg[0]
###### ; Rejestr numer 1 czyli mnemonic: Reg[1]
###### ; Wpisujemy do rejestru 0 znak '9' w ASCII, a do rejestru 1 wpisujemy '0' w ASCII
###### mov Reg[0], '9'
###### mov Reg[1], '0'

###### ; Etykieta - punkt w kodzie
###### LOOP:

###### ; Wypisujemy na ekran liczbę, która jest obecnie w rejestrze 0
###### ; W MineCore do pamięci ram podłączony jest wyświetlacz,
###### ; na adresie numer 5 znajduje się 1 znak, więc:
###### ; wpisując z rejestru 0 do Ram[5] wypiszemy znak na ekran
###### mov Ram[5], Reg[0]
		
###### ; Zmniejszamy liczbe w rejestrze 0 o 1
###### sub Reg[0], 1

###### ; Dopóki rejestr 0 nie jest równy rejestrowi 1, skacz do etykiety 'LOOP'
###### if Reg[0] != Reg[1]	goto LOOP

###### ; Zakoncz program
###### exit
