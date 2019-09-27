//
// Created by Jákup Vuljam Dam on 24/09/2019.
//

/*
 En simpel cshell der kan udføre nogle system calls og nogle "builtin"
 funktioner.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


//Metoden der læser kommandoen og splitter den op i tokens

#define DELIM " \t\n\r\a"
char **readCommand(){

    // Variabler for memory og linjen bliver specificeret

    char *linje = NULL;
    ssize_t buffer = 0;

    // getline er metoden der får indput fra brugeren

    getline(&linje, &buffer, stdin);


    // Mere variabler for tokens/token og en specificeret størrelse af tokens i memory

    // Der er ingen håndtering af hvis linjen er større end hvad der er blevet specificeret.
    // Dette ville man gøre ved brug af realloc senere, men jeg har valgt at bare lave et eimpelt program

    int buffersize = 200;
    char *token, **tokens = malloc(buffersize * sizeof(char*));


    // strtok splitter linjen op ved brug af DELIM, som er alle whitespaces, newline osv. Det er så en token.

    token = strtok(linje, DELIM);
    int i = 0;

    // while loopen itererer så igennem linjen som er splittet og indsætter hver token i et index i arrayet
    // hos linjen. Dvs. array[0] vil altid være kommandoen, mens array[1] osv. vil være argumenter/parameter

    while (token != NULL){
        //printf("%s\n", token);
        tokens[i] = token;
        i++;
        token = strtok(NULL, DELIM);
    }
    tokens[i] = NULL;

    // Returnerer linjen af tokens, som vil sige at isteded for at linjen havde et array af characters før
    // nu, har den et array af tokens, som er specificeret alt efter hvad DELIM var ovenfor.

    return tokens;
}

#define TRUE 1
int main (void) {
    int status;

    // Uendelig loop, der gør såsom man kan blive ved med at skrive kommandoer, indtil man skriver exit

    while (TRUE) {
        printf("$> ");

        // Henter linjen af tokens

        char **linje = readCommand();

        // Et "builtin" system call til at lukke for programmet og processen

        if (strcmp(linje[0],"exit") == 0){
            exit(EXIT_SUCCESS);
        }

        // Builtin system call til at skifte directory

        if (strcmp(linje[0],"cd") == 0) {
            chdir(linje[1]);
            exit(EXIT_SUCCESS);
        }

        // Om der er en child process, venter programmet indtil den process er færdig

        if (fork() != 0) {
            waitpid(-1, &status, 0);
        }

        // Et system call der eksekverer et program alt efter hvad for en kommandoe der bliver indtastet
        // De kommandoer er nogle programmer der ligger i /bin mappen i et UNIX system

        else {
            execvp(linje[0], linje);
        }
    }
    return 0;
}