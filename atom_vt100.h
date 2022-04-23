#ifndef __ATOM_VT100_H_
#define __ATOM_VT100_H_

//----------------------- ASCII -------------------
enum ASCII_TABLE
{
	ASCII__NULL,                                           // NULL
	ASCII_SOH,                                             // Start of Heading
	ASCII_STX,                                             // Start of Text
	ASCII_ETX,                                             // End of Text
	ASCII_EOT,                                             // End of Transmission
	ASCII_ENQ,                                             // Enquiry
	ASCII_ACK,                                             // Acknowledgement
	ASCII_BEL,                                             // Bell
	ASCII_BS,                                              // Backspace
	ASCII_TAB,                                             // Horizontal Tab
	ASCII_LF,                                              // Line Feed 
	ASCII_VT,                                              // Vertical Tab 
	ASCII_FF,                                              // Form Feed 
	ASCII_CR,                                              // Carriage Return
	ASCII_SO,                                              // Shift Out 
	ASCII_SI,                                              // Shift In 
	ASCII_DLE,                                             // Data Link Escape 
	ASCII_DC1,                                             // Device Control 1
	ASCII_DC2,                                             // Device Control 2
	ASCII_DC3,                                             // Device Control 3
	ASCII_DC4,                                             // Device Control 4
	ASCII_NAK,                                             // Negative Acknowledgement 
	ASCII_SYN,                                             // Synchronous Idle 
	ASCII_ETB,                                             // End of Transmission Block 
	ASCII_CAN,                                             // Cancel
	ASCII_EM,                                              // End of Medium 
	ASCII_SUB,                                             // Substitute 
	ASCII_ESC,                                             // Escape
	ASCII_SF,                                              // File Separator
	ASCII_GS,                                              // Group Separator
	ASCII_RS,                                              // Record Separator
	ASCII_US,                                              // Unit Separator

	//misc characters				                 

	ASCII_SPACE,                                           // space
	ASCII_EXCLAMAION_MARK,                                 // !
	ASCII_QUOTATION_MARK_DOUBLE,                           // "
	ASCII_HASHTAG,                                         // #
	ASCII_SING_DOLLAR,                                     // $
	ASCII_PRECENT,                                         // %
	ASCII_AMPERSANT,                                       // &
	ASCII_QUOTATION_MARK_SINGLE,                           // '
	ASCII_PARENTHESES_OPEN,                                // (
	ASCII_PARENTHESES_CLOSE,                               // )
	ASCII_ASTERISK,                                        // *
	ASCII_PLUS,                                            // +
	ASCII_COMMA,                                           // ,
	ASCII_MINUS,                                           // -
	ASCII_DOT,                                             // .
	ASCII_SLASH_FOWARD,                                    // /
	ASCII_NUMBER_0,                                        // 0
	ASCII_NUMBER_1,                                        // 1
	ASCII_NUMBER_2,                                        // 2
	ASCII_NUMBER_3,                                        // 3
	ASCII_NUMBER_4,                                        // 4
	ASCII_NUMBER_5,                                        // 5
	ASCII_NUMBER_6,                                        // 6
	ASCII_NUMBER_7,                                        // 7
	ASCII_NUMBER_8,                                        // 8
	ASCII_NUMBER_9,                                        // 9
	ASCII_COLON,                                           // :
	ASCII_COLON_SEMI,                                      // ;
	ASCII_LESS_THAN,                                       // <
	ASCII_EQUAL_TO,                                        // =
	ASCII_GREATER_THAN,                                    // >
	ASCII_QUESTION_MARK,                                   // ?
	ASCII_SING_AT,                                         // @

	//upper case alphabet			                 

	ASCII_A_UPPER,                                         // A
	ASCII_B_UPPER,                                         // B
	ASCII_C_UPPER,                                         // C
	ASCII_D_UPPER,                                         // D
	ASCII_E_UPPER,                                         // E
	ASCII_F_UPPER,                                         // F
	ASCII_G_UPPER,                                         // G
	ASCII_H_UPPER,                                         // H
	ASCII_I_UPPER,                                         // I
	ASCII_J_UPPER,                                         // J
	ASCII_K_UPPER,                                         // K
	ASCII_L_UPPER,                                         // L
	ASCII_M_UPPER,                                         // M
	ASCII_N_UPPER,                                         // N
	ASCII_O_UPPER,                                         // O
	ASCII_P_UPPER,                                         // P
	ASCII_Q_UPPER,                                         // Q
	ASCII_R_UPPER,                                         // R
	ASCII_S_UPPER,                                         // S
	ASCII_T_UPPER,                                         // T
	ASCII_U_UPPER,                                         // U
	ASCII_V_UPPER,                                         // V
	ASCII_W_UPPER,                                         // W
	ASCII_X_UPPER,                                         // X
	ASCII_Y_UPPER,                                         // Y
	ASCII_Z_UPPER,                                         // Z

	//misc characters				                 

	ASCII_BRACKET_OPEN,                                    // [
	ASCII_SLASH_BACKWARD,                                  //
	ASCII_BRACKET_CLOSE,                                   // ]
	ASCII_CARET,                                           // ^
	ASCII_UNDERSCORE,                                      // _
	ASCII_GRAVE_ACCENT,                                    // ` 

	//lower case alphabet			                 

	ASCII_A_LOWER,                                         // a
	ASCII_B_LOWER,                                         // b
	ASCII_C_LOWER,                                         // c
	ASCII_D_LOWER,                                         // d
	ASCII_E_LOWER,                                         // e
	ASCII_F_LOWER,                                         // f
	ASCII_G_LOWER,                                         // g
	ASCII_H_LOWER,                                         // h
	ASCII_I_LOWER,                                         // i
	ASCII_J_LOWER,                                         // j
	ASCII_K_LOWER,                                         // k
	ASCII_L_LOWER,                                         // l
	ASCII_M_LOWER,                                         // m
	ASCII_N_LOWER,                                         // n
	ASCII_O_LOWER,                                         // o
	ASCII_P_LOWER,                                         // p
	ASCII_Q_LOWER,                                         // q
	ASCII_R_LOWER,                                         // r
	ASCII_S_LOWER,                                         // s
	ASCII_T_LOWER,                                         // t
	ASCII_U_LOWER,                                         // u
	ASCII_V_LOWER,                                         // v
	ASCII_W_LOWER,                                         // w
	ASCII_X_LOWER,                                         // x
	ASCII_Y_LOWER,                                         // y
	ASCII_Z_LOWER,                                         // z

	//misc characters				                 

	ASCII_BRACKET_CURLY_OPEN,                              // {
	ASCII_VERTICAL_BAR,                                    // |
	ASCII_BRACKET_CURLY_CLOSE,                             // }
	ASCII_TILDE,                                           // ~

	ASCII_DEL,                                             // Delete

};

//const char vt100_curse_up[]             =   {"\033[A"};
//const char vt100_curse_down[]           =   {"\033[B"};
const char vt100_curse_left[]           =   {"\033[D"};
const char vt100_curse_right[]          =   {"\033[C"};
const char vt100_clear_curse_right[]    =   {"\033[K"};
const char vt100_clear_curse_left[]     =   {"\033[1K"};  
//const char vt100_delete[]               =   {"\033[3~"};
//const char vt100_home[]                 =   {"\033[H"};
//const char vt100_end[]                  =   {"\033[4~"};
#endif //__ATOM_VT100_H_
