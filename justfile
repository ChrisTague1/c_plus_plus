day ending="":
    @d="scratch/$(date +%y/%m)" && mkdir -p "$d" && f="$d/day-$(date +%d){{ending}}.cpp" && if [ ! -f "$f" ]; then cp scratch/base.cpp "$f"; fi && clang++ -std=c++20 "$f" && ./a.out

parser file="sample.json":
    @cd mini_projects/json_parser && make release && ./main {{file}}

debug file="sample.json":
    @cd mini_projects/json_parser && make debug && ./main {{file}}

files:
    @clang++ -std=c++20 experiments/files/files.cpp && ./a.out