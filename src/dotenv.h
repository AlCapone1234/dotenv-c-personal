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

    dotenv_dict* dotenv_get(const char* path, const char* key);
    void dotenv_dict_cleanup(dotenv_dict* dict);

    #ifdef __cplusplus
        }
    #endif
#endif


#ifdef DOTENV_IMPLEMENTATION
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    dotenv_dict* dotenv_get(const char* path, const char* key)
    {
        char line[DOTENV__MAX_LINE_LENGTH];
        FILE* file;
        dotenv_dict* dict;
        unsigned char foundValue;

        foundValue = 0;
        file = fopen(path, "r");
        dict = (dotenv_dict*)malloc(sizeof(dotenv_dict));

        if (file == NULL)
        {
            return NULL;
        }

        if (dict == NULL)
        {
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

            sscanf(line, DOTENV__TEMPLATE, _key, value);

            if (strcmp(_key, key) != 0)
            {
                continue;
            }

            foundValue = 1;
            strcpy(dict->key, _key);
            strcpy(dict->value, value);
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