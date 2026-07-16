- There needs to be lookahead on the keywords so `trueah` isn't valid
- Add lots of test cases
- Error handling when shit goes wrong
- Benchmark tf out of it
- Make the sdk good
- try and make a lazy version

# Notes for self

We are essentially building a JsonValue object which is a union of:

- number
- null
- boolean
- string
- object
- array

For the root of a document the only allowed values are object and array