#ifndef USER_AUTH_H
#define USER_AUTH_H

int authenticate();
void addUser();

extern char loggedInUser[20];
extern int isAdmin;

#endif
