#ifndef DOTENV_H
#define DOTENV_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DOTENV__MAX_LINE_LENGTH
#define DOTENV__MAX_LINE_LENGTH 256
#endif

#ifndef DOTENV_MAX_VARIABLE_LENGTH
#define DOTENV_MAX_VARIABLE_LENGTH 128
#endif

#define DOTENV__TEMPLATE "%[^=]=%[^\n]"

    typedef struct
    {
        char key[DOTENV_MAX_VARIABLE_LENGTH];
        char value[DOTENV_MAX_VARIABLE_LENGTH];
    }dotenv_dict;

    dotenv_dict* dotenv_get_value_from_key(const char* path, const char* key);
    void dotenv_dict_cleanup(dotenv_dict* dict);

#ifdef __cplusplus
}
#endif
#endif


#ifdef DOTENV_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void dotenv__sscanf(const char* line, char* key, char* value)
{
#ifndef _WIN32
    sscanf(line, DOTENV__TEMPLATE, key, value);
#else
    sscanf_s(line, DOTENV__TEMPLATE, key, DOTENV_MAX_VARIABLE_LENGTH, value, DOTENV_MAX_VARIABLE_LENGTH);
#endif
}

void dotenv__strcpy(char* destination, char* source)
{
#ifndef _WIN32
    strcpy(destination, source);
#else
    strcpy_s(destination, DOTENV_MAX_VARIABLE_LENGTH, source);
#endif
}

dotenv_dict* dotenv_get_value_from_key(const char* path, const char* key)
{
    char line[DOTENV__MAX_LINE_LENGTH];
    FILE* file;
    dotenv_dict* dict;
    unsigned char foundValue;

    foundValue = 0;

#ifdef _WIN32
    errno_t file_errno = fopen_s(&file, path, "r");

    if (file_errno != 0)
    {
        printf("[dotenv] Could not open file!\n");
        return NULL;
    }
#else
    file = fopen(path, "r");
#endif
    dict = (dotenv_dict*)malloc(sizeof(dotenv_dict));

    if (file == NULL)
    {
        if (dict)
        {
            free(dict);
            dict = NULL;
        }

        return NULL;
    }

    if (dict == NULL)
    {
        fclose(file);
        return NULL;
    }


    while (fgets(line, DOTENV__MAX_LINE_LENGTH, file))
    {

        char _key[DOTENV_MAX_VARIABLE_LENGTH];
        char value[DOTENV_MAX_VARIABLE_LENGTH];

        /* Comment found */
        if (line[0] == '#')
        {
            continue;
        }

#ifdef DOTENV__DEBUG
        printf("%s", line);
#endif

        dotenv__sscanf(line, _key, value);


#ifdef DOTENV__DEBUG
        printf("[%s] [%s]", _key, key);
#endif
        if (strcmp(_key, key) != 0)
        {
            continue;
        }

        foundValue = 1;
        dotenv__strcpy(dict->key, _key);
        dotenv__strcpy(dict->value, value);
    }

    if (foundValue == 0)
    {
        free(dict);
        dict = NULL;
        fclose(file);
        file = NULL;
        return NULL;
    }

#ifdef DOTENV__DEBUG
    printf("\n");
#endif

    fclose(file);
    return dict;
}

void dotenv_dict_cleanup(dotenv_dict* dict)
{
    if (dict == NULL)
    {
        return;
    }

    free(dict);
    dict = NULL;
}
#endif