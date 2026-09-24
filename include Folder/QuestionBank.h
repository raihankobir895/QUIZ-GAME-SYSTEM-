#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include <string>
#include <vector>

struct Question
{
    std::string questionText;
    std::string options[4];
    int correctOption;
};

class QuestionBank
{
public:
    explicit QuestionBank(const std::string &filePath);

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
