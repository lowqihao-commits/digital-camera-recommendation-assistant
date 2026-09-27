# Test Cases

The program asks for photography purpose, experience level, main priority, and budget sensitivity. It shows one best match and two alternatives.

## Representative scenarios

| Purpose and experience | Main priority | Budget | Expected best match | Score |
| --- | --- | ---: | --- | ---: |
| Family, beginner | Portability | 5 | Compact Digital Camera | 81 |
| Travel, beginner | Portability | 3 | Entry-Level Mirrorless Camera | 70 |
| Portrait, intermediate | Image quality | 2 | Enthusiast Mirrorless Camera | 70 |
| Sports/wildlife, advanced | Autofocus/speed | 1 | High-Speed Mirrorless Camera | 66 |
| Professional, advanced | Image quality | 1 | Professional Full-Frame Mirrorless Camera | 70 |
| Family, beginner | Video | 5 | Compact Digital Camera (tied with Entry-Level) | 73 |

These results use simple category ratings and bonuses. They are comparisons within the program, not measured camera performance or live prices.

## Automated C++ checks

`tests/scoring_test.cpp` checks all **300** valid answer combinations (5 purposes x 3 experience levels x 4 priorities x 5 budget values). It compares each score with the assignment's rating and bonus tables, checks the top three are in score order, and confirms ties keep a consistent order. **28** combinations produce a tie for best match.

The same test also checks invalid input, two questionnaires in one run, exit, and end-of-input after the last answer. The C++ program and test compiled with GCC 15.2.0 in C++17 mode and warning flags enabled.

To run the C++ checks from the project folder:

```bash
g++ tests/scoring_test.cpp -o scoring_test
./scoring_test
```

On Windows PowerShell, use `.\scoring_test.exe` for the second command.
