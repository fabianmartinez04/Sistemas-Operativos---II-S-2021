// Spy program
// run command: gcc -o spy spy.c


// libs

#include <stdlib.h>
#include <stdio.h>


// functions definition
int showMenu();

// show the menu options
int showMenu() {
    int option;
    printf("--------------- MENU ------------\n");
    printf("1. Memory status\n");
    printf("2. Threads status\n");
    printf("3. Exit\n");
    printf("Option: ");
    scanf("%d", &option);
    return option;
}

int main() {

    // variables
    int option;

    do {
        option = showMenu();
    }while (option != 3);
    
    return 0;
}


