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
        print(f"{(curr_val, rotation)} -> {(curr_val + rotation) % MAX_VAL}")
        num_rotations = int(rotation / MAX_VAL)
        if num_rotations:
            rotation -= num_rotations * MAX_VAL

        num_rotations = abs(num_rotations)

        new_val = curr_val + rotation
        if curr_val and (new_val <= 0 or new_val >= MAX_VAL):
            num_rotations += 1

        num_zeros += num_rotations
        print(f"Num rotations: {num_rotations}")

        curr_val = new_val % MAX_VAL
        print(curr_val)

    print(f"\nNumber of zeros: {num_zeros}")
