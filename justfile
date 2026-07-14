parser file="sample.json":
    @clang++ -std=c++20 mini_projects/json_parser/main.cpp && ./a.out mini_projects/json_parser/{{file}}

files:
    @clang++ -std=c++20 experiments/files/files.cpp && ./a.out