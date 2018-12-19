#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <cstring>
#include <Windows.h>

#include "Account.h"
#include "DBmanager.h"

FILE* dbPointer;

Account listOfAccounts[100];

int initDB()
{
    dbPointer = fopen("database.txt", "r+");
    if(dbPointer==NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int addAccount(Account accountToAdd)
{
    fseek(dbPointer,0, SEEK_END);
    fprintf(dbPointer, "%s %s %i %c %s %lu %i \n", accountToAdd.name, accountToAdd.surname, accountToAdd.age, accountToAdd.gender, accountToAdd.login, accountToAdd.passwordHash, accountToAdd.balance);
    fseek(dbPointer,0, SEEK_SET);
}

Account * findAccount(char* login, unsigned long inPasswordHash)
{
    Account * buffer = (Account*)malloc(sizeof(Account));

    //TODO: get rid of those malloc for something more dynamic
    (*buffer).name = (char*)malloc(sizeof(char)*30);
    (*buffer).surname = (char*)malloc(sizeof(char)*30);

    fseek(dbPointer,0 , SEEK_SET);

    do
    {
        //Remember that scanf and fscanf require a pointer to function, buffer->name means (*buffer).name
        fscanf(dbPointer, "%s %s %i %c %s %lu %i \n", buffer->name, buffer->surname, &(buffer->age), &(buffer->gender), &(buffer->login), &(buffer->passwordHash), &(buffer->balance) );
        //printf("%s %s %i %c %s %lu %i \n", buffer->name, buffer->surname, buffer->age, buffer->gender, buffer->login, buffer->passwordHash, buffer->balance);
        if(!strcmp(login, buffer->login))
        {
            if(buffer->passwordHash==inPasswordHash)
            {
                fseek(dbPointer,0 , SEEK_SET);
                return buffer;
            }
        }



        //getch();
    }while(!feof(dbPointer));
    fseek(dbPointer,0 , SEEK_SET);
}

int updateAccount(Account * newData)
{
    Account * buffer = (Account*)malloc(sizeof(Account));

    //TODO: get rid of those malloc for something more dynamic
    (*buffer).name = (char*)malloc(sizeof(char)*30);
    (*buffer).surname = (char*)malloc(sizeof(char)*30);

    fseek(dbPointer,0 , SEEK_SET);
    long int targetStreamPos=0;

    do
    {
        //Remember that scanf and fscanf require a pointer to function, buffer->name means (*buffer).name
        targetStreamPos=ftell(dbPointer);
        fscanf(dbPointer, "%s %s %i %c %s %lu %i \n", buffer->name, buffer->surname, &(buffer->age), &(buffer->gender), &(buffer->login), &(buffer->passwordHash), &(buffer->balance) );
        if(!strcmp(newData->login, buffer->login))
        {
            fseek(dbPointer,targetStreamPos,SEEK_SET);
            fprintf(dbPointer, "%s %s %i %c %s %lu %i \n", newData->name, newData->surname, newData->age, newData->gender, newData->login, newData->passwordHash, newData->balance);
            break;
        }

        //getch();
    }while(!feof(dbPointer));
    fseek(dbPointer,0 , SEEK_SET);
    return 1;
}

unsigned char * toUnsignedChar(char * inputString)
{
    //The string characters + null termination
    unsigned char * outputString = (unsigned char *) malloc(sizeof(unsigned char)*(strlen(inputString)+1));

    for(int i=0;i<strlen(inputString)+1;i++)
    {
        *(outputString+i)=(unsigned char)(*(inputString+i));
    }
    return outputString;
}

unsigned long hashString(unsigned char *str)
{
    unsigned long temp = 5381;
    int c;

    while (c = *str++)
        temp = ((temp << 5) + temp) + c; /* hash * 33 + c */

    return temp;
}
