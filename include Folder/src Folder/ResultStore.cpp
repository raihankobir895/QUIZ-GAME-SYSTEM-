#include "../ResultStore.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

ResultStore::ResultStore(const std::string &filePath) : filePath(filePath) {}

std::string ResultStore::currentTimestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    char buffer[20]; // "YYYY-MM-DD HH:MM:SS" + '\0'
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return std::string(buffer);
}

std::string ResultStore::trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> ResultStore::splitLine(const std::string &line, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, delimiter))
        tokens.push_back(trim(token));

    return tokens;
}

bool ResultStore::saveResult(const ResultRecord &record) const
{
    // Write the header comment once, only when the file doesn't exist yet.
    std::ifstream probe(filePath.c_str());
    bool isNewFile = !probe.good();
    probe.close();

    // Append mode: every past attempt is kept, never overwritten.
    std::ofstream file(filePath.c_str(), std::ios::app);
    if (!file.is_open())
        return false;

    if (isNewFile)
    {
        file << "# Quiz Game - Results History\n";
        file << "# Timestamp|TotalQuestions|Correct|Incorrect|Points|Accuracy\n";
    }

    file << record.timestamp << "|"
         << record.totalQuestions << "|"
         << record.correct << "|"
         << record.incorrect << "|"
         << record.points << "|"
         << std::fixed << std::setprecision(1) << record.accuracy << "\n";

    return !file.fail();
}

std::vector<ResultRecord> ResultStore::loadResults() const
{
    std::vector<ResultRecord> results;

    std::ifstream file(filePath.c_str());
    if (!file.is_open())
        return results; // No history yet - not an error.

    std::string line;
    while (std::getline(file, line))
    {
        std::string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#')
            continue;

        std::vector<std::string> tokens = splitLine(trimmed, '|');
        if (tokens.size() != 6)
            continue; // Skip malformed lines instead of aborting.

        try
        {
            ResultRecord record;
            record.timestamp = tokens[0];
            record.totalQuestions = std::stoi(tokens[1]);
            record.correct = std::stoi(tokens[2]);
            record.incorrect = std::stoi(tokens[3]);
            record.points = std::stoi(tokens[4]);
            record.accuracy = std::stod(tokens[5]);
            results.push_back(record);
        }
        catch (...)
        {
            continue; // Non-numeric field on this line - skip it.
        }
    }

    return results;
}

int ResultStore::getBestScore() const
{
    std::vector<ResultRecord> results = loadResults();
    if (results.empty())
        return -1;

    int best = results[0].points;
    for (const ResultRecord &r : results)
        if (r.points > best)
            best = r.points;

    return best;
}

int ResultStore::getAttemptCount() const
{
    return static_cast<int>(loadResults().size());
}
