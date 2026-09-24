#include <iostream>
#include <fstream>
#include <sstream>
#include "../QuestionBank.h"

using namespace std;

QuestionBank::QuestionBank(const string &filePath) : filePath(filePath) {}

string QuestionBank::trim(const string &str) const
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

vector<string> QuestionBank::splitLine(const string &line, char delimiter) const
{
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (getline(ss, token, delimiter))
        tokens.push_back(trim(token));

    return tokens;
}

bool QuestionBank::validateQuestion(const Question &q, int lineNumber) const
{
    if (q.questionText.empty())
    {
        cout << "Warning: Skipping line " << lineNumber << " - question text is empty.\n";
        return false;
    }

    for (int i = 0; i < 4; i++)
    {
        if (q.options[i].empty())
        {
            cout << "Warning: Skipping line " << lineNumber
                 << " - option " << static_cast<char>('A' + i) << " is empty.\n";
            return false;
        }
    }

    if (q.correctOption < 1 || q.correctOption > 4)
    {
        cout << "Warning: Skipping line " << lineNumber
             << " - correct option must be between 1 and 4.\n";
        return false;
    }

    return true;
}

bool QuestionBank::loadQuestions()
{
    ifstream file(filePath.c_str());

    if (!file.is_open())
    {
        cout << "Error: Could not open question file: " << filePath << "\n";
        return false;
    }

    questions.clear();
    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber++;
        string trimmedLine = trim(line);

        if (trimmedLine.empty() || trimmedLine[0] == '#')
            continue;

        vector<string> tokens = splitLine(trimmedLine, '|');

        if (tokens.size() != 6)
        {
            cout << "Warning: Skipping line " << lineNumber
                 << " - expected 6 fields, found " << tokens.size() << ".\n";
            continue;
        }

        Question q;
        q.questionText = tokens[0];
        q.options[0] = tokens[1];
        q.options[1] = tokens[2];
        q.options[2] = tokens[3];
        q.options[3] = tokens[4];

        try
        {
            q.correctOption = stoi(tokens[5]);
        }
        catch (...)
        {
            cout << "Warning: Skipping line " << lineNumber
                 << " - correct option is not a valid number.\n";
            continue;
        }

        if (validateQuestion(q, lineNumber))
            questions.push_back(q);
    }

    file.close();

    if (questions.empty())
    {
        cout << "Warning: No valid questions were loaded from file.\n";
        return false;
    }

    cout << questions.size() << " question(s) loaded successfully.\n";
    return true;
}

int QuestionBank::getQuestionCount() const
{
    return static_cast<int>(questions.size());
}

const Question &QuestionBank::getQuestion(int index) const
{
    return questions.at(index);
}

void QuestionBank::displayQuestion(int index) const
{
    const Question &q = questions.at(index);

    cout << "\nQ" << (index + 1) << ". " << q.questionText << "\n";
    cout << "   A. " << q.options[0] << "\n";
    cout << "   B. " << q.options[1] << "\n";
    cout << "   C. " << q.options[2] << "\n";
    cout << "   D. " << q.options[3] << "\n";
}

void QuestionBank::displayAllQuestions() const
{
    for (size_t i = 0; i < questions.size(); i++)
        displayQuestion(static_cast<int>(i));
}
