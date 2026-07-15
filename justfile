parser file="sample.json":
    @cd mini_projects/json_parser && clang++ -std=c++20 main.cpp && ./a.out {{file}}

debug:
    @cd mini_projects/json_parser && make run-debug

files:
    @clang++ -std=c++20 experiments/files/files.cpp && ./a.out