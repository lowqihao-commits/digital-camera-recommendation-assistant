"""Optional standard-library test driver; the application itself is C++17 only."""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
NAMES = [
    "Compact Digital Camera", "Entry-Level Mirrorless Camera",
    "Enthusiast Mirrorless Camera", "High-Speed Mirrorless Camera",
    "Professional Full-Frame Mirrorless Camera",
]
PROFILES = [[3, 5, 2, 3, 5], [4, 4, 3, 4, 4], [5, 3, 4, 5, 3],
            [5, 2, 5, 4, 2], [5, 2, 5, 5, 1]]
PURPOSES = [[15, 10, 3, 0, 0], [12, 15, 8, 2, 1], [2, 8, 15, 5, 10],
            [0, 3, 8, 18, 12], [0, 2, 10, 12, 20]]
EXPERIENCES = [[8, 12, 3, 0, 0], [2, 5, 12, 6, 3], [0, 2, 7, 10, 12]]
SCENARIOS = [
    ("Family / casual", [1, 1, 2, 5], 0, 81),
    ("Travel", [2, 1, 2, 3], 1, 70),
    ("Portrait", [3, 2, 1, 2], 2, 70),
    ("Sports / wildlife", [4, 3, 3, 1], 3, 66),
    ("Professional", [5, 3, 1, 1], 4, 70),
    ("Budget-sensitive beginner", [1, 1, 4, 5], 0, 73),
]


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def run_suite(build, compiler):
    suffix = ".exe" if os.name == "nt" else ""
    executable = build / ("cameraRecommender" + suffix)
    flags = ["-std=c++17", "-Wall", "-Wextra", "-Wpedantic", "-Wconversion",
             "-Wshadow", "-Werror", "-D_GLIBCXX_ASSERTIONS"]
    for source, output in [(ROOT / "main.cpp", executable),
                           (ROOT / "tests/scoring_test.cpp", build / ("scoring_test" + suffix))]:
        subprocess.run([compiler, *flags, str(source), "-o", str(output)], check=True)
    scoring = subprocess.run([str(build / ("scoring_test" + suffix))],
                             capture_output=True, text=True, check=True, timeout=60)
    print(scoring.stdout.strip())
    check("46875 scoring/ranking combinations" in scoring.stdout, "Exhaustive check incomplete")
    calls = 0
    longest = 0

    def run(values, trailing_newline=True):
        nonlocal calls, longest
        content = "\n".join(map(str, values)) + ("\n" if trailing_newline else "")
        process = subprocess.run([str(executable)], input=content, text=True,
                                 capture_output=True, timeout=5)
        calls += 1
        check(process.returncode == 0, f"Nonzero exit: {process.stderr}")
        check(not process.stderr, f"Unexpected stderr: {process.stderr}")
        output = process.stdout
        check(output.isascii(), "Console contains non-ASCII characters")
        check(all(len(line) <= 72 for line in output.splitlines()), "Console line exceeds 72 columns")
        check("\n\n\n\n" not in output, "Excessive blank lines")
        check(not any(line.strip().startswith('>') and 'priority' in line
                      for line in output.splitlines()), "Prompts ran together")
        longest = max(longest, max(map(len, output.splitlines()), default=0))
        return output

    def verify_result(output, values):
        purpose, experience, priority, budget = values
        ratings = [1, 1, 1, 1, budget]
        ratings[priority - 1] = 5
        scores = [sum(a * b for a, b in zip(ratings, profile))
                  + PURPOSES[purpose - 1][i] + EXPERIENCES[experience - 1][i]
                  for i, profile in enumerate(PROFILES)]
        order = sorted(range(5), key=lambda index: -scores[index])
        match = re.search(r"BEST MATCH\s+([^\n]+)\n\s+Suitability score: (\d+)", output)
        check(match is not None, "Best match missing")
        check(match[1].strip() == NAMES[order[0]], "Incorrect best category")
        check(int(match[2]) == scores[order[0]], "Incorrect best score")
        alternatives = re.findall(r"^  ([2-5])\. (.+)\n     Suitability score: (\d+) points",
                                  output, re.M)
        check(len(alternatives) == 4, "Missing alternative categories")
        for rank, name, score in alternatives:
            index = order[int(rank) - 1]
            check(name == NAMES[index] and int(score) == scores[index], "Incorrect alternative")
        check(("Joint top score" in output) == (scores[order[0]] == scores[order[1]]),
              "Tie disclosure is incorrect")
        labels = ["Image quality", "Portability", "Autofocus / speed", "Video capability"]
        check(re.search(r"Main priority:\s+" + re.escape(labels[priority - 1]), output),
              "Main priority summary missing")
        check(re.search(r"Budget sensitivity:\s+" + str(budget) + "/5", output),
              "Budget summary missing")
        for factor in [priority - 1, 4]:
            label = (labels + ["Affordability"])[factor]
            capability = PROFILES[order[0]][factor]
            check(re.search(re.escape(label) + r"\s+Profile: " + str(capability) + "/5", output),
                  f"Missing priority explanation: {label}")
        expected_tradeoffs = sum(ratings[i] >= 4 and PROFILES[order[0]][i] <= 2
                                 for i in [priority - 1, 4])
        check(output.count("Trade-off:") == expected_tradeoffs, "Trade-off explanation mismatch")
        check("EXAMPLE MODELS IN THIS CATEGORY" in output, "Missing model examples")
        return NAMES[order[0]], scores[order[0]]

    records = []
    for title, inputs, expected_index, expected_score in SCENARIOS:
        output = run(inputs + [2])
        result = verify_result(output, inputs)
        check(result == (NAMES[expected_index], expected_score), "Scenario expectation failed")
        check(output.count("Goodbye!") == 1, "Exit message count")
        (build / (title.lower().replace(" / ", "-").replace(" ", "-") + ".txt")).write_text(output)
        records.append({"case": title, "inputs": inputs, "best": result[0], "score": result[1]})

    # Exercise every reachable four-answer questionnaire through the real console app.
    for purpose in range(1, 6):
        for experience in range(1, 4):
            for priority in range(1, 5):
                for budget in range(1, 6):
                    values = [purpose, experience, priority, budget]
                    output = run(values + [2])
                    verify_result(output, values)
                    for question in range(1, 5):
                        check(output.count(f"QUESTION {question} OF 4") == 1,
                              "Questionnaire does not have exactly four questions")

    # Each prompt must recover from each invalid token without consuming the next answer.
    baseline = SCENARIOS[1][1] + [2]
    invalid_tokens = ["abc", "", "  ", "0", "-1", "9", "3.5", "2abc", "1 2",
                      "2147483648", "9" * 4096, "1e0"]
    for position in range(5):
        for token in invalid_tokens:
            values = baseline[:position] + [token] + baseline[position:]
            output = run(values)
            check(output.count("Invalid input.") == 1, "Incorrect validation count")
            verify_result(output, baseline[:4])
            check("Thanks for using" in output, "Input recovery failed")
    output = run([" \t" + str(value) + "\t " for value in baseline])
    verify_result(output, baseline[:4])
    check("Invalid input." not in output, "Surrounding whitespace rejected")

    for length in range(5):
        output = run(baseline[:length], trailing_newline=(length > 0))
        check(output.count("Input closed. Goodbye!") == 1, "EOF did not exit exactly once")
        check(output.count("YOUR CAMERA RECOMMENDATION") == (1 if length == 4 else 0),
              "Incomplete preferences produced a recommendation")
    output = run(baseline[:4], trailing_newline=False)
    verify_result(output, baseline[:4])
    check(output.count("Input closed. Goodbye!") == 1, "Final line without newline failed")
    verify_result(run(baseline, trailing_newline=False), baseline[:4])

    first, second = SCENARIOS[1][1], SCENARIOS[4][1]
    output = run(first + [1] + second + [2])
    parts = output.split("YOUR CAMERA RECOMMENDATION")
    check(len(parts) == 3, "Restart did not complete two questionnaires")
    verify_result(parts[1], first)
    verify_result(parts[2], second)
    check(output.count("Thanks for using") == 1, "Restart session did not exit")
    (build / "restart-session.txt").write_text(output)
    values = []
    for index in range(25):
        values += (first if index % 2 == 0 else second) + [2 if index == 24 else 1]
    output = run(values)
    check(output.count("YOUR CAMERA RECOMMENDATION") == 25, "Repeated restart failed")
    summary = {"console_processes_passed": calls, "maximum_output_columns": longest,
               "exhaustive_scoring": scoring.stdout.strip(), "scenarios": records}
    (build / "test-results.json").write_text(json.dumps(summary, indent=2))
    print(f"PASS: {calls} console sessions; 72-column layout; invalid input, EOF and restart.")
    print(f"Results: {build / 'test-results.json'}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, help="Optional directory for binaries and evidence")
    parser.add_argument("--compiler", default="g++")
    arguments = parser.parse_args()
    if arguments.build_dir:
        arguments.build_dir.mkdir(parents=True, exist_ok=True)
        run_suite(arguments.build_dir.resolve(), arguments.compiler)
    else:
        with tempfile.TemporaryDirectory(prefix="camera-tests-") as temporary:
            run_suite(Path(temporary), arguments.compiler)
