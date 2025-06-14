#include "JsonResponsePacketSerializer.h"

Buffer JsonResponsePacketSerializer::serializeErrorResponse(ErrorResponse response)
{
	json j;
	j["message"] = response.message;

	return buildResponseBuffer(Response_Error, j);
}

Buffer JsonResponsePacketSerializer::serializeLoginResponse(LoginResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_Login, j);
}

Buffer JsonResponsePacketSerializer::serializeSignupResponse(SignupResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_Signup, j);
}

Buffer JsonResponsePacketSerializer::serializeLogoutResponse(LogoutResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_Logout, j);
}

Buffer JsonResponsePacketSerializer::serializeGetRoomsResponse(GetRoomsResponse response)
{
	json j;
	j["status"] = response.status;

	j["rooms"] = json::array();
	for (const RoomData& room : response.rooms)
	{
		json roomJson;
		roomJson["id"] = room.id;
		roomJson["name"] = room.name;
		roomJson["maxPlayers"] = room.maxPlayers;
		roomJson["numOfQuestionsInGame"] = room.numOfQuestionsInGame;
		roomJson["timePerQuestion"] = room.timePerQuestion;
		roomJson["status"] = room.status;

		j["rooms"].push_back(roomJson);
	}

	return buildResponseBuffer(Response_GetRooms, j);
}

Buffer JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(GetPlayersInRoomResponse response)
{
	json j;
	j["players"] = response.players;

	return buildResponseBuffer(Response_GetPlayersInRoom, j);
}

Buffer JsonResponsePacketSerializer::serializeJoinRoomResponse(JoinRoomResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_JoinRoom, j);
}

Buffer JsonResponsePacketSerializer::serializeCreateRoomResponse(CreateRoomResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_CreateRoom, j);
}

Buffer JsonResponsePacketSerializer::serializegetHighScoreResponse(getHighScoreResponse response)
{
	json j;
	j["status"] = response.status;
	j["statistics"] = response.statistics;

	return buildResponseBuffer(Response_GetHighScore, j);
}

Buffer JsonResponsePacketSerializer::serializegetPersonalStatsResponse(getPersonalStatsResponse response)
{
	json j;
	j["status"] = response.status;
	j["statistics"] = response.statistics;

	return buildResponseBuffer(Response_GetPersonalStatus, j);
}

Buffer JsonResponsePacketSerializer::serializeCloseRoomResponse(CloseRoomResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_CloseRoom, j);
}

Buffer JsonResponsePacketSerializer::serializeStartGameResponse(StartGameResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_StartGame, j);
}

Buffer JsonResponsePacketSerializer::serializeGetRoomStateResponse(GetRoomStateResponse response)
{
	json j;
	j["status"] = response.status;
	j["hasGameBegun"] = response.hasGameBegun;
	j["players"] = response.players;
	j["questionCount"] = response.questionCount;
	j["answerTimeOut"] = response.answerTimeOut;

	return buildResponseBuffer(Response_GetRoomState, j);
}

Buffer JsonResponsePacketSerializer::serializeLeaveRoomResponse(LeaveRoomResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_LeaveRoom, j);
}

Buffer JsonResponsePacketSerializer::serializeLeaveGameResponse(LeaveGameResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Response_LeaveGame, j);
}

Buffer JsonResponsePacketSerializer::serializeGetQuestionResponse(GetQuestionResponse response)
{
	json j;
	j["status"] = response.status;
	j["question"] = response.question;
	for (const auto& pair : response.answers)
		j[std::to_string(pair.first)] = pair.second;

	return buildResponseBuffer(Response_GetQuestion, j);
}

Buffer JsonResponsePacketSerializer::serializeSubmitAnswerResponse(SubmitAnswerResponse response)
{
	json j;
	j["status"] = response.status;
	j["correctAnwserId"] = response.correctAnwserId;

	return buildResponseBuffer(Response_SubmitAnswer, j);
}

Buffer JsonResponsePacketSerializer::serializeGetGameResultsResponse(GetGameResultsResponse response)
{
	json j;
	j["status"] = response.status;

	json j_results = json::array();
	for (const PlayerResults& pr : response.results)
	{
		json j_player;
		j_player["username"] = pr.username;
		j_player["correctAnswerCount"] = pr.correctAnswerCount;
		j_player["wrongAnswersCount"] = pr.wrongAnswersCount;
		j_player["avarageAnswerTime"] = pr.avarageAnswerTime;

		j_results.push_back(j_player);
	}
	j["results"] = j_results;

	return buildResponseBuffer(Response_GetGameResults, j);	
}

Buffer JsonResponsePacketSerializer::buildResponseBuffer(unsigned char code, json& j)
{
	string jsonStr = j.dump();
	unsigned int size = jsonStr.size();
	Buffer buffer;

	//insert the message code
	buffer.push_back(code);
	//insert the massage size
	buffer.insert(buffer.end(), {
		static_cast<unsigned char>(size >> 24),
		static_cast<unsigned char>(size >> 16),
		static_cast<unsigned char>(size >> 8),
		static_cast<unsigned char>(size)
		});

	//insert the massage
	buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
	return buffer;
}