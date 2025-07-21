/**
 * @file style.h
 * @brief Header file for console styling functions.
 * 
 * @author Oussama Amara
 * @date 2024-07-21
 * @version 1.0
 */
#ifndef STYLE_H
#define STYLE_H

typedef enum {
    THEME_CLASSIC,
    THEME_BLUE,
    THEME_MATRIX,
    THEME_ALERT
} ConsoleTheme;
/**
 * @brief Applies the specified console theme.
 * This function changes the console text and background colors based on the selected theme.
 * @param theme The console theme to apply.
 * This function uses platform-specific APIs to set the console colors.
 * On Windows, it uses the Windows Console API to set text attributes.
 * On other systems, it uses ANSI escape codes to change text colors.
 * This function does not return a value.
 */
void apply_theme(ConsoleTheme theme);
/**
 * @brief Resets the console style to default.
 * This function resets the console text and background colors to their default values.
 * It uses platform-specific APIs to reset the console colors.
 * On Windows, it uses the Windows Console API to set text attributes.
 * On other systems, it uses ANSI escape codes to reset text styles.
 */
void reset_console_style();

#endif
