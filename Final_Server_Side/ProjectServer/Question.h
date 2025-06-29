#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

class Question
{
public:
	Question(string, string, vector<string>);
	
	//Methods:
	string getQuestion();
	vector<string> getPossibleAnswers();

	unsigned int getCorrectAnswerId();
	bool operator==(const Question&);
private:
	string m_question;
	string m_rightAnswer;
	vector<string> m_possibleAnswers;
};