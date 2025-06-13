using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Windows;
using System.Windows.Threading;
using TriviaClient.Network;

namespace WpfApp
{
    public partial class RoomWaitWindow : Window
    {
        private DispatcherTimer refreshTimer;
        private int roomId;

        public RoomWaitWindow(int roomId)
        {
            InitializeComponent();
            this.roomId = roomId;

            refreshTimer = new DispatcherTimer();
            refreshTimer.Interval = TimeSpan.FromSeconds(3);
            refreshTimer.Tick += RefreshRoomState;
            refreshTimer.Start();
        }

        private void RefreshRoomState(object sender, EventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });

            string response = ServerCommunicator.SendRequest((byte)Requests.Request_GetRoomState, json);

            try
            {
                var data = JsonSerializer.Deserialize<JsonElement>(response);

                if (data.TryGetProperty("players", out JsonElement playersElement))
                {
                    var playersList = playersElement.EnumerateArray().Select(p => p.GetString()).ToList();
                    PlayersListBox.ItemsSource = playersList;
                }

                if (data.TryGetProperty("hasGameBegun", out var started) && started.GetBoolean())
                {
                    MessageBox.Show("Game has started!");
                    refreshTimer.Stop();
                    //open new game window
                    //
                }

                if (data.TryGetProperty("status", out var status) && status.GetInt32() == 0)
                {
                    MessageBox.Show("Room was closed.");
                    refreshTimer.Stop();
                    this.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error parsing room state: " + ex.Message);
            }
        }
    }
}
