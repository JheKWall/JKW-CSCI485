/**
 * Cosmetic methods implementation file.
 * Implements a function for typewriter printing, removing lines in the terminal, and clearing the terminal.
 *
 * @author  J. Kenneth Wallace
 * @version 0.1
 * @since   2024-04-13
 */

// Used for clearing the terminal when starting program.
#ifdef _WIN32
    #define OS "Windows"
#elif __linux__
    #define OS "Linux"
#else
    #define OS "Unknown"
#endif

#include "cosmetic.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

/* Global Variables */
int DELAY = 10000;      // Typewriter effect delay.
int text_indent = 2;    // How many spaces to prefix each string.

void _toggleDelay() {
    if (DELAY == 10000) {
        DELAY = 0;
    }
    else {
        DELAY = 10000;
    }
}

void _clearTerminal() {
    if (OS == "Windows") {
        system("cls");
    }
    else if (OS == "Linux") {
        system("clear");
    }
}

void _typewriterPrint(char *str, int new_lines)
{
    char *text_ptr = str;

    for (int i = 0; i < new_lines; i++) {
        printf("\n");
    }

    for (int i = 0; i < text_indent; i++) {
        printf(" ");
    }

    while (*text_ptr != '\0') {
        putchar(*text_ptr); // Print current character.
        fflush(stdout);     // Flush output buffer.
        text_ptr++;         // Increment pointer.
        
        if (OS == "Windows") {
            Sleep(DELAY / 1000);
        }
        else if (OS == "Linux") {
            usleep(DELAY);
        }
    }
}

void _removeLines(int num_lines) {
    for (int i = 0; i < num_lines; i++) {
        printf("\033[F\033[K");
    }
}
