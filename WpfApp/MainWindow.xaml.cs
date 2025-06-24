using System.Windows;
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
    }
}
