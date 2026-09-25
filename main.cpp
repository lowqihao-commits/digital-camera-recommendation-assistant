#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct CameraCategory {
    std::string name;
    int imageQuality;
    int portability;
    int autofocusSpeed;
    int videoCapability;
    int affordability;
    std::vector<std::string> exampleModels;
    int score = 0;
};

struct UserPreferences {
    int photographyPurpose = 0;
    int experienceLevel = 0;
    int imageQualityPriority = 1;
    int portabilityPriority = 1;
    int autofocusSpeedPriority = 1;
    int videoPriority = 1;
    int budgetSensitivity = 0;
    int mainPriority = 0;
};

void applyMainPriority(UserPreferences& preferences, int mainPriority) {
    // The chosen feature gets weight 5; other features retain baseline weight 1.
    preferences.imageQualityPriority = 1;
    preferences.portabilityPriority = 1;
    preferences.autofocusSpeedPriority = 1;
    preferences.videoPriority = 1;
    preferences.mainPriority = mainPriority;
    switch (mainPriority) {
        case 1: preferences.imageQualityPriority = 5; break;
        case 2: preferences.portabilityPriority = 5; break;
        case 3: preferences.autofocusSpeedPriority = 5; break;
        case 4: preferences.videoPriority = 5; break;
    }
}

std::vector<CameraCategory> createCameraCategories() {
    return {
        {"Compact Digital Camera", 3, 5, 2, 3, 5,
         {"Canon PowerShot G7 X Mark III", "Sony RX100 VII"}},
        {"Entry-Level Mirrorless Camera", 4, 4, 3, 4, 4,
         {"Canon EOS R50", "Nikon Z50II", "Sony a6400"}},
        {"Enthusiast Mirrorless Camera", 5, 3, 4, 5, 3,
         {"Canon EOS R7", "Nikon Z6III", "Sony a6700"}},
        {"High-Speed Mirrorless Camera", 5, 2, 5, 4, 2,
         {"Canon EOS R5 Mark II", "Nikon Z8", "Sony a9 III"}},
        {"Professional Full-Frame Mirrorless Camera", 5, 2, 5, 5, 1,
         {"Canon EOS R1", "Nikon Z9", "Sony a1 II"}}
    };
}

int getPurposeBonus(int purpose, std::size_t categoryIndex) {
    // Columns follow createCameraCategories(); rows follow the purpose menu.
    static const int bonuses[5][5] = {
        {15, 10, 3, 0, 0},
        {12, 15, 8, 2, 1},
        {2, 8, 15, 5, 10},
        {0, 3, 8, 18, 12},
        {0, 2, 10, 12, 20}
    };
    return bonuses[purpose - 1][categoryIndex];
}

int getExperienceBonus(int experience, std::size_t categoryIndex) {
    // The same category order is used for beginner, intermediate and advanced.
    static const int bonuses[3][5] = {
        {8, 12, 3, 0, 0},
        {2, 5, 12, 6, 3},
        {0, 2, 7, 10, 12}
    };
    return bonuses[experience - 1][categoryIndex];
}

int calculateScore(const UserPreferences& preferences,
                   const CameraCategory& category,
                   std::size_t categoryIndex) {
    // A larger budget sensitivity rewards affordability, not a higher price.
    const int preferenceScore =
        preferences.imageQualityPriority * category.imageQuality
        + preferences.portabilityPriority * category.portability
        + preferences.autofocusSpeedPriority * category.autofocusSpeed
        + preferences.videoPriority * category.videoCapability
        + preferences.budgetSensitivity * category.affordability;
    return preferenceScore
        + getPurposeBonus(preferences.photographyPurpose, categoryIndex)
        + getExperienceBonus(preferences.experienceLevel, categoryIndex);
}

void calculateCategoryScores(std::vector<CameraCategory>& categories,
                             const UserPreferences& preferences) {
    for (std::size_t index = 0; index < categories.size(); ++index) {
        categories[index].score = calculateScore(preferences, categories[index], index);
    }
}

std::vector<CameraCategory> rankRecommendations(std::vector<CameraCategory> categories) {
    // Equal scores keep their original category order for reproducible results.
    std::stable_sort(categories.begin(), categories.end(),
              [](const CameraCategory& left, const CameraCategory& right) {
                  return left.score > right.score;
              });
    return categories;
}

std::string getPhotographyPurposeName(int purpose) {
    static const std::string names[] = {
        "Family / Casual Photography", "Travel Photography",
        "Portrait Photography", "Sports / Wildlife Photography",
        "Professional / Commercial Photography"
    };
    return names[purpose - 1];
}

void displayHeading(const std::string& title) {
    std::cout << '\n' << std::string(72, '=') << '\n'
              << "  " << title << '\n' << std::string(72, '=') << "\n\n";
}

