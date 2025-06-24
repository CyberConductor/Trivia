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
        private readonly DispatcherTimer refreshTimer;
        private readonly int roomId;
        private string admin = "";

        public RoomWaitWindow(int roomId)
        {
            InitializeComponent();
            this.roomId = roomId;

            refreshTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(3)
            };
            refreshTimer.Tick += RefreshRoomState;
            refreshTimer.Start();

            RefreshRoomState(null, null); // Immediate load
        }

        private void RefreshRoomState(object sender, EventArgs e)
        {
            // First: Get players in the room
            string playerJson = JsonSerializer.Serialize(new { roomId = this.roomId });
            string playerResponse = App.Communicator.SendRequest((byte)Requests.Request_GetPlayersInRoom, playerJson);

            try
            {
                var playersData = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(playerResponse);

                if (playersData != null && playersData.TryGetValue("players", out JsonElement playersElement))
                {
                    var playersList = playersElement.EnumerateArray()
                        .Select(p => p.GetString())
                        .Where(p => !string.IsNullOrEmpty(p))
                        .ToList();

                    PlayersListBox.ItemsSource = playersList;

                    if (playersList.Count > 0)
                    {
                        admin = playersList[0];
                        AdminTextBlock.Text = $"Admin: {admin}";
                        AdminControlsPanel.Visibility = (App.CurrentUser == admin) ? Visibility.Visible : Visibility.Collapsed;
                    }
                }
                else
                {
                    AdminTextBlock.Text = "No players found.";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error getting players:\n" + ex.Message + "\nRaw response:\n" + playerResponse);
                return;
            }

            // Second: Check if game has started or room is closed
            string stateJson = JsonSerializer.Serialize(new { roomId = this.roomId });
            string stateResponse = App.Communicator.SendRequest((byte)Requests.Request_GetRoomState, stateJson);

            try
            {
                var stateData = JsonSerializer.Deserialize<JsonElement>(stateResponse);

                if (stateData.TryGetProperty("hasGameBegun", out JsonElement started) && started.GetBoolean())
                {
                    refreshTimer.Stop();
                    MessageBox.Show("Game has started!");
                    // TODO: Open game window
                    this.Close();
                }

                if (stateData.TryGetProperty("status", out JsonElement status) && status.GetInt32() == 0)
                {
                    refreshTimer.Stop();
                    MessageBox.Show("Room was closed.");
                    this.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error checking room state:\n" + ex.Message + "\nRaw response:\n" + stateResponse);
            }
        }


        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string response = App.Communicator.SendRequest((byte)Requests.Request_StartGame, json);
        }

        private void CloseRoomButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string response = App.Communicator.SendRequest((byte)Requests.Request_CloseRoom, json);
            refreshTimer.Stop();
            MessageBox.Show("Room closed.");
            this.Close();
        }

        private void LeaveRoomButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string response = App.Communicator.SendRequest((byte)Requests.Request_LeaveRoom, json);

            try
            {
                var data = JsonSerializer.Deserialize<JsonElement>(response);
                if (data.TryGetProperty("status", out var status) && status.GetInt32() == 1)
                {
                    refreshTimer.Stop();
                    MessageBox.Show("You have left the room.");
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Failed to leave the room.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error processing leave room response: " + ex.Message);
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            refreshTimer?.Stop();
            base.OnClosed(e);
        }
    }
}
