using System;
using System.Collections.Generic;
using System.Linq;
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
            public string name { get; set; }
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
                        RoomsListBox.Items.Refresh();  
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


            string joinJson = JsonSerializer.Serialize(new { roomId = selectedRoom.id });
            string joinResponse = App.Communicator.SendRequest((byte)Requests.Request_JoinRoom, joinJson);
            

            try
            {
                var joinData = JsonSerializer.Deserialize<JsonElement>(joinResponse);
                if (joinData.TryGetProperty("status", out var status) && status.GetInt32() == 1)
                {
                   
                    var waitWindow = new RoomWaitWindow(selectedRoom.id);
                    waitWindow.Show();

                 
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Failed to join the room.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error joining room: " + ex.Message + "\nResponse: " + joinResponse);
            }
        }


        protected override void OnClosed(EventArgs e)
        {
            _refreshTimer.Stop();
            base.OnClosed(e);
        }
    }
}
