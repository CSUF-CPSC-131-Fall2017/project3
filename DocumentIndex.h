#ifndef	DocumentIndex_h
#define	DocumentIndex_h

//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<map>
#include	<set>
#include	<string>

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

class	DocumentFile
{
	public:
		DocumentFile() : pageNumber_(1) { }
		
		void	Close();
		
		int		GetPageNumber();			//	Returns the current pge number.
		
		string	GetWord();					//	Returns the next legal word not on the exception list;
											//	returns an empty string if there are no more words in
											//	the line.
		
		bool	LoadExclusions(const string& name);	//	Loads a list of words to be excluded from the index
													//	from a file of the given name.
		
		bool	Open(const string& name);	//	Opens a document file of the given name.
		
		bool	Read();						//	Reads the next line of the document file, skipping over
											//	the double empty lines that mark page separations.
											//	Returns false if there are no more lines in the file.
		
	private:
		StringSize	beginPosition_;
		
		fstream		file_;
		
		int			pageNumber_;
		
		string		text_;
};

class	DocumentIndex
{
	public:
		void	Create(DocumentFile& documentFile);	//	Creates an index for the given document file.

		void	Show(ostream& stream);

		void	Write(ostream& indexFile);	//	Writes the index to the given file.
											//	The argument is a stream so that this function
											//	can be called to wrtite its output to cout for
											//	test purposes.
	
	private:

};

#endif
