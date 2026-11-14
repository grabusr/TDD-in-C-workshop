# Ściąga: Google Test / Google Mock

## Nagłówki i podstawy

```cpp
#include <gtest/gtest.h>   // tylko Google Test
#include <gmock/gmock.h>   // Google Mock + matchery (zawiera też gtest)

extern "C" {               // tylko gdy nagłówek C nie ma własnego #ifdef __cplusplus
#include <Deck.h>
}

using namespace ::testing;
```

Nazewnictwo: `TEST(Kontekst, PodmiotOrzeczenieWynik[WhenWarunek])`, PascalCase, **bez podkreśleń**.

## Asercje

| `ASSERT_*`                  | `EXPECT_*`                  | Uwagi                                      |
|-----------------------------|-----------------------------|--------------------------------------------|
| `ASSERT_THAT(v, matcher)`   | `EXPECT_THAT(v, matcher)`   | **preferowane**                            |
| `ASSERT_TRUE(c)`            | `EXPECT_TRUE(c)`            | `_FALSE`                                   |
| `ASSERT_EQ(a, b)`           | `EXPECT_EQ(a, b)`           | `_NE`, `_LT`, `_LE`, `_GT`, `_GE`          |
| `ASSERT_STREQ(a, b)`        | `EXPECT_STREQ(a, b)`        | C-stringi, `_STRNE`, `_STRCASEEQ`          |
| `ASSERT_FLOAT_EQ(a, b)`     | `EXPECT_DOUBLE_EQ(a, b)`    | 4 ULP                                      |
| `ASSERT_NEAR(a, b, eps)`    | `EXPECT_NEAR(a, b, eps)`    |                                            |
| `ASSERT_DEATH(stmt, regex)` | `EXPECT_DEATH(stmt, regex)` | `abort()`, `assert()`; suite `*DeathTest`  |

- `ASSERT_*` - przerywa test, `EXPECT_*` - test leci dalej.
- Własny komunikat: `EXPECT_THAT(x, Eq(3)) << "kontekst: " << i;`
- `FAIL()`, `ADD_FAILURE()`, `SUCCEED()`, `GTEST_SKIP() << "powód";`

## TEST

```cpp
TEST(RingBuffer, GetReturnsOldestElementFirst)
{
    // given
    RingBuffer* buffer = ringBufferCreate(3);
    ringBufferPut(buffer, 1);
    ringBufferPut(buffer, 2);

    // when
    const int result = ringBufferGet(buffer);

    // then
    EXPECT_THAT(result, Eq(1));
    ringBufferDestroy(buffer);
}
```

## TEST_F (fixture)

```cpp
namespace
{
class FullRingBuffer : public Test
{
protected:
    void SetUp() override            // przed każdym testem
    {
        buffer = ringBufferCreate(1);
        ringBufferPut(buffer, 42);
    }

    void TearDown() override         // po każdym teście (też po porażce)
    {
        ringBufferDestroy(buffer);
    }

    RingBuffer* buffer = nullptr;
};
} // namespace

TEST_F(FullRingBuffer, PutRejectsElement)
{
    EXPECT_THAT(ringBufferPut(buffer, 7), IsFalse());
}
```

- Nowa instancja fixture dla **każdego** testu.
- Wspólne przygotowanie danych tylko w `SetUp()`.
- Jednorazowo na suite: `static void SetUpTestSuite()` / `static void TearDownTestSuite()`.

## TEST_P (testy parametryzowane)

```cpp
namespace
{
struct CardValueCase
{
    const char* symbol;
    int expectedValue;
};

class CardValue : public TestWithParam<CardValueCase>
{
};
} // namespace

TEST_P(CardValue, ValueReturnsPointsForSymbol)
{
    const auto& [symbol, expectedValue] = GetParam();

    EXPECT_THAT(cardValue(cardFromString(symbol)), Eq(expectedValue));
}

INSTANTIATE_TEST_SUITE_P(
    NumberCards, CardValue,
    Values(CardValueCase{"2", 2}, CardValueCase{"9", 9}, CardValueCase{"10", 10}));

INSTANTIATE_TEST_SUITE_P(
    FaceCards, CardValue,
    Values(CardValueCase{"J", 10}, CardValueCase{"Q", 10}, CardValueCase{"K", 10}));
```

Generatory parametrów:

| Generator                  | Opis                                     |
|----------------------------|------------------------------------------|
| `Values(a, b, c)`          | podane wartości                          |
| `ValuesIn(container)`      | z kontenera / tablicy                    |
| `Range(begin, end, step)`  | `[begin, end)`                           |
| `Bool()`                   | `false`, `true`                          |
| `Combine(g1, g2)`          | iloczyn kartezjański → `std::tuple`      |

