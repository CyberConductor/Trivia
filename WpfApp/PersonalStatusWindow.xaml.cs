using System.Text.Json;
using System.Windows;
using TriviaClient.Network; // <-- Ensure you have a reference to this namespace
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
            
            var requestPayload = new
            {
                
            };

            string json = JsonSerializer.Serialize(requestPayload);

            byte requestCode = (byte)Requests.Request_GetPersonalStats;

            string response = App.Communicator.SendRequest(requestCode, json);

            PersonalStatsTextBlock.Text = response;
            MessageBox.Show(response, "Server Response");
        }
    }
}
