from argparse import ArgumentParser
import random


def random_arr(min_num: int, max_num: int, size: int) -> list[int]:
    return [random.randint(min_num, max_num) for _ in range(size)]


def print_arr(arr: list[int]) -> None:
    print(", ".join(map(str, arr)))


def generate_matrix(m: int, n: int, max_num: int):
    # matrix size
    print(f"{m}, {n}")

    # how many values are going to be in the matrix
    value_count = random.randint(1, m * n)

    values = random_arr(1, max_num, value_count)
    print_arr(values)

    row_indices = random_arr(0, m, value_count)
    print_arr(row_indices)

    column_indices = random_arr(0, value_count, value_count)
    column_indices.sort()
    print_arr(column_indices)


def main():
    parser = ArgumentParser()
    parser.add_argument('-m', type=int, required=True)
    parser.add_argument('-n', type=int, required=True)
    parser.add_argument('--max', type=int, default=100)
    parser.add_argument('--seed', type=int, default=0)
    args = parser.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    generate_matrix(args.m, args.n, args.max)


if __name__ == '__main__':
    main()