Własna nazwa przypadku (4. argument):

```cpp
INSTANTIATE_TEST_SUITE_P(Faces, CardValue, Values(/*...*/),
    [](const TestParamInfo<CardValueCase>& info) { return std::string{info.param.symbol}; });
```

Fixture z parametrem i `SetUp()`: `class X : public Test, public WithParamInterface<T>`.

## Mock w C - interfejs jako struktura wskaźników na funkcje

Kod C (produkcyjny):

```c
// Random.h
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Random
{
    size_t (*next)(void* context, size_t upperExclusive);
    void* context;
} Random;

#ifdef __cplusplus
}
#endif
```

Mock (C++):

```cpp
// test/mocks/RandomMock.h
#pragma once
#include <Random.h>
#include <gmock/gmock.h>

namespace test::mocks
{
class RandomMock
{
public:
    MOCK_METHOD(size_t, next, (size_t upperExclusive));

    Random mock{
        .next = &RandomMock::nextMockMethod,
        .context = this,
    };

private:
    static size_t nextMockMethod(void* context, size_t upperExclusive)
    {
        return static_cast<RandomMock*>(context)->next(upperExclusive);
    }
};
} // namespace test::mocks
```

Użycie:

```cpp
NiceMock<mocks::RandomMock> randomMock;           // pole fixture

ON_CALL(randomMock, next(_)).WillByDefault(Return(0));
shuffle(deck, &randomMock.mock);                  // C dostaje Random*
```

## Mock w C - wolna funkcja (podmiana na etapie linkowania)

Gdy kod C woła funkcję bezpośrednio (`uint32_t clockNow(void);`), w testach linkujemy
plik z implementacją przekierowującą do globalnego mocka zamiast prawdziwego `Clock.c`:

```cpp
// test/mocks/ClockMock.h
class ClockMock
{
public:
    ClockMock() { instance = this; }
    ~ClockMock() { instance = nullptr; }

    MOCK_METHOD(uint32_t, now, ());

    static inline ClockMock* instance = nullptr;
};

// test/mocks/ClockMock.cpp
extern "C" uint32_t clockNow(void)
{
    return ClockMock::instance->now();
}
```

## Mock - ON_CALL / EXPECT_CALL

```cpp
ON_CALL(mock, method(matchery)).WillByDefault(akcja);   // zachowanie, bez weryfikacji

EXPECT_CALL(mock, method(matchery))                     // weryfikowane oczekiwanie
    .Times(n)
    .WillOnce(akcja)
    .WillRepeatedly(akcja);
```

- Preferuj `ON_CALL` + `NiceMock`, `EXPECT_CALL` tylko gdy wywołanie jest tym, co testujemy.
- `EXPECT_CALL` ustawiamy **przed** wywołaniem kodu testowanego.
- Późniejsze `EXPECT_CALL` ma pierwszeństwo przed wcześniejszym.

| Rodzaj mocka       | Niespodziewane wywołanie |
|--------------------|--------------------------|
| `NiceMock<M>`      | ignorowane               |
| `M` (domyślnie)    | ostrzeżenie              |
| `StrictMock<M>`    | błąd testu               |

**Times:** `Times(0)`, `Times(3)`, `Times(AtLeast(1))`, `Times(AtMost(2))`, `Times(Between(1, 3))`, `Times(AnyNumber())`

**Kolejność:**

```cpp
InSequence seq;            // wszystkie EXPECT_CALL w zakresie po kolei
EXPECT_CALL(m, a());
EXPECT_CALL(m, b());

Sequence s1;               // częściowa kolejność
EXPECT_CALL(m, a()).InSequence(s1);
```

**Akcje:**

| Akcja                         | Opis                                        |
|-------------------------------|---------------------------------------------|
| `Return(v)`                   | zwraca wartość                              |
| `Return()`                    | dla `void`                                  |
| `ReturnArg<N>()`              | zwraca N-ty argument                        |
| `ReturnPointee(&x)`           | zwraca aktualną wartość `x`                 |
| `SetArgPointee<N>(v)`         | `*argN = v` (parametr wyjściowy)            |
| `SetArrayArgument<N>(b, e)`   | kopiuje zakres do tablicy w argumencie N    |
| `SaveArg<N>(&x)`              | zapisuje argument                           |
| `DoAll(a1, a2, ...)`          | kilka akcji, ostatnia zwraca wartość        |
| `Invoke(f)`                   | woła funkcję / lambdę z tymi samymi arg.    |
| `InvokeWithoutArgs(f)`        | j.w. bez argumentów                         |
| `Assign(&x, v)`               | `x = v`                                     |
| `ReturnRoundRobin({1, 2, 3})` | kolejne wartości cyklicznie                 |

