#include <iostream>
#include <algorithm>
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
    int imageQualityPriority = 0;
    int portabilityPriority = 0;
    int autofocusSpeedPriority = 0;
    int videoPriority = 0;
    int budgetSensitivity = 0;
};

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
    std::sort(categories.begin(), categories.end(),
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

void displayExplanation(const UserPreferences& preferences,
                        const CameraCategory& recommendation) {
    std::cout << "\nWhy this may suit you:\n"
              << "You selected " << getPhotographyPurposeName(preferences.photographyPurpose)
              << ". The category's capabilities and your ratings contributed to its score.\n";

    if (preferences.imageQualityPriority >= 4) {
        std::cout << "- You gave image quality a high priority.\n";
    }
    if (preferences.portabilityPriority >= 4) {
        std::cout << "- You prefer a more portable camera.\n";
    }
    if (preferences.autofocusSpeedPriority >= 4) {
        std::cout << "- Fast autofocus and shooting speed matter to you.\n";
    }
    if (preferences.videoPriority >= 4) {
        std::cout << "- You value video capability.\n";
    }
    if (preferences.budgetSensitivity >= 4) {
        std::cout << "- Keeping cost low is important to you.\n";
    }
    std::cout << "The profile for " << recommendation.name
              << " performed best against those inputs and the selected purpose and experience bonuses.\n";
}

void displayRecommendation(const std::vector<CameraCategory>& rankedCategories,
                           const UserPreferences& preferences) {
    if (rankedCategories.size() < 3) return;

    const CameraCategory& best = rankedCategories[0];
    std::cout << "\n============================================================\n"
              << "                 YOUR CAMERA RECOMMENDATION\n"
              << "============================================================\n"
              << "Best match: " << best.name << "\n"
              << "Suitability score: " << best.score << "\n"
              << "Best for: " << getPhotographyPurposeName(preferences.photographyPurpose)
              << "\n\nExample models in this category (examples only):\n";
    for (const std::string& model : best.exampleModels) {
        std::cout << "- " << model << "\n";
    }
    displayExplanation(preferences, best);

    std::cout << "\nAlternative options:\n";
    for (std::size_t index = 1; index < 3; ++index) {
        std::cout << index + 1 << ". " << rankedCategories[index].name
                  << " (score " << rankedCategories[index].score << ")\n";
    }
}

int getValidatedInteger(const std::string& prompt, int minValue, int maxValue) {
    std::string inputLine;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, inputLine)) {
            std::cout << "\nInput closed. Goodbye!\n";
            return 0;
        }
        std::istringstream inputStream(inputLine);
        int inputValue = 0;
        char extraCharacter = '\0';
        if ((inputStream >> inputValue) && !(inputStream >> extraCharacter)
            && inputValue >= minValue && inputValue <= maxValue) {
            return inputValue;
        }
        std::cout << "Invalid input. Enter a whole number from "
                  << minValue << " to " << maxValue << ".\n";
    }
}

void displayWelcome() {
    std::cout << "============================================================\n"
              << "        DIGITAL CAMERA RECOMMENDATION ASSISTANT\n"
              << "============================================================\n"
              << "Find a digital-camera category that matches your\n"
              << "photography purpose and personal priorities.\n\n"
              << "Digital photography serves many needs. This program\n"
              << "will help you explore camera categories for yours.\n\n";
}

int getPhotographyPurpose() {
    std::cout << "Select your main photography purpose:\n"
              << "1. Family / Casual Photography\n"
              << "2. Travel Photography\n"
              << "3. Portrait Photography\n"
              << "4. Sports / Wildlife Photography\n"
              << "5. Professional / Commercial Photography\n";
    return getValidatedInteger("Purpose [1-5]: ", 1, 5);
}

int getExperienceLevel() {
    std::cout << "\nSelect your photography experience:\n"
              << "1. Beginner\n"
              << "2. Intermediate\n"
              << "3. Advanced\n";
    return getValidatedInteger("Experience [1-3]: ", 1, 3);
}

int getRestartChoice() {
    std::cout << "\nWhat would you like to do?\n"
              << "1. Start another recommendation\n"
              << "2. Exit\n";
    return getValidatedInteger("Choice [1-2]: ", 1, 2);
}

int getPriorityRating(const std::string& factor) {
    return getValidatedInteger(factor + " priority [1-5]: ", 1, 5);
}

void collectPriorityRatings(UserPreferences& preferences) {
    std::cout << "\nRate each factor from 1 (Not Important) to 5 (Very Important).\n";
    preferences.imageQualityPriority = getPriorityRating("Image quality");
    preferences.portabilityPriority = getPriorityRating("Portability");
    preferences.autofocusSpeedPriority = getPriorityRating("Autofocus / speed");
    preferences.videoPriority = getPriorityRating("Video capability");
    std::cout << "For budget, 1 means price is not a major concern;\n"
              << "5 means keeping cost low is very important.\n";
    preferences.budgetSensitivity = getPriorityRating("Budget sensitivity");
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
        collectPriorityRatings(preferences);
        if (std::cin.eof()) return 0;

        calculateCategoryScores(cameraCategories, preferences);
        const std::vector<CameraCategory> rankedCategories =
            rankRecommendations(cameraCategories);
        displayRecommendation(rankedCategories, preferences);

        const int restartChoice = getRestartChoice();
        if (restartChoice == 0) return 0;
        if (restartChoice == 2) {
            std::cout << "\nThanks for using the Digital Camera Recommendation Assistant. Goodbye!\n";
            return 0;
        }
        cameraCategories = createCameraCategories();
    }
}
