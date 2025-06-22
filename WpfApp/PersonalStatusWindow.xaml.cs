using System;
using System.Text.Json;
using System.Windows;
using TriviaClient.Network;
using WpfApp;

namespace WpfApp1
{
    public partial class PersonalStatusWindow : Window
    {
        public PersonalStatusWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Request payload is empty for getPersonalStats
            var requestPayload = new { };

            string json = JsonSerializer.Serialize(requestPayload);
            byte requestCode = (byte)Requests.Request_GetPersonalStats;

            // Send request and receive response from server
            string response = App.Communicator.SendRequest(requestCode, json);

            try
            {
                var options = new JsonSerializerOptions
                {
                    PropertyNameCaseInsensitive = true
                };

                PersonalStatsResponse statsResponse = JsonSerializer.Deserialize<PersonalStatsResponse>(response, options);

                if (statsResponse != null && statsResponse.status == 1)
                {
                    UserStatistics stats = statsResponse.statistics;

                    PersonalStatsTextBlock.Text =
                        $"Username: {stats.username}\n" +
                        $"Average Answer Time: {stats.averageAnswerTime:F2} sec\n" +
                        $"Total Answers: {stats.totalAnswers}\n" +
                        $"Correct Answers: {stats.correctAnswers}\n" +
                        $"Games Played: {stats.gamesPlayed}\n" +
                        $"Total Score: {stats.totalScore}";
                }
                else
                {
                    PersonalStatsTextBlock.Text = "Failed to load statistics. Try again later.";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to parse server response:\n{ex.Message}", "Error");
            }
        }
    }

    public class PersonalStatsResponse
    {
        public int status { get; set; }
        public UserStatistics statistics { get; set; }
    }

    public class UserStatistics
    {
        public string username { get; set; }
        public double averageAnswerTime { get; set; }
        public int totalAnswers { get; set; }
        public int correctAnswers { get; set; }
        public int gamesPlayed { get; set; }
        public int totalScore { get; set; }
    }
}
