import random

def generate_input_file(filename, num_numbers):
    with open(filename, 'w') as file:
        for _ in range(num_numbers):
            file.write(str(random.randint(1, num_numbers*10)))
            file.write(' ')

if __name__ == "__main__":
    filename = "input.txt"
    num_numbers = 10_000_000
    generate_input_file(filename, num_numbers)