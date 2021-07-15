/*! \file low_level_funcs_tiva.h
 * Set of functions at the bottom level for the 3662 calculator
 * mini-project. These are the hardware drivers.
 *
 */



#ifndef LOW_LEVEL_FUNCS_TIVA_H
#define LOW_LEVEL_FUNCS_TIVA_H

// ======================== Keyboard functions ========================
//! \name Keyboard functions
//@{

/*! This should perform all the initialisation specific to the two ports
 * used for output to the keyboard columns and input from their rows.
 */
void InitKeyboardPorts( void );

/*! Select which column will be examined when the rows are read by ReadKeyboardRow().
 *
 * \param [in] nibble The 4-bit quantity to write. Exactly one bit of this should be set.
 *
 * This should write a nibble (4-bit quantity) to the four bits allocated to the
 * keyboard columns.
 * The quantity output should be the least significant four bits of the
 * parameter.
 * The other four bits should be ignored (and are thus harmless if set).
 * The allocation of bits of the nibble to columns is:
 * 	- leftmost (keys 1,4,7,*) is bit 3 (0x8);
 * 	- next (2,5,8,0) is bit 2 (0x4);
 * 	- next (3,6,9.#) is bit 1 (0x2);
 * 	- rightmost (A,B,C,D) is bit 0 (0x1).
 *
 * Extra marks will be awarded for software which checks that exactly one bit
 * has been set.
 *
 * The use of this function assumes that InitKeyboardPorts() has already
 * been called.
 */
void WriteKeyboardCol( unsigned char nibble );

/*! Read one row to see which (if any) of its key(s) has/have been pressed.
 *
 * This should read the four bits allocated to the input from the keyboard rows.
 * These four bits should be returned as the least four bits of the result.
 * The other four bits should be set to zero.
 * The allocation of bits of the nibble to rows is:
 *  	- top (keys 1,2,3,A) is bit 3 (0x8);
 * 	- next (4,5,6,B) is bit 2 (0x4);
 * 	- next (7,8,9,C) is bit 1 (0x2);
 * 	- bottom (*,0,#,D) is bit 0 (0x1).
 *
 * The use of this function assumes that InitKeyboardPorts() and
 * WriteKeyboardCol() have already been called.
 */
unsigned char ReadKeyboardRow( void );

//@}
// End of Keyboard functions

// ======================== Display functions ========================
//! \name Display functions
//@{

/*! Send one nibble of data or an instruction to the display.
 *
 * \param [in] nibble The nibble to be sent. It must be in the least
 * 		four significant bits of the char.
 * \param [in] instruction_or_data 0 for instruction,
 * 				   1 for data (i.e. text to display).
 *
 * This must
 * 	1. shift the nibble into the correct four port bits
 * 	2. send the bits
 * 	3. change EN to 1
 * 	4. wait 450 ns
 * 	5. change EN to 0.
 *
 * Note that the other delays (e.g. 37 microseconds for sending instructions
 * or data) are not performed here. They are performed by functions calling
 * this one.
 */
void SendDisplayNibble( unsigned char byte, unsigned char instruction_or_data );

/*! Send one byte of data or an instruction to the display.
 *
 * \param [in] byte The byte to be sent.
 * \param [in] instruction_or_data 0 for instruction,
 * 				   1 for data (i.e. text to display).
 *
 * Send the byte to the port (as two nibbles), waiting the
 * appropriate time after each send.
 *
 * A wait of at least 550 ns is needed between the two nibbles.
 * This is because the second EN pulse must not start until at least
 * 1000 ns after the first one starts.
 *
 * A delay of 37 microseconds is needed after the second nibble. This is
 * to allow the display to process the byte.
 *
 * This function can be used by almost all of the other display operations.
 * (The exception is in InitDisplayPort(), where some early instructions
 * have unusual timing and format.)
 *
 * \note The parameter is \a unsigned \a char, which is really a shorthand for an
 * unsigned 8-bit integer. This is a misnomer: a \a char is a representation
 * of ink on paper, and sign (+/-) is an irrelevant concept.
 */
void SendDisplayByte( unsigned char byte, unsigned char instruction_or_data );

/*! This should perform all the initialisation specific to the port
 * used for output to the LCD display.
 *
 * There are many functions of the Hitachi HD44780U display (e.g. setting cursor mode,
 * setting entry mode, setting display on) which only need to be called once
 * at the start, and with standard values. They can be called from within this
 * function rather than inventing separate functions like \a SetEntryMode().
 */
void InitDisplayPort( void );

/*! Clear the display.
 *
 * The only real requirement is that there is no information left on the
 * display. Whatever the hardware-specific Clear Display instruction does
 * will be satisfactory.
 */
void ClearDisplay( void );

/*! Turn the cursor on or off.
 *
 * \param [in] On 0 for off, any non-zero quantity for on.
 */
void TurnCursorOnOff( short int On );

/*! Set the print position for the next character printed.
 *
 * \param [in] line The line number, 1 for top or 2 for bottom.
 * \param [in] char_pos The character position, counting from 1 at the
 * 		left to 16 at the right.
 *
 * Extra marks will be given for software which checks that the parameters are
 * within range. (What to do with invalid parameters is another question ...)
 *
 * Note that these parameters count from 1, but the Hitachi HD44780U addresses
 * count from 0.
 */
 void SetPrintPosition( short int line, short int char_pos );

/*! Print a character at the current position, then increment that position
 * ready for any following character.
 *
 * \param [in] char The character to be displayed.
 *
 * Note: the Hitachi HD44780U automatically performs the display position
 * increment.
 *
 * This autoinrement can leave the next print position beyond the end of the
 * display, so extra marks will be given for software that checks for valid
 * position before sending the character to the display.
 */
void PrintChar( char ch );

// End of Display functions
//@}

// Constant for flash memory:
// ======================== Flash memory functions ========================
//! \name Flash memory functions
//@{

/*! Address in flash where the previous answer is stored.
 *
 * It is up to the prorammer to choose a value for this.
 */
#define ANSWER_FLASH_ADDRESS	0x00

/*! Initialise flash memory.
 */
void InitFlash( void );

/*! Write a double-precision floating point number to flash memory.
 *
 * \param [in] number The number to store.
 *
 * Store it in the address ANSWER_FLASH_ADDRESS.
 */
void WriteDoubleToFlash( double number );

/*! Read a double-precision floating point number from flash memory.
 *
 * \return The number read.
 *
 * Read it from the address ANSWER_FLASH_ADDRESS.
 */
double ReadDoubleFromFlash( void );

 // End of Flash memory functions
//@}

// ======================== Sundry functions ========================
//! \name Sundry functions
//@{

// Those other than keyboard, display and flash, e.g. clock initialization.

/*! Initialise everything other than keyboard, display and flash.
 */
void InitAllOther( void );

/*! Initialise everything.
 *
 * This will probably call other init functions
 * rather than handling the hardware itself.
 */
void InitAllHardware( void );

/*! Wait a specified number of microseconds.
 *
 * \param [in] wait_microsecs The time (in microseconds) to delay.
 */
void WaitMicrosec( long int wait_microsecs );


// End of Sundry functions.
//@}
#endif // of #ifndef LOW_LEVEL_FUNCS_TIVA_H
