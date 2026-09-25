// Exercise the production functions while keeping the app's entry point separate.
#define main cameraProgramMain
#include "../main.cpp"
#undef main
#include <array>
#include <stdexcept>

void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

int main() {
    try {
        // Specification fixtures; do not calculate expected values from runtime profiles.
        const int profiles[5][5] = {
            {3,5,2,3,5}, {4,4,3,4,4}, {5,3,4,5,3}, {5,2,5,4,2}, {5,2,5,5,1}
        };
        const int purposes[5][5] = {
            {15,10,3,0,0}, {12,15,8,2,1}, {2,8,15,5,10}, {0,3,8,18,12}, {0,2,10,12,20}
        };
        const int experiences[3][5] = {{8,12,3,0,0}, {2,5,12,6,3}, {0,2,7,10,12}};
        const auto original = createCameraCategories();
        require(original.size() == 5, "Expected five categories");
        UserPreferences chosen;
        chosen.budgetSensitivity = 3;
        for (int priority = 1; priority <= 4; ++priority) {
            applyMainPriority(chosen, priority);
            const int weights[] = {chosen.imageQualityPriority, chosen.portabilityPriority,
                                   chosen.autofocusSpeedPriority, chosen.videoPriority};
            for (int index = 0; index < 4; ++index) {
                require(weights[index] == (index == priority - 1 ? 5 : 1),
                        "Main priority mapping or reset failed");
            }
            require(chosen.mainPriority == priority && chosen.budgetSensitivity == 3,
                    "Main priority changed unrelated answers");
        }
        int combinations = 0;
        int tiedResults = 0;
        for (int purpose = 1; purpose <= 5; ++purpose) {
            for (int experience = 1; experience <= 3; ++experience) {
                for (int encoded = 0; encoded < 3125; ++encoded) {
                    int remaining = encoded;
                    std::array<int, 5> ratings{};
                    for (int& rating : ratings) {
                        rating = remaining % 5 + 1;
                        remaining /= 5;
                    }
                    UserPreferences preferences{purpose, experience, ratings[0],
                        ratings[1], ratings[2], ratings[3], ratings[4]};
                    auto categories = original;
                    calculateCategoryScores(categories, preferences);
                    for (std::size_t index = 0; index < 5; ++index) {
                        int expected = purposes[purpose - 1][index]
                            + experiences[experience - 1][index];
                        for (std::size_t factor = 0; factor < 5; ++factor) {
                            expected += ratings[factor] * profiles[index][factor];
                        }
                        require(categories[index].score == expected, "Score mismatch");
                        require(categories[index].exampleModels.size() >= 2,
                                "Missing model examples");
                    }
                    const auto ranked = rankRecommendations(categories);
                    require(ranked.size() == 5, "Lost category");
                    std::array<bool, 5> seen{};
                    std::size_t previousIndex = 0;
                    for (std::size_t position = 0; position < 5; ++position) {
                        std::size_t index = 0;
                        while (index < 5 && original[index].name != ranked[position].name) ++index;
                        require(index < 5, "Unknown category");
                        require(!seen[index], "Duplicate category");
                        seen[index] = true;
                        require(ranked[position].score == categories[index].score,
                                "Score moved to wrong category");
                        if (position > 0) {
                            require(ranked[position - 1].score >= ranked[position].score,
                                    "Ranking not descending");
                            if (ranked[position - 1].score == ranked[position].score) {
                                require(previousIndex < index, "Unstable tie ordering");
                            }
                        }
                        previousIndex = index;
                    }
                    require(categories[0].name == original[0].name, "Source order changed");
                    if (ranked[0].score == ranked[1].score) ++tiedResults;
                    if (preferences.budgetSensitivity < 5) {
                        ++preferences.budgetSensitivity;
                        const auto before = categories;
                        calculateCategoryScores(categories, preferences);
                        for (std::size_t index = 0; index < 5; ++index) {
                            require(categories[index].score - before[index].score
                                    == profiles[index][4], "Budget direction reversed");
                        }
                    }
                    ++combinations;
                }
            }
        }
        require(rankRecommendations({}).empty(), "Empty ranking failed");
        std::cout << "PASS: " << combinations << " scoring/ranking combinations; "
                  << tiedResults << " joint-top combinations; budget direction checked.\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
