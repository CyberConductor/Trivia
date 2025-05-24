#include "JsonResponsePacketSerializer.h"
#include <string>
#include <vector>
#include <cstring>

Buffer JsonResponsePacketSerializer::serializeErrorResponse(ErrorResponse response)
{
	json j;
	j["message"] = response.message;

	return buildResponseBuffer(Responses::Error, j);
}

Buffer JsonResponsePacketSerializer::serializeLoginResponse(LoginResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Responses::Login, j);
}

Buffer JsonResponsePacketSerializer::serializeSignupResponse(SignupResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Responses::Signup, j);
}

Buffer JsonResponsePacketSerializer::serializeLogoutResponse(LogoutResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Responses::Logout, j);
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

	return buildResponseBuffer(Responses::GetRooms, j);
}

Buffer JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(GetPlayersInRoomResponse response)
{
	json j;
	j["players"] = response.players;

	return buildResponseBuffer(Responses::GetPlayersInRoom, j);
}

Buffer JsonResponsePacketSerializer::serializeJoinRoomResponse(JoinRoomResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Responses::JoinRoom, j);
}

Buffer JsonResponsePacketSerializer::serializeCreateRoomResponse(CreateRoomResponse response)
{
	json j;
	j["status"] = response.status;

	return buildResponseBuffer(Responses::CreateRoom, j);
}

Buffer JsonResponsePacketSerializer::serializegetHighScoreResponse(getHighScoreResponse response)
{
	json j;
	j["status"] = response.status;
	j["statistics"] = response.statistics;

	return buildResponseBuffer(Responses::GetHighScore, j);
}

Buffer JsonResponsePacketSerializer::serializegetPersonalStatsResponse(getPersonalStatsResponse response)
{
	json j;
	j["status"] = response.status;
	j["statistics"] = response.statistics;

	return buildResponseBuffer(Responses::GetPersonalStatus, j);
}

Buffer JsonResponsePacketSerializer::buildResponseBuffer(unsigned char code, json& j)
{
	string jsonStr = j.dump();
	unsigned int size = jsonStr.size();
	Buffer buffer;

	//insert the size of the massage
	buffer.push_back(code);
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