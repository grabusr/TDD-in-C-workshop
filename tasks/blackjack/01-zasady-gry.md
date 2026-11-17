# Blackjack: zasady gry (wersja warsztatowa)

Ten dokument opisuje zasady, które implementujemy na warsztacie. To uproszczona wersja kasynowego blackjacka. Różnice są
zebrane w sekcji [Uproszczenia](#uproszczenia-względem-kasyna). Przykłady rozdań na końcu dokumentu nadają się wprost na
przypadki testowe.

## Cel gry

Gracz gra przeciwko krupierowi (ang. *dealer*). Wygrywa ten, kto ma sumę punktów bliższą 21, nie przekraczając 21. Kto
przekroczy 21, ten przegrywa („przebija”, ang. *bust*).

## Karty i talia

- Standardowa talia ma 52 karty: 13 rang w 4 kolorach.
- **Kolory nie mają w blackjacku znaczenia**, dlatego w projekcie ich nie modelujemy. Karta to tylko ranga:
  `A 2 3 4 5 6 7 8 9 10 J Q K`.
- Gra może korzystać z kilku talii połączonych w jeden stos (w kasynach zwykle 6–8). Przy *n* taliach każda ranga
  występuje *4·n* razy.
- Przed grą karty są tasowane.
- Karty można też wczytać z pliku tekstowego z rangami oddzielonymi białymi znakami, np. `A K 9 7 5 10`. Wtedy kolejność
  kart jest znana z góry, co bardzo przydaje się w testach.

## Wartości kart

| Karta   | Wartość                                         |
|---------|-------------------------------------------------|
| 2–10    | wartość nominalna                               |
| J, Q, K | 10                                              |
| A (as)  | 1 albo 11, zależnie od tego, co lepsze dla ręki |

## Wartość ręki

Wartość ręki to suma wartości jej kart, z jedną regułą dla asów:

> As liczy się jako **11**, jeśli suma nie przekroczy wtedy 21. W przeciwnym razie liczy się jako **1**.

Jako 11 może liczyć się najwyżej jeden as, bo dwa asy po 11 dają już 22. Wystarczy więc policzyć wszystkie asy jako 1, a
potem dodać 10, jeśli w ręce jest as i suma nie przekroczy 21.

Ręka, w której as liczy się jako 11, to ręka **miękka** (*soft*). Kolejna karta nie może jej przebić, bo as „spadnie”
do 1. Pozostałe ręce to ręce **twarde** (*hard*).

| Karty    | Wartość | Uwagi                    |
|----------|---------|--------------------------|
| *(brak)* | 0       | pusta ręka               |
| 5, K     | 15      |                          |
| A, 7     | 18      | miękka 18                |
| A, 7, 9  | 17      | as liczony jako 1        |
| A, A     | 12      | jeden as = 11, drugi = 1 |
| A, K     | 21      |                          |
| A, A, 9  | 21      |                          |
| K, Q, A  | 21      | as liczony jako 1        |
| K, Q, 2  | 22      | przebicie                |

## Przebieg rundy

1. **Rozdanie.** Karty rozdaje się na zmianę: gracz, krupier, gracz, krupier. Każdy ma po dwie karty. Pierwsza karta
   krupiera jest **odkryta** (*up card*) i gracz ją widzi.
2. **Tura gracza.** Gracz zna swoją rękę i odkrytą kartę krupiera. Decyduje, czy:
    - **dobiera** (*hit*): bierze kolejną kartę z talii,
    - **pasuje** (*stand*): kończy swoją turę.

   Decyzje powtarzają się, dopóki gracz nie spasuje albo nie przekroczy 21. **Gracz, który przebił, od razu przegrywa.**
   Krupier wtedy już nie gra.
3. **Tura krupiera.** Krupier nie podejmuje decyzji, tylko działa według stałej reguły:
    - dobiera, dopóki ma **mniej niż 17**,
    - przy **17 lub więcej pasuje**, także przy miękkiej 17 (np. A, 6).
4. **Rozstrzygnięcie** według reguł poniżej.

## Rozstrzygnięcie

Reguły sprawdzamy po kolei:

1. Gracz przebił → **wygrywa krupier**.
2. Krupier przebił → **wygrywa gracz**.
3. Wyższa wartość ręki → wygrywa jej właściciel.
4. Równe wartości → **remis** (*push*).

## Wynik gry

Gra składa się z kolejnych rund rozgrywanych z tej samej talii. Liczymy trzy wartości:

- wygrane gracza,
- wygrane krupiera,
- remisy.

Rozpoczęcie nowej gry zeruje wynik.

## Koniec kart

Jeśli w trakcie rundy w talii zabraknie kart, **runda jest nieważna**: nie zmienia wyniku, a gra się kończy. Nie
tasujemy kart ponownie.

## Uproszczenia względem kasyna

- Nie ma zakładów ani żetonów. Liczymy tylko wygrane, przegrane i remisy.
- Nie ma rozdzielania par (*split*), podwajania (*double down*), ubezpieczenia (*insurance*) ani poddania (*surrender*).
- **Naturalny blackjack** (as + karta za 10 w dwóch pierwszych kartach) nie jest wyróżniony. To zwykłe 21.
- Gracz z 21 nadal jest pytany o decyzję. Rozsądna strategia spasuje.
- Krupier pasuje na miękkiej 17.

## Strategie gracza

Decyzję „dobierz czy pasuj” podejmuje wymienna strategia. Gra nie wie, czy gra człowiek, czy program:

| Strategia           | Reguła                                                           |
|---------------------|------------------------------------------------------------------|
| ostrożny gracz      | dobiera tylko przy wartości ręki ≤ 11, bo wtedy nie może przebić |
| gracz „jak krupier” | dobiera przy wartości < 17                                       |
| gracz konsolowy     | pyta człowieka: `h` dobierz, `s` pasuj                           |

Symulacja rozgrywa wiele rund wybraną strategią i porównuje wyniki.

## Przykładowe rozdania

Karty w talii podano od góry. Kolejność rozdania: gracz, krupier, gracz, krupier, potem dobierane karty.

| # | Talia                  | Przebieg                                                                | Wynik           |
|---|------------------------|-------------------------------------------------------------------------|-----------------|
| 1 | `10 10 8 6 K`          | gracz 10+8 = 18 pasuje; krupier 10+6 = 16 dobiera K → 26                | wygrywa gracz   |
| 2 | `10 10 5 8 K`          | gracz 10+5 = 15 dobiera K → 25, przebicie; krupier (18) już nie dobiera | wygrywa krupier |
| 3 | `10 9 7 K`             | gracz 17 pasuje; krupier 9+K = 19 pasuje; odkryta karta krupiera to 9   | wygrywa krupier |
| 4 | `10 10 8 8`            | gracz 18, krupier 18                                                    | remis           |
| 5 | `10 2 8 3 2 2 K 5`     | gracz 18 pasuje; krupier 5 → 7 → 9 → 19 pasuje; w talii zostaje 5       | wygrywa krupier |
| 6 | `2 10 3 7 2 2`         | gracz 5 dobiera → 7, dobiera → 9, pasuje; krupier 17 pasuje             | wygrywa krupier |
| 7 | `10 10 7`              | brak czwartej karty przy rozdaniu                                       | runda nieważna  |
| 8 | `10 10 8 6 K 10 9 7 K` | runda 1 jak przykład 1, runda 2 jak przykład 3                          | 1 : 1           |

## Słowniczek

| Polski                 | Angielski        | W kodzie         |
|------------------------|------------------|------------------|
| krupier                | dealer           | `dealer`         |
| ręka                   | hand             | `Hand`           |
| talia                  | deck             | `Deck`           |
| tasowanie              | shuffle          | `deckShuffle`    |
| dobierz                | hit              | `DECISION_HIT`   |
| pasuj                  | stand            | `DECISION_STAND` |
| przebicie              | bust             | `handIsBust`     |
| remis                  | push             | `ROUND_PUSH`     |
| odkryta karta krupiera | up card          | `dealerUpCard`   |
| ręka miękka / twarda   | soft / hard hand | –                |
