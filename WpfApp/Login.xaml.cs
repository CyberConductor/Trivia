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
using System.Windows.Shapes;
using System.Text.Json;
using TriviaClient.Network;
using WpfApp1;

namespace WpfApp
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Login : Window
    {
        public Login()
        {
            InitializeComponent();
        }

        private void SignUp_Click(object sender, RoutedEventArgs e)
        {
            SignUp signupWindow = new SignUp();
            signupWindow.Show();
            this.Close();
        }

        private async void LoginButton_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameBox.Text.Trim();
            string password = PasswordBox.Password;

            if (string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))
            {
                StatusTextBlock.Text = "Please enter username and password.";
                return;
            }

            StatusTextBlock.Text = "Logging in...";
            bool success = false;

            try
            {
              
                success = await Task.Run(() =>
                {
                    var payload = new { username = username, password = password };
                    string json = JsonSerializer.Serialize(payload);
                    string response = App.Communicator.SendRequest((byte)Requests.Request_Login, json);

                    
                    //deserialize response
                    var respObj = JsonSerializer.Deserialize<JsonElement>(response);

                    if (respObj.TryGetProperty("status", out JsonElement statusElement) && statusElement.GetInt32() == 1)
                        return true;

                    return false;
                });
            }
            catch (Exception ex)
            {
                StatusTextBlock.Text = "Error: " + ex.Message;
                return;
            }

            if (success)
            {
                StatusTextBlock.Foreground = Brushes.Green;
                StatusTextBlock.Text = "Login successful!";

                Menu menuWindow = new Menu();
                menuWindow.Show();
                this.Close();
            }

            else
            {
                StatusTextBlock.Foreground = System.Windows.Media.Brushes.Red;
                StatusTextBlock.Text = "Login failed. Check your credentials.";
            }
        }
    }
}

