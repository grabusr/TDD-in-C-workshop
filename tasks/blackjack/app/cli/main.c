#include <App.h>
#include <ConsoleLogger.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint64_t randomSeed(void)
{
    uint64_t seed = 0;
    FILE* const source = fopen("/dev/urandom", "rb");
    if (source != NULL)
    {
        const size_t read = fread(&seed, sizeof(seed), 1, source);
        fclose(source);
        if (read == 1)
        {
            return seed;
        }
    }
    return (uint64_t)time(NULL) ^ ((uint64_t)clock() << 32);
}

int main(const int argc, const char* const* const argv)
{
    AppConfig config;
    ConsoleLogger console;
    consoleLoggerInit(&console, stdout);
    if (!appConfigParse(argc, argv, &config))
    {
        ConsoleLogger errors;
        consoleLoggerInit(&errors, stderr);
        appPrintUsage(argv[0], &errors.logger);
        return EXIT_FAILURE;
    }
    if (config.help)
    {
        appPrintUsage(argv[0], &console.logger);
        return EXIT_SUCCESS;
    }
    return appRun(&config, randomSeed(), stdin, &console.logger);
}