Weryfikacja w trakcie testu: `Mock::VerifyAndClearExpectations(&mock);`

## Matchery

**Ogólne / porównania**

| Matcher                   | Pasuje gdy                    |
|---------------------------|-------------------------------|
| `_`                       | cokolwiek                     |
| `A<T>()` / `An<T>()`      | dowolna wartość typu `T`      |
| `Eq(v)` / `Ne(v)`         | `==` / `!=`                   |
| `Lt(v)` `Le(v)` `Gt(v)` `Ge(v)` | `<` `<=` `>` `>=`       |
| `IsTrue()` / `IsFalse()`  | wartość logiczna              |
| `IsNull()` / `NotNull()`  | wskaźnik                      |
| `Ref(x)`                  | referencja do `x`             |
| `TypedEq<T>(v)`           | `==` z wymuszonym typem       |

**Zmiennoprzecinkowe:** `FloatEq(v)`, `DoubleEq(v)`, `FloatNear(v, eps)`, `DoubleNear(v, eps)`, `NanSensitiveDoubleEq(v)`

**Napisy** (`const char*` i `std::string`)

| Matcher                    | Pasuje gdy                    |
|----------------------------|-------------------------------|
| `StrEq(s)` / `StrNe(s)`    | równe / różne                 |
| `StrCaseEq(s)`             | równe bez wielkości liter     |
| `HasSubstr(s)`             | zawiera                       |
| `StartsWith(s)` / `EndsWith(s)` | prefiks / sufiks         |
| `MatchesRegex(r)`          | cały napis pasuje do regex    |
| `ContainsRegex(r)`         | fragment pasuje do regex      |
| `IsEmpty()`                | pusty                         |

**Kontenery** (STL, `std::span`, tablice C o znanym rozmiarze)

| Matcher                          | Pasuje gdy                                  |
|----------------------------------|---------------------------------------------|
| `ElementsAre(m1, m2, ...)`       | dokładnie te elementy w tej kolejności      |
| `ElementsAreArray(arr)`          | j.w. z tablicy / kontenera                  |
| `UnorderedElementsAre(m1, ...)`  | te elementy w dowolnej kolejności           |
| `UnorderedElementsAreArray(arr)` | j.w. z tablicy                              |
| `Contains(m)`                    | co najmniej jeden element pasuje            |
| `Contains(m).Times(n)`           | dokładnie `n` elementów pasuje              |
| `Each(m)`                        | każdy element pasuje                        |
| `IsEmpty()`                      | pusty                                       |
| `SizeIs(m)`                      | rozmiar pasuje, np. `SizeIs(3)`             |
| `IsSubsetOf(...)` / `IsSupersetOf(...)` | podzbiór / nadzbiór                  |
| `WhenSorted(m)`                  | po posortowaniu pasuje do `m`               |
| `Pointwise(m, other)`            | parami z `other`, np. `Pointwise(Eq(), v)`  |
| `BeginEndDistanceIs(m)`          | liczba elementów                            |

Wskaźnik + rozmiar z C → `std::span`:
`EXPECT_THAT((std::span{deckCards(deck), deckSize(deck)}), ElementsAre(...));`

**Struktury i wskaźniki**

| Matcher                         | Pasuje gdy                         |
|---------------------------------|------------------------------------|
| `Field(&S::pole, m)`            | pole struktury pasuje              |
| `Field("nazwa", &S::pole, m)`   | j.w. z nazwą w komunikacie         |
| `Property(&C::getter, m)`       | wynik gettera pasuje               |
| `Pointee(m)`                    | `*ptr` pasuje                      |
| `Pointer(m)`                    | surowy wskaźnik (smart ptr)        |
| `ResultOf(f, m)`                | `f(arg)` pasuje                    |
| `FieldsAre(m1, m2, ...)`        | pola struktury / krotki po kolei   |
| `Pair(m1, m2)`                  | `std::pair`                        |
| `Optional(m)`                   | `std::optional` z wartością        |
| `VariantWith<T>(m)`             | `std::variant` z typem `T`         |

**Złożone**

| Matcher                    | Pasuje gdy                     |
|----------------------------|--------------------------------|
| `AllOf(m1, m2, ...)`       | wszystkie                      |
| `AnyOf(m1, m2, ...)`       | którykolwiek                   |
| `Not(m)`                   | nie pasuje                     |
| `Conditional(c, m1, m2)`   | `c ? m1 : m2`                  |

