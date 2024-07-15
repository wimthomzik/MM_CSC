from scipy.sparse import csc_matrix
from numpy import matrix
from argparse import ArgumentParser
import subprocess

def parse_line(line: str, fn) -> list:
    return [fn(x) for x in line.split(",")]

def create_file(path: str):
    f = open(path, "w")
    f.write("")
    f.close()

def parse_matrix(matrix: str) -> csc_matrix:
    lines = matrix.split("\n")

    dimensions = parse_line(lines[0], int)
    m = dimensions[0]
    n = dimensions[1]
    
    values = parse_line(lines[1], float)
    row_indices = parse_line(lines[2], int)
    col_ptrs = parse_line(lines[3], int)

    return csc_matrix((values, row_indices, col_ptrs), shape=(m, n))

def read_file(path: str) -> str:
    with open(path, "r") as f:
        c = f.read()
        if c == "":
            print(f"Empty file: {path}")
            exit(1)
        return c
    raise Exception("file error")

def multiply_with_c(a_path: str, b_path: str) -> matrix:
    result_path = "./Beispiele/Ergebnis.txt"
    create_file(result_path)
    handle = subprocess.run(["./Implementierung/main", "-a", a_path, "-b", b_path, "-o", result_path])
    if handle.returncode != 0:
        print(f"Error executing c matrix multiplication: process exited with status {handle.returncode}")
        exit(1)
    f = read_file(result_path)
    return parse_matrix(f).todense()

def run(a_path: str, b_path: str):
    a = parse_matrix(read_file(a_path))
    b = parse_matrix(read_file(a_path))

    python_result = a.multiply(b).todense()
    c_result = multiply_with_c(a_path, b_path)
    
    print("Python result:")
    print(python_result)
    
    print("C result:")
    print(c_result)

    print("Result:")
    if c_result == python_result:
        print("The arrays are equal")
    else:
        print("The arrays are ** Not ** equal")

def main():
    parser = ArgumentParser()
    parser.add_argument('a', type=str)
    parser.add_argument('b', type=str)
    args = parser.parse_args()

    run(args.a, args.b)
    
if __name__ == "__main__":
    main()