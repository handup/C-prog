#define _XOPEN_SOURCE
#include <unistd.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
//A program that decrypts UNIX passwords

//Example of encrypted passwords: 50q/EEJnOmtxc, 50yoN9fp966dU, HAYRs6vZAb4wo

int main(int n, string argv[])
{
    if(n!=2)
    {
        printf("Wrong format!\n");
        return -1;
    }
    string crack = (char *) malloc(100);
    strcpy(crack, argv[1]);
    
    
    string salt = (char *) malloc(100);
    strncat(salt, crack, 2);
    
    int ok = 0;
    FILE *fp;
    char *password = (char *) malloc(100);
    fp = fopen("/usr/share/dict/words", "r");
    int num = 0;
    
    if (fp == NULL) 
    {
        fprintf(stderr, "Can't open input file in.list!\n");
        exit(1);
    }
    while(ok==0 && fscanf(fp, "%s", password)==1)
    {
    num++;
    
    if(strcmp(crypt(password, salt), crack)==0)
    {
    printf("%s\n", password);
    ok=1;
    return 1;
    }
    }
    printf("Not found!\n");
    return 0;
}
