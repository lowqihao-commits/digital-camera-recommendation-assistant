# Digital Camera Recommendation Assistant

A C++17 console program that recommends a digital-camera category based on a user's photography purpose, experience level, and priorities.

## Inputs

Choose one photography purpose and an experience level. Rate image quality, portability, autofocus and speed, video capability, and budget sensitivity from 1 (not important) to 5 (very important). For budget sensitivity, 5 means keeping cost low matters most.

## Outputs

The program displays the highest-scoring camera category, its suitability score, two alternatives, example models in the top category, and an explanation linked to the selected purpose and highly rated priorities. Examples are illustrative; the recommendation is for a category, not a specific model.

## Recommendation Logic

Each category has capability ratings from 1 to 5. The program multiplies each capability by the corresponding user priority and adds the results. It then adds small bonuses for the selected photography purpose and experience level, and ranks all five categories by total score.

## How to Compile

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o cameraRecommender
```

## How to Run

On Windows, run `cameraRecommender.exe`. On Linux or macOS, run `./cameraRecommender`.

## Connection to Part 1

Digital photography evolved from specialist technology into multiple consumer and professional market segments. This program applies that development to a practical modern task by matching different digital-camera categories with different user needs.

## Testing

See [TEST_CASES.md](TEST_CASES.md) for the manual input and recommendation scenarios used to check the program.
