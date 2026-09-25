#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
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
         {"Canon PowerShot G7 X Mark III", "Sony ZV-1 II"}},
        {"Entry-Level Mirrorless Camera", 4, 4, 3, 4, 4,
         {"Canon EOS R50", "Sony a6400"}},
        {"Enthusiast Mirrorless Camera", 5, 3, 4, 5, 3,
         {"Fujifilm X-T5", "Sony a6700"}},
        {"High-Speed Mirrorless Camera", 5, 2, 5, 4, 2,
         {"Sony a9 III", "Canon EOS R6 Mark II"}},
        {"Professional Full-Frame Mirrorless Camera", 5, 2, 5, 5, 1,
         {"Sony a1 II", "Canon EOS R5 Mark II"}}
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
    std::cout << "\nCurrent top match: " << rankedCategories.front().name
              << " (score " << rankedCategories.front().score << ").\n";
    return 0;
}
