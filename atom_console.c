#include "bsp.h"
#include "atom_console.h"
#include "atom_vt100.h"

//--------------------------------- Define ---------------------------------
char console_buf[CONSOLE_BUFFER_SIZE];
char console_history[CONSOLE_HISTORY_MAX][CONSOLE_BUFFER_SIZE];
int console_history_refer_idx;
int console_history_update_idx;
int console_ptr;
int curse_ptr;

int atom_cmd_help(uint8_t argc, char **argv);
int atom_cmd_example(uint8_t argc, char **argv);
int atom_cmd_history(uint8_t argc, char **argv);
//--------------------------------- Regist ---------------------------------
atom_console_entry_t atom_functions[] =
{
    // Command           Entry API               Description
    {"help",            atom_cmd_help,           "Help funtion"},
    {"example",         atom_cmd_example,        "Example,-v return version, -x [number] -s [string]"},
    {"example2",         atom_cmd_example,        "Example,-v return version, -x [number] -s [string]"},
    {"history",         atom_cmd_history,        "List history command"},
};

//--------------------------------- Utils ---------------------------------
/**
 * @brief help funtions
 *
 * @param argc
 * @param argv
 * @return int
 */
int atom_cmd_help(uint8_t argc, char **argv)
{
    int i = 0;

    int len = sizeof(atom_functions) / sizeof(atom_console_entry_t);

    AtomPrint("\r\n----------------- Atom Console Help ----------------\r\n");
    AtomPrint("Supported Commands:\r\n");
    for (i = 0; i < len; i++)
    {
        if (strlen(atom_functions[i].name) <= 7)
            AtomPrint("%s\t\t%s\n\r", atom_functions[i].name, atom_functions[i].help);
        else
            AtomPrint("%s\t%s\n\r", atom_functions[i].name, atom_functions[i].help);
    }
    return 0;
}

/**
 * @brief Example of param parsing
 *
 * @param argc
 * @param argv
 * @return int
 */
int atom_cmd_example(uint8_t argc, char **argv)
{
    int i, len = argc;
    int opt_v = 0, opt_x = 0, opt_s = 0;
    char *str;
    int val;

    for (i = 0; i < len; i++)
    {
        if (0 == strcmp("-v", argv[i]))
        {
            opt_v = 1;
        }

        if (0 == strcmp("-x", argv[i]))
        {
            if ((i + 1) >= len)
            {
                AtomPrint("ERR: Invalid param %s without value\r\n", argv[i]);
            }

            if (0 != atom_strtoi(argv[i + 1], &val))
            {
                AtomPrint("ERR: Invalid param %s value out of range\r\n", argv[i]);
                return -1;
            }
            opt_x = 1;
        }

        if (0 == strcmp("-s", argv[i]))
        {
            if ((i + 1) >= len)
            {
                AtomPrint("ERR: Invalid param %s without value\r\n", argv[i]);
                return -1;
            }

            str = argv[i + 1];
            opt_s = 1;
        }
    }

    if (opt_v)
    {
        AtomPrint("Example Version 12.345\r\n");
        return 0;
    }

    if (opt_x)
    {
        AtomPrint("Value=%d\r\n", val);
    }

    if (opt_s)
    {
        AtomPrint("Path=%s\r\n", str);
    }

    return 0;
}

/**
 * @brief history command
 *
 * @param argc
 * @param argv
 * @return int
 */
int atom_cmd_history(uint8_t argc, char **argv)
{
    int i = 0;
    int refer_idx = console_history_refer_idx;
    for (i = 0; i < CONSOLE_HISTORY_MAX - 1; i++)
    {
        if (0 == refer_idx)
            refer_idx = CONSOLE_HISTORY_MAX - 1;
        else
            refer_idx--;
        
        if(console_history[refer_idx][0])
        {
            AtomPrint("%s\r\n", console_history[refer_idx]);
        }
    }
    return 0;
}
//--------------------------------- Base functions ---------------------------------

/**
 * @brief replacement of the strtoi of stdlib
 * 
 * @param str 
 * @param value 
 * @return int 
 */
