//
// Created by Jákup Vuljam Dam on 25/09/2019.
//

cshell er et program der opfører sig som et CLI (Command Line Interface), hvor man kan indtaste kommandoer
og derefter vil shellen lave en operation, dette vliver udført via UNIX system calls.
Denne readme vil give dig et indblik i hvordan en shell fungerer og hvordan man kan udføre de kommandoer.


Nedenfor er der en tutorial over hvordan man kompilerer programmet og kører det.

1# : Kompilerer klassen
        Brug kommandoen "gcc [klassenavn.c] -o [navn på filen der bliver genereret]" 

2# : Kør den genererede fil
        Brug kommandoen "./[navnet på filen]"
            Nu kører shellen og du kan nu indtaste kommandoer

For at bruge de forskellige kommandoer, skal man bara indtaste i terminalen og derefter bliver operationen
udført.
I shellen er der nogle specifikke kommandoer der kan bruges og de har jeg skrevet nedenfor.
    System Calls
        exec, exit, chdir
            exec (/bin/"programmer")
                "ls", "ls -l", "htop", "nano", "vi", "mkdir", "rmdir", "cat" ... etc.
            exit (builtin)
                "exit"
            chdir (builtin)
                "cd"


* What is a System Call?
 Et system call er en måde hvordan et program kan kommunikere med computerens kernel.
 Måden denne shell har brugt system call, er ved at kalde på metoderne fork(), execvp(), exit() & chdir()
    fork() duplikerer den kørende process og laver en ny en, som hedder "child", mens den originale er "parent"
    execvp() eksekverer programmer der ligger i en PATH, som derefter laver en eller anden operation. Som f.eks. cp til at kopiere
    exit() fortæller kernel at programmet skal lukkes og derfor skal denne kørende process termineres.
    chdir() siger til systemet at nu skal der skiftes directory

* What is I / O Redirection?
 I/O redirection er en måde at skifte hvor standard input / output bliver vist
    * Output bliver redirected ved brug af symbolet " > " Dvs. Om man ville skrive "echo hej > text.txt" Ville man istedet for at printe hej på skærmen, så ville det blive sendt ind i filen "text.txt"
    Hvis man bruger " > " igen, vil man slette det som var i filen. Om man vil indsætte flere ting bagefter "hej" bruger man " >> "
    * Input er det omvendte symbol " < " og som eksempel, bliver brugt når man skal bruge en hel fil som input
    Dette kan være om man skal sænde en mail og har skrevet det i en fil et eller andet sted. Så kan man bare bruge kommandoen "mail -s "subject" email@email.com < filnavn"
    * Error plejer at være som standard på skærmen, men man kan bruge redirect til at sende fejlen til en fil istedet.

    Input, Output og Error har sin egen "Filedescriptor" / "FD". Input = FD0, Output = FD1 & Error = FD2

    For at bruge redirect af en fejl, skal man bruge kommandoen "kommando-som-giver-fejl 2> filnavn"
    dette vil så printe alle relevante fejle, som kommandoen udleverer til den specificerede fil.

* Program Environment?



* Background Program Execution?
 Hvis man vil køre et program i baggrunden når man arbejder med terminalen. Man skal først køre programmet
 og derefter lukke for programmet med tastatur shortcuttet "CTRL Z" og derefter skrive bg i terminalen.
 Dette vil så lade programmet køre i baggrunden mens du kan arbejde videre i terminalen og gøre noget andet.



NOTES:
cd kommandoen virker ikke helt som den skal. **Den laver en ny process hver gang den bliver kaldt** og det
er ikke meningen. Dette gør så at når man flytter rundt i systemet med cd, skal man bruge "exit" lige så
ofte, for at komme ud af shellen igen. Dette er et struktur problem i koden hos mig, men som et simpelt
projekt, beviser det bare hvordan kommandoen bliver brugt.





### Bugs:

- Crashes on an empty string
- Doesn't show error message if the entered command doesn't exist
- Pipe does not implemented?  



