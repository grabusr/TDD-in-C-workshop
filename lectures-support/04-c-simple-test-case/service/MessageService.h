#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MessageService
{
    void (*send)(void* context, const char* message);
    void* context;
} MessageService;

#ifdef __cplusplus
}
#endif
