#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_logger.h"

int first = 1;
int valid_card(char *input)
{
    if(
    !strcmp(input, "strike") ||
    !strcmp(input, "defend") ||
    !strcmp(input, "defensiveAction") ||
    !strcmp(input, "hunker") ||
    !strcmp(input, "skies") ||
    !strcmp(input, "slipstream") ||
    !strcmp(input, "airstrike") ||
    !strcmp(input, "carpet") ||
    !strcmp(input, "unleash") ||
    !strcmp(input, "skyfall")
    )
    return 1;

    return 0;
}

void add_card(FILE *custom, char *input)
{
    if(first)
    {
        first = 0;

        fprintf(custom, "\n\t\t\"%s\"",input);
    }
    else
    {
        fprintf(custom, ",\n\t\t\"%s\"",input);
    }
}

void custom_deck_write()
{
    FILE *custom;
    custom = fopen("config/deck.json","w");

    int quantity;
    int i;
    char *input = (char *)malloc(20);
    int done = 0;
    fprintf(custom,"{\n\t\"deck\":\n\t[");
        

    while(!done)
    {
        printf("Enter name of the card to add to your deck, or \"done\" if finished\n");
        scanf("%s",input);
        slog(input);
        if(strcmp(input,"done")==0)
        {
            done = 1;
        }
        else if(valid_card(input))
        {
            printf("How many?\n");
            scanf("%i", &quantity);
            slog("quanity: %i", quantity);

            if(quantity <= 0)
            quantity = 1;

            for(i = 0; i < quantity; i++)
            {
                add_card(custom,input);
            }
        }
        else
        {
            printf("Invalid card name try again");
        }
    }
    fprintf(custom,"\n\t]\n}");
    fclose(custom);
}
