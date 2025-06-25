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
                Interval = TimeSpan.FromSeconds(60)
            };
            refreshTimer.Tick += RefreshRoomState;
            refreshTimer.Start();

            RefreshRoomState(null, null); // Initial immediate refresh
        }

        private void RefreshRoomState(object sender, EventArgs e)
        {
            try
            {
                string stateJson = JsonSerializer.Serialize(new { roomId = this.roomId });
                string stateResponse = App.Communicator.SendRequest((byte)Requests.Request_GetRoomState, stateJson);
                var stateData = JsonSerializer.Deserialize<JsonElement>(stateResponse);

                // Update players and admin controls
                if (stateData.TryGetProperty("players", out JsonElement playersElement))
                {
                    var playersList = playersElement.EnumerateArray()
                        .Select(p => p.GetString())
                        .Where(p => !string.IsNullOrEmpty(p))
                        .ToList();

                    Dispatcher.Invoke(() =>
                    {
                        PlayersListBox.ItemsSource = null;
                        PlayersListBox.ItemsSource = playersList;

                        if (playersList.Count > 0)
                        {
                            admin = playersList[0];
                            AdminTextBlock.Text = $"Admin: {admin}";
                            AdminControlsPanel.Visibility = (App.CurrentUser == admin)
                                ? Visibility.Visible
                                : Visibility.Collapsed;
                        }
                        else
                        {
                            AdminTextBlock.Text = "No players found.";
                            AdminControlsPanel.Visibility = Visibility.Collapsed;
                        }
                    });
                }

                // Check if game started
                if (stateData.TryGetProperty("hasGameBegun", out JsonElement started) && started.GetBoolean())
                {
                    refreshTimer.Stop();
                    GoToGame();
                    return;
                }

                // Check if room closed
                if (stateData.TryGetProperty("status", out JsonElement status) && status.GetInt32() == 0)
                {
                    refreshTimer.Stop();
                    GoToMenu("Room was closed.");
                    return;
                }
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(() =>
                {
                    MessageBox.Show("Error refreshing room state:\n" + ex.Message);
                });
            }
        }

        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string res = App.Communicator.SendRequest((byte)Requests.Request_StartGame, json);
            MessageBox.Show(res);
        }

        private void CloseRoomButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
			
            string response = App.Communicator.SendRequest((byte)Requests.Request_CloseRoom, json);
            MessageBox.Show(response);

            refreshTimer.Stop();
            GoToMenu("Room was closed.");
        }

        private void LeaveRoomButton_Click(object sender, RoutedEventArgs e)
        {
            refreshTimer.Stop(); // Stop before making network calls

            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string response;

            try
            {
                response = App.Communicator.SendRequest((byte)Requests.Request_LeaveRoom, json);

                var data = JsonSerializer.Deserialize<JsonElement>(response);
                if (data.TryGetProperty("status", out var status) && status.GetInt32() == 1)
                {
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
