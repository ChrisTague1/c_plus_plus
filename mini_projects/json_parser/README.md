- There needs to be lookahead on the keywords so `trueah` isn't valid
- Add lots of test cases
- Error handling when shit goes wrong
- Benchmark tf out of it
- Make the sdk good
- try and make a lazy version

# Notes for self

- lexer should keep track of position of each character in the file for errors
- lexer should load things into a buffer instead of literally going character by character
- lexer should throw errors when you do something wack
- parser should throw errors when you do something wack
- make an sdk for access
- benchmark tf out of this and optimize
