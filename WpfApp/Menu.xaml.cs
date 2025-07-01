using System.Text.Json;
using System;
using System.Windows;
using WpfApp1;

namespace WpfApp
{
    public partial class Menu : Window
    {
        public Menu()
        {
            InitializeComponent();
            UpdateSignOutButtonVisibility();
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom joinroom = new JoinRoom();
            joinroom.Show();
            this.Close();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Create Room clicked");
            CreateRoomWindow createroom = new CreateRoomWindow();
            createroom.Show();
            this.Close();
        }

        //private void HighScore_Click(object sender, RoutedEventArgs e)
        //{
        //    //MessageBox.Show("High Score clicked");
        //    //HighScore highscore = new HighScore();
        //    //highscore.Show();
        //    //this.Close();
        //}

        private void Statistics_Click(object sender, RoutedEventArgs e)
        {
            Statistics statistics = new Statistics();
            statistics.Show();
            this.Close();


        }
        private void UpdateSignOutButtonVisibility()
        {
            SignOutButton.Visibility = string.IsNullOrEmpty(App.CurrentUser) ? Visibility.Collapsed : Visibility.Visible;
        }
        private void SignOutButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string json = JsonSerializer.Serialize(new { }); // if no params needed
                string response = App.Communicator.SendRequest((byte)Requests.Request_Signout, json);

                // Clear current user info
                App.CurrentUser = null;
                App.Communicator.Close();

                MessageBox.Show("You have signed out.");

                Login login = new Login();
                login.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Sign out failed: " + ex.Message);
            }
        }
    }
}