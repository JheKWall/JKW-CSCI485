/**
 * Cosmetic methods header file.
 *
 * @author  J. Kenneth Wallace
 * @version 0.1
 * @since   2024-04-13
 */

#ifndef COSMETIC_H
#define COSMETIC_H

/**
 * Toggles typewriter effect.
 */
void _toggleDelay();

/**
 * Determines if the OS is Windows or Linux and clears the terminal. If not either, does nothing.
 */
void _clearTerminal();

/**
 * Cosmetic function that applies a typewriter effect to all printed text.
 */
void _typewriterPrint(char *str, int new_lines);

/**
 * Moves up and clears a specified number of lines in the terminal.
 */
void _removeLines(int num_lines);

#endif // COSMETIC_H