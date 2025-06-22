using System;
using System.Windows;

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

        }

        private void PersonalStatsButton_Click(object sender, RoutedEventArgs e)
        {
            
           
        }

        
        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu();
            menu.Show();
            this.Close();
        }
    }
}
