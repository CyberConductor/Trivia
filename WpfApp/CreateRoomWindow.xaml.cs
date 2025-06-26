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

            //check if values are valid:
            if (string.IsNullOrWhiteSpace(roomName) ||
                !int.TryParse(questionTimeStr, out int questionTime) ||
                !int.TryParse(playersCountStr, out int playersCount))
            {
                MessageBox.Show("Please enter valid values for all fields.", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            //build the JSON payload
            var payload = JsonSerializer.Serialize(new
            {
                roomName = roomName,
                answerTimeOut = questionTime,
                questionCount = 5, 
                maxUsers = playersCount
            });

            //send code to server
            string response = App.Communicator.SendRequest((byte)Requests.Request_CreateRoom, payload);

            MessageBox.Show(response, "Server Response", MessageBoxButton.OK, MessageBoxImage.Information);
        }
        private void BackArrowButton_Click(object sender, RoutedEventArgs e)
        {;

            //RoomWaitWindow roomwait = new RoomWaitWindow();
            //roomwait.Show();
            //this.Close();
        }

    }
}