int atom_strtoi(char *str, int *value)
{
    int i = 0;
    int val = 0;
    int len = strlen(str);
    int hex = 0;
    char *ptr = str;
    int neg = 0;

    for (i = 0; i < len; i++)
    {
        if ((str[i] < '0') || (str[i] > '9'))
        {
            if ('x' == str[i])
            {
                if (i + 1 >= len)
                    return -1;
                ptr = &str[i + 1];
                hex = 1;
            }
            else if ((str[i] >= 'a') && (str[i] <= 'f'))
            {
                hex = 1;
            }
            else if ((str[i] >= 'A') && (str[i] <= 'F'))
            {
                str[i] += ('a' - 'A');
                hex = 1;
            }
            else if ('-' == str[i])
            {
                if (0 == i)
                {
                    ptr = &str[1];
                    neg = 1;
                }
                else
                    return -1;
            }
            else
                return -1;
        }
    }

    if (neg && hex)
        return -1; // seriously?

    len = strlen(ptr);
    if (hex)
    {
        for (i = 0; i < len; i++)
        {
            val <<= 4;
            int t = (ptr[i] > 0x39) ? (ptr[i] - 'a' + 0xa) : (ptr[i] - 0x30);
            val |= t;
        }
    }
    else
    {
        int t = 1;
        for (i = len - 1; i >= 0; i--)
        {
            val += ((ptr[i] - 0x30) * t);
            t *= 10;
        }
        if (neg)
            val *= -1;
    }

    *value = val;
    return 0;
}

/**
 * @brief remove the space and \t and split buffer
 * 
 * @param ptr 
 * @return char* 
 */
char *str_strip(char *ptr)
{
    int i = 0, j = 0;
    int len = strlen(ptr);
    char *res;

    for (i = 0; i < len; i++)
    {
        if ((ptr[i] != ' ') && (ptr[i] != '\t'))
            break;
    }
    res = &ptr[i];

    for (i = len - 1; i >= 0; i--)
    {
        if ((ptr[i] != 0x0) && (ptr[i] != ' ') && (ptr[i] != '\t'))
        {
            for (j = i + 1; j < len; j++)
                ptr[j] = 0;
            break;
        }
    }

    return res;
}

/**
 * @brief Search command in the 
 * 
 * @param ptr 
 */
void console_find_cmd(char *ptr)
{
    int i;
    int list_len = sizeof(atom_functions) / sizeof(atom_console_entry_t);
    int err = 0;

    char *argv[CONSOLE_PARAM_MAX];
    uint8_t argc = 0;

    ptr = str_strip(ptr);
    int len = strlen(ptr);

    for (i = 0; i < len; i++)
    {
        if (ptr[i] < 32 || ptr[i] > 126)
            ptr[i] = 0;
    }

    int cmd = 0;
    int s = -1;

    for (i = 0; i < CONSOLE_PARAM_MAX; i++)
        argv[i] = NULL;

    for (i = 0; i <= len; i++)
    {
        if (argc >= CONSOLE_PARAM_MAX)
        {
            AtomPrint("AtomConsole: too many args\r\n");
            return;
        }

        if ((' ' == ptr[i]) || (0 == ptr[i]) || ('\t' == ptr[i]))
        {
            if (0 == cmd)
            {
                cmd = 1;
            }
            else
            {
                argv[argc++] = &ptr[s];
            }
            s = -1;
            ptr[i] = 0;
        }
        else
        {
            if (-1 == s)
            {
                s = i;
            }
        }
    }

    for (i = 0; i < list_len; i++)
    {
        if (0 == strcmp(ptr, atom_functions[i].name))
        {
            if (0 == argc)
                err = atom_functions[i].func(0, NULL);
            else
                err = atom_functions[i].func(argc, argv);

            if (err < 0)
            {
                AtomPrint("ERR: Execute command %s error\r\n", ptr);
            }
            return;
        }
    }

    AtomPrint("AtomConsole: command not found: %s: \r\n", ptr);
}

enum AT_BUF_OPS
{
    AT_BUF_OP_BACKSPACE = 0,
    AT_BUF_OP_DEL,
    AT_BUF_OP_ADD,
};

void buffer_ops(char *buf, int *buf_ptr, int *cur_ptr, int cmd_data)
{
    int i;
    int len = *buf_ptr;
    int idx = *cur_ptr;
    uint8_t cmd = cmd_data & 0xff;
    uint8_t data = (cmd_data >> 8);
    
    if(AT_BUF_OP_BACKSPACE == cmd)
    {
        if(idx == 0)
            return;

        for(i = idx; i < len; i++)
        {
            buf[i - 1] = buf[i];
        }
        buf[len - 1] = 0;
        *buf_ptr-=1;
        *cur_ptr-=1;
    }
    else if(AT_BUF_OP_DEL == cmd)
    {
        if(0 == len)
            return;
        
        buf[idx] = 0;
        for(i = idx; i < (len-1); i++)
        {
            buf[i] = buf[i+1];
        }
        buf[len - 1] = 0;
        *buf_ptr -= 1;
    }
    else if(AT_BUF_OP_ADD == cmd)
    {
        if(len >= (CONSOLE_BUFFER_SIZE - 1))
            return; //almost full, need reserve size -1 for 0
        for(i = len ; i > idx; i--)
        {
            buf[i] = buf[i - 1];
        }
        buf[idx] = data;
        *buf_ptr+=1;
        *cur_ptr+=1;
    }
}

