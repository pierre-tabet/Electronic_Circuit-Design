/* mid_level_funcs.c
 *
 * Set of functions at middle level (above low-level hardware drivers but below the
 * high level) for the 3662 calculator mini-project.
 * 
 */

#include "TExaS.h"
#include "mid_level_funcs.h"
#include "low_level_funcs_tiva.h"


// ------------------------ Additional function declerations ------------------------
int convertrow (unsigned char);
void antispam ();
// ------------------------ Keyboard functions ------------------------

char GetKeyboardChar()
{
	int row;
	int col;
	KeyboardReadRowCol(&row,&col);
	return KeyboardRowCol2Char(row, col);
	//PrintChar(KeyboardRowCol2Char(row,col));
} // GetKeyboardChar

void KeyboardReadRowCol( int *row, int *col )
{
	antispam ();

	int countrow = 0;
	int localrow = 0;

	int countcol = 0;
	int localcol = 0;

	do{
	 countrow = 0;
	 localrow = 0;

	 countcol = 0;
	 localcol = 0;

		for(int i = 0; i < 4; i++){            //make each Col high one at a time
		WriteKeyboardCol(1 << i);
			if (ReadKeyboardRow() != 0x00 ){     //check if any of the rows are high
			countcol ++;                         //take note to how many Cols are high to ignore double button presses
			localcol = i;                        //take note of the col

			}
		}

		if (countcol == 1){                   //only if one Col is high
		WriteKeyboardCol(1 << localcol);
			if(ReadKeyboardRow() == 0x01 || ReadKeyboardRow() == 0x02 || ReadKeyboardRow() == 0x04 || ReadKeyboardRow() == 0x08){  //only if one Row is high
			localrow = convertrow(ReadKeyboardRow());  //convert hex to bit number
			countrow =1;

			}
		}


	}while(countcol != 1 || countrow !=1);

*col = localcol + 1 ;		//add 1 to deal with the hitachi LCD
*row = localrow;





} // KeyboardReadRowCol

char KeyboardRowCol2Char( int row, int col )
{
unsigned const char character[4][4] = {{'1','2','3','A'},
																				{'4','5','6','B'},
																				{'7','8','9','C'},
																				{'*','0','#','D'}};

	if(row >= 1 && row <= 4 && col >= 1 && col <= 4){    // Check if row and Col are valid
	return character[row-1][col-1];                     // row and col start at 1,1 but array starts at 0,0
	}else{
	return '?' ;
	}

} // KeyboardRowCol2Char

// ------------------------ Display functions ------------------------

void PrintString( short int line, short int char_pos, const char *string )
{
	SetPrintPosition(line, char_pos);
	  int i = 0;
		while(char_pos < 17 && string[i] != '\0' ){		//only print up to the null character and not off the screen
			PrintChar(string[i]);
			i++;				//only print up to the end of screen
			char_pos ++;

	}


} // PrintString

// ------------------------ Additional functions ------------------------
int convertrow (unsigned char nibble)  //convert from Hex to bit number
{
if(nibble == 0x1){return 1;}
if(nibble == 0x2){return 2;}
if(nibble == 0x4){return 3;}
if(nibble == 0x8){return 4;}
}
void antispam () //if a button is being held remian in an infinite loop to prevent spam
{
int test = 0;
	for(int i = 0; i < 4; i++){
	WriteKeyboardCol(1 << i);
			do {
				if (ReadKeyboardRow() == 0x01 || ReadKeyboardRow() == 0x02 || ReadKeyboardRow() == 0x04 || ReadKeyboardRow() == 0x08){
				test = 1;
				}else{
				test = 0;
				};

			}while(test == 1);
	}
}
