// Week 9 - Module testing.
//
// Exercises the three platform-independent modules (QuestionBank, Score,
// ResultStore) directly, without needing Windows or the GUI. Build and run
// with a plain g++ - no mingw / windows.h required:
//
//   g++ -std=c++17 -Wall -Wextra tests/test_core.cpp
//     "include Folder/src Folder/QuestionBank.cpp"
//     "include Folder/src Folder/Score.cpp"
//     "include Folder/src Folder/ResultStore.cpp"
//     -o tests/test_core
//   ./tests/test_core
// (each line above is one argument to g++; join them with spaces, or a
// trailing backslash, when typing the command)
//
// Exits 0 if every check passes, 1 otherwise, so it can be wired into CI.

#include "../include Folder/QuestionBank.h"
#include "../include Folder/Score.h"
#include "../include Folder/ResultStore.h"

#include <cstdio>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>

namespace
{
    int checksRun = 0;
    int checksFailed = 0;

    void check(bool condition, const std::string &description)
    {
        checksRun++;
        if (condition)
        {
            std::printf("  [PASS] %s\n", description.c_str());
        }
        else
        {
            checksFailed++;
            std::printf("  [FAIL] %s\n", description.c_str());
        }
    }

    // Writes `content` to `path`, overwriting anything already there.
    void writeFile(const std::string &path, const std::string &content)
    {
        std::ofstream file(path.c_str(), std::ios::trunc);
        file << content;
    }
} // namespace

void testQuestionBankValidFile()
{
    std::printf("QuestionBank - valid file loads cleanly\n");

    const std::string path = "test_questions_valid.txt";
    writeFile(path,
             "# comment lines and blanks are ignored\n"
             "\n"
             "2+2=?|3|4|5|6|2\n"
             "Capital of Japan?|Seoul|Beijing|Tokyo|Osaka|3\n");

    QuestionBank bank(path);
    bool loaded = bank.loadQuestions();

    check(loaded, "loadQuestions() returns true for a well-formed file");
    check(bank.getQuestionCount() == 2, "both valid questions were counted");
    check(bank.getWarnings().empty(), "no warnings for a clean file");

    if (bank.getQuestionCount() == 2)
    {
        const Question &q1 = bank.getQuestion(0);
        check(q1.questionText == "2+2=?", "question text parsed correctly");
        check(q1.options[1] == "4", "option B parsed correctly");
        check(q1.correctOption == 2, "correct-option index parsed correctly");
    }

    std::remove(path.c_str());
}

void testQuestionBankMalformedLines()
{
    std::printf("QuestionBank - malformed lines are skipped, not fatal\n");

    const std::string path = "test_questions_malformed.txt";
    writeFile(path,
             "Only one good question|A|B|C|D|1\n"
             "Missing a field|A|B|C|1\n"        // wrong field count (5)
             "|A|B|C|D|1\n"                     // empty question text
             "Bad index|A|B|C|D|9\n"            // correctOption out of range
             "Not a number|A|B|C|D|x\n");       // non-numeric correct option

    QuestionBank bank(path);
    bool loaded = bank.loadQuestions();

    check(loaded, "loadQuestions() still succeeds when at least one line is valid");
    check(bank.getQuestionCount() == 1, "exactly the one well-formed question was kept");
    check(bank.getWarnings().size() == 4, "all four bad lines were recorded as warnings");

    std::remove(path.c_str());
}

void testQuestionBankMissingFile()
{
    std::printf("QuestionBank - missing file fails without crashing\n");

    QuestionBank bank("this_file_does_not_exist.txt");
    bool loaded = bank.loadQuestions();

    check(!loaded, "loadQuestions() returns false when the file can't be opened");
    check(bank.getQuestionCount() == 0, "no questions are available");
    check(!bank.getWarnings().empty(), "a warning explains why nothing loaded");
}

void testScoreTracking()
{
    std::printf("Score - points, counts and accuracy\n");

    Score score;
    check(score.getTotalAnswered() == 0, "starts with zero answers");
    check(score.getAccuracy() == 0.0, "accuracy is 0% before any answers (avoids /0)");

    score.recordAnswer(true);
    score.recordAnswer(true);
    score.recordAnswer(false);

    check(score.getPoints() == 20, "10 points per correct answer (2 correct = 20)");
    check(score.getCorrectCount() == 2, "correct count tracked");
    check(score.getIncorrectCount() == 1, "incorrect count tracked");
    check(score.getTotalAnswered() == 3, "total answered = correct + incorrect");

    double accuracy = score.getAccuracy();
    check(accuracy > 66.6 && accuracy < 66.7, "accuracy is 2/3 = ~66.67%");
}

void testResultStoreRoundTrip()
{
    std::printf("ResultStore - save/load round trip and best-score tracking\n");

    const std::string path = "test_results.txt";
    std::remove(path.c_str());

    ResultStore store(path);
    check(store.getAttemptCount() == 0, "no attempts before the first save");
    check(store.getBestScore() == -1, "best score is -1 with no history");

    ResultRecord first{"2026-01-01 10:00:00", 10, 6, 4, 60, 60.0};
    ResultRecord second{"2026-01-01 10:05:00", 10, 9, 1, 90, 90.0};

    check(store.saveResult(first), "first save succeeds");
    check(store.saveResult(second), "second save succeeds (appends, doesn't overwrite)");

    std::vector<ResultRecord> results = store.loadResults();
    check(results.size() == 2, "both saved attempts are read back");
    check(store.getAttemptCount() == 2, "attempt count matches saved records");
    check(store.getBestScore() == 90, "best score is the higher of the two attempts");

    if (results.size() == 2)
    {
        check(results[0].points == 60, "first record's points preserved");
        check(results[1].correct == 9, "second record's correct count preserved");
    }

    std::remove(path.c_str());
}

void testResultStoreIgnoresMalformedLines()
{
    std::printf("ResultStore - tolerates a corrupted results file\n");

    const std::string path = "test_results_malformed.txt";
    writeFile(path,
             "# header comment\n"
             "2026-01-01 10:00:00|10|8|2|80|80.0\n"
             "this line is garbage\n"
             "2026-01-01 10:10:00|10|not-a-number|2|80|80.0\n");

    ResultStore store(path);
    std::vector<ResultRecord> results = store.loadResults();

    check(results.size() == 1, "only the one well-formed record is loaded");
    check(store.getBestScore() == 80, "best score comes from the valid record");

    std::remove(path.c_str());
}

int main()
{
    testQuestionBankValidFile();
    testQuestionBankMalformedLines();
    testQuestionBankMissingFile();
    testScoreTracking();
    testResultStoreRoundTrip();
    testResultStoreIgnoresMalformedLines();

    std::printf("\n%d/%d checks passed.\n", checksRun - checksFailed, checksRun);

    return checksFailed == 0 ? 0 : 1;
}
