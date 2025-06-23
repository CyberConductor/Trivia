using System;
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
        private string admin;

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
            string response = App.Communicator.SendRequest((byte)Requests.Request_GetRoomState, json);

            try
            {
                var data = JsonSerializer.Deserialize<JsonElement>(response);

                if (data.TryGetProperty("players", out JsonElement playersElement))
                {
                    var playersList = playersElement.EnumerateArray()
                        .Select(p => p.GetString())
                        .Where(p => !string.IsNullOrEmpty(p))
                        .ToList();

                    PlayersListBox.ItemsSource = playersList;

                    admin = playersList.FirstOrDefault() ?? "Unknown";
                    AdminTextBlock.Text = $"Admin: {admin}";

                    AdminControlsPanel.Visibility = (App.CurrentUser == admin)
                        ? Visibility.Visible
                        : Visibility.Collapsed;
                }

                if (data.TryGetProperty("hasGameBegun", out var started) && started.GetBoolean())
                {
                    refreshTimer.Stop();
                    MessageBox.Show("Game has started!");
                    // TODO: Open actual game window here
                    this.Close();
                }

                if (data.TryGetProperty("status", out var status) && status.GetInt32() == 0)
                {
                    refreshTimer.Stop();
                    MessageBox.Show("Room was closed.");
                    this.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error parsing room state: " + ex.Message);
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
