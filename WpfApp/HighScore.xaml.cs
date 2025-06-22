using System;
using System.Text.Json;
using System.Windows;
using TriviaClient.Network; // Ensure this namespace and class exist
using WpfApp;

namespace WpfApp1
{
    public partial class HighScore : Window
    {
        public HighScore()
        {
            InitializeComponent();
            LoadHighScores();
        }

        private void LoadHighScores()
        {
            try
            {
                
                string responseJson = App.Communicator.SendRequest((byte)Requests.Request_GetHighScore, "{}");

                using JsonDocument doc = JsonDocument.Parse(responseJson);
                JsonElement root = doc.RootElement;

                if (root.TryGetProperty("status", out JsonElement statusElem) && statusElem.GetInt32() == 1)
                {
                    if (root.TryGetProperty("statistics", out JsonElement scoresElem) && scoresElem.ValueKind == JsonValueKind.Array)
                    {
                        foreach (JsonElement playerElem in scoresElem.EnumerateArray())
                        {
                            ScoresListBox.Items.Add(playerElem.GetString());
                        }
                    }
                    else
                    {
                        MessageBox.Show("Failed to load high scores.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                }
                else
                {
                    MessageBox.Show("Server returned an error.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error loading high scores:\n" + ex.Message, "Exception", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
