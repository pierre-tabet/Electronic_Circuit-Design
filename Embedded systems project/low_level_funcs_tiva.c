/* low_level_funcs_tiva.c
 *
 * Set of functions at the bottom level for the 3662 calculator mini-project.
 * These are the hardware drivers.
 *
 * For documentation, see the documentation in the corresponding .h file.
 * 
 */

#include "TExaS.h"
#include "low_level_funcs_tiva.h"
#include "PLL.h"

// =========================== CONSTANTS ============================

/* ----- What you need to do with these #define constants -----
 *
 * You will need to define many hardware-specific #define constants
 * to make your program work with the Tiva. They should all be
 * defined here.
 *
 * It is your job to decide which constants you need to define.
 * Any which you do define must have the names given here. This is
 * the same as asking you to use specified names for functions.
 * Rather than giving you a list of required names, you are given
 * the definitions but without the values entered. In fact, they
 * all have a value entered (0x0) to make sure the module will
 * compile.
 *
 * The port allocations (i.e. what bit of what port does what job)
 * are given in Appendix C of the Mini-project Handout. For your
 * convenience they are copied into the comment at the start of each
 * port below.
 *
 * For Ports A, B, D and E and the clocks, all the constants have
 * been listed for you. This is simpler than deciding which you
 * will use in your program. For definitions you don't need,
 * just leave them as they are (with value 0x0) - it's not worth
 * deleting them. Unused #define definitions do no harm.
 *
 * These port and clock constants have been given the standard
 * names, so there is no explanation of what they mean.
 * There are also some special LCD-related definitions which are
 * not Tiva standards. These are at the end of these constants
 * and have comments explaining what they are.
 */

// --------------------------- Ports -------------------------

// Port A (bit 2 is EN, bit 3 is RS):
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_PDR_R        (*((volatile unsigned long *)0x40004514))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))

// Port B (PORTB[2:5] are LCD DB4 to DB7):
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_PDR_R        (*((volatile unsigned long *)0x40005514))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

// Port D (PORTD[0:3] are the outputs to the columns):
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_PDR_R        (*((volatile unsigned long *)0x40007514))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))

// Port E (PORTE[0:3] are the inputs from the rows):
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_PDR_R        (*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))


// --------------------------- Clocks --------------------------
#if 0
/* You are not asked to define the following because they are defined
 * in PLL.h, which is #included by TExaS.h, which is #included above.
 */

//PLL related Defines
#define SYSCTL_RIS_R		(*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RCC_R		(*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R		(*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RCGC1_R		(*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

//SysTick related Defines
#define NVIC_ST_CTRL_R        	(*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R      	(*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R     	(*((volatile unsigned long *)0xE000E018))

#endif
// ------------------- Special definitions ----------------------

/* LCD-related definitions
 *
 * These are explained in Appendix C of the Mini-project Handout;
 * before you write this code you should re-read that part of
 * that Appendix. The port allocations are copied into the
 * comments at the start of each port above.
 * These have all been defined here with the simple value 0 (so the
 * program will compile), but you should think carefully about
 * them. For instance, if you need to access bit 6 of a port,
 * should you define the value to be 6? Or should you write a
 * definition like (*((volatile unsigned long *) ... )) to make
 * the port access this bit directly? It is your decision.
 */

#define LCD_RS			(*((volatile unsigned long *)0x40004020))
			 /*
			 * The single port bit connected to the
			 * RS (Register Select) pin of the LCD.
			 */
#define LCD_EN			(*((volatile unsigned long *)0x40004010))
	     /*
			 * The single port bit connected to the
			 * EN (ENable data transfer) pin of the LCD.
			 */
#define LCD_DATA		(*((volatile unsigned long *)0x400050F0))
     	/*
			 * The set of four adjacent bits connected
			 * to the four data transfer bits (DB4 to
			 * DB7) of the LCD. */



/* Incidentlly, a  comment on C-writing technique:
 * You will have noticed that the comments above use to hold C
 * comment form starting with slash-star and ending with
 * star-slash, rather than the newer slash-slash. This is because
 * a slash-slash comment ends at the line end. When you edit a
 * program, it is an easy mistake to copy and paste something in the
 * middle of a comment. It compiles, but the comment now refers to
 * the wrong lines of code. But with the old form it probably won't
 * compile, so you find the problem.
 * This is also why the comments above start on the same line as
 * the #define, even though there is only room for the opening
 * slash-star. (At least, there would only be room for that if you
 * used the (*((volatile unsigned long *) ... )) form.)
 * This is one of the personal tricks I have invented over the years
 * to stop me making mistaakes.
 */

// =========================== FUNCTIONS ============================

// ------------------------ Keyboard functions ------------------------

void InitKeyboardPorts( void )
{
	 //PORTE[0:3] inputs (rows) pulldown and PORTD [0:3] outputs (columns high one at a time)
	volatile unsigned long delay_D;
  SYSCTL_RCGC2_R |= 0x00000008;     // 1) D clock
  delay_D = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTD_CR_R = 0x0F;           // allow changes to PE[0:3]
  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTD_DIR_R = 0x0F;          // 5) PD[0:3] are outputs
  GPIO_PORTD_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTD_PDR_R = 0x00;          // disable pulldown resistors
  GPIO_PORTD_DEN_R = 0x0F;          // 7) enable digital pins Pd[0:3]

	 //PORTE[0:3] inputs (rows) pulldown and PORTD [0:3] outputs (columns high one at a time)
	volatile unsigned long delay_E;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1) E clock
  delay_E = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTE_CR_R = 0x0F;           // allow changes to PE[0:3]
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTE_DIR_R = 0x00;          // 5) PE[0:3] are inputs
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTE_PDR_R = 0x0F;          // enable pulldown resistors
  GPIO_PORTE_DEN_R = 0x0F;          // 7) enable digital pins PE[0:3]

} // InitKeyboardPorts

