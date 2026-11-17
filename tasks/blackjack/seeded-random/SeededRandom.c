#include "SeededRandom.h"

static uint64_t nextRaw(SeededRandom* const self)
{
    self->state += 0x9E3779B97F4A7C15ull;
    uint64_t z = self->state;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
    return z ^ (z >> 31);
}

static size_t next(void* const context, const size_t min, const size_t max)
{
    SeededRandom* const self = context;
    const uint64_t span = (uint64_t)max - (uint64_t)min + 1u;
    if (span == 0u)
    {
        return (size_t)nextRaw(self);
    }

    /// Values below `rejected` would make the lower part of the range more probable (modulo bias), so they are drawn again.
    const uint64_t rejected = (0u - span) % span;
    uint64_t value = nextRaw(self);
    while (value < rejected)
    {
        value = nextRaw(self);
    }
    return (size_t)(min + value % span);
}

void seededRandomInit(SeededRandom* const self, const uint64_t seed)
{
    self->state = seed;
    self->generator.next = &next;
    self->generator.context = self;
}
