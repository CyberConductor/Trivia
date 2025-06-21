using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using TriviaClient.Network;

namespace WpfApp
{
    /// <summary>
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : Window
    {
        private class RoomData
        {
            public int id { get; set; }
            public string roomName { get; set; }
            public int maxPlayers { get; set; }
            public int numOfQuestionsInGame { get; set; }
            public int timePerQuestion { get; set; }
        }
        public JoinRoom()
        {
            InitializeComponent();
        }

        private void LoadRooms()
        {
            string response = App.Communicator.SendRequest((byte)Requests.Request_GetRooms, "{}");

            try
            {
                var root = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(response);

                if (root != null && root.ContainsKey("rooms"))
                {
                    var rooms = root["rooms"].Deserialize<List<RoomData>>();
                    RoomsListBox.ItemsSource = rooms;
                }
                else
                {
                    MessageBox.Show("No rooms found.");
                }
            }
            catch
            {
                MessageBox.Show("Error loading room list.\nRaw response:\n" + response);
            }
        }

        private void JoinRoomButton_Click(object sender, RoutedEventArgs e)
        {
            if (RoomsListBox.SelectedItem is not RoomData selectedRoom)
            {
                MessageBox.Show("Please select a room.");
                return;
            }

            var payload = new { roomId = selectedRoom.id };
            string json = JsonSerializer.Serialize(payload);

            string response = App.Communicator.SendRequest((byte)Requests.Request_GetPlayersInRoom, json);

            try
            {
                var data = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(response);
                var players = data["players"].Deserialize<List<string>>();

                string admin = players.Count > 0 ? players[0] : "Unknown";

                MessageBox.Show($"Players in Room '{selectedRoom.roomName}':\n" +
                                $"{string.Join("\n", players)}\n\nAdmin: {admin}");
            }
            catch
            {
                MessageBox.Show("Error parsing players list.\nRaw response:\n" + response);
            }
        }
    }   
}
