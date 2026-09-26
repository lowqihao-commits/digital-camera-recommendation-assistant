# 📷 Digital Camera Recommendation Assistant

A simple C++17 console program that helps users choose a digital-camera category based on their needs.

You answer four questions, receive a ranked recommendation, and see alternative categories with example models.

---

## 🎯 What the Program Does

- Matches photography purpose to a camera category
- Considers experience level
- Uses a main priority: image quality, portability, autofocus/speed, or video
- Considers budget sensitivity
- Displays a best match, alternatives, scores, and a short explanation

The project connects with Part 1 by applying digital-photography development to a practical camera-selection task.

---

## ▶️ How to Run

### Compile

```bash
g++ main.cpp -o cameraRecommender
```

### Start the program

Windows PowerShell:

```powershell
.\cameraRecommender.exe
```

Linux or macOS:

```bash
./cameraRecommender
```

---

## 📝 User Inputs

1. Photography purpose
2. Experience level
3. Main priority
4. Budget sensitivity

Invalid answers are rejected and the program asks again.

---

## 🧮 Recommendation Method

The program compares five camera categories using simple capability scores. The selected main priority receives extra weight, while purpose and experience add category bonuses. A higher budget-sensitivity value gives more weight to affordability.

The scores are comparison points for this program, not percentages or official camera ratings.

---

## ✅ Testing

Detailed test cases and reproducible C++ checks are available in [TEST_CASES.md](TEST_CASES.md).

No external libraries or online services are required.
