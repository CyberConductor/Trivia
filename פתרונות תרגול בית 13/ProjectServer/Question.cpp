#include "Question.h"

Question::Question(string question, string rightAnswer, vector<string> possibleAnswes)
	: m_question(question), m_rightAnswer(rightAnswer), m_possibleAnswers(possibleAnswes){}

string Question::getQuestion()
{
	return m_question;
}

vector<string> Question::getPossibleAnswers()
{
	return m_possibleAnswers;
}

int Question::getCorrectAnswerId()
{
	return 0;
}


