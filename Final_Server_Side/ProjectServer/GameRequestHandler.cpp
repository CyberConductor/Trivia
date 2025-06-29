#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Game& game, RoomMember* handler)
	: m_handlerFactory(factory),
	m_user(user),
	m_gameManager(factory.getGameManager()),
	m_game(game),
	m_preHandler(handler)
{}

bool GameRequestHandler::isRequestRelevant(RequestInfo req)
{
	return (req.id >= Request_LeaveGame && req.id <= Request_GetGameResults);
}

RequestResult GameRequestHandler::handleRequest(RequestInfo req)
{
	if(!isRequestRelevant(req))
		return generateErrorResponse("Unrecognized request type", this);
	switch (req.id) 
	{
	case Request_LeaveGame:
		return leaveGame(req);
	case Request_GetQuestion:
		return getQuestion(req);
	case Request_SubmitAnswer:
		return submitAnswer(req);
	case Request_GetGameResults:
		return getGameResults(req);
	}
}

RequestResult GameRequestHandler::getQuestion(RequestInfo)
{
	if (m_game.m_questions.empty())
		return generateErrorResponse("no more questions left, game is over.", m_preHandler);

	Question q = m_game.getQuestionForUser(m_user);

	map<unsigned int, string> possibleAnswers;
	int i = 0;
	for ( auto answer : q.getPossibleAnswers())
		possibleAnswers.emplace(i++, answer);

	return { JsonResponsePacketSerializer::serializeGetQuestionResponse({ 1, q.getQuestion(), possibleAnswers }), this };
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo requestInfo)
{
	SubmitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.buffer);
	unsigned int answerId = m_game.m_players.find(m_user)->second.currentQuestion.getCorrectAnswerId();

	return { JsonResponsePacketSerializer::serializeSubmitAnswerResponse({ m_game.submitAnswer(m_user, req.answerId, req.anwserTime), answerId }), this };
}

RequestResult GameRequestHandler::getGameResults(RequestInfo)
{
	if (!m_game.m_questions.empty())
		return generateErrorResponse("game isn't over yet", this);

	GetGameResultsResponse results;
	results.status = 1;
	for (auto player : m_game.m_players)
		results.results.push_back({
			player.first.getUsername(),
			player.second.correctAnswerCount,
			player.second.wrongAnswerCount,
			player.second.avarageAnswerTime });

	return { JsonResponsePacketSerializer::serializeGetGameResultsResponse(results), this };
}

RequestResult GameRequestHandler::leaveGame(RequestInfo)
{
	m_game.removePlayer(m_user.getUsername());

	return { JsonResponsePacketSerializer::serializeLeaveGameResponse({ 0 }), m_preHandler};
}