#!/usr/bin/env python3

from __future__ import annotations

import argparse
import random
import shutil
import string
from pathlib import Path


DEFAULT_SEED = 20260413
DEFAULT_TOP_LEVEL_DIRS = 12
DEFAULT_DEPTH = 3
DEFAULT_BRANCH_FACTOR = 4
DEFAULT_FILES_PER_DIR = 12
DEFAULT_LINES_PER_FILE = 120
DEFAULT_OUTPUT = "test_data/generated"

MARKER_STRINGS = [
    "grabber-target-alpha",
    "grabber-target-bravo",
    "grabber-target-charlie",
    "special invoice 8472 ready for audit",
    "needle: deterministic search payload",
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Generate a large deterministic corpus for grabber to search. "
            "The output is random-looking but repeatable from the same seed."
        )
    )
    parser.add_argument(
        "--output",
        default=DEFAULT_OUTPUT,
        help=f"Directory to populate. Default: {DEFAULT_OUTPUT}",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=DEFAULT_SEED,
        help=f"Random seed for deterministic generation. Default: {DEFAULT_SEED}",
    )
    parser.add_argument(
        "--top-level-dirs",
        type=int,
        default=DEFAULT_TOP_LEVEL_DIRS,
        help="How many top-level directories to create.",
    )
    parser.add_argument(
        "--depth",
        type=int,
        default=DEFAULT_DEPTH,
        help="How many nested directory levels to create below each top-level directory.",
    )
    parser.add_argument(
        "--branch-factor",
        type=int,
        default=DEFAULT_BRANCH_FACTOR,
        help="How many child directories each directory creates while descending.",
    )
    parser.add_argument(
        "--files-per-dir",
        type=int,
        default=DEFAULT_FILES_PER_DIR,
        help="How many files to create in each generated directory.",
    )
    parser.add_argument(
        "--lines-per-file",
        type=int,
        default=DEFAULT_LINES_PER_FILE,
        help="How many lines of text to write into each file.",
    )
    return parser.parse_args()


def random_word(rng: random.Random, min_len: int = 4, max_len: int = 18) -> str:
    alphabet = string.ascii_lowercase + string.digits
    size = rng.randint(min_len, max_len)
    return "".join(rng.choice(alphabet) for _ in range(size))


def random_line(rng: random.Random) -> str:
    token_count = rng.randint(8, 20)
    tokens = [random_word(rng) for _ in range(token_count)]
    if rng.random() < 0.18:
        tokens.insert(rng.randint(0, len(tokens)), str(rng.randint(1000, 999999)))
    return " ".join(tokens)


def iter_directory_paths(
    root: Path,
    top_level_dirs: int,
    depth: int,
    branch_factor: int,
) -> list[Path]:
    all_dirs: list[Path] = []
    frontier = []

    for index in range(top_level_dirs):
        top_dir = root / f"segment_{index:03d}"
        all_dirs.append(top_dir)
        frontier.append((top_dir, 1))

    while frontier:
        current_dir, level = frontier.pop(0)
        if level > depth:
            continue

        for branch in range(branch_factor):
            child = current_dir / f"branch_{level:02d}_{branch:02d}"
            all_dirs.append(child)
            frontier.append((child, level + 1))

    return all_dirs


def marker_locations(
    all_dirs: list[Path],
    files_per_dir: int,
    lines_per_file: int,
    marker_strings: list[str],
    seed: int,
) -> list[tuple[str, Path, int, int]]:
    placement_rng = random.Random(seed ^ 0x5A5A5A5A)
    used_positions: set[tuple[Path, int, int]] = set()
    placements: list[tuple[str, Path, int, int]] = []

    for marker in marker_strings:
        while True:
            directory = all_dirs[placement_rng.randrange(len(all_dirs))]
            file_index = placement_rng.randrange(files_per_dir)
            line_index = placement_rng.randrange(lines_per_file)
            key = (directory, file_index, line_index)
            if key not in used_positions:
                used_positions.add(key)
                placements.append((marker, directory, file_index, line_index))
                break

    return placements


def build_file_contents(
    rng: random.Random,
    lines_per_file: int,
    marker_for_lines: dict[int, list[str]],
) -> str:
    lines = [random_line(rng) for _ in range(lines_per_file)]
    for line_index, markers in marker_for_lines.items():
        prefix = random_line(rng)
        lines[line_index] = f"{prefix} {' '.join(markers)}"
    return "\n".join(lines) + "\n"


def validate_args(args: argparse.Namespace) -> None:
    numeric_fields = [
        ("top-level-dirs", args.top_level_dirs),
        ("depth", args.depth),
        ("branch-factor", args.branch_factor),
        ("files-per-dir", args.files_per_dir),
        ("lines-per-file", args.lines_per_file),
    ]
    for name, value in numeric_fields:
        if value <= 0:
            raise ValueError(f"{name} must be positive, got {value}")


def main() -> int:
    args = parse_args()
    validate_args(args)

    repo_root = Path(__file__).resolve().parent
    output_dir = (repo_root / args.output).resolve()

    if output_dir.exists():
        shutil.rmtree(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    rng = random.Random(args.seed)
    all_dirs = iter_directory_paths(
        output_dir,
        args.top_level_dirs,
        args.depth,
        args.branch_factor,
    )

    for directory in all_dirs:
        directory.mkdir(parents=True, exist_ok=True)

    placements = marker_locations(
        all_dirs,
        args.files_per_dir,
        args.lines_per_file,
        MARKER_STRINGS,
        args.seed,
    )

    marker_map: dict[tuple[Path, int], dict[int, list[str]]] = {}
    for marker, directory, file_index, line_index in placements:
        file_key = (directory, file_index)
        marker_map.setdefault(file_key, {}).setdefault(line_index, []).append(marker)

    file_count = 0
    file_names: dict[tuple[Path, int], str] = {}
    for directory in all_dirs:
        for file_index in range(args.files_per_dir):
            ext = rng.choice([".txt", ".log", ".csv", ".dat"])
            file_name = f"payload_{file_index:03d}{ext}"
            file_path = directory / file_name
            contents = build_file_contents(
                rng,
                args.lines_per_file,
                marker_map.get((directory, file_index), {}),
            )
            file_path.write_text(contents, encoding="utf-8")
            file_names[(directory, file_index)] = file_name
            file_count += 1

    manifest_path = output_dir / "SEARCH_TARGETS.txt"
    manifest_lines = [
        f"seed={args.seed}",
        f"directories={len(all_dirs)}",
        f"files={file_count}",
        f"lines_per_file={args.lines_per_file}",
        "",
        "Known search strings and their locations:",
    ]
    for marker, directory, file_index, line_index in placements:
        manifest_lines.append(
            f"{marker} -> "
            f"{directory.relative_to(output_dir)}/{file_names[(directory, file_index)]} "
            f"line {line_index + 1}"
        )
    manifest_lines.append("")
    manifest_lines.append("Example:")
    manifest_lines.append("  ./build/grabber --path test_data/generated --search grabber-target-alpha")
    manifest_path.write_text("\n".join(manifest_lines) + "\n", encoding="utf-8")

    print(f"Wrote corpus to {output_dir}")
    print(f"Created {len(all_dirs)} directories and {file_count} files")
    print(f"Known targets listed in {manifest_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
