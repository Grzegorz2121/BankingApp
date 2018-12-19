#ifndef DBMANAGER_H_INCLUDED
#define DBMANAGER_H_INCLUDED

#include "Account.h"

int initDB(void);

int addAccount(Account accountToAdd);

Account * findAccount(char* login, unsigned long inPasswordHash);

int updateAccount(Account * newData);

unsigned char * toUnsignedChar(char * inputString);

unsigned long hashString(unsigned char *str);

#endif // DBMANAGER_H_INCLUDED
