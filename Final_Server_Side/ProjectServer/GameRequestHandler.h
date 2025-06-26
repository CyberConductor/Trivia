#pragma once
#include "GameManager.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RoomMember.h"

class GameRequestHandler : public IRequestHandler
{
public:
	//Constructor
	GameRequestHandler(RequestHandlerFactory&, LoggedUser, Game&, RoomMember*);
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
private:
	//Methods
	RequestResult getQuestion(RequestInfo);
	RequestResult submitAnswer(RequestInfo);
	RequestResult getGameResults(RequestInfo);
	RequestResult leaveGame(RequestInfo);
	//attributes
	Game& m_game;
	LoggedUser m_user;
	RoomMember* m_preHandler;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;
};