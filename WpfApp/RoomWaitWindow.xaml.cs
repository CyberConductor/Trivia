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
        private readonly int timeOut;
        private readonly int questionCount;
        private string admin = "";

        public RoomWaitWindow(int roomId, int timeOut, int questionCount)
        {
            InitializeComponent();
            this.roomId = roomId;
            this.timeOut = timeOut;
            this.questionCount = questionCount;

            refreshTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(3)
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

                            bool isAdmin = (App.CurrentUser == admin);

                            AdminControlsPanel.Visibility = isAdmin
                                ? Visibility.Visible
                                : Visibility.Collapsed;

                            LeaveRoomButton.Visibility = isAdmin
                                ? Visibility.Collapsed
                                : Visibility.Visible;
                        }
                        else
                        {
                            AdminTextBlock.Text = "No players found.";
                            AdminControlsPanel.Visibility = Visibility.Collapsed;
                            LeaveRoomButton.Visibility = Visibility.Visible;
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
                    //means that room was closed:
                    if (ex.Message.Contains("'E' is an invalid start of a value"))
                    {
                        GoToMenu("Room was closed or no longer exists.");
                    }
                    else
                    {
                        MessageBox.Show("Error refreshing room state:\n" + ex.Message);
                    }
                });
            }
        }

        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { roomId = this.roomId });
            string res = App.Communicator.SendRequest((byte)Requests.Request_StartGame, json);

            try
            {
                var data = JsonSerializer.Deserialize<JsonElement>(res);

                if (data.TryGetProperty("status", out JsonElement status) && status.GetInt32() == 1)
                {
                    GameWindow gameWindow = new GameWindow(questionCount, timeOut);
                    gameWindow.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Failed to start game. Server response: " + res);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error parsing server response: " + ex.Message);
            }
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
            refreshTimer.Stop(); 

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
