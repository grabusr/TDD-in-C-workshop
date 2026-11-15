# Blackjack

Cała implementacja zadania powinna odbywać się w parach metodologią Test Driven Development. Zmiana następuje przy zmnianie:
1. Test - Osoba 1
2. Minimalna implementacja - Osoba 2
3. Refactoring - Osoba 1
4. Test - Osoba 2
5. ...

Zalecenie jest prowadzenie prac na osobnym branchu, commit po każdej zmienie.

Prowadzący szkolenie jest Product Ownerem Waszego zespołu. 

## Zadanie 1

Zapoznajcie się z [zasadami Blackjack](01-zasady-gry.md). Przedyskutujcie ogólną architekturę rozwiązania.

## Zadanie 2

Zaimplementujcie funkcję do obliczania wartości kart jaką posiada na ręce gracz/krupier.

## Zadanie 3

Zaimplementujcie tasowanie listy kart wykorzystując już instniejącą implementację deck parsera.

## Zadanie 4

Zaimplementujcie model obsługujący logikę gry blackjack

## Zadanie 5

Wykonajcie review całego modułu deck parsera i dokonajcie refactoringu.

## Zadanie 6

Zaimplementujcie model symulacji gry. Model ma mieć możliwość przyjmowania różnych strategii gracza (przykłady strategii są opisane w [Strategiach graczy](01-zasady-gry.md#strategie-gracza)). Zaimplementujcie jedną z automatycznych strategii.

## Zadanie 7

Zaimplementujcie interaktywną aplikację konsolową, która:
1. Pozwala na wczytanie listy kart kart z dysku.
2. Pozwala wybrać ziarno losowości
3. Pozwala na podanie ilości talii (52 karty), którą aplikacja wygeneruje zamiast wczytywania jej z pliku.

## Zadanie 8

Okazuje się, że aplikacja ma trafić na urządzenia, które mają dość ograniczoną ilość miejsca dla dynamicznie alokowanej pamięci. Zaprojektujcie scenariusze zachowania się aplikacji dla nieudanych alokacji pamięci.
Zaimplementujcie testy sprawdzające poprawność alokacji pamięci.