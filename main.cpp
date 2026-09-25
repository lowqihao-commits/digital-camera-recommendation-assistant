#include <iostream>

void displayWelcome() {
    std::cout << "============================================================\n"
              << "        DIGITAL CAMERA RECOMMENDATION ASSISTANT\n"
              << "============================================================\n"
              << "Find a digital-camera category that matches your\n"
              << "photography purpose and personal priorities.\n\n"
              << "Digital photography serves many needs. This program\n"
              << "will help you explore camera categories for yours.\n\n";
}

int main() {
    displayWelcome();
    return 0;
}
