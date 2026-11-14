# Testing convention

## TDD Workflow

- Always write failing tests BEFORE implementation
- Use AAA pattern: Arrange-Act-Assert
- One assertion per test when possible
- Test names describe behavior: "should_return_empty_when_no_items"

## Test-First Rules

- When I ask for a feature, write tests first
- Tests should FAIL initially (no implementation exists)
- Only after tests are written, implement minimal code to pass

## Coding rules

### Test implementation rules

- Google Mock syntax sometimes test can contain many sections with same names. Sometimes `// expected` can be before and
  after `// when`
- During mocking when something is not required prefer using ON_CALL() over EXPECT_CALL and prefer using
  `testing::NiceMock`
- Common data preparation make only in fixture `SetUp()` method, avoid making any other common functions, make them only
  if it is really necessary
- Test code is not production code, do not put `if` conditions, avoid making loops
- Prefer using `ASSERT_THAT()` and `EXPECT_THAT()` with proper named matchers over other EXPECT_EQ
- Wrap fixtures into anonymous namespace

### Google Test naming convention

    TEST(Context, SubjectPredicateExpectedResult[WhenCondition])
    TEST_F(Fixture, SubjectPredicateExpectedResult[WhenCondition])

- **Suite name (first argument)**: the context, meaning the module under test or a fixture describing a specific
  scenario/setup, e.g. `RingBuffer`, `FullFlashMemory`, `FormattedFlashMemory`.
- **Test name (second argument)** reads as a sentence:
    - **Subject**: the function or unit under test, e.g. `Write`, `Get`, `Crc16`.
    - **Predicate**: a verb in the third person singular, e.g. `Returns`, `Rejects`, `Stores`, `Is`.
    - **Expected result**: what it produces, e.g. `OldestElement`, `NoSpaceError`.
    - **Condition** (optional): starts with `When`, e.g. `WhenFull`.

Use PascalCase and never underscores (GoogleTest reserves them). If a name needs "And", split it into two tests.

Good:

- `TEST(RingBuffer, GetReturnsOldestElementFirst)`
- `TEST(RingBuffer, PutRejectsElementWhenFull)`
- `TEST_F(FullFlashMemory, WriteReturnsNoSpaceError)`
- `TEST_F(FormattedFlashMemory, ReadReturnsErasedValue)`

Bad:

- `TEST(RingBuffer, Test1)`: no subject, predicate or result
- `TEST_F(FullFlashMemory, ReturnsError)`: missing subject
- `TEST(RingBuffer, Put_Works)`: underscore, "works" is not a result