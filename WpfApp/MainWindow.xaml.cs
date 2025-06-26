using System.Transactions;
using System.Windows;
using TriviaClient.Network;
using WpfApp;
using WpfApp1;

namespace SimpleWpfGui
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void RegisterButton_Click(object sender, RoutedEventArgs e)
        {
            SignUp signupWindow = new SignUp();
            signupWindow.Show();
            this.Close();
        }

        private void LoginButton_Click(object sender, RoutedEventArgs e)
        {
            Login loginWindow = new Login();
            loginWindow.Show();
            this.Close();
        }

        private void CreateRoomButton_Click(object sender, RoutedEventArgs e)
        {
            CreateRoomWindow room = new CreateRoomWindow();
            room.Show();
            this.Close();
        }

        private void JoinRoomButton_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom joinRoomWindow = new JoinRoom();
            joinRoomWindow.Show();
            this.Close();
        }

        private void StatisticsButton_Click(object sender, RoutedEventArgs e)
        {
            Statistics statsWindow = new Statistics();
            statsWindow.Show();
            this.Close();
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}
