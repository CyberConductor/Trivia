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
        public class CreateRoomResponse
        {
            public int id { get; set; }
            public int status { get; set; }
        }

        private void CreateRoomButton_Click(object sender, RoutedEventArgs e)
        {
            string roomName = RoomNameTextBox.Text;
            string questionTimeStr = QuestionTimeTextBox.Text;
            string playersCountStr = PlayersCountTextBox.Text;
            string questionCountStr = QuestionCountTextBox.Text;

            // Validate input
            if (string.IsNullOrWhiteSpace(roomName) ||
                !int.TryParse(questionTimeStr, out int questionTime) ||
                !int.TryParse(playersCountStr, out int playersCount) ||
                !int.TryParse(questionCountStr, out int questionCount))
            {
                MessageBox.Show("Please enter valid values for all fields.", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            // Build the JSON payload
            var payload = JsonSerializer.Serialize(new
            {
                roomName = roomName,
                answerTimeOut = questionTime,
                questionCount = questionCount,
                maxUsers = playersCount
            });

            // Send request to server
            string response = App.Communicator.SendRequest((byte)Requests.Request_CreateRoom, payload);
            
            try
            {
                var roomResponse = JsonSerializer.Deserialize<CreateRoomResponse>(response);

                if (roomResponse.status == 1)
                {
                    RoomWaitWindow waitRoom = new RoomWaitWindow(roomResponse.id, questionTime, questionCount)
                    {

                    };
                    waitRoom.Show();
                    this.Close(); 
                }
                else
                {
                    MessageBox.Show("Failed to create room. Please try again.", "Server Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Invalid response from server:\n{ex.Message}", "Deserialization Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private void BackArrowButton_Click(object sender, RoutedEventArgs e)
        {;

            //RoomWaitWindow roomwait = new RoomWaitWindow();
            //roomwait.Show();
            //this.Close();
        }

    }
}

