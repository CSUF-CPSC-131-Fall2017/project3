#ifndef GetLine_h
#define GetLine_h

//****************************************************************************************
//	INCLUDE FILES
#include	<iostream>
#include	<string>

using namespace std;

//****************************************************************************************
//	CONSTANT DEFINITIONS

//****************************************************************************************
//	CLASSES, TYPEDEFS AND STRUCTURES

//****************************************************************************************
//	FUNCTION PROTOTYPES
bool	GetLine(istream& stream, string& text);

bool	GetLine(istream& stream, string& text, const string& delimiter);

#endif
