using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Text.Json;
using TriviaClient.Network;
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
                string responseJson = ServerCommunicator.SendRequest((byte)Requests.Request_GetHighScore, "{}");

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
