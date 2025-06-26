using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using TriviaClient.Network;
using WpfApp;

namespace WpfApp1
{
    public partial class SignUp : Window
    {
        public SignUp()
        {
            InitializeComponent();
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            Login loginWindow = new Login(); 
            loginWindow.Show();
            this.Close();
        }

        private void RemoveText(object sender, RoutedEventArgs e)
        {
            TextBox tb = sender as TextBox;
            if (tb != null && tb.Foreground == Brushes.Gray)
            {
                tb.Text = "";
                tb.Foreground = Brushes.Black;
            }
        }

        private void AddText(object sender, RoutedEventArgs e)
        {
            TextBox tb = sender as TextBox;
            if (tb != null && string.IsNullOrWhiteSpace(tb.Text))
            {
                tb.Foreground = Brushes.Gray;

                if (tb == UsernameBox)
                    tb.Text = "Username";
                else if (tb == EmailBox)
                    tb.Text = "Email";
            }
        }

        private void SignUp_Click(object sender, RoutedEventArgs e)
        {
            var username = UsernameBox.Text;
            var password = PasswordBox.Password;
            var email = EmailBox.Text;

            var payload = JsonSerializer.Serialize(new
            {
                username = username,
                password = password,
                email = email
            });

            string response = App.Communicator.SendRequest((int)Requests.Request_Signup, payload);

            MessageBox.Show(response);
        }
    }
}