std::string getMainPriorityName(int priority) {
    static const std::string names[] = {
        "Image quality", "Portability", "Autofocus / speed", "Video capability"
    };
    return names[priority - 1];
}

void displayUserSummary(const UserPreferences& preferences) {
    static const std::string experienceNames[] = {
        "Beginner", "Intermediate", "Advanced"
    };
    std::cout << "  YOUR PREFERENCES\n"
              << "  " << std::left << std::setw(22) << "Purpose:"
              << getPhotographyPurposeName(preferences.photographyPurpose) << '\n'
              << "  " << std::setw(22) << "Experience:"
              << experienceNames[preferences.experienceLevel - 1] << '\n'
              << "  " << std::setw(22) << "Main priority:"
              << getMainPriorityName(preferences.mainPriority)
              << '\n'
              << "  " << std::setw(22) << "Budget sensitivity:"
              << preferences.budgetSensitivity << "/5\n";
}

void displayExplanation(const UserPreferences& preferences,
                        const CameraCategory& recommendation) {
    std::cout << "\n  WHY THIS MATCHES YOUR INPUTS\n\n";
    switch (preferences.photographyPurpose) {
        case 1:
            std::cout << "  Family / casual use favours convenient, affordable options.\n";
            break;
        case 2:
            std::cout << "  Travel favours portable cameras with versatile capabilities.\n";
            break;
        case 3:
            std::cout << "  Portrait work favours image quality and creative control.\n";
            break;
        case 4:
            std::cout << "  Sports / wildlife favours fast autofocus and action capture.\n";
            break;
        case 5:
            std::cout << "  Commercial work favours capable cameras for demanding use.\n";
            break;
    }
    switch (preferences.experienceLevel) {
        case 1:
            std::cout << "  Beginner bonuses favour accessible compact and entry models.\n";
            break;
        case 2:
            std::cout << "  Intermediate bonuses favour more creative control.\n";
            break;
        case 3:
            std::cout << "  Advanced bonuses favour speed and professional capability.\n";
            break;
    }

    const std::string factorNames[] = {
        "Image quality", "Portability", "Autofocus / speed",
        "Video capability", "Affordability"
    };
    const int priorities[] = {
        preferences.imageQualityPriority, preferences.portabilityPriority,
        preferences.autofocusSpeedPriority, preferences.videoPriority,
        preferences.budgetSensitivity
    };
    const int capabilities[] = {
        recommendation.imageQuality, recommendation.portability,
        recommendation.autofocusSpeed, recommendation.videoCapability,
        recommendation.affordability
    };
    std::cout << "\n  How the selected profile fits your priorities:\n"
              << "  (Profile ratings are simplified comparisons, not test scores.)\n\n";
    for (int index = 0; index < 5; ++index) {
        if (index == preferences.mainPriority - 1 || index == 4) {
            std::cout << "  - " << std::left << std::setw(20) << factorNames[index]
                      << "Profile: " << capabilities[index] << "/5\n";
            if (priorities[index] >= 4 && capabilities[index] <= 2) {
                std::cout << "    Trade-off: this is a weaker part of the selected profile.\n";
            }
        }
    }
    if (preferences.budgetSensitivity >= 4) {
        std::cout << "\n  Your budget answer gives affordability a strong influence.\n";
    } else if (preferences.budgetSensitivity <= 2) {
        std::cout << "\n  Your budget answer gives affordability a smaller influence.\n";
    } else {
        std::cout << "\n  Your budget answer gives affordability a moderate influence.\n";
    }
    std::cout << "  Together, your four answers give this category a top score.\n";
}

void displayRecommendation(const std::vector<CameraCategory>& rankedCategories,
                           const UserPreferences& preferences) {
    if (rankedCategories.empty()) return;
    const CameraCategory& best = rankedCategories.front();
    displayHeading("YOUR CAMERA RECOMMENDATION");
    displayUserSummary(preferences);

    std::cout << '\n' << std::string(72, '-') << "\n\n"
              << "  BEST MATCH\n\n"
              << "  " << best.name << '\n'
              << "  Suitability score: " << best.score << " points\n"
              << "  Compare scores within this result; they are not percentages.\n";
    if (rankedCategories.size() > 1 && best.score == rankedCategories[1].score) {
        std::cout << "\n  Joint top score: another category is equally suitable by score.\n"
                  << "  Compare the tied alternatives below before choosing.\n";
    }
    displayExplanation(preferences, best);

    std::cout << "\n  EXAMPLE MODELS IN THIS CATEGORY\n\n";
    for (const std::string& model : best.exampleModels) {
        std::cout << "  - " << model << '\n';
    }
    std::cout << "\n  Examples only. Models can span categories; features and cost vary.\n";

    for (std::size_t index = 1; index < rankedCategories.size(); ++index) {
        if (index == 1) std::cout << "\n  ALTERNATIVE OPTIONS\n\n";
        if (index == 3) std::cout << "  OTHER CATEGORIES\n\n";
        std::cout << "  " << index + 1 << ". " << rankedCategories[index].name
                  << "\n     Suitability score: " << rankedCategories[index].score
                  << " points\n\n";
    }
    std::cout << "  Equal scores share suitability; their display order is fixed.\n";
}

