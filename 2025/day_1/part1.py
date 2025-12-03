import argparse


parser = argparse.ArgumentParser()
parser.add_argument('file')
args = parser.parse_args()

START_VAL = 50
MAX_VAL   = 100

if __name__ == "__main__":

    with open(args.file) as file:
        rotations = []
        for val in file.read().splitlines():
            if val[0] == "L":
                rotations.append(-int(val[1:]))
            else:
                rotations.append(int(val[1:]))

    curr_val = START_VAL
    num_zeros = 0
    for rotation in rotations:
        curr_val = (curr_val + rotation) % MAX_VAL
        print(curr_val)
        if not curr_val:
            num_zeros += 1

    print(f"\nNumber of zeros: {num_zeros}")
