using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApp
{
    public enum Requests
    {
        Request_Login = 0,
        Request_Signup,
        Request_Signout,
        Request_GetRooms,
        Request_GetPlayersInRoom,
        Request_getStatistics,
        Request_GetPersonalStats,
        Request_GetHighScore,
        Request_JoinRoom,
        Request_CreateRoom
    }
}