int getValidatedInteger(const std::string& prompt, int minValue, int maxValue) {
    std::string inputLine;
    while (true) {
        std::cout << "  " << prompt << "\n  > " << std::flush;
        if (!std::getline(std::cin, inputLine)) {
            std::cout << "\n\n  Input closed. Goodbye!\n";
            return 0;
        }
        // Separate prompts even when input is redirected and not echoed.
        std::cout << '\n';
        std::istringstream inputStream(inputLine);
        int inputValue = 0;
        char extraCharacter = '\0';
        if ((inputStream >> inputValue) && !(inputStream >> extraCharacter)
            && inputValue >= minValue && inputValue <= maxValue) {
            return inputValue;
        }
        std::cout << "  Invalid input. Enter a whole number from "
                  << minValue << " to " << maxValue << ".\n\n";
    }
}

void displayWelcome() {
    displayHeading("DIGITAL CAMERA RECOMMENDATION ASSISTANT");
    std::cout << "  Find a camera category for your purpose and personal priorities.\n\n"
              << "  Digital photography expanded into consumer and professional use.\n"
              << "  This assistant connects that development to choosing a camera.\n";
}

int getPhotographyPurpose() {
    displayHeading("QUESTION 1 OF 4 - PHOTOGRAPHY PURPOSE");
    std::cout << "  Select your main photography purpose:\n\n"
              << "  1. Family / Casual Photography\n"
              << "  2. Travel Photography\n"
              << "  3. Portrait Photography\n"
              << "  4. Sports / Wildlife Photography\n"
              << "  5. Professional / Commercial Photography\n\n";
    return getValidatedInteger("Purpose [1-5]: ", 1, 5);
}

int getExperienceLevel() {
    displayHeading("QUESTION 2 OF 4 - EXPERIENCE LEVEL");
    std::cout << "  Select your photography experience:\n\n"
              << "  1. Beginner\n"
              << "  2. Intermediate\n"
              << "  3. Advanced\n\n";
    return getValidatedInteger("Experience [1-3]: ", 1, 3);
}

int getRestartChoice() {
    displayHeading("WHAT WOULD YOU LIKE TO DO?");
    std::cout << "  1. Start another recommendation\n"
              << "  2. Exit\n\n";
    return getValidatedInteger("Choice [1-2]: ", 1, 2);
}

int getMainPriority() {
    displayHeading("QUESTION 3 OF 4 - MAIN PRIORITY");
    std::cout << "  Which feature matters most to you?\n"
              << "  Your choice gets extra weight; the other features still count.\n\n"
              << "  1. Image quality\n"
              << "  2. Portability\n"
              << "  3. Autofocus / speed\n"
              << "  4. Video capability\n\n";
    return getValidatedInteger("Main priority [1-4]:", 1, 4);
}

int getBudgetSensitivity() {
    displayHeading("QUESTION 4 OF 4 - BUDGET SENSITIVITY");
    std::cout << "  How important is keeping the cost low?\n\n"
              << "  1. Not important - price is not a major concern\n"
              << "  2. Slightly important\n"
              << "  3. Moderately important\n"
              << "  4. Important\n"
              << "  5. Very important - keeping cost low matters most\n\n";
    return getValidatedInteger("Budget sensitivity [1-5]:", 1, 5);
}

int main() {
    displayWelcome();
    std::vector<CameraCategory> cameraCategories = createCameraCategories();
    while (true) {
        UserPreferences preferences;
        preferences.photographyPurpose = getPhotographyPurpose();
        if (preferences.photographyPurpose == 0) return 0;
        preferences.experienceLevel = getExperienceLevel();
        if (preferences.experienceLevel == 0) return 0;
        const int mainPriority = getMainPriority();
        if (mainPriority == 0) return 0;
        applyMainPriority(preferences, mainPriority);
        preferences.budgetSensitivity = getBudgetSensitivity();
        if (preferences.budgetSensitivity == 0) return 0;

        calculateCategoryScores(cameraCategories, preferences);
        const std::vector<CameraCategory> rankedCategories =
            rankRecommendations(cameraCategories);
        displayRecommendation(rankedCategories, preferences);

        const int restartChoice = getRestartChoice();
        if (restartChoice == 0) return 0;
        if (restartChoice == 2) {
            std::cout << "\n  Thanks for using the Camera Recommendation Assistant. Goodbye!\n\n";
            return 0;
        }
        cameraCategories = createCameraCategories();
    }
}
