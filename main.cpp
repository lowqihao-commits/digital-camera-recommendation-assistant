#include <iostream>
#include <sstream>
#include <string>

const int CATEGORY_COUNT = 5;
const int FEATURE_COUNT = 5;

// These tables describe broad camera categories, not individual product ratings.
// Keep the same category order in every table so each column matches its name.
const std::string CAMERA_NAMES[CATEGORY_COUNT] = {
    "Compact Digital Camera",
    "Entry-Level Mirrorless Camera",
    "Enthusiast Mirrorless Camera",
    "High-Speed Mirrorless Camera",
    "Professional Full-Frame Mirrorless Camera"
};

// Relative ratings from 1 (low) to 5 (high): image quality, portability,
// autofocus/speed, video, and affordability.
const int CAMERA_RATINGS[CATEGORY_COUNT][FEATURE_COUNT] = {
    {3, 5, 2, 3, 5},
    {4, 4, 3, 4, 4},
    {5, 3, 4, 5, 3},
    {5, 2, 5, 4, 2},
    {5, 2, 5, 5, 1}
};

// A purpose bonus helps the category suit the user's intended photography.
// Each row follows the photography-purpose menu (1 to 5).
const int PURPOSE_BONUSES[5][CATEGORY_COUNT] = {
    {15, 10, 3, 0, 0},
    {12, 15, 8, 2, 1},
    {2, 8, 15, 5, 10},
    {0, 3, 8, 18, 12},
    {0, 2, 10, 12, 20}
};

// Experience bonuses follow the beginner, intermediate, and advanced menu.
const int EXPERIENCE_BONUSES[3][CATEGORY_COUNT] = {
    {8, 12, 3, 0, 0},
    {2, 5, 12, 6, 3},
    {0, 2, 7, 10, 12}
};

// These model names are examples of each category, not scored separately.
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
// Return 0 if the input stream closes; all valid menu answers start at 1.
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

// Score one category using all four answers. The chosen priority has five
// times the normal feature weight, while a larger budget answer puts more
// weight on affordability.
int calculateScore(int category, int purpose, int experience,
                   int mainPriority, int budget) {
    int score = 0;
    for (int feature = 0; feature < 4; ++feature) {
        // Count every feature, but give the user's main priority more influence.
        int weight = 1;
        if (feature == mainPriority - 1) {
            weight = 5;
        }
        score += weight * CAMERA_RATINGS[category][feature];
    }

    score += budget * CAMERA_RATINGS[category][4];
    score += PURPOSE_BONUSES[purpose - 1][category];
    score += EXPERIENCE_BONUSES[experience - 1][category];
    return score;
}

// Calculate every category before selecting the one with the highest score.
void calculateAllScores(int purpose, int experience, int mainPriority,
                        int budget, int scores[CATEGORY_COUNT]) {
    for (int category = 0; category < CATEGORY_COUNT; ++category) {
        scores[category] = calculateScore(category, purpose, experience,
                                          mainPriority, budget);
    }
}

// If scores tie, keep the first category in the table for a stable result.
int findBestCategory(const int scores[CATEGORY_COUNT]) {
    int best = 0;
    for (int category = 1; category < CATEGORY_COUNT; ++category) {
        if (scores[category] > scores[best]) {
            best = category;
        }
    }
    return best;
}

// Explain the match in the same order as the questions the user answered.
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
    // Mention trade-offs only when the chosen category is weak in that area.
    if (CAMERA_RATINGS[best][mainPriority - 1] <= 2) {
        std::cout << "  This is a trade-off for the suggested category.\n";
    }
    if (budget >= 4 && CAMERA_RATINGS[best][4] <= 2) {
        std::cout << "  Cost is another trade-off to consider.\n";
    }
}

// Keep the result focused on one category, its reason, and two model examples.
void showResult(int purpose, int experience, int mainPriority, int budget) {
    int scores[CATEGORY_COUNT];
    calculateAllScores(purpose, experience, mainPriority, budget, scores);
    int best = findBestCategory(scores);

    showHeading("YOUR CAMERA RECOMMENDATION");
    std::cout << "  BEST MATCH\n"
              << "  " << CAMERA_NAMES[best] << '\n';

    explainResult(purpose, experience, mainPriority, budget, best);

    std::cout << "\n  EXAMPLE MODELS\n"
              << "  1. " << EXAMPLE_MODELS[best][0] << '\n'
              << "  2. " << EXAMPLE_MODELS[best][1] << '\n';
}

int askRestart() {
    showHeading("WHAT WOULD YOU LIKE TO DO?");
    std::cout << "  1. Start another recommendation\n"
              << "  2. Exit\n\n";
    return readChoice("Choice [1-2]:", 1, 2);
}

// Run another questionnaire only when the user selects the restart option.
int main() {
    showWelcome();
    while (true) {
        int purpose = askPurpose();
        if (purpose == 0) {
            return 0;
        }

        int experience = askExperience();
        if (experience == 0) {
            return 0;
        }

        int mainPriority = askMainPriority();
        if (mainPriority == 0) {
            return 0;
        }

        int budget = askBudget();
        if (budget == 0) {
            return 0;
        }

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
