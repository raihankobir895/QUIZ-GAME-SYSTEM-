#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include <string>
#include <vector>

// Represents a single quiz question with four options and the correct answer.
// correctOption is 1-based: 1 = A, 2 = B, 3 = C, 4 = D
struct Question
{
    std::string questionText;
    std::string options[4];
    int correctOption;
};

// Handles creating, loading, validating and displaying the question bank.
// Questions are persisted in a plain text file (pipe-delimited), satisfying
// the "no database" file-handling requirement of the project.
class QuestionBank
{
public:
    explicit QuestionBank(const std::string &filePath);

    // Reads questions from filePath into memory, skipping/reporting invalid rows.
    bool loadQuestions();

    int getQuestionCount() const;
    const Question &getQuestion(int index) const;

    void displayQuestion(int index) const;
    void displayAllQuestions() const;

private:
    std::string filePath;
    std::vector<Question> questions;

    bool validateQuestion(const Question &q, int lineNumber) const;
    std::vector<std::string> splitLine(const std::string &line, char delimiter) const;
    std::string trim(const std::string &str) const;
};

#endif
