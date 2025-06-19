#include "Question.h"
#include <algorithm>
#include <random>

Question::Question(string question, string rightAnswer, vector<string> possibleAnswes)
	: m_question(question),
	m_rightAnswer(rightAnswer),
	m_possibleAnswers(possibleAnswes)
{
	std::shuffle(m_possibleAnswers.begin(), m_possibleAnswers.end(), std::mt19937(std::random_device{}()));
}

string Question::getQuestion() { return m_question; }
vector<string> Question::getPossibleAnswers() { return m_possibleAnswers; }

unsigned int Question::getCorrectAnswerId()
{
	for (unsigned int i = 0; i < m_possibleAnswers.size(); i++)
		if (m_rightAnswer == m_possibleAnswers[i])
			return i;
}