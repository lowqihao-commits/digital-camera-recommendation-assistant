# Verified Test Cases

The test suite compiles the actual `main.cpp` with GCC 15.2.0 in C++17 mode, using `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -D_GLIBCXX_ASSERTIONS`. Both the program and scoring harness compiled without warnings. The application remains standard C++17; Python is used only for optional test automation.

## Recommendation Scenarios

The questionnaire has four inputs: purpose, experience, main priority, and budget sensitivity. Main priority 1 = image quality, 2 = portability, 3 = autofocus/speed, 4 = video. The selected feature gets weight 5 and other features get weight 1. Each case also checks all other category scores and ordering, the four-answer summary, explanation, model-example heading, and exit.

| Case | Purpose / experience | Main priority / budget | Expected and actual best match | Score | Status |
| --- | --- | --- | --- | ---: | --- |
| 1 | Family / beginner | Portability / 5 | Compact Digital Camera | 81 | PASS |
| 2 | Travel / beginner | Portability / 3 | Entry-Level Mirrorless Camera | 70 | PASS |
| 3 | Portrait / intermediate | Image quality / 2 | Enthusiast Mirrorless Camera | 70 | PASS |
| 4 | Sports / advanced | Autofocus/speed / 1 | High-Speed Mirrorless Camera | 66 | PASS |
| 5 | Professional / advanced | Image quality / 1 | Professional Full-Frame Mirrorless Camera | 70 | PASS |
| 6 | Budget-sensitive family beginner | Video / 5 | Compact and Entry-Level Mirrorless share the top score; compact is displayed first | 73 | PASS |

## Exhaustive Scoring Checks

`tests/scoring_test.cpp` checks the internal scoring functions with **46,875** combinations: 5 purposes x 3 experience levels x 5^5 weight combinations. This is broader than the 300 questionnaires reachable through the new four-input interface. All passed:

- Exact scores agree with independent fixtures transcribed from the project specification.
- All five categories appear once, in descending score order; scores stay attached to the correct names.
- Equal scores retain the documented display order. **3,494** combinations have a joint top score.
- Increasing budget sensitivity rewards each category by its affordability rating.
- Sorting does not change the source category order used by the bonus tables.
- Changing the selected main priority sets one feature to weight 5, resets the others to 1, and preserves the budget answer.

These tests establish consistency with the educational scoring model; they do not prove that one camera is objectively best for every real-world photographer. No profile weights required adjustment in the six representative scenarios.

## Console and Layout Checks

**376 separate console sessions passed**, including every valid four-answer questionnaire:

| Check | Coverage / observed behavior | Status |
| --- | --- | --- |
| Representative scenarios | Six cases above | PASS |
| Exhaustive questionnaires | All 300 combinations: 5 purposes x 3 experience levels x 4 main priorities x 5 budget levels; exact ranking, ties, trade-offs and exactly four question headings | PASS |
| Invalid menu / budget / restart input | 12 invalid tokens at each of five prompts: letters, blanks, whitespace, 0, -1, 9, decimals, trailing letters, multiple numbers, integer overflow, 4,096 digits and scientific notation; one error then recovery | PASS |
| Surrounding whitespace | Valid integers with spaces and tabs accepted | PASS |
| End of input | EOF at each prompt exits once; incomplete questionnaires show no result | PASS |
| Missing final newline | Complete questionnaire still produces a result; explicit exit works | PASS |
| Restart | Travel then professional questionnaires in one process; both full rankings checked | PASS |
| Repeated restart | 25 alternating questionnaires, followed by exit | PASS |
| Priorities / ties | Selected main priority and budget influence explained; joint top scores disclosed | PASS |
| Terminal spacing | Every captured output line uses ASCII and is at most 72 columns; no joined prompts or excessive blank-line runs | PASS |

A real interactive Windows terminal run also verifies menu spacing, echoed answers, the four-answer summary, sports results, and exit.

## Reproduce

From the project directory, with GCC and Python 3 available:

```bash
python tests/test_program.py
```

To retain test binaries, transcripts and a JSON result file locally:

```bash
python tests/test_program.py --build-dir work/test-results
```

Expected summary:

```text
PASS: 46875 scoring/ranking combinations; 3494 joint-top combinations; budget direction checked.
PASS: 376 console sessions; 72-column layout; invalid input, EOF and restart.
```
