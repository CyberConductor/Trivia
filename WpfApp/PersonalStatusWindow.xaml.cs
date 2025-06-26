using SimpleWpfGui;
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
        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
           
            Statistics prst = new Statistics();
            prst.Show();
            this.Close();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            var requestPayload = new { };
            string json = JsonSerializer.Serialize(requestPayload);
            byte requestCode = (byte)Requests.Request_GetPersonalStats;
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
                    var stats = statsResponse.statistics;

                    UsernameText.Text = stats.username;
                    AvgAnswerTimeText.Text = $"{stats.averageAnswerTime:F2} sec";
                    TotalAnswersText.Text = stats.totalAnswers.ToString();
                    CorrectAnswersText.Text = stats.correctAnswers.ToString();
                    GamesPlayedText.Text = stats.gamesPlayed.ToString();
                    TotalScoreText.Text = stats.totalScore.ToString();
                }
                else
                {
                    MessageBox.Show("Failed to load personal stats.", "Error");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error parsing server response:\n{ex.Message}", "Error");
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
}
