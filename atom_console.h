#ifndef __ATOM_CONSOLE_H_
#define __ATOM_CONSOLE_H_

//--------------------------------- Include ---------------------------------
#include "bsp.h"

//--------------------------------- Control ---------------------------------
#define CONSOLE_BUFFER_SIZE 128     /* BUffer size for UART receiving */
#define CONSOLE_PARAM_MAX   32      /* Param number supported in a command(included)*/
#define CONSOLE_HISTORY_MAX 3       /* History command saver size*/

#define AtomPrint       printf          /* Print Function */
#define AtomPutchar     uart2_putchar   /* UART output single char */
#define AtomGetchar     uart2_getchar   /* Read UART Rx, return 0 indicate successful, output value from input reference param-0 */
#define AtomFlush()     fflush(stdout)  /* For the cases that some device would not flush stdout unless get \r\n */

//--------------------------------- Define ---------------------------------
typedef int (*atom_entry_t)(uint8_t argc, char **argv);

typedef struct atom_console_entry_s
{
    const char      *name;  /* Registered funtion name */
    atom_entry_t    func;   /* Registered funtion API entry */
    const char      *help;  /* Registered funtion help information */
}atom_console_entry_t;

//--------------------------------- Function ---------------------------------
/**
 * @brief Console Entry
 * 
 */
void atom_console();

/**
 * @brief Convert string to value, support hex/dec
 * 
 * @param str 
 * @param value 
 * @return int return 0 indicate success, negative value indicate failure
 */
int atom_strtoi(char *str, int *value);

char *str_strip(char *ptr);

#endif //__ATOM_CONSOLE_H_