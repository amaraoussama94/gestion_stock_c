/**
 * @file utils_input.c
 * @brief Utility functions for handling user input in a console application.   
 * 
 * @author Oussama Amara
 * @date 2024-07-21
 * @version 1.0
 */

#include <stdio.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "utils_input.h"
/**
 * @brief Allows the user to select an option using arrow keys.
 * This function displays a list of options and allows the user to navigate through them using the up and down arrow keys.
 * The user can select an option by pressing the Enter key.
 * @param options An array of strings representing the options to choose from.
 * @param count The number of options in the array.
 * @return The index of the selected option. If the user exits without selecting, it returns -1.
 */
int get_arrow_selection(const char* options[], int count) {
    int selected = 0;
    char c;
    while (1) {
        // Clear screen and redraw
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("Choisissez un thème avec ↑ ↓ puis appuyez sur Entrée:\n\n");
        for (int i = 0; i < count; ++i) {
            if (i == selected) {
                printf(" > %s <\n", options[i]);
            } else {
                printf("   %s\n", options[i]);
            }
        }

        #ifdef _WIN32
            c = _getch();
            if (c == 0 || c == 224) {
                c = _getch();
                if (c == 72) selected = (selected - 1 + count) % count; // up
                else if (c == 80) selected = (selected + 1) % count;    // down
            } else if (c == 13) return selected; // enter
        #else
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            c = getchar();
            if (c == 27 && getchar() == 91) {
                c = getchar();
                if (c == 'A') selected = (selected - 1 + count) % count; // up
                else if (c == 'B') selected = (selected + 1) % count;    // down
            } else if (c == '\n') {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return selected;
            }

            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        #endif
    }
}
