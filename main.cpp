#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <cstring>
#include <Windows.h>

#include "Account.h"
#include "DBmanager.h"

using namespace std;

enum charCheck
{
    number=0,
    letter=1,
    num_letter=2,
    special=3,
    password=4
};

void moveTo(int x=0, int y=0);

void clearScreen();

void createNewAccount();

void logIn();

char* readString(charCheck option, int max_length=-1);

int main()
{

    if(!initDB())
    {
        printf("database not found!\n");
        exit(-1);
    }

    while(true)
    {

        clearScreen();

        printf("Welcome to the bank! \n");
        printf("1-create new account \n");
        printf("2-check if exists \n");
        printf("q-exit \n");

        char c;
        c=getch();

        clearScreen();

        switch(c)
        {
            case '1':
                createNewAccount();
            break;

            case '2':
                logIn();
            break;

            case 'q':
               exit(0);
            break;
        }

    }

    return 0;
}

void moveTo(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    _COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hConsole, pos);
}

void clearScreen()
{
    for(int i=0; i<30; i++)
    {
        printf("\n");
    }

}

int checkChar(char c, charCheck option)
{
    switch(option)
    {
        case number:
            return ((48<=c)&&(c<=57)) ? 1 : 0;
        break;
        case letter:
            return (((97<=c)&&(c<=122)||(65<=c)&&(c<=90)) ? 1 : 0);
        break;
        case num_letter:
            return (checkChar(c, number)||checkChar(c, letter) ? 1 : 0);
        break;
        case special:
            return (checkChar(c, num_letter)||((33<=c)&&(c<=47)) ? 1 : 0);
        break;
        case password:
            return checkChar(c, special);
        break;
    }
}

char* readString(charCheck option, int max_length)
{
    int current_char=0;
    char *char_ptr;

    do{
    char_ptr = (char*)calloc(1,sizeof(char));
    }while(char_ptr==NULL);

    while(true)
    {

        char c = getch();

        if(c==13)
        {
            *(char_ptr+current_char)=0;
            //printf("\n\n");
/*
            for(int i=0;i<current_char+1;i++)
            {
                printf("%i \n", *(char_ptr+i));
            }*/
            return char_ptr;
        }
        if(checkChar(c, option))
        {
            if((!(current_char<max_length))&&(max_length>0))
            {
                continue;
            }

            if(option!=password)
            {
                printf("%c",c);
            }
            else
            {
                printf("*");
            }

            *(char_ptr+current_char)=c;
            current_char++;

            do
            {
                char_ptr = (char*)realloc(char_ptr, sizeof(char)*(current_char+1));
            }while(char_ptr==NULL);


        }
        if(c==8)
        {
            if(current_char==0)
            {
                continue;
            }
            printf("%c",8);
            printf(" ");
            printf("%c",8);
            current_char--;
            if(current_char==0)
            {
                free(char_ptr);

                do{
                char_ptr = (char*)calloc(1,sizeof(char));
                }while(char_ptr==NULL);

            }
            else
            {
                char * next_pointer;

                do{
                next_pointer = (char*)calloc((current_char+1),sizeof(char));
                }while(next_pointer==NULL);

                memcpy(next_pointer, char_ptr, (current_char)*sizeof(char));

                free(char_ptr);
                char_ptr=next_pointer;
                next_pointer=NULL;
            }

        }

    }

}

void createNewAccount()
{
    printf("New account \n\n");

    Account * accountNew = (Account*)malloc(sizeof(Account));

    accountNew->name = (char*)malloc(sizeof(char*));
    accountNew->surname = (char*)malloc(sizeof(char*));

    printf("Please provide your name: ");
    accountNew->name=readString(letter);
    printf("\n");
    printf("Please provide your surname: ");
    (*accountNew).surname=readString(letter);
    printf("\n");
    printf("Please provide your age: ");
    accountNew->age=atoi(readString(number));
    printf("\n");
    printf("Please provide your gender (m/f): ");
    accountNew->gender=*(readString(letter,1));
    printf("\n\n");
    printf("Please provide your login: ");
    memcpy((*accountNew).login,readString(num_letter,20),sizeof(char)*21);
    printf("\n");
    printf("Please provide your password: ");
    accountNew->passwordHash=hashString(toUnsignedChar(readString(password)));

    printf("\n\n");
    printf("Name: %s \n", (*accountNew).name);
    printf("Account details: \n");
    printf("Surname: %s \n", (*accountNew).surname);
    printf("Age: %i \n", (*accountNew).age);
    printf("Gender: %c \n", (*accountNew).gender);
    printf("Login: %s \n", (*accountNew).login);
    printf("Password: %lu \n", (*accountNew).passwordHash);

    addAccount(*accountNew);
    getch();
    }


void logIn()
{
        Account * foundAccount;
        char * login;
        unsigned long password;

        printf("Please type in your login: ");
        login = readString(num_letter, 20);
        printf("\n\n");

        printf("Please type in your password: ");
        password = hashString(toUnsignedChar(readString(special)));
        printf("\n\n");

        foundAccount = findAccount(login, password);

        if(foundAccount!=NULL)
        {
            int logOut=0;
            while(!logOut)
            {
                clearScreen();
                 printf("Account details:\n"
                       "Name: %s\n"
                       "Surname: %s\n\n"
                       , foundAccount->name, foundAccount->surname);
                printf("Account managment:\n"
                       "1-Show balance\n"
                       "2-Take a loan\n"
                       "3-Take out money\n"
                       "q-Log out\n\n");
                char c = getch();
                switch(c)
                {
                    case '1':
                        printf("Current balance: %i\n"
                               "Press any key to continue...", foundAccount->balance);
                        getch();
                    break;

                    case '2':
                        printf("Specify loan size: \n");
                            foundAccount->balance += atoi(readString(number));
                        printf("\n");
                        printf("Success!");
                        getch();
                    break;

                    case '3':
                        {
                            printf("Specify amount: \n");
                            int amount = atoi(readString(number));
                            printf("\n");
                            if(foundAccount->balance-amount<0)
                            {
                                printf("Amount too big!");
                                getch();
                            }
                            else
                            {
                                foundAccount->balance-=amount;
                                printf("Success!");
                                getch();
                            }
                        }
                    break;

                    case 'q':
                        printf("Press enter to continue...");
                        updateAccount(foundAccount);
                        logOut=1;
                        free(foundAccount->name);
                        free(foundAccount->surname);
                        free(foundAccount);
                    break;
                }
            }

        }
        else
        {
            printf("Account couldn't be found!");
        }

        free(login);

        getch();
}