void atom_resetline()
{
    AtomPrint("\033[2K\r");
    AtomFlush();
    AtomPrint(">>%s", console_buf);
    AtomFlush();
    if((console_ptr - curse_ptr)!=0)
        AtomPrint("\033[%dD", console_ptr - curse_ptr);
    AtomFlush();
}

void atom_newline(int *get_line, int switch_line)
{
    *get_line = 0;
    memset(console_buf, 0, CONSOLE_BUFFER_SIZE);
    console_ptr = 0;
    curse_ptr = 0;

    if(switch_line)
        AtomPrint("\r\n>>");
    else
        AtomPrint(">>");
    AtomFlush();
}

int str_subcmp(char *sub, const char *src)
{
    int i = 0;

    int len1 = strlen(sub);
    int len2 = strlen(src);

    if(len1 > len2)
        return -1;
    
    for(i = 0; i < len1; i++)
    {
        if(sub[i] != src[i])
            break;
    }
    if(i >= len1)
        return 0;
    
    return -2;
}

int atom_tab()
{
    int i;
    int x;
    int list_len = sizeof(atom_functions) / sizeof(atom_console_entry_t);
    int cnt = 0;
    int only_id = -1;

    for(i = 0; i < list_len; i++)
    {
        x = str_subcmp(console_buf, atom_functions[i].name);
        if(0 == x)
        {
            if(only_id < 0)
            {
                only_id = i;
                cnt = 1;
            }
            else
            {
                if(cnt == 1)
                {
                    AtomPrint("\r\n%s\t\t", atom_functions[only_id].name);
                    AtomFlush();
                }
                AtomPrint("%s\t\t", atom_functions[i].name);
                AtomFlush();
                cnt++;
                if(0 == (cnt % 3))
                    AtomPrint("\r\n");
            }
        }
    }

    if (only_id < 0)
        return 0;

    if(1 == cnt)
    {
        memset(console_buf, 0, console_ptr);
        strcpy(console_buf, atom_functions[only_id].name);
        console_ptr = strlen(atom_functions[only_id].name);
        curse_ptr = console_ptr;

        atom_resetline();
        return 0;
    }


    return 1;
}

