# Digital Camera Recommendation Assistant

A C++17 console program that recommends a digital-camera category based on a user's photography purpose, experience level, and priorities.

## Inputs

Answer four questions:

1. **Photography purpose:** family/casual, travel, portrait, sports/wildlife, or professional/commercial.
2. **Experience:** beginner, intermediate, or advanced.
3. **Main priority:** image quality, portability, autofocus/speed, or video capability.
4. **Budget sensitivity:** 1 means price is not a major concern; 5 means keeping cost low is very important.

## Outputs

The program displays your four answers, the highest-scoring camera category, two main alternatives, and the remaining categories. Each result includes a suitability score. The explanation connects your purpose, experience, main priority and budget to the selected profile, including important trade-offs. After viewing the result, choose to start another recommendation or exit.

Example models illustrate the selected category. Categories overlap, and individual models can have different features and costs from the simplified profile.

## Recommendation Logic

Each category has capability ratings from 1 to 5. Your main priority receives weight 5; the other three features receive weight 1. Your budget answer supplies the affordability weight. The program multiplies capabilities by these weights, adds the results, then adds purpose and experience bonuses. A high budget-sensitivity rating rewards affordability.

Scores are comparison points within one questionnaire, not percentages or measured camera performance. Equal scores indicate equal suitability in this model. Ties retain the category order: compact, entry-level, enthusiast, high-speed, professional. The supplied profile values and bonuses are unchanged; they are educational assumptions, not manufacturer ratings or live prices.

## How to Compile

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o cameraRecommender
```

## How to Run

In Windows PowerShell:

```powershell
.\cameraRecommender.exe
```

On Linux or macOS, run `./cameraRecommender`. Use a terminal at least 72 columns wide. Enter one whole-number answer per prompt; invalid input is rejected with a retry prompt.

## Connection to Part 1

Digital photography evolved from specialist technology into multiple consumer and professional market segments. This program applies that development to a practical modern task by matching different digital-camera categories with different user needs.

## Testing

See [TEST_CASES.md](TEST_CASES.md) for verified scenarios and repeatable checks. The C++ application needs no external libraries or network connection. An optional Python standard-library test driver builds the C++ program and checks console sessions:

```bash
python tests/test_program.py
```

## Example Model Sources

Model names were checked against manufacturer pages. These links identify real models; they do not establish local availability, current prices, or the program's capability ratings.

- Canon: [PowerShot G7 X Mark III](https://www.usa.canon.com/support/p/powershot-g7-x-mark-iii); [EOS R50, R7, R5 Mark II and R1](https://www.usa.canon.com/digital-cameras/eos-webcam-utility/compatible-cameras).
- Nikon: [Z50II, Z6III, Z8 and Z9](https://imaging.nikon.com/imaging/lineup/mirrorless/index.html).
- Sony: [RX100 VII](https://www.sony.com/electronics/support/product/dsc-rx100m7/downloads), [a6400](https://www.sony.com/electronics/support/e-mount-body-ilce-6000-series/ilce-6400), [a6700](https://www.sony.com/electronics/support/e-mount-body-ilce-6000-series/ilce-6700), [a9 III](https://electronics.sony.com/imaging/interchangeable-lens-cameras/all-interchangeable-lens-cameras/p/ilce9m3b), [a1 II](https://electronics.sony.com/imaging/interchangeable-lens-cameras/full-frame/p/ilce1m2b).
