from argparse import ArgumentParser
from scipy import sparse

def print_arr(acc: str, arr: list[int]) -> str:
    return acc + ",".join(map(str, arr)) + "\n"

def main():
    parser = ArgumentParser()
    parser.add_argument('-m', type=int, required=True)
    parser.add_argument('-n', type=int, required=True)
    parser.add_argument('--density', type=float, default=0.01)
    parser.add_argument('--seed', type=int, default=0)
    parser.add_argument('--save', default=False, action='store_true')
    args = parser.parse_args()


    m = args.m
    n = args.n
    density = args.density

    M = sparse.random(m, n, density=density, format='csc', random_state=args.seed)

    acc = ""
    acc = print_arr(acc, [m,n])
    acc = print_arr(acc, [round(x, 2) for x in M.data])
    acc = print_arr(acc, M.indices)
    acc = print_arr(acc, M.indptr)

    if args.save:
        path = f"./Beispiele/{m}-{n}-{density}-{args.seed}.txt"
        f = open(path, "w")
        f.write(acc)
        f.close()
    else:
        print(acc, end="")

if __name__ == '__main__':
    main()