void atom_console()
{
    uint8_t readval;
    int get_line = 0;
    int i = 0;

    memset(console_buf, 0, CONSOLE_BUFFER_SIZE);
    console_ptr = 0;
    curse_ptr = 0;
    for (i = 0; i < CONSOLE_HISTORY_MAX; i++)
        memset(console_history[i], 0, CONSOLE_BUFFER_SIZE);
    console_history_refer_idx = 0;
    console_history_update_idx = 0;

    AtomPrint("\r\n\r\n================ Atom Console ================\r\n>>");
    AtomFlush();

    while (1)
    {
        if(AtomGetchar(&readval) != 0)
            continue;
        //AtomPutchar(readval);
        //printf("0x%x\r\n", readval);

        if ('\n' == readval || '\r' == readval)
        {
            AtomPutchar(readval);
            get_line = 1;
        }
        else if (ASCII_BS == readval)   //backspace
        {
            if (curse_ptr != 0)
            {
                AtomPutchar(readval);
                buffer_ops(console_buf, &console_ptr, &curse_ptr, AT_BUF_OP_BACKSPACE);

                if(curse_ptr == console_ptr)
                {
                    AtomPrint("%s", vt100_clear_curse_right);
                    AtomFlush();
                }
                else
                {
                    atom_resetline();
                }
            }
        }
        else if (ASCII_DEL == readval)  //delete
        {
            if(console_ptr != 0)
            {
                AtomPutchar(readval);
                buffer_ops(console_buf, &console_ptr, &curse_ptr, AT_BUF_OP_DEL);

                atom_resetline();

            }
        }
        else if(ASCII_TAB == readval)
        {
            if(console_ptr)
            {
                int switch_line = atom_tab();
                if(switch_line)
                {
                    atom_newline(&get_line, 1);
                }                
            }

        }
        else if(ASCII_ESC == readval)   //esc, and vt100 code
        {
            while (AtomGetchar(&readval));
            if('[' == readval)
            {
                while (AtomGetchar(&readval));
                if (('A' == readval) || ('B' == readval)) //key up - down
                {
                    int refer_idx = console_history_refer_idx;
                    
                    int break_flag = 0;

                    for (i = 0; i < CONSOLE_HISTORY_MAX; i++)
                    {
                        if (console_history[refer_idx][0])
                        {
                            memset(console_buf, 0, console_ptr);
                            strcpy(console_buf, console_history[refer_idx]);
                            console_ptr = strlen(console_buf);
                            curse_ptr = console_ptr;

                            atom_resetline();
                            break_flag = 1;
                            
                        }

                        if ('A' == readval)
                        {
                            if (0 == refer_idx)
                                refer_idx = CONSOLE_HISTORY_MAX - 1;
                            else
                                refer_idx--;
                        }
                        else
                        {
                            refer_idx++;
                            refer_idx %= CONSOLE_HISTORY_MAX;
                        }

                        if(break_flag)
                        {
                            console_history_refer_idx = refer_idx;
                            break;
                        }
                    }
                }
                else if('C' == readval) //key right
                {
                    if(curse_ptr < console_ptr)
                        curse_ptr++;
                    AtomPrint("%s", vt100_curse_right);
                    AtomFlush();
                }
                else if('D' == readval) //key left
                {
                    if (curse_ptr)
                        curse_ptr--;
                    AtomPrint("%s", vt100_curse_left);
                    AtomFlush();
                }
                else if('1' == readval)
                {
                    while (AtomGetchar(&readval));
                    if('~' == readval)  //home
                    {
                        AtomPrint("\033[%dD", curse_ptr);
                        AtomFlush();
                        curse_ptr = 0;
                    }
                }
                else if('3' == readval)
                {
                    while (AtomGetchar(&readval));
                    if('~' == readval)  //delete - putty
                    {
                        if (console_ptr != 0)
                        {
                            AtomPutchar(readval);
                            AtomPrint("%s", vt100_clear_curse_left);
                            AtomFlush();
                            buffer_ops(console_buf, &console_ptr, &curse_ptr, AT_BUF_OP_DEL);
                        }
                    }
                }
                else if('4' == readval)
                {
                    while (AtomGetchar(&readval));
                    if ('~' == readval) //end
                    {
                        AtomPrint("\033[%dC", console_ptr - curse_ptr);
                        AtomFlush();
                        curse_ptr = console_ptr;
                    }
                }
            }
        }
        else
        {
            buffer_ops(console_buf, &console_ptr, &curse_ptr, AT_BUF_OP_ADD | (readval << 8));
            if(console_ptr == curse_ptr)
            {
                AtomPutchar(readval);
            }
            else
            {
                atom_resetline();
            }
            
            if (console_ptr >= (CONSOLE_BUFFER_SIZE - 1)) // protect the \0
            {
                AtomPrint("ERR: Command too long\r\n>>");
                AtomFlush();
                memset(console_buf, 0, CONSOLE_BUFFER_SIZE);
                console_ptr = 0;
                curse_ptr = 0;
            }
        }

        if (get_line == 1)
        {
            AtomPrint("\r\n");
            console_buf[CONSOLE_BUFFER_SIZE - 1] = 0;

            for(i = 0; i < console_ptr; i++)
            {
                if(console_buf[i] != ' ')
                    break;
            }
            if((i >= console_ptr) || (0 == console_ptr))
            {
                atom_newline(&get_line, 0);
                continue;
            }

            //printf("Get = %s\r\n", console_buf);

            for(i = 0; i < CONSOLE_HISTORY_MAX; i++)
            {
                if(console_history[i][0])
                {
                    if(0 == strcmp(console_history[i], console_buf))
                        break;
                }
            }

            if(i >= CONSOLE_HISTORY_MAX)    //no history recorded
            {
                console_history_refer_idx = console_history_update_idx;
                strcpy(console_history[console_history_update_idx++], console_buf);
                console_history_update_idx %= CONSOLE_HISTORY_MAX;  
            }

            console_find_cmd(console_buf);

            atom_newline(&get_line, 1);

        }
    }
}
