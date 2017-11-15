#ifndef	DocumentIndex_h
#define	DocumentIndex_h
//
#include	<fstream>
#include	<map>
#include	<set>
#include	<string>
#include    <vector>

using namespace std;

typedef	string::size_type	StringSize;
typedef set<string> StringSet;
typedef StringSet::iterator    StringSetIterator;
typedef vector<int> NumberVector;

struct WordData
{
    WordData():count_(0) {}
    int count_;
    NumberVector pageNumber_;
};

typedef map <string, WordData>      WordMap;
typedef WordMap::iterator           WordMapIterator;
typedef WordMap::value_type         WordMapValue;
typedef pair<WordMapIterator, bool> WordMapStatus;


class	DocumentFile
{
public:
    DocumentFile() : pageNumber_(1) { }
    
    void	Close();                            //  Closes current document
    int		GetPageNumber();                    //	Returns the current pge number.
    string	GetWord();                          //	Returns the next legal word not on the exception list;
                                                //	returns an empty string if there are no more words in
                                                //	the line.
    bool	LoadExclusions(const string& name);	//	Loads a list of words to be excluded from the index
                                                //	from a file of the given name.
    bool	Open(const string& name);           //	Opens a document file of the given name.
    bool	Read();                             //	Reads the next line of the document file, skipping over
                                                //	the double empty lines that mark page separations.
                                                //	Returns false if there are no more lines in the file.
    
    
private:
    
    StringSize      beginPosition_;
    StringSet       exclusion_;
    
    fstream         file_;
    int             pageNumber_;
    int             page_ = 0;
    string          text_;
    vector <string> line_;
    
};

class	DocumentIndex
{
public:
    void	Create(DocumentFile& documentFile);	//	Creates an index for the given document file.
    void	Show (ostream& stream);
    void	Write(ostream& indexFile);	//	Writes the index to the given file.
                                        //	The argument is a stream so that this function
                                        //	can be called to wrtite its output to cout for
                                        //	test purposes.
private:
    
};

#endif

