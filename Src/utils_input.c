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
    int key;  // use int to capture extended key codes

    while (1) {
        // Clear screen
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("Choisissez un thème avec ↑ ↓ puis appuyez sur Entrée:\n\n");
        for (int i = 0; i < count; ++i) {
            if (i == selected)
                printf(" > %s <\n", options[i]);
            else
                printf("   %s\n", options[i]);
        }

        #ifdef _WIN32
            key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72) selected = (selected - 1 + count) % count; // Up
                else if (key == 80) selected = (selected + 1) % count;    // Down
            } else if (key == 13) {
                return selected; // Enter
            }
        #else
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            key = getchar();
            if (key == 27 && getchar() == 91) {
                key = getchar();
                if (key == 'A') selected = (selected - 1 + count) % count; // Up
                else if (key == 'B') selected = (selected + 1) % count;    // Down
            } else if (key == '\n') {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return selected; // Enter
            }

            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        #endif
    }
}
