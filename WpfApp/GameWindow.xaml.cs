using System.Collections.Generic;
using System.Windows;

namespace WpfApp
{
    public partial class GameWindow : Window
    {
        private int roomId;
        private List<string> players;
        private int questionCount;
        private int answerTimeOut;

        public GameWindow(int roomId, List<string> players, int questionCount, int answerTimeOut)
        {
            InitializeComponent();

            this.roomId = roomId;
            this.players = players;
            this.questionCount = questionCount;
            this.answerTimeOut = answerTimeOut;

            InitializeGame();
        }

        private void InitializeGame()
        {
            //part 4
        }
    }
}
