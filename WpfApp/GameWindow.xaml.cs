using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;

namespace WpfApp
{
    public partial class GameWindow : Window
    {
        private int roomId;
        private List<string> players;
        private int questionCount;
        private int answerTimeOut;

        private int currentQuestionIndex = 0;
        private Dictionary<int, string> currentAnswers = new();

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
            currentQuestionIndex = 0;
            LoadNextQuestion();
        }

        private void LoadNextQuestion()
        {
            if (currentQuestionIndex >= questionCount)
            {
                ShowGameResults();
                return;
            }

            try
            {
                string response = App.Communicator.SendRequest((byte)Requests.Request_GetQuestion, "{}");
                MessageBox.Show(response);
                var jsonDoc = JsonDocument.Parse(response);
                var root = jsonDoc.RootElement;

                if (root.GetProperty("status").GetInt32() == 1)
                {
                    string question = root.GetProperty("question").GetString();
                    var answersJson = root.GetProperty("answers").EnumerateObject();

                    currentAnswers = answersJson.ToDictionary(
                        a => int.Parse(a.Name),
                        a => a.Value.GetString()
                    );

                    QuestionTextBlock.Text = $"Q{currentQuestionIndex + 1}: {question}";
                    AnswersListBox.ItemsSource = currentAnswers.ToList();
                    AnswersListBox.DisplayMemberPath = "Value";
                    AnswersListBox.SelectedValuePath = "Key";
                    AnswersListBox.SelectedIndex = -1;

                    TimerTextBlock.Text = $"Question {currentQuestionIndex + 1} of {questionCount}";
                }
                else
                {
                    string error = root.TryGetProperty("message", out var msg) ? msg.GetString() : "Failed to load question.";
                    MessageBox.Show(error);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error loading question: " + ex.Message);
            }
        }

        private void SubmitAnswerButton_Click(object sender, RoutedEventArgs e)
        {
            if (AnswersListBox.SelectedItem is KeyValuePair<int, string> selectedAnswer)
            {
                int answerId = selectedAnswer.Key;

                var json = JsonSerializer.Serialize(new
                {
                    answerId = answerId,
                    roomId = roomId
                });

                try
                {
                    string response = App.Communicator.SendRequest((byte)Requests.Request_SubmitAnswer, json);


                    currentQuestionIndex++;
                    LoadNextQuestion();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error submitting answer: " + ex.Message);
                }
            }
            else
            {
                MessageBox.Show("Please select an answer before submitting.");
            }
        }

        private void LeaveGameButton_Click(object sender, RoutedEventArgs e)
        {
            var result = MessageBox.Show("Are you sure you want to leave the game?", "Confirm Leave", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (result == MessageBoxResult.Yes)
            {
                try
                {
                    string leaveResponse = App.Communicator.SendRequest((byte)Requests.Request_LeaveGame, "{}");


                    Menu menu = new Menu();
                    menu.Show();
                    this.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error leaving game: " + ex.Message);
                }
            }
        }

        private void ShowGameResults()
        {
            try
            {
                string response = App.Communicator.SendRequest((byte)Requests.Request_GetGameResults, "{}");
                var jsonDoc = JsonDocument.Parse(response);
                var root = jsonDoc.RootElement;

                if (root.TryGetProperty("status", out var statusProp) && statusProp.GetInt32() == 1 &&
                    root.TryGetProperty("results", out var resultsElement))
                {
                    List<string> scores = new();

                    foreach (var player in resultsElement.EnumerateArray())
                    {
                        string username = player.GetProperty("username").GetString();
                        int correct = player.GetProperty("correctAnswersCount").GetInt32();
                        int wrong = player.GetProperty("wrongAnswersCount").GetInt32();

                        scores.Add($"{username}: ✔️ {correct} | ❌ {wrong}");
                    }

                    string message = "Game Over!\n\nResults:\n" + string.Join("\n", scores);
                    MessageBox.Show(message, "Game Results");
                }
                else
                {
                    MessageBox.Show("Failed to retrieve game results.");
                }

                string leaveResponse = App.Communicator.SendRequest((byte)Requests.Request_LeaveGame, "{}");

                Menu menu = new Menu();
                menu.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error getting game results or leaving game: " + ex.Message);
            }
        }
    }
}
