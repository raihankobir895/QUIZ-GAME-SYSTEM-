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

    // Week 9: any lines skipped while loading (empty text, missing option,
    // bad correct-option index, wrong field count, etc.), one entry per
    // skipped line, in the format "Line N: reason". Empty if every line
    // loaded cleanly. Populated by loadQuestions().
    const std::vector<std::string> &getWarnings() const;

private:
    std::string filePath;
    std::vector<Question> questions;
    std::vector<std::string> warnings;

    void addWarning(int lineNumber, const std::string &reason);
    bool validateQuestion(const Question &q, int lineNumber);
    std::vector<std::string> splitLine(const std::string &line, char delimiter) const;
    std::string trim(const std::string &str) const;
};

#endif
