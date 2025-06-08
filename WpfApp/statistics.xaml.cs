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


        private void PersonalStatsButton_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Personal statistics window would open here.");
           
        }

        private void HighScoresButton_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("High scores table would open here.");
        }
    }
}
