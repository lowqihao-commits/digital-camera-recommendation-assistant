#include <iostream>
#include <sstream>
#include <string>

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

int main() {
    displayWelcome();
    int photographyPurpose = getPhotographyPurpose();
    if (photographyPurpose == 0) return 0;
    std::cout << "Purpose selected: " << photographyPurpose << "\n";
    return 0;
}
