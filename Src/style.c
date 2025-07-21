/**
 * @file utils.c
 * @brief Fonctions utilitaires pour la gestion des entrées utilisateur.
 * 
 * @author Oussama Amara
 * @date 2024-06-28
 * @version 1.0
 */

#include "style.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

void apply_theme(ConsoleTheme theme) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (theme) {
        case THEME_CLASSIC:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white text, black bg
            break;
        case THEME_BLUE:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
            break;
        case THEME_MATRIX:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | BACKGROUND_BLUE);
            break;
        case THEME_ALERT:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_INTENSITY);
            break;
    }
#else
    switch (theme) {
        case THEME_CLASSIC:
            printf("\033[0m"); // reset
            break;
        case THEME_BLUE:
            printf("\033[37;44m"); // white text, blue background
            break;
        case THEME_MATRIX:
            printf("\033[32;40m"); // green text, black background
            break;
        case THEME_ALERT:
            printf("\033[31;43m"); // red text, yellow background
            break;
    }
#endif
}

void reset_console_style() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    printf("\033[0m");
#endif
}



 /**  legacy code for setting console color on Windows 
void setConsoleColorBlue() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
    printf("\033[0;34m");
#endif
}

void setConsoleBlueOnGray() {

Constant	           Color
BACKGROUND_BLUE	       Blue
BACKGROUND_GREEN	   Green
BACKGROUND_RED	        Red
BACKGROUND_INTENSITY	Brightens background
You can mix them like BACKGROUND_BLUE | BACKGROUND_INTENSITY.

#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // BACKGROUND_GRAY with FOREGROUND_BLUE for example
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_WHITE);
#else
    printf("\033[44m"); // Background blue
    printf("\033[37m"); // Text white
   // printf("\033[0m"); // Resets colors
#endif
}*/