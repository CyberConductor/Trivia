using System.Windows;
using WpfApp1;

namespace WpfApp
{
    public partial class Menu : Window
    {
        public Menu()
        {
            InitializeComponent();
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
    }
}
