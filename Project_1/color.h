#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

void UPCOLOR () {
  printf("\033[0;32m");
}

void DOWNCOLOR() {
  printf("\033[0;33m");
}

void LEFTCOLOR() {
  printf("\033[0;34m");
}

void RIGHTCOLOR() {
  printf("\033[0;36m");
}

void RESETCOLOR () {
  printf("\033[0m");
}

void colorHelp() {
  printf("\nColor direction\n");
  UPCOLOR();
  printf("UP direction\n");
  DOWNCOLOR();
  printf("DOWN direction\n");
  LEFTCOLOR();
  printf("LEFT direction\n");
  RIGHTCOLOR();
  printf("RIGHT direction\n");
  RESETCOLOR();
}

#endif