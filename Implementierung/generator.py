from argparse import ArgumentParser
import random


def random_arr(min_num: int, max_num: int, size: int) -> list[int]:
    return [random.randint(min_num, max_num) for _ in range(size)]


def print_arr(write, arr: list[int]) -> None:
    write(",".join(map(str, arr)))


def generate_matrix(write, m: int, n: int, max_num: int):
    # matrix size
    write(f"{m}, {n}")

    # how many values are going to be in the matrix
    value_count = random.randint(1, m * n)

    values = random_arr(1, max_num, value_count)
    print_arr(write, values)

    row_indices = random_arr(0, m, value_count)
    print_arr(write, row_indices)

    column_indices = random_arr(0, value_count, value_count)
    column_indices.sort()
    print_arr(write, column_indices)

acc = ""
def main():
    parser = ArgumentParser()
    parser.add_argument('-m', type=int, required=True)
    parser.add_argument('-n', type=int, required=True)
    parser.add_argument('--max', type=int, default=100)
    parser.add_argument('--seed', type=int, default=0)
    parser.add_argument('--save', type=bool, default=False)
    args = parser.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    if args.save:    
        def write(s: str):
            global acc
            acc = f"{acc}{s}\n"
        generate_matrix(write, args.m, args.n, args.max)
        with open(f"Beispiele/{args.m}={args.n}={args.max}={args.seed}.txt", "w") as f:
            f.write(acc)
    else:
        generate_matrix(print, args.m, args.n, args.max)

if __name__ == '__main__':
    main()
