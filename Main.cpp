//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<iostream>
#include	<sstream>
#include	<string>
#include	<vector>

#include	"DocumentIndex.h"
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
typedef	string::size_type	StringSize;

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
bool	CreateIndexTest();

bool	ExcessiveAppearancesTest();

bool	ExclusionTest();

bool	GetWordTest();

bool	PageNumberTest();

//****************************************************************************************
//
//	MAIN PROGRAM
//
//****************************************************************************************
int main (int argc, char * const argv[])
{
	//************************************************************************************
	//	LOCAL DATA
	bool			pass;
	
	int				passCount;

	int				score;

	bool			success;
	
	string			word;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	score = 0;
	passCount = 0;

	cout << endl << "PageNumberTest (10 points)" << endl;
	pass = PageNumberTest();
	if (pass)
	{
		cout << "  Pass" << endl;
		++passCount;
		score += 10;
	}
	else
	{
		cout << "  Fail" << endl;
	}

	cout << endl << "ExclusionTest (10 points)" << endl;
	pass = ExclusionTest();
	if (pass)
	{
		cout << "  Pass" << endl;
		++passCount;
		score += 10;
	}
	else
	{
		cout << "  Fail" << endl;
	}
	
	cout << endl << "ExcessiveAppearancesTest (10 points)" << endl;
	pass = ExcessiveAppearancesTest();
	if (pass)
	{
		cout << "  Pass" << endl;
		++passCount;
		score += 10;
	}
	else
	{
		cout << "  Fail" << endl;
	}
	
	cout << endl << "GetWordTest (20 points)" << endl;
	pass = GetWordTest();
	if (pass)
	{
		cout << "  Pass" << endl;
		++passCount;
		score += 20;
	}
	else
	{
		cout << "  Fail" << endl;
	}
	
	cout << endl << "CreateIndexTest (30 points)" << endl;
	pass = CreateIndexTest();
	if (pass)
	{
		cout << "  Pass" << endl;
		++passCount;
		score += 30;
	}
	else
	{
		cout << "  Fail" << endl;
	}
	
	if (passCount == 5)
	{
		cout << endl << "All tests passed." << endl
		<< "Score: 80/80." << endl;
	}
	else
	{
		cout << endl << passCount << " of 5 tests passed." << endl
			<< "Score: " << score << "/80." << endl;
	}

	return(0);
}

//****************************************************************************************
//
//	CreateIndexTest
//
//****************************************************************************************
bool	CreateIndexTest()
{
	//************************************************************************************
	//	LOCAL DATA
	fstream			actualIndexFile;

	string			actualLine;
	
	DocumentFile	documentFile;
	
	DocumentIndex	documentIndex;

	uint64_t		endActual = 1;
	
	uint64_t		endExpected = 2;
	
	uint64_t		endStatus;
	
	fstream			expectedIndexFile;

	string			expectedLine;
	
	bool			pass;
	
	bool			success;
	
	string			word;
	
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	success = documentFile.Open("Document.txt");
	if (!success)
	{
		cout << "Can't open document file." << endl;
		return(false);
	}
	
	actualIndexFile.open("ActualIndex.txt", ios::in | ios::out | ios::trunc);
	if (actualIndexFile.fail())
	{
		cout << "Can't create index file." << endl;
		return(false);
	}
	
	expectedIndexFile.open("ExpectedIndex.txt", ios::in);
	if (actualIndexFile.fail())
	{
		cout << "Can't create index file." << endl;
		return(false);
	}
	
	documentIndex.Create(documentFile);
	
	documentIndex.Write(actualIndexFile);
	
//	indexFile << endl << endl;
//	documentIndex.Show(indexFile);

	actualIndexFile.seekg(0, ios::beg);
	
	endStatus = 0;
	while (true)
	{
		GetLine(actualIndexFile, actualLine);
		if (actualIndexFile.fail())
		{
			endStatus |= endActual;
		}

		GetLine(expectedIndexFile, expectedLine);
		if (expectedIndexFile.fail())
		{
			endStatus |= endExpected;
		}
		
		if (endStatus == (endActual | endExpected))
		{
			pass = true;
			break;
		}
		
		if (endStatus == 0)
		{
			if (actualLine != expectedLine)
			{
				pass = false;
				break;
			}
		}
		else
		{
			pass = false;
			break;
		}
	}

	return(pass);
}

