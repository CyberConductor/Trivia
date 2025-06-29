using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

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

        private DispatcherTimer questionTimer;
        private int secondsRemaining;
        private DateTime questionStartTime;

        private int correctAnswersCount = 0;

        public GameWindow(int roomId, List<string> players, int questionCount, int answerTimeOut)
        {
            InitializeComponent();

            this.roomId = roomId;
            this.players = players;
            this.questionCount = questionCount;
            this.answerTimeOut = answerTimeOut;

            questionTimer = new DispatcherTimer();
            questionTimer.Interval = TimeSpan.FromSeconds(1);
            questionTimer.Tick += QuestionTimer_Tick;

            InitializeGame();
        }

        private void InitializeGame()
        {
            currentQuestionIndex = 0;
            correctAnswersCount = 0;
            CorrectAnswersTextBlock.Text = $"Correct answers: {correctAnswersCount}";
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
                var jsonDoc = JsonDocument.Parse(response);
                var root = jsonDoc.RootElement;

                if (root.GetProperty("status").GetInt32() == 1)
                {
                    string question = root.GetProperty("question").GetString();

                    currentAnswers = root.EnumerateObject()
                        .Where(p => int.TryParse(p.Name, out _))
                        .ToDictionary(p => int.Parse(p.Name), p => p.Value.GetString());

                    QuestionTextBlock.Text = $"Q{currentQuestionIndex + 1}: {question}";
                    AnswersListBox.ItemsSource = currentAnswers.ToList();
                    AnswersListBox.DisplayMemberPath = "Value";
                    AnswersListBox.SelectedValuePath = "Key";
                    AnswersListBox.SelectedIndex = -1;

                    TimerTextBlock.Text = $"Question {currentQuestionIndex + 1} of {questionCount}";

                    // Start timer
                    secondsRemaining = answerTimeOut;
                    questionStartTime = DateTime.Now;
                    TimeRemainingTextBlock.Text = $"Time left: {secondsRemaining}s";

                    questionTimer.Start();
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

        private void QuestionTimer_Tick(object sender, EventArgs e)
        {
            secondsRemaining--;
            TimeRemainingTextBlock.Text = $"Time left: {secondsRemaining}s";

            if (secondsRemaining <= 0)
            {
                questionTimer.Stop();
                MessageBox.Show("Time's up! Moving to next question.");

                var json = JsonSerializer.Serialize(new
                {
                    answerId = -1,
                    timeTaken = 0
                });
                App.Communicator.SendRequest((byte)Requests.Request_SubmitAnswer, json);

                currentQuestionIndex++;
                LoadNextQuestion();
            }
        }

        private void SubmitAnswerButton_Click(object sender, RoutedEventArgs e)
        {
            if (AnswersListBox.SelectedItem is KeyValuePair<int, string> selectedAnswer)
            {
                questionTimer.Stop();

                int answerId = selectedAnswer.Key;
                double timeTakenSeconds = (DateTime.Now - questionStartTime).TotalSeconds;

                var json = JsonSerializer.Serialize(new
                {
                    answerId = answerId,
                    timeTaken = Math.Round(timeTakenSeconds, 2)
                });

                try
                {
                    string response = App.Communicator.SendRequest((byte)Requests.Request_SubmitAnswer, json);

                    var jsonDoc = JsonDocument.Parse(response);
                    var root = jsonDoc.RootElement;

                    if (root.TryGetProperty("correctAnswerId", out var correctAnswerIdProp))
                    {
                        int correctAnswerId = correctAnswerIdProp.GetInt32();

                        if (answerId == correctAnswerId)
                        {
                            correctAnswersCount++;
                            CorrectAnswersTextBlock.Text = $"Correct answers: {correctAnswersCount}";
                        }
                    }

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
                    questionTimer.Stop();
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
                questionTimer.Stop();

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
                        double avgTime = root.GetProperty("averageAnswerTime").GetDouble();

                        scores.Add($"{username}: Correct: {correct} | Wrong: {wrong} | Avg Time: {avgTime:F2} sec");
                    }

                    string message = "Game Over!\n\nResults:\n" + string.Join("\n", scores);

                    // Determine winner by highest correct answers (tie possible)
                    int maxCorrect = resultsElement.EnumerateArray().Max(p => p.GetProperty("correctAnswersCount").GetInt32());
                    var winners = resultsElement.EnumerateArray()
                        .Where(p => p.GetProperty("correctAnswersCount").GetInt32() == maxCorrect)
                        .Select(p => p.GetProperty("username").GetString())
                        .ToList();

                    message += "\n\nWinner(s): " + string.Join(", ", winners);

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
