using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApp
{
    public enum Requests 
	{
		//1
		Request_Login = 1,// skip 0 - error
		Request_Signup,
		//2
		Request_Signout,
		Request_GetRooms,
		Request_GetPlayersInRoom,
		Request_JoinRoom,
		Request_CreateRoom,
		Request_GetHighScore,
		Request_GetPersonalStats,
		//3
		Request_CloseRoom,
		Request_StartGame,
		Request_GetRoomState,
		Request_LeaveRoom,
		//4
		Request_LeaveGame,
		Request_GetQuestion,
		Request_SubmitAnswer,
		Request_GetGameResults
    }

    public enum Responses
    {
		//1
		Response_Error = 0,
		Response_Login,
		Response_Signup,
		//2
		Response_Logout,
		Response_GetRooms,
		Response_GetPlayersInRoom,
		Response_JoinRoom,
		Response_CreateRoom,
		Response_GetHighScore,
		Response_GetPersonalStatus,
		//3
		Response_CloseRoom,
		Response_StartGame,
		Response_GetRoomState,
		Response_LeaveRoom,
		//4
		Response_LeaveGame,
		Response_GetQuestion,
		Response_SubmitAnswer,
		Response_GetGameResults
	};
}
