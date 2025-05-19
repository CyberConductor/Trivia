#include "qnData.h"
#include <iostream>
#include <string>
/*
function that prints the column names and their values from a database 
input:
    NotUsed - unused parameter for the callback signature
    argc - number of columns in the result row
    argv - array of strings for fields in the row
    azColName - array of strings for column names
output:
    int - always returns 0 to indicate success
*/
int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

void createTriviaTable(sqlite3* db) {
    std::string sql =
        "CREATE TABLE IF NOT EXISTS trivia_questions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "question TEXT NOT NULL,"
        "answer1 TEXT NOT NULL,"
        "answer2 TEXT NOT NULL,"
        "answer3 TEXT NOT NULL,"
        "answer4 TEXT NOT NULL,"
        "correct_answer INTEGER NOT NULL CHECK(correct_answer >= 1 AND correct_answer <= 4));";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Create table error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void insertSampleQuestions(sqlite3* db) {
    std::string sql =
        "INSERT INTO trivia_questions (question, answer1, answer2, answer3, answer4, correct_answer) VALUES "
        "('What is illegal to own in Switzerland?', 'A guinea pig', 'A cat with one eye', 'Left shoes', 'A hamster wheel', 1),"
        "('What does a kangaroo fart sound like?', 'Like thunder', 'Like applause', 'Like a flute', 'It doesn’t make a sound', 2),"
        "('What happens if you lift a kangaroo’s tail?', 'It can’t jump', 'It dances', 'It falls asleep', 'It turns red', 1),"
        "('What animal did NASA send to space wearing a tuxedo?', 'A monkey', 'A cat', 'A spider', 'None', 4),"
        "('How many chickens would it take to kill a lion?', 'One very smart chicken', 'About 500', 'Too many to count', 'Lions are afraid of chickens', 2),"
        "('What food can you legally marry in South Korea?', 'Kimchi', 'Rice', 'Pizza', 'No food', 4),"
        "('What animal can’t fart?', 'Octopus', 'Sloth', 'Shark', 'Giraffe', 4),"
        "('What happens when you microwave grapes?', 'They explode', 'They whistle', 'They create plasma', 'They dance', 3),"
        "('Why did a man marry his pillow?', 'It was cheaper', 'He lost a bet', 'He loved it', 'He wanted to be famous', 3),"
        "('What’s banned in Venice?', 'Feeding pigeons', 'Eating pasta outside', 'Wearing Crocs', 'Singing loudly', 1);";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Insert error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "Inserted sample questions.\n";
    }
}

void printAllQuestions(sqlite3* db) {
    std::string sql = "SELECT * FROM trivia_questions;";
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Select error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
