// Build this file separately from main.cpp to check the program's functions.
#define main cameraProgramMain
#include "../main.cpp"
#undef main

#include <stdexcept>

void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

std::string runProgram(const std::string& answers) {
    std::istringstream input(answers);
    std::ostringstream output;
    std::streambuf* originalInput = std::cin.rdbuf(input.rdbuf());
    std::streambuf* originalOutput = std::cout.rdbuf(output.rdbuf());
    std::cin.clear();
    int exitCode = cameraProgramMain();
    std::cin.rdbuf(originalInput);
    std::cout.rdbuf(originalOutput);
    std::cin.clear();
    require(exitCode == 0, "Program did not exit normally");
    return output.str();
}

int main() {
    try {
        // Values from the assignment specification, kept separate from the app tables.
        const int ratings[5][5] = {
            {3, 5, 2, 3, 5}, {4, 4, 3, 4, 4}, {5, 3, 4, 5, 3},
            {5, 2, 5, 4, 2}, {5, 2, 5, 5, 1}
        };
        const int purposeBonuses[5][5] = {
            {15, 10, 3, 0, 0}, {12, 15, 8, 2, 1}, {2, 8, 15, 5, 10},
            {0, 3, 8, 18, 12}, {0, 2, 10, 12, 20}
        };
        const int experienceBonuses[3][5] = {
            {8, 12, 3, 0, 0}, {2, 5, 12, 6, 3}, {0, 2, 7, 10, 12}
        };

        int questionnaires = 0;
        int ties = 0;
        for (int purpose = 1; purpose <= 5; ++purpose) {
            for (int experience = 1; experience <= 3; ++experience) {
                for (int priority = 1; priority <= 4; ++priority) {
                    for (int budget = 1; budget <= 5; ++budget) {
                        int scores[5];
                        calculateAllScores(purpose, experience, priority,
                                           budget, scores);
                        for (int category = 0; category < 5; ++category) {
                            int expected = purposeBonuses[purpose - 1][category]
                                + experienceBonuses[experience - 1][category]
                                + budget * ratings[category][4];
                            for (int feature = 0; feature < 4; ++feature) {
                                int weight = 1;
                                if (feature == priority - 1) weight = 5;
                                expected += weight * ratings[category][feature];
                            }
                            require(scores[category] == expected, "Incorrect score");
                        }

                        // The earliest category wins when several share the top score.
                        int expectedBest = 0;
                        for (int category = 1; category < 5; ++category) {
                            if (scores[category] > scores[expectedBest]) {
                                expectedBest = category;
                            }
                        }
                        require(findBestCategory(scores) == expectedBest,
                                "Incorrect best-match category");
                        for (int category = expectedBest + 1; category < 5;
                             ++category) {
                            if (scores[category] == scores[expectedBest]) {
                                ++ties;
                                break;
                            }
                        }

                        // Run the complete console flow as well as the score
                        // calculation, then check the category actually shown.
                        std::ostringstream answers;
                        answers << purpose << '\n' << experience << '\n'
                                << priority << '\n' << budget << "\n2\n";
                        const std::string result = runProgram(answers.str());
                        const std::string expectedLabel =
                            "  BEST MATCH\n  " + CAMERA_NAMES[expectedBest] + '\n';
                        require(result.find(expectedLabel) != std::string::npos,
                                "Console displayed the wrong best match");
                        ++questionnaires;
                    }
                }
            }
        }

        // One run checks invalid answers, two separate results, and restart.
        const std::string transcript = runProgram(
            "abc\n2\n1\n2\n3\n1\n4\n3\n3\n1\n2\n");
        require(transcript.find("Invalid input.") != std::string::npos,
                "Invalid input was not rejected");
        require(transcript.find("Entry-Level Mirrorless Camera") != std::string::npos,
                "Travel result missing");
        require(transcript.find("High-Speed Mirrorless Camera") != std::string::npos,
                "Sports result missing");
        require(transcript.find("OTHER GOOD MATCHES") == std::string::npos,
                "Extra recommendation section is still shown");
        require(transcript.find("Scores compare") == std::string::npos
                    && transcript.find(" points") == std::string::npos,
                "Internal score explanation is still shown");
        require(transcript.find("Canon EOS R50") != std::string::npos
                    && transcript.find("Nikon Z8") != std::string::npos,
                "Example models are missing from the two results");
        require(transcript.find("Thank you. Goodbye!") != std::string::npos,
                "Exit message missing");

        // EOF after a complete fourth answer must still show the result.
        const std::string noFinalNewline = runProgram("4\n3\n3\n1");
        require(noFinalNewline.find("High-Speed Mirrorless Camera") != std::string::npos,
                "Final answer without newline was lost");

        // This answer set ties; the first category must remain the sole result.
        const std::string tiedResult = runProgram("1\n1\n4\n5\n2\n");
        require(tiedResult.find("Compact Digital Camera") != std::string::npos
                    && tiedResult.find("Entry-Level Mirrorless Camera")
                        == std::string::npos,
                "Tied result displayed more than one category");

        // Reject out-of-range, decimal and blank answers before continuing.
        const std::string invalidAnswers = runProgram(
            "0\n6\n2.5\n \n1\n1\n4\n5\n2\n");
        int invalidCount = 0;
        std::size_t position = 0;
        while ((position = invalidAnswers.find("Invalid input.", position))
               != std::string::npos) {
            ++invalidCount;
            ++position;
        }
        require(invalidCount == 4
                    && invalidAnswers.find("Compact Digital Camera")
                        != std::string::npos,
                "Invalid answers were not rejected and recovered from");

        const std::string emptyInput = runProgram("");
        require(emptyInput.find("Input closed. Goodbye!") != std::string::npos
                    && emptyInput.find("BEST MATCH") == std::string::npos,
                "Empty input did not exit cleanly");

        std::cout << "PASS: " << questionnaires
                  << " questionnaires, scores and best matches checked.\n"
                  << "PASS: " << ties << " cases have a tie for best match.\n"
                  << "PASS: invalid input, restart, exit and end-of-input checked.\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
