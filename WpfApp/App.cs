using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace WpfApp
{
    public partial class App : Application  // 👈 Must inherit from Application
    {
        public static string CurrentUser { get; set; }
    }
}
