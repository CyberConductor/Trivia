using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Windows;
using System.Windows.Threading;
using TriviaClient.Network;

namespace WpfApp
{
    public partial class JoinRoom : Window
    {
        // Updated RoomData class
        private class RoomData
        {
            public int id { get; set; }
            public string name { get; set; }  // Changed from roomName to name
            public int maxPlayers { get; set; }
            public int numOfQuestionsInGame { get; set; }
            public int timePerQuestion { get; set; }

            public override string ToString()
            {
                return $"{name} (ID: {id}) - {maxPlayers} Players, {numOfQuestionsInGame} Qs, {timePerQuestion}s";
            }
        }

        private DispatcherTimer _refreshTimer;

        public JoinRoom()
        {
            InitializeComponent();

            _refreshTimer = new DispatcherTimer();
            _refreshTimer.Interval = TimeSpan.FromSeconds(3);
            _refreshTimer.Tick += RefreshTimer_Tick;
            _refreshTimer.Start();
        }

        private void RefreshTimer_Tick(object sender, EventArgs e)
        {
            LoadRooms();
        }

        private void LoadRooms()
        {
            string response = App.Communicator.SendRequest((byte)Requests.Request_GetRooms, "");

            MessageBox.Show(response, "GetRooms Response");

            try
            {
                var root = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(response);

                if (root != null && root.ContainsKey("rooms"))
                {
                    var rooms = root["rooms"].Deserialize<List<RoomData>>();

                    if (rooms != null && rooms.Count > 0)
                    {
                        RoomsListBox.ItemsSource = rooms;
                        RoomsListBox.Items.Refresh();  // Refresh the ListBox
                    }
                    else
                    {
                        MessageBox.Show("Server returned empty rooms list.");
                    }
                }
                else
                {
                    MessageBox.Show("JSON does not contain a 'rooms' array.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading room list:\n{ex.Message}\nRaw response:\n{response}");
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

                // Change from 'selectedRoom.roomName' to 'selectedRoom.name'
                MessageBox.Show($"Players in Room '{selectedRoom.name}':\n" +
                                $"{string.Join("\n", players)}\n\nAdmin: {admin}");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error loading room list:\n" + ex.Message + "\n\nRaw response:\n" + response);
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            _refreshTimer.Stop();
            base.OnClosed(e);
        }
    }
}
