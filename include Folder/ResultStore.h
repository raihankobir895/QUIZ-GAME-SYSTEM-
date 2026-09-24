#ifndef RESULTSTORE_H
#define RESULTSTORE_H

#include <string>
#include <vector>

// One completed quiz attempt, as saved to / loaded from the results file.
struct ResultRecord
{
    std::string timestamp;  // e.g. "2026-09-24 14:05:10"
    int totalQuestions;
    int correct;
    int incorrect;
    int points;
    double accuracy; // percentage, 0-100
};

// Week 7/8: persists every finished quiz attempt to a plain text file
// (pipe-delimited, same style as the question bank) so results survive
// between runs. No database is used, per the course requirement -
// everything goes through std::ifstream / std::ofstream.
class ResultStore
{
public:
    explicit ResultStore(const std::string &filePath);

    // Appends one record to the file (creates it on first use, together
    // with a header comment). Returns false if the file could not be
    // opened for writing; the caller decides how to surface that.
    bool saveResult(const ResultRecord &record) const;

    // Reads every previously saved record. Malformed lines are skipped
    // rather than aborting the read. Returns an empty vector - not an
    // error - if the file does not exist yet (i.e. first ever run).
    std::vector<ResultRecord> loadResults() const;

    // Highest points value across all previously saved attempts.
    // Returns -1 if there is no history yet.
    int getBestScore() const;

    // How many attempts have been recorded so far.
    int getAttemptCount() const;

    // Current timestamp formatted as "YYYY-MM-DD HH:MM:SS".
    static std::string currentTimestamp();

private:
    std::string filePath;

    static std::string trim(const std::string &str);
    static std::vector<std::string> splitLine(const std::string &line, char delimiter);
};

#endif
