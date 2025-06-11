using System;
using System.Text.Json;
using System.Windows;
using TriviaClient.Network;
using WpfApp;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        public CreateRoomWindow()
        {
            InitializeComponent();
        }

        private void CreateRoomButton_Click(object sender, RoutedEventArgs e)
        {
            string roomName = RoomNameTextBox.Text;
            string questionTimeStr = QuestionTimeTextBox.Text;
            string playersCountStr = PlayersCountTextBox.Text;

            // Basic validation
            if (string.IsNullOrWhiteSpace(roomName) ||
                !int.TryParse(questionTimeStr, out int questionTime) ||
                !int.TryParse(playersCountStr, out int playersCount))
            {
                MessageBox.Show("Please enter valid values for all fields.", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            // Build the JSON payload
            var payload = JsonSerializer.Serialize(new
            {
                roomName = roomName,
                answerTimeOut = questionTime,
                questionCount = 5, // or make this user-input later
                maxUsers = playersCount
            });

            // Send to server
            string response = ServerCommunicator.SendRequest((byte)Requests.Request_CreateRoom, payload);

            MessageBox.Show(response, "Server Response", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
