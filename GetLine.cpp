//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<iostream>
#include	<string>

#include	"GetLine.h"

using namespace std;

//****************************************************************************************
//
//	CONSTANT DEFINITIONS
//
//****************************************************************************************

//****************************************************************************************
//
//	CLASSES, TYPEDEFS AND STRUCTURES
//
//****************************************************************************************

//****************************************************************************************
//
//	MACROS
//
//****************************************************************************************

//****************************************************************************************
//
//	PUBLIC DATA
//
//****************************************************************************************

//****************************************************************************************
//
//	PRIVATE DATA
//
//****************************************************************************************

//****************************************************************************************
//
//	FUNCTION PROTOTYPES
//
//****************************************************************************************

//****************************************************************************************
//
//	GetLine
//
//	This function reads a series of characters from a stream and puts it into a string,
//	stopping when it sees a line break:
//		1. A carriage return (CR)
//		2. A line feed (LF)
//		3. A CR/LF pair
//		4. A LF/CR pair
//	The #3 and #4 pairs are a single line break.
//	The line break (any of the four) is consumed but not added to the output string.
//
//	The return status is true if at least one character, oncluidng a line break, is read;
//	the status is false if an end-of-file is immediately encountered.
//
//****************************************************************************************
bool	GetLine(istream& stream, string& text)
{
	//************************************************************************************
	//	LOCAL DATA
	int		c;
	
	int		p;
	
	bool	success;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	text.erase();
	success = false;
	while (true)
	{
		c = stream.get();
		if (stream.good())
		{
			success = true;
			
			if (c == '\r')
			{
				p = stream.peek();
				if (p == '\n')
				{
					stream.ignore();
				}
				break;
			}
			else if (c == '\n')
			{
				p = stream.peek();
				if (p == '\r')
				{
					stream.ignore();
				}
				break;
			}
			else
			{
				text += c;
			}
		}
		else
		{
			break;
		}
	}

	//	If we reached the end of file, but at least one character was seen,
	//	including any delimiter, clear the stream's state so the caller won't
	//	ignore the last line of a file.
	if (success)
	{
		stream.clear();
	}

	return(success);
}

//****************************************************************************************
//
//	GetLine
//
//	This function reads a series of characters from a stream and puts it into a string,
//	stopping when it sees any character from a specified delimiter set. The delimiter
//	is consumed but not added to the output string.
//
//****************************************************************************************
bool	GetLine(istream& stream, string& text, const string& delimiter)
{
	//************************************************************************************
	//	LOCAL DATA
	const uint32_t	initialMask = 0x80000000;

	const uint32_t	columnMask = 0x1F;

	const uint32_t	rowMask = 0x07;

	const uint32_t	rowShift = 5;

	char		c;

	uint32_t	flag[8];

	uint32_t	i;

	uint32_t	mask;

	uint32_t	row;

	uint32_t	shift;

	bool		success;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	text.erase();

	//	Set up flags array.
	for (row = 0; row < (sizeof(flag) / sizeof(flag[0])); ++row)
	{
		flag[row] = 0;
	}
	for (i = 0; i < delimiter.size(); ++i)
	{
		c = delimiter[i];
		row = (c >> rowShift) & rowMask;
		shift = c & columnMask;
		mask = initialMask >> shift;
		flag[row] |= mask;
	}

	//	Get characters until a delimiter is seen or the end of the file
	//	is reached.
	success = false;
	while (true)
	{
		stream.get(c);
		if (stream.good())
		{
			//	Remember that at least one character has been seen.
			success = true;

			//	Check for a delimiter.
			row = (c >> rowShift) & rowMask;
			shift = c & columnMask;
			mask = initialMask >> shift;
			if ((flag[row] & mask) == 0)
			{
				//	Character isn't a delimiter, save it.
				text += c;
			}
			else
			{
				//	Character is a delimiter, leave loop.
				break;
			}
		}
		else
		{
			break;
		}
	}

	//	If we reached the end of file, but at least one character was seen,
	//	including any delimiter, clear the stream's state so the caller won't
	//	ignore the last line of a file.
	if (success)
	{
		stream.clear();
	}

	return(success);
}