//****************************************************************************************
//
//	ExcessiveAppearancesTest
//
//****************************************************************************************
bool	ExcessiveAppearancesTest()
{
	//************************************************************************************
	//	LOCAL DATA
	string			actualText;
	
	DocumentFile	documentFile;
	
	DocumentIndex	documentIndex;

	string			expectedText = "first 1\nsecond 1\n";
	
	stringstream	indexStream;
	
	bool			success;
	
	string			word;
	
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	success = documentFile.Open("ExcessiveAppearances.txt");
	if (!success)
	{
		cout << "Can't open document file." << endl;
		return(false);
	}
	
	documentIndex.Create(documentFile);
	
	documentIndex.Write(indexStream);
	actualText = indexStream.str();
	if (actualText == expectedText)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

//****************************************************************************************
//
//	ExclusionTest
//
//****************************************************************************************
bool	ExclusionTest()
{
	//************************************************************************************
	//	LOCAL DATA
	vector<string>	actualWord;
	
	DocumentFile	documentFile;
	
	vector<string>	expectedWord =
	{
		"some", "words", "to", "include", "and", "some", "to", "or", "to", "or", "to"
	};

	bool			pass;
	
	bool			success;
	
	string			word;
	
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	success = documentFile.Open("ExclusionTest.txt");
	if (!success)
	{
		cout << "Can't open document file." << endl;
		return(false);
	}
	
	success = documentFile.LoadExclusions("ExclusionWords.txt");
	if (!success)
	{
		cout << "Can't load exclusions." << endl;
		return(false);
	}
	
	while (true)
	{
		success = documentFile.Read();
		if (!success)
		{
			break;
		}
		
		while (true)
		{
			word = documentFile.GetWord();
			if (!word.empty())
			{
				actualWord.push_back(word);
			}
			else
			{
				break;
			}
		}
	}
	
	if (actualWord.size() == expectedWord.size())
	{
		pass = true;
		for (uint64_t i = 0; i < actualWord.size(); ++i)
		{
			if (actualWord[i] != expectedWord[i])
			{
				pass = false;
				break;
			}
		}
	}
	else
	{
		pass = false;
	}

	return(pass);
}

//****************************************************************************************
//
//	GetWordTest
//
//****************************************************************************************
bool	GetWordTest()
{
	//************************************************************************************
	//	LOCAL DATA
	vector<string>	actualWord;
	
	DocumentFile	documentFile;
	
	vector<string>	expectedWord =
	{
		"a", "legal", "word", "and", "another", "period", "comma", "colon", "semicolon",
		"open", "parenthesis", "closed", "parenthesis",
		"opening", "double", "quote", "closing", "double", "quote",
		"opening", "single", "quote", "closing", "single", "quote",
		"OK", "Good"
	};
	
	bool			pass;
	
	bool			success;

	string			word;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	success = documentFile.Open("GetWord.txt");
	if (!success)
	{
		cout << "Can't open document file." << endl;
		return(false);
	}
	
	while (true)
	{
		success = documentFile.Read();
		if (!success)
		{
			break;
		}

		while (true)
		{
			word = documentFile.GetWord();
			if (!word.empty())
			{
				actualWord.push_back(word);
			}
			else
			{
				break;
			}
		}
	}

	if (actualWord.size() == expectedWord.size())
	{
		pass = true;
		for (uint64_t i = 0; i < actualWord.size(); ++i)
		{
			if (actualWord[i] != expectedWord[i])
			{
				pass = false;
				break;
			}
		}
	}
	else
	{
		pass = false;
	}

	return(pass);
}

//****************************************************************************************
//
//	PageNumberTest
//
//****************************************************************************************
bool	PageNumberTest()
{
	//************************************************************************************
	//	LOCAL DATA
	int				actualPageNumber;
	
	DocumentFile	documentFile;

	vector<int>		expectedPageNumber = { 1, 1, 1, 2, 2, 3, 3, 3, 3 };

	bool			pass;
	
	bool			success;
	
	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	success = documentFile.Open("PageNumber.txt");
	if (!success)
	{
		cout << "Can't open document file." << endl;
		return(false);
	}
	
	pass = true;
	for (uint64_t i = 0; i < expectedPageNumber.size(); ++i)
	{
		success = documentFile.Read();
		if (!success)
		{
			break;
		}
		
		actualPageNumber = documentFile.GetPageNumber();
		if (actualPageNumber != expectedPageNumber[i])
		{
			pass = false;
			break;
		}
	}
	
	return(pass);
}

#if 0
//****************************************************************************************
//
//	Test
//
//****************************************************************************************
bool	Test()
{
	//************************************************************************************
	//	LOCAL DATA
	bool	pass;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS

	return(pass);
}

#endif
