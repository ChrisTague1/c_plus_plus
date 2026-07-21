import argparse
import json
import math
import random
import string
import sys
from pathlib import Path

################################################################################
# Random helpers
################################################################################

ESCAPES = [
    "\n",
    "\t",
    "\\",
    "\"",
]

SAFE_PRINTABLE = (
    string.ascii_letters +
    string.digits +
    string.punctuation +
    " "
)


def random_string(rng: random.Random) -> str:
    """Generate a variety of strings."""
    mode = rng.randint(0, 4)

    if mode == 0:
        return "".join(
            rng.choice(string.ascii_letters)
            for _ in range(rng.randint(0, 40))
        )

    if mode == 1:
        return "".join(
            rng.choice(SAFE_PRINTABLE)
            for _ in range(rng.randint(0, 100))
        )

    if mode == 2:
        return "".join(
            rng.choice(ESCAPES)
            for _ in range(rng.randint(5, 50))
        )

    if mode == 3:
        return "A" * rng.randint(100, 500)

    return ""


def random_number(rng: random.Random):
    choice = rng.randint(0, 4)

    if choice == 0:
        return rng.randint(-(2**63), 2**63 - 1)

    if choice == 1:
        # Regular decimal floats only
        return round(rng.uniform(-1e12, 1e12), rng.randint(0, 6))

    if choice == 2:
        return round(rng.random(), rng.randint(0, 6))

    if choice == 3:
        return round(
            rng.randint(-1000, 1000) / rng.randint(1, 100),
            rng.randint(0, 6)
        )

    return 0.0


################################################################################
# Recursive generation
################################################################################


def generate_value(rng: random.Random, depth: int):
    if depth <= 0:
        leaf = rng.randint(0, 5)

        if leaf == 0:
            return None
        if leaf == 1:
            return rng.choice([True, False])
        if leaf == 2:
            return random_number(rng)
        if leaf == 3:
            return random_string(rng)
        if leaf == 4:
            return []
        return {}

    t = rng.randint(0, 5)

    if t == 0:
        return None

    if t == 1:
        return rng.choice([True, False])

    if t == 2:
        return random_number(rng)

    if t == 3:
        return random_string(rng)

    if t == 4:
        return [
            generate_value(rng, depth - 1)
            for _ in range(rng.randint(0, 25))
        ]

    obj = {}

    for _ in range(rng.randint(0, 20)):
        obj[random_string(rng)] = generate_value(rng, depth - 1)

    return obj


################################################################################
# Large structures
################################################################################


def make_large_array(rng: random.Random, count: int):
    return [
        generate_value(rng, 4)
        for _ in range(count)
    ]


def make_wide_object(rng: random.Random, count: int):
    return {
        f"key_{i}_{rng.randint(0, 999999)}": generate_value(rng, 3)
        for i in range(count)
    }


def make_deep_object(depth: int):
    root = {}
    current = root

    for i in range(depth):
        nxt = {}
        current[f"level_{i}"] = nxt
        current = nxt

    current["end"] = True
    return root


def make_deep_array(depth: int):
    arr = []

    root = arr
    for _ in range(depth):
        nxt = []
        arr.append(nxt)
        arr = nxt

    arr.append("bottom")
    return root


################################################################################
# Main
################################################################################


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--seed",
        type=int,
        help="Random seed. Uses a random seed if omitted."
    )

    parser.add_argument(
        "--output",
        default="huge.temp.json"
    )

    parser.add_argument(
        "--array-size",
        type=int,
        default=1000,
        help="Number of entries in the large array."
    )

    parser.add_argument(
        "--object-size",
        type=int,
        default=100,
        help="Number of keys in the wide object."
    )

    parser.add_argument(
        "--deep-depth",
        type=int,
        default=10,
        help="Depth of nested structures."
    )

    args = parser.parse_args()

    if args.seed is None:
        args.seed = random.randrange(sys.maxsize)

    print(f"Seed: {args.seed}")

    rng = random.Random(args.seed)

    document = {
        "seed": args.seed,

        "null": None,
        "true": True,
        "false": False,

        "integers": [
            0,
            1,
            -1,
            2**31,
            -(2**31),
            2**63 - 1,
            -(2**63),
        ],

        "floats": [
            0.0,
            -0.0,
            1.5,
            -1.5,
            3.14159,
            -273.15,
            1000000.123456,
        ],

        "strings": [
            "",
            "hello",
            "quote: \"",
            "backslash: \\",
            "newline\nhere",
            "A" * 50000,
        ],

        "mixed": [
            None,
            True,
            False,
            123,
            5.67,
            "abc",
            [],
            {},
        ],

        "deep_object": make_deep_object(args.deep_depth),

        "deep_array": make_deep_array(args.deep_depth),

        "wide_object": make_wide_object(
            rng,
            args.object_size,
        ),

        "large_array": make_large_array(
            rng,
            args.array_size,
        ),
    }

    output = Path(args.output)

    with output.open("w", encoding="utf-8") as f:
        f.write(json.dumps(document, indent=4))

    size_mb = output.stat().st_size / (1024 * 1024)

    print(f"Wrote {output}")
    print(f"Size: {size_mb:.2f} MB")


if __name__ == "__main__":
    main()