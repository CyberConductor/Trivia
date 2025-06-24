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
            try
            {
                string stateJson = JsonSerializer.Serialize(new { roomId = this.roomId });
                string stateResponse = App.Communicator.SendRequest((byte)Requests.Request_GetRoomState, stateJson);
                var stateData = JsonSerializer.Deserialize<JsonElement>(stateResponse);

                // Update players list from "players" property
                if (stateData.TryGetProperty("players", out JsonElement playersElement))
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
                    else
                    {
                        AdminTextBlock.Text = "No players found.";
                        AdminControlsPanel.Visibility = Visibility.Collapsed;
                    }
                }

                // Check if the game has started
                if (stateData.TryGetProperty("hasGameBegun", out JsonElement started) && started.GetBoolean())
                {
                    refreshTimer.Stop();
                    GoToGame();
                    return;
                }

                // Check if the room was closed
                if (stateData.TryGetProperty("status", out JsonElement status) && status.GetInt32() == 0)
                {
                    refreshTimer.Stop();
                    GoToMenu("Room was closed.");
                    return;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error refreshing room state:\n" + ex.Message);
            }
        }

        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string response = App.Communicator.SendRequest((byte)Requests.Request_StartGame, json);

            // Do NOT open game window here — wait for RefreshRoomState to detect change
            // You can optionally parse response and show error messages if needed
        }

        private void CloseRoomButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string response = App.Communicator.SendRequest((byte)Requests.Request_CloseRoom, json);

            refreshTimer.Stop();
            GoToMenu("Room was closed.");
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
                    GoToMenu();
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

        private void GoToGame()
        {
            string stateJson = JsonSerializer.Serialize(new { roomId = this.roomId });
            string stateResponse = App.Communicator.SendRequest((byte)Requests.Request_GetRoomState, stateJson);
            var stateData = JsonSerializer.Deserialize<JsonElement>(stateResponse);

            List<string> playersList = new List<string>();
            int questionCount = 5;
            int answerTimeOut = 10;

            if (stateData.TryGetProperty("players", out JsonElement playersElement))
            {
                playersList = playersElement.EnumerateArray()
                    .Select(p => p.GetString())
                    .Where(p => !string.IsNullOrEmpty(p))
                    .ToList();
            }

            if (stateData.TryGetProperty("questionCount", out JsonElement questionCountElement))
            {
                questionCount = questionCountElement.GetInt32();
            }

            if (stateData.TryGetProperty("answerTimeOut", out JsonElement answerTimeOutElement))
            {
                answerTimeOut = answerTimeOutElement.GetInt32();
            }

            GameWindow gameWindow = new GameWindow(this.roomId, playersList, questionCount, answerTimeOut);
            gameWindow.Show();
            this.Close();
        }


        private void GoToMenu(string message = null)
        {
            if (!string.IsNullOrEmpty(message))
            {
                MessageBox.Show(message);
            }

            Menu menu = new Menu();
            menu.Show();
            this.Close();
        }

        protected override void OnClosed(EventArgs e)
        {
            refreshTimer?.Stop();
            base.OnClosed(e);
        }
    }
}
