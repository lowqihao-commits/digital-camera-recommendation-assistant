# Manual Test Cases

| Case | Inputs / action | Expected result |
| --- | --- | --- |
| Invalid purpose | Enter text, blank input, or a number outside 1–5, then a valid choice | Show an error and repeat the purpose prompt |
| Invalid experience | Enter a value outside 1–3, then a valid choice | Show an error and repeat the experience prompt |
| Invalid priority | Enter letters or a value outside 1–5 for a rating, then a valid choice | Show an error and repeat that rating prompt |
| Travel / beginner | Purpose 2, experience 1, ratings 5, 4, 3, 2, 1 | Produce a ranked result; compact or entry-level categories should benefit from portability, affordability, travel, and beginner bonuses |
| Professional / advanced | Purpose 5, experience 3, ratings 5, 2, 5, 5, 1 | Produce a ranked result; professional or enthusiast categories should benefit from capability and purpose/experience bonuses |
| Restart | At the end of a result, choose 1 and complete a second questionnaire | Start a fresh questionnaire and calculate a new ranking |
| Exit | At the end of a result, choose 2 | Display goodbye and exit |
| Invalid restart | Enter text or a value outside 1–2, then a valid choice | Show an error and repeat the restart prompt |
| End of input | Close input during a questionnaire | Exit cleanly without crashing |
