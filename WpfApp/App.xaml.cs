using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using TriviaClient.Network;

namespace WpfApp
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static ServerCommunicator Communicator { get; private set; }

        protected override void OnStartup(StartupEventArgs e)
        {
            Communicator = new ServerCommunicator();
            base.OnStartup(e);
        }
    }
}
