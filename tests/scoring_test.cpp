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

                        bool shown[5] = {false};
                        int previous = -1;
                        for (int place = 0; place < 3; ++place) {
                            int category = findNextBest(scores, shown);
                            require(category >= 0 && !shown[category],
                                    "Duplicate or missing top-three category");
                            if (previous >= 0) {
                                require(scores[previous] >= scores[category],
                                        "Top three out of order");
                                if (scores[previous] == scores[category]) {
                                    require(previous < category,
                                            "Tie order changed");
                                }
                            }
                            if (place == 1 && scores[previous] == scores[category]) {
                                ++ties;
                            }
                            shown[category] = true;
                            previous = category;
                        }
                        ++questionnaires;
                    }
                }
            }
        }

        // One run checks invalid answers, the three-result layout and restart.
        const std::string transcript = runProgram(
            "abc\n2\n1\n2\n3\n1\n4\n3\n3\n1\n2\n");
        require(transcript.find("Invalid input.") != std::string::npos,
                "Invalid input was not rejected");
        require(transcript.find("Entry-Level Mirrorless Camera") != std::string::npos,
                "Travel result missing");
        require(transcript.find("High-Speed Mirrorless Camera") != std::string::npos,
                "Sports result missing");
        require(transcript.find("OTHER CATEGORIES") == std::string::npos,
                "Extra category section is still shown");
        require(transcript.find("Thank you. Goodbye!") != std::string::npos,
                "Exit message missing");

        // EOF after a complete fourth answer must still show the result.
        const std::string noFinalNewline = runProgram("4\n3\n3\n1");
        require(noFinalNewline.find("High-Speed Mirrorless Camera") != std::string::npos,
                "Final answer without newline was lost");

        std::cout << "PASS: " << questionnaires
                  << " questionnaires, scores and top-three ranking checked.\n"
                  << "PASS: " << ties << " cases have a tie for best match.\n"
                  << "PASS: invalid input, restart, exit and end-of-input checked.\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
