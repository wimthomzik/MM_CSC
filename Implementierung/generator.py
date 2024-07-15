from argparse import ArgumentParser
from scipy import sparse

def print_arr(arr: list[int]) -> None:
    print(",".join(map(str, arr)))
    

def main():
    parser = ArgumentParser()
    parser.add_argument('-m', type=int, required=True)
    parser.add_argument('-n', type=int, required=True)
    parser.add_argument('--density', type=float, default=0.01)
    parser.add_argument('--seed', type=int, default=0)
    args = parser.parse_args()


    m = args.m
    n = args.n
    density = args.density

    M = sparse.random(m, n, density=density, format='csc', random_state=args.seed)

    print_arr([m,n])
    print_arr([round(x, 2) for x in M.data])
    print_arr(M.indices)
    print_arr(M.indptr)

if __name__ == '__main__':
    main()
