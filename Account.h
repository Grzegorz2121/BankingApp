#ifndef ACCOUNT_H_INCLUDED
#define ACCOUNT_H_INCLUDED

struct Account
{
  char login[21];
  unsigned long passwordHash;

  char* name;
  char* surname;
  char gender; //because it dosent want to allocate singluar chars
  int age;

  int balance=0;
};

#endif // ACCOUNT_H_INCLUDED
