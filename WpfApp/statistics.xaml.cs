using System;
using System.Windows;
using WpfApp1;

namespace WpfApp
{
    public partial class Statistics : Window
    {
        public Statistics()
        {
            InitializeComponent();
        }

        private void HighScoresButton_Click(object sender, RoutedEventArgs e)
        {
            HighScore highscore = new HighScore();
            highscore.Show();
            this.Close();
        }

        private void PersonalStatsButton_Click(object sender, RoutedEventArgs e)
        {
            PersonalStatusWindow stWindow = new PersonalStatusWindow();
            stWindow.Show();
            this.Close();

        }
        private void Back_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu(); 
            menu.Show();
            this.Close();
        }



        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu();
            menu.Show();
            this.Close();
        }
    }
}