**Wiele argumentów mocka:** `.With(Args<0, 1>(Lt()))` - relacja między argumentami.

## Własny matcher

Najprościej - `MATCHER` (nazwa → komunikat: „is card with rank …”):

```cpp
MATCHER(IsAce, "")
{
    return arg.rank == RANK_ACE;
}

MATCHER_P(HasRank, expectedRank, "")
{
    return arg.rank == expectedRank;
}

MATCHER_P2(IsCard, rank, suit, "")
{
    *result_listener << "rank=" << arg.rank << " suit=" << arg.suit;   // dodatkowy opis porażki
    return arg.rank == rank && arg.suit == suit;
}

EXPECT_THAT(card, IsAce());
EXPECT_THAT(cards, Each(HasRank(RANK_KING)));
```

- `MATCHER` musi być w zakresie namespace (nie w funkcji ani klasie).
- Opis `""` → generowany z nazwy; można dać własny: `MATCHER_P(HasRank, r, "has rank " + PrintToString(r))`.
- Łączenie istniejących zamiast pisania nowego:

```cpp
auto IsKingOfHearts()
{
    return AllOf(Field(&Card::rank, Eq(RANK_KING)), Field(&Card::suit, Eq(SUIT_HEARTS)));
}
```

Pełna klasa (gdy potrzebny polimorfizm / lepsze opisy):

```cpp
class HasRankMatcher
{
public:
    using is_gtest_matcher = void;

    explicit HasRankMatcher(Rank rank) : rank(rank) {}

    bool MatchAndExplain(const Card& card, std::ostream* os) const
    {
        if (os) *os << "rank is " << card.rank;
        return card.rank == rank;
    }
    void DescribeTo(std::ostream* os) const { *os << "has rank " << rank; }
    void DescribeNegationTo(std::ostream* os) const { *os << "does not have rank " << rank; }

private:
    Rank rank;
};

Matcher<const Card&> HasRankClass(Rank rank) { return HasRankMatcher(rank); }
```

## Ładne wypisywanie typów C w komunikatach

```cpp
void PrintTo(const Card& card, std::ostream* os)   // w tym samym namespace co typ (dla C - globalny)
{
    *os << "Card{" << card.rank << ", " << card.suit << "}";
}
```

## Inne

**Wyłączanie testu:** prefiks `DISABLED_` w nazwie testu lub suite.

**Śledzenie kontekstu:** `SCOPED_TRACE("opis");` - dopisuje opis do porażek w zakresie.

**Asercje w funkcji pomocniczej z `ASSERT_*`:** funkcja musi zwracać `void`; po wywołaniu
`if (HasFatalFailure()) return;` (lub `ASSERT_NO_FATAL_FAILURE(helper());`).

**Typowane testy:**

```cpp
template <typename T> class Container : public Test {};
using Types = ::testing::Types<int, long>;
TYPED_TEST_SUITE(Container, Types);
TYPED_TEST(Container, SizeIsZeroWhenCreated) { TypeParam value{}; /* ... */ }
```

**main** - zwykle linkujemy `GTest::gmock_main` / `GTest::gtest_main`, albo:

```cpp
int main(int argc, char** argv)
{
    InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
```

**CMake:**

```cmake
find_package(GTest REQUIRED)
add_executable(DeckTests DeckTests.cpp)
target_link_libraries(DeckTests PRIVATE Blackjack::Cards GTest::gmock_main)
include(GoogleTest)
gtest_discover_tests(DeckTests)
```

**Flagi uruchomienia:**

| Flaga                              | Opis                                  |
|------------------------------------|---------------------------------------|
| `--gtest_filter=Suite.*:-*Slow*`   | wybór testów (`*`, `?`, `-` wyklucza) |
| `--gtest_list_tests`               | lista testów                          |
| `--gtest_repeat=N`                 | powtórz N razy                        |
| `--gtest_shuffle`                  | losowa kolejność                      |
| `--gtest_break_on_failure`         | zatrzymaj debugger na porażce         |
| `--gtest_also_run_disabled_tests`  | uruchom też `DISABLED_`               |
| `--gtest_brief=1`                  | wypisuj tylko porażki                 |
| `--gtest_output=xml:wynik.xml`     | raport XML (JUnit)                    |
| `--gmock_verbose=info`             | wszystkie wywołania mocków            |

`ctest --output-on-failure -R Deck` - uruchomienie przez CTest.
