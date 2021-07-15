/* high_level_funcs.c
 *
 * Set of functions at high level (above mid level but below main)
 * for the 3662 calculator mini-project.
 *
 * For documentation, see the corresponding .h file.
 *
 *
 */

#include "TExaS.h"
#include "high_level_funcs.h"
#include "mid_level_funcs.h"
#include "low_level_funcs_tiva.h"


// ------------------------ Keyboard functions ---------------------

void ReadAndEchoInput( char *input_buffer, int input_buffer_size )
{




int shift_pressed = 0;
int rubout_pressed = 0;
int position = 1;
char input = GetKeyboardChar();
const char conversion[3][2] = {{'+','x'},
															 {'-','/'},
															 {'.','E'}};
ClearDisplay();
for(int i = 0; i < input_buffer_size - 1; i++){input_buffer[i] = '\0';}		//Clear the input_buffer
TurnCursorOnOff(1);
do{

	if (input == '*'){break;}  															//end the loop when = is pressed
	else if (input == 'D' ){shift_pressed ^= 1;} 						// toggle shift on and off
	else if (input == '#'){ 																//if rubout is pressed, check if shift is true or false
			if (shift_pressed == 1){
				shift_pressed = 0;
				for(int i = 0; i < input_buffer_size - 1; i++){		//if shift is on clear the entire display
					input_buffer[i] = '\0';
          ClearDisplay();
				}
				position = 1;
			}else if(shift_pressed == 0){                      //if shift is off clear the last character
				if((position >1) ){
				position --;                                     //subtract first to get rubout the previous character
				SetPrintPosition(1, position);
				PrintChar(' ');
				SetPrintPosition(1, position);
				input_buffer[position-1]='\0';

				}
		  }

	}
  else if((input == 'A' ||input == 'B'||input == 'C' )&&(position < input_buffer_size - 1)){    //convert A,B,C into their respective symbols
		if(shift_pressed == 0){																	//insuring shift is either 0 or 1
		PrintChar(conversion[input-65][0]);
		input_buffer[position-1]= conversion[input-65][0];
		position ++;
		}else if (shift_pressed == 1){
		PrintChar(conversion[input-65][1]);
		input_buffer[position-1]= conversion[input-65][1];
		position ++;
		}

	}
	else if(position < input_buffer_size - 1){									//Printing of numbers at correct position within 16 characters
	PrintChar(input);
	input_buffer[position-1]= input;
	position ++;                                              //increment posistion after is is printed and stored in the buffer
	}


input = GetKeyboardChar();

}while(1);


} // ReadAndEchoInput

// ------------------------ Display functions ------------------------

void DisplayResult( double answer )
{
	TurnCursorOnOff(0);
	char out[17];
	sprintf(out,"%16.14G",answer);  //14 significant figures and %E or %f, depending on what is shortest
	PrintString(2,1,out);

} // DisplayResult

void DisplayErrorMessage( const char *error_message_line1,
			  const char *error_message_line2 )
{
TurnCursorOnOff(0);
	PrintString(1,1,error_message_line1);
	PrintString(2,1,error_message_line2);
} // DisplayErrorMessage
