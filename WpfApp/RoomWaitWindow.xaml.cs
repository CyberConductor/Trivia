using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Threading;
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
        private bool hasGameBegun;

        private Thread listenerThread;
        private bool keepListening = true;


        public RoomWaitWindow(int roomId, int timeOut, int questionCount)
        {
            InitializeComponent();
            this.roomId = roomId;
            this.timeOut = timeOut;
            this.questionCount = questionCount;

            refreshTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(100)
            };
            refreshTimer.Tick += RefreshRoomState;
            refreshTimer.Start();

            listenerThread = new Thread(ListenToServer);
            listenerThread.Start();

            RefreshRoomState(null, null); // Initial immediate refresh
        }

        private void RefreshRoomState(object sender, EventArgs e)
        {
            if (!keepListening)
                return;

            try
            {
                string stateJson = JsonSerializer.Serialize(new { roomId = this.roomId });
                string stateResponse = App.Communicator.SendRequest((byte)Requests.Request_GetRoomState, stateJson);
                var stateData = JsonSerializer.Deserialize<JsonElement>(stateResponse);

                stateData.TryGetProperty("hasGameBegun", out JsonElement begun);
                this.hasGameBegun = begun.GetBoolean();

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
                
                else if (stateData.TryGetProperty("status", out JsonElement status) && status.GetInt32() == 0)
                {
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

        private void ListenToServer()
        {
            while (keepListening)
            {
                try
                {
                    string response = App.Communicator.TryReadMessage(); 

                    if (!string.IsNullOrEmpty(response))
                    {
                        byte code = (byte)response[0];
                        string json = response.Substring(1);

                        switch (code)
                        {
                            case (byte)Responses.Response_StartGame:
                                Dispatcher.Invoke(() =>
                                {
                                    GoToGame();
                                });
                                break;

                            case (byte)Responses.Response_LeaveRoom:
                                Dispatcher.Invoke(() =>
                                {
                                    GoToMenu("Room was closed.");
                                });
                                break;

                            default:
                                // other codes – you can log or ignore
                                break;
                        }
                    }

                    Thread.Sleep(200); // don't overheat CPU
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error processing checking server messages: " + ex.Message);
                    GoToMenu();
                }
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
                    GoToGame();
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

        private void GoToMenu(string? message = default)
        {
            keepListening = false;
            refreshTimer.Stop();

            if (!string.IsNullOrEmpty(message))
            {
                MessageBox.Show(message);
            }

            Menu menu = new Menu();
            menu.Show();
            this.Close();
        }

        private void GoToGame()
        {
            keepListening = false;
            refreshTimer.Stop();
            GameWindow gameWindow = new GameWindow(questionCount, timeOut, this);
            gameWindow.Show();
            this.Close();
        }

        protected override void OnClosed(EventArgs e)
        {
            keepListening = false;
            refreshTimer?.Stop();
            base.OnClosed(e);
        }
    }
}