
#define DOTENV_IMPLEMENTATION
#include "../src/dotenv.h"

#include <assert.h>
#include "string.h"
#include <stdio.h>

#define ENV_LOCATION "tests/.env"

int main(void)
{
    dotenv_dict* test1 = dotenv_get_value_from_key(ENV_LOCATION, "APPLICATION_KEY");
    assert(strcmp(test1->value, "2138921jmd89128dkoasdipasd091289038921") == 0);
    dotenv_dict_cleanup(test1);

    dotenv_dict* test2 = dotenv_get_value_from_key(ENV_LOCATION, "#");
    assert(test2 == NULL);
    dotenv_dict_cleanup(test2);

    dotenv_dict* test3 = dotenv_get_value_from_key(ENV_LOCATION, "GURT_THING");
    assert(strcmp(test3->key, "GURT_THING") == 0);
    dotenv_dict_cleanup(test3);

    printf("Tests have passed!\n");
    return 0;
}