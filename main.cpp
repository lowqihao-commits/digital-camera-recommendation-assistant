#include <iostream>
#include <sstream>
#include <string>

const int CATEGORY_COUNT = 5;
const int FEATURE_COUNT = 5;

// Keep the same category order in every table below.
const std::string CAMERA_NAMES[CATEGORY_COUNT] = {
    "Compact Digital Camera",
    "Entry-Level Mirrorless Camera",
    "Enthusiast Mirrorless Camera",
    "High-Speed Mirrorless Camera",
    "Professional Full-Frame Mirrorless Camera"
};

// Ratings: image quality, portability, autofocus/speed, video, affordability.
const int CAMERA_RATINGS[CATEGORY_COUNT][FEATURE_COUNT] = {
    {3, 5, 2, 3, 5},
    {4, 4, 3, 4, 4},
    {5, 3, 4, 5, 3},
    {5, 2, 5, 4, 2},
    {5, 2, 5, 5, 1}
};

// Each row follows the photography-purpose menu (1 to 5).
const int PURPOSE_BONUSES[5][CATEGORY_COUNT] = {
    {15, 10, 3, 0, 0},
    {12, 15, 8, 2, 1},
    {2, 8, 15, 5, 10},
    {0, 3, 8, 18, 12},
    {0, 2, 10, 12, 20}
};

// Rows are beginner, intermediate and advanced.
const int EXPERIENCE_BONUSES[3][CATEGORY_COUNT] = {
    {8, 12, 3, 0, 0},
    {2, 5, 12, 6, 3},
    {0, 2, 7, 10, 12}
};

const std::string EXAMPLE_MODELS[CATEGORY_COUNT][2] = {
    {"Canon PowerShot G7 X Mark III", "Sony RX100 VII"},
    {"Canon EOS R50", "Nikon Z50II"},
    {"Canon EOS R7", "Sony a6700"},
    {"Nikon Z8", "Sony a9 III"},
    {"Canon EOS R1", "Nikon Z9"}
};

void showHeading(const std::string& title) {
    std::cout << "\n============================================================\n"
              << "  " << title << '\n'
              << "============================================================\n\n";
}

// Read one complete line so a bad answer does not affect the next question.
int readChoice(const std::string& prompt, int minimum, int maximum) {
    std::string line;
    while (true) {
        std::cout << "  " << prompt << "\n  > " << std::flush;
        if (!std::getline(std::cin, line)) {
            std::cout << "\n\n  Input closed. Goodbye!\n";
            return 0;
        }
        std::cout << '\n';

        std::istringstream answer(line);
        int choice = 0;
        char extra = '\0';
        if ((answer >> choice) && !(answer >> extra)
            && choice >= minimum && choice <= maximum) {
            return choice;
        }
        std::cout << "  Invalid input. Enter a whole number from "
                  << minimum << " to " << maximum << ".\n\n";
    }
}

void showWelcome() {
    showHeading("DIGITAL CAMERA RECOMMENDATION ASSISTANT");
    std::cout << "  Find a camera category for your photography needs.\n"
              << "  Answer four short questions to get a recommendation.\n";
}

int askPurpose() {
    showHeading("QUESTION 1 OF 4 - PHOTOGRAPHY PURPOSE");
    std::cout << "  1. Family / Casual Photography\n"
              << "  2. Travel Photography\n"
              << "  3. Portrait Photography\n"
              << "  4. Sports / Wildlife Photography\n"
              << "  5. Professional / Commercial Photography\n\n";
    return readChoice("Purpose [1-5]:", 1, 5);
}

int askExperience() {
    showHeading("QUESTION 2 OF 4 - EXPERIENCE LEVEL");
    std::cout << "  1. Beginner\n"
              << "  2. Intermediate\n"
              << "  3. Advanced\n\n";
    return readChoice("Experience [1-3]:", 1, 3);
}

int askMainPriority() {
    showHeading("QUESTION 3 OF 4 - MAIN PRIORITY");
    std::cout << "  Which feature matters most to you?\n\n"
              << "  1. Image quality\n"
              << "  2. Portability\n"
              << "  3. Autofocus / speed\n"
              << "  4. Video capability\n\n";
    return readChoice("Main priority [1-4]:", 1, 4);
}

int askBudget() {
    showHeading("QUESTION 4 OF 4 - BUDGET SENSITIVITY");
    std::cout << "  How important is keeping the cost low?\n\n"
              << "  1. Not important\n"
              << "  2. Slightly important\n"
              << "  3. Moderately important\n"
              << "  4. Important\n"
              << "  5. Very important\n\n";
    return readChoice("Budget sensitivity [1-5]:", 1, 5);
}

