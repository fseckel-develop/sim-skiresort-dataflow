#include <skiresort/input.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


static struct termios original_terminal;
static int terminal_saved = 0;


/*  IMPORTANT NOTE:
 *  The ANSI C (C90) standard does not provide support for asynchronous
 *  keyboard input or non-blocking terminal input. By default, terminal
 *  input is line-buffered, meaning it is processed only after pressing Enter.
 *  To address this limitation, platform-specific and non-standard libraries
 *  are required. On macOS, where this program was developed, the <termios.h>
 *  library is used in combination with <unistd.h> and <fcntl.h> to configure
 *  the terminal in non-canonical mode. This allows non-blocking key input
 *  to simulate real-time interaction in the Ski Resort simulation.
 */


/*  enable_raw_terminal_mode():
 *  Modifies the terminal settings to enable non-canonical mode (raw mode) and
 *  allowing input to be read character-by-character rather than line-by-line.
 *  Echoing is disabled to prevent characters from being displayed when typed.
 *      params: none
 *      return: none
 */
void enable_non_blocking_terminal_mode(void) {
    struct termios terminal;
    /* Getting current terminal attributes */
    tcgetattr(STDIN_FILENO, &original_terminal);
    terminal_saved = 1;
    terminal = original_terminal;
    /* Disabling canonical mode (buffered input) and echoing */
    terminal.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
    /* Applying modified terminal attributes */
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}


/*  key_is_pressed():
 *  Checks without blocking program execution if a key has been pressed on the keyboard.
 *  First the original terminal flags are retrieved, then non-blocking mode is enabled.
 *  The input is read for checking before blocking mode is restored via the original flags.
 *      params: none
 *      return: 1 (TRUE) or 0 (FALSE)
 */
int key_is_pressed(void) {
    /* Getting current file status flags of stdin */
    const int initial_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    /* Setting stdin to non-blocking mode */
    fcntl(STDIN_FILENO, F_SETFL, initial_flags | O_NONBLOCK);
    /* Attempting to read a character from stdin */
    const int pressed_key = getchar();
    /* Restoring original stdin file status flags */
    fcntl(STDIN_FILENO, F_SETFL, initial_flags);
    /* Has a key been read? */
    if (pressed_key != EOF) {
        /* Putting the character back into stdin for future reads */
        ungetc(pressed_key, stdin);
        return 1;
    }
    /* No key was pressed */
    return 0;
}


/*  disable_raw_terminal_mode():
 *  Disables non-canonical mode of the terminal and activates echoing again.
 *      params: none
 *      return: none
 */
void disable_non_blocking_terminal_mode(void) {
    /* Have terminal attributes been overwritten? */
    if (terminal_saved) {
        /* Applying original terminal attributes */
        tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal);
    }
}