void WriteKeyboardCol( unsigned char nibble )
{
	 GPIO_PORTD_DATA_R &= 0x00;   //clear the byte first
	 GPIO_PORTD_DATA_R = nibble;  //write a nibble to it

} // WriteKeyboardCol

unsigned char ReadKeyboardRow( void )
{
	return (GPIO_PORTE_DATA_R & 0xF); //only needs to ruturn 4 bits
} // ReadKeyboardRow


// ------------------------ Display functions ------------------------

void SendDisplayNibble( unsigned char byte, unsigned char instruction_or_data )
{
		LCD_RS = instruction_or_data << 3; //needs to be shifted first to access bit 3 for the data register
		LCD_DATA = byte << 2;  //needs to be shifted first to access bit 2
		LCD_EN = 0x04;
		SysTick_Wait(40); //wait just over 450ns
		LCD_EN &= ~0x04;  //clear the bit 2

} // SendDisplayInstruction

void SendDisplayByte( unsigned char byte, unsigned char instruction_or_data )
{

	SendDisplayNibble(byte >> 4, instruction_or_data);
	SysTick_Wait(45); //wait just over 550ns
	SendDisplayNibble(byte , instruction_or_data);
	WaitMicrosec(37);

} // SendDisplayInstruction

void InitDisplayPort( void )
{
  volatile unsigned long delay_A;
  SYSCTL_RCGC2_R |= 0x00000001;     // 1) A clock
  delay_A = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTA_LOCK_R = 0x4C4F434B;   // 2) unlock PortA
  GPIO_PORTA_CR_R = 0x0C;           // allow changes to PA[2:3]
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTA_DIR_R = 0x0C;          // 5) PA output PA[2:3]
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTA_PUR_R = 0x00;          // disable pullup resistors
  GPIO_PORTA_DEN_R = 0x0C;          // 7) enable digital pins PA[2:3]

	volatile unsigned long delay_B;
	SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
  delay_B = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock PortB
  GPIO_PORTB_CR_R = 0x3C;           // allow changes to PB[2:5]
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTB_DIR_R = 0x3C;          // 5) PB output PB[2:5]
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_PUR_R = 0x00;          // disable pullup resistors
  GPIO_PORTB_DEN_R = 0x3C;          // 7) enable digital pins PB[2:5]

	WaitMicrosec(15100); //Wait more then 15ms
	SendDisplayNibble(0x3, 0);  //Initialising by Instruction
	WaitMicrosec(4200);  //Wait more then 4.1ms
	SendDisplayNibble(0x3, 0);
	WaitMicrosec(110);  //Wait more then 4.1ms
	SendDisplayNibble(0x3, 0);
	WaitMicrosec(37);
	SendDisplayByte(0x28,0);	// 4 bits long, 2 display lines
	SendDisplayByte(0x08,0);
	SendDisplayByte(0x01,0);  //Clear the display
	WaitMicrosec(1600);
	SendDisplayByte(0x06,0); //Entry mode
	TurnCursorOnOff(1);
} // InitDisplayPort

void ClearDisplay()
{
	SendDisplayByte(0x01, 0);
	WaitMicrosec(1600);
} // ClearDisplay

void TurnCursorOnOff( short int On )
{
	if(On == 0) {
		SendDisplayByte(0x0C, 0);
	}else {
		SendDisplayByte(0x0D, 0);
	}

} // TurnCursorOnOff

void SetPrintPosition( short int line, short int char_pos )
{
	char_pos --; // subtract 1 to start at 0
	if(line == 1 && char_pos >= 0x00 && char_pos <= 0x0F ){//check to see if it char_pos is valid

			SendDisplayByte(char_pos | 0x80 , 0);


	} else if (line ==2 && char_pos >= 0x00 && char_pos <= 0x0F){ //check to see if it char_pos is valid

			SendDisplayByte(char_pos | 0x80 | 0x40, 0); // line 2 starts at 0x40 or 64th byte

	}else{}
 // SetPrintPosition
}

void PrintChar( char ch )
{

	SendDisplayByte(ch,1);

} // PrintChar


// ------------------------ Flash memory functions ------------------------

void InitFlash()
{
	// Write your code here.
} // InitFlash

void WriteDoubleToFlash( double number )
{
	// Write your code here.
} // WriteFloatToFlash

double ReadDoubleFromFlash()
{
	// Write your code here.
} // ReadFloatFromFlash


// ------------------------ Sundry functions ------------------------

void InitAllOther()
{
	SysTick_Init();
	PLL_Init();
} // InitAllOther

void InitAllHardware()
{
	InitAllOther();
	InitKeyboardPorts();
	InitFlash();
	InitDisplayPort();

} // InitAllHardware

void WaitMicrosec( long int wait_microsecs )
{
	SysTick_Wait(80 * wait_microsecs	); //wait  12.5ns, 80 times for 1 microsec
} // WaitMicrosec
