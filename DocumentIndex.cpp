#include	<fstream>
#include	<iostream>
#include	<string>
#include    <sstream>
#include    <iterator>
#include    <cstdint>
#include    <map>

#include	"DocumentIndex.h"
#include	"GetLine.h"

using namespace std;
//****************************************************************************************
//	DocumentFile::Close()
//****************************************************************************************
void	DocumentFile::Close()
{
    file_.close();
    file_.clear();
    return;
}
//****************************************************************************************
//	DocumentFile::GetPageNumber
//****************************************************************************************
int	DocumentFile::GetPageNumber()
{
//    cout << "page number " << endl;
//    int  pageNumber_ = line_[page_][5] - '0';
//    cout << pageNumber_ << endl;
//    page_++;
//    cout << pageNumber_;
    
    return(pageNumber_);
}
//****************************************************************************************
//	DocumentFile::GetWord
//****************************************************************************************
string	DocumentFile::GetWord()
{
    
    StringSize          endPosition;
    StringSetIterator   iterator;
    StringSize          position;
    
    bool        isLegal;
    string      word;
    string      exclusions_;
    string      legalBack  = ".,:;)\"'";
    string      legalFront = "(\"'";
    string      letters    = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   
    while (true) {
        beginPosition_ = text_.find_first_not_of(" \t", beginPosition_);
        if (beginPosition_ == string::npos)
        {
            word.erase();
            break;
        }
            endPosition = text_.find_first_not_of(" \t", beginPosition_);
            if (endPosition != string::npos)
            {
                word.assign(text_, beginPosition_, endPosition - beginPosition_);
                beginPosition_ = endPosition + 1;
            }
        else
        {
            word.assign(text_, beginPosition_, text_.size());
            beginPosition_ = text_.size();
        }
            position = legalFront.find(word.front());
            if (position != string::npos)
            {
                word.erase(0,1);
            }
        position = legalBack.find(word.back());
        if (position != string::npos)
        {
            word.pop_back();
        }
            isLegal = true;
            for (uint64_t i =0; i < word.size(); ++i)
            {
                position = letters.find(word[i]);
                if (position == string::npos)
            {
        isLegal = false;
        }
            iterator = exclusion_.find(word);
            if (iterator != exclusion_.end())
            {
                isLegal = false;
            }
        if (isLegal)
        {
            break;
        }
      }
    } return(word);
  }
//****************************************************************************************
//	DocumentFile::LoadExclusions
//****************************************************************************************
bool	DocumentFile::LoadExclusions(const string& name)
{

    bool	success = false;
    
    cout << "Exclusions loaded fine: " << endl;
    cout << text_ << endl;
    Close();
    file_.open(name, ios::in);
    string line;
    vector<string> exV;
    std::string token;
    
    while(!file_.eof())
    {
        GetLine(file_, text_);
        std::istringstream ss(line);
        while(std::getline(ss, token, ' '))
        {
            exclusion_.insert(token);
            std::cout << token << '\n';
        }

    }
    //use a iterator for that set.
    StringSetIterator it;
    
    for (it =exclusion_.begin(); it != exclusion_.end(); ++it)
    {
        cout << *it << endl;
    }
//    
//
//    cout << vstrings[0] << endl;
//    cout << "exclusions are:  " << endl;
//    cout << exclusion_[0] << endl;
//    cout << exclusion_[1] << endl;
//    cout << exclusion_[2] << endl;
//    
//    
    if(exclusion_.size())
    {
        success = true;
    }
    else{
        success = false;
    }

    
    return(success);
}
//****************************************************************************************
//	DocumentFile::Open
//****************************************************************************************
bool	DocumentFile::Open(const string& name)
{

    file_.open(name, ios::in);
    if (!file_.fail())
    {
        Read();
        
        return(true);
    }
    else
    {
        return(false);
    }
}
//****************************************************************************************
//	DocumentFile::Read
//****************************************************************************************
bool	DocumentFile::Read()
{
    int empty;
    bool success;
    
    success = true;
    empty = 0;
    
    while (true)
    {
        GetLine(file_, text_);
        if (file_.fail())
        {
            success = false;
            break;
        }
        beginPosition_ =0;
        if (text_.empty())
        {
                ++empty;
            }
                else
                {
                    if (empty > 1)
                    {
                        if (pageNumber_ == 3)
                        {
                            cout << "Pause" << endl;
                    }
                break;
            }
        }
    }

    return(success);
}
//****************************************************************************************
//	DocumentIndex::Create
//****************************************************************************************
void	DocumentIndex::Create(DocumentFile& documentFile)
{

    int    pageNumber;
    bool   success;
    string word;
    
    WordData        wordData;
    WordMapIterator wordIterator;
    WordMapStatus   wordStatus;
    WordMap         wordMap_;

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
            if (word.empty())
            {
            break;
            }
                    wordIterator = wordMap_.find(word);
                    if (wordIterator == wordMap_.end())
                    {
                        wordStatus = wordMap_.insert(WordMapValue (word, wordData));
                        wordIterator = wordStatus.first;
                    }
                            ++wordIterator->second.count_;
                            pageNumber = documentFile.GetPageNumber();
                
                                if (wordIterator->second.pageNumber_.empty())
                                {
                                wordIterator->second.pageNumber_.push_back(pageNumber);
                                }
                
        else if (pageNumber!= wordIterator->second.pageNumber_.back())
        {
            wordIterator->second.pageNumber_.push_back(pageNumber);
        }
    }
}
    return;
}
//****************************************************************************************
//	DocumentIndex::Write
//****************************************************************************************
void	DocumentIndex::Write(ostream& indexStream)
{
    return;
}
//****************************************************************************************
//	DocumentIndex::Show
//****************************************************************************************
void	DocumentIndex::Show(ostream& stream)//
{
    
    string              word;
    WordData            wordData;
    WordMapIterator     wordIterator;
    WordMap             wordMap_;
    
    for (wordIterator = wordMap_.begin(); wordIterator != wordMap_.end(); ++wordIterator)
    {
        
        wordData = wordIterator->second;
        word =wordIterator->first;
        stream << word << " (" << wordData.count_ << ") ";
            if (wordData.count_ > 10)
            {
                stream << "***" << endl;
            }
        else
        {
            stream << endl;
        }
        for (uint64_t i=0; i < wordData.pageNumber_.size();++i)
        {
            stream << " " << i << ":" << wordData.pageNumber_[i] << endl;
            ++i;
        }
        
    }
    return;
    
}
//****************************************************************************************
//	Project Finished
//****************************************************************************************