// Score one category using the four answers.
int calculateScore(int category, int purpose, int experience,
                   int mainPriority, int budget) {
    int score = 0;
    for (int feature = 0; feature < 4; ++feature) {
        int weight = 1;
        if (feature == mainPriority - 1) {
            weight = 5;
        }
        score += weight * CAMERA_RATINGS[category][feature];
    }

    // A budget answer of 5 rewards more affordable categories most.
    score += budget * CAMERA_RATINGS[category][4];
    score += PURPOSE_BONUSES[purpose - 1][category];
    score += EXPERIENCE_BONUSES[experience - 1][category];
    return score;
}

void calculateAllScores(int purpose, int experience, int mainPriority,
                        int budget, int scores[CATEGORY_COUNT]) {
    for (int category = 0; category < CATEGORY_COUNT; ++category) {
        scores[category] = calculateScore(category, purpose, experience,
                                          mainPriority, budget);
    }
}

// Pick the highest score that has not already been displayed.
int findNextBest(const int scores[CATEGORY_COUNT],
                 const bool alreadyShown[CATEGORY_COUNT]) {
    int best = -1;
    for (int category = 0; category < CATEGORY_COUNT; ++category) {
        if (!alreadyShown[category]
            && (best == -1 || scores[category] > scores[best])) {
            best = category;
        }
    }
    return best;
}

void explainResult(int purpose, int experience, int mainPriority,
                   int budget, int best) {
    std::cout << "\n  WHY IT MATCHES\n";
    if (purpose == 1) {
        std::cout << "  Family photos benefit from easy, convenient cameras.\n";
    } else if (purpose == 2) {
        std::cout << "  Travel benefits from a portable, versatile camera.\n";
    } else if (purpose == 3) {
        std::cout << "  Portraits benefit from strong image quality and control.\n";
    } else if (purpose == 4) {
        std::cout << "  Sports and wildlife benefit from fast autofocus.\n";
    } else {
        std::cout << "  Professional work benefits from advanced capability.\n";
    }

    if (experience == 1) {
        std::cout << "  Your beginner experience favours approachable options.\n";
    } else if (experience == 2) {
        std::cout << "  Your experience favours more creative control.\n";
    } else {
        std::cout << "  Your advanced experience favours capable options.\n";
    }

    const std::string features[4] = {
        "image quality", "portability", "autofocus / speed", "video"
    };
    std::cout << "  You chose " << features[mainPriority - 1]
              << " as your main priority.\n";
    if (CAMERA_RATINGS[best][mainPriority - 1] <= 2) {
        std::cout << "  This is a trade-off for the suggested category.\n";
    }
    if (budget >= 4 && CAMERA_RATINGS[best][4] <= 2) {
        std::cout << "  Cost is another trade-off to consider.\n";
    }
}

void showResult(int purpose, int experience, int mainPriority, int budget) {
    int scores[CATEGORY_COUNT];
    calculateAllScores(purpose, experience, mainPriority, budget, scores);

    bool alreadyShown[CATEGORY_COUNT] = {false};
    int topThree[3];
    for (int place = 0; place < 3; ++place) {
        topThree[place] = findNextBest(scores, alreadyShown);
        alreadyShown[topThree[place]] = true;
    }

    showHeading("YOUR CAMERA RECOMMENDATION");
    std::cout << "  BEST MATCH\n"
              << "  " << CAMERA_NAMES[topThree[0]] << '\n'
              << "  Suitability score: " << scores[topThree[0]] << " points\n";
    if (scores[topThree[0]] == scores[topThree[1]]) {
        std::cout << "  The first two categories share the top score.\n";
    }

    explainResult(purpose, experience, mainPriority, budget, topThree[0]);

    std::cout << "\n  EXAMPLE MODELS\n"
              << "  - " << EXAMPLE_MODELS[topThree[0]][0] << '\n'
              << "  - " << EXAMPLE_MODELS[topThree[0]][1] << '\n'
              << "  These are examples, not specific product recommendations.\n";

    std::cout << "\n  OTHER GOOD MATCHES\n";
    for (int place = 1; place < 3; ++place) {
        std::cout << "  " << place + 1 << ". " << CAMERA_NAMES[topThree[place]]
                  << " (" << scores[topThree[place]] << " points)\n";
    }
    std::cout << "\n  Scores compare categories in this program, not percentages.\n";
}

int askRestart() {
    showHeading("WHAT WOULD YOU LIKE TO DO?");
    std::cout << "  1. Start another recommendation\n"
              << "  2. Exit\n\n";
    return readChoice("Choice [1-2]:", 1, 2);
}

int main() {
    showWelcome();
    while (true) {
        int purpose = askPurpose();
        if (purpose == 0) return 0;

        int experience = askExperience();
        if (experience == 0) return 0;

        int mainPriority = askMainPriority();
        if (mainPriority == 0) return 0;

        int budget = askBudget();
        if (budget == 0) return 0;

        showResult(purpose, experience, mainPriority, budget);

        int restart = askRestart();
        if (restart != 1) {
            if (restart == 2) {
                std::cout << "\n  Thank you. Goodbye!\n";
            }
            return 0;
        }
    }
}
