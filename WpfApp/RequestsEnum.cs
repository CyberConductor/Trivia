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
}
