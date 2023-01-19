import os
import time
import random
import subprocess
import sys

solver_executable_path = os.path.join(os.getcwd(), "a.out")

def print_tetravex(data):
    data = data.split("\n")
    size = int(len(data) ** 0.5)
    print(("+" + "-" * 5) * size + "+")
    for i in range(size):
        for tile_orientation in range(3):
            print("|", end=" ")
            for j in range(size):
                if tile_orientation == 0:
                    print(" " + data[i * size + j][0], end="  ")
                elif tile_orientation == 1:
                    print(data[i * size + j][1], end=" ")
                    print(data[i * size + j][2], end=" ")
                elif tile_orientation == 2:
                    print(" " + data[i * size + j][3], end="  ")
                print("|", end=" ")
            print()
        if (i != size - 1):
            print(("+" + "-" * 5) * size + "+")
    print(("+" + "-" * 5) * size + "+")
    

def test_solver_on_file(input_path, expected_output_path):
    output_folder = "output"
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    else:
        for file in os.listdir(output_folder):
            os.remove(os.path.join(output_folder, file))

    input_filename = input_path.split("/")[-1]
    
    # Path to the output file
    output_path = os.path.join(
        output_folder, input_filename.split(".")[0] + "-out.txt")

    # Execute the solver
    subprocess.run([solver_executable_path, input_path, output_path])

    # Compare the output with the expected output
    with open(output_path, "r") as output_file:
        output = output_file.read()
    with open(expected_output_path, "r") as expected_output_file:
        expected_output = expected_output_file.read()
    # Remove trailing newlines
    output = output.rstrip()
    expected_output = expected_output.rstrip()
    if output == expected_output:
        print("\033[92m" + input_filename + " : OK" + "\033[0m")
    else:
        print("\033[91m" + input_filename + " : KO" + "\033[0m")
        print("Expected output :")
        print_tetravex(expected_output)
        print()
        print("Output :")
        print_tetravex(output)
    
def test_solver_on_folder(input_folder, expected_output_folder):
    # List of input files sorted by name
    input_files = sorted(os.listdir(input_folder))
    
    # List of expected output files
    expected_output_paths = os.listdir(expected_output_folder)
    
    # Execute the solver on each input file and compare the output with the expected output
    for input_file in input_files:
        if os.path.isdir(os.path.join(input_folder, input_file)):
            continue
        input_path = os.path.join(input_folder, input_file)
        expected_output_path = None
        for e_path in expected_output_paths:
            cpy_e_path = e_path.split("/")[-1]
            if cpy_e_path.split("-ref")[0] == input_file.split(".")[0]:
                expected_output_path = os.path.join(expected_output_folder, e_path)
                break
        if expected_output_path == None:
            print("\033[91m" + input_file + " : KO" + "\033[0m")
            print("Expected output file not found")
            continue
        test_solver_on_file(input_path, expected_output_path)
        
def generate_tetravex(size, nb):
    for k in range(nb):
        # Generate a random tetravex
        tiles = [[0, 0, 0, 0] for _ in range(size * size)]
        for i in range(size):
            for j in range(size):
                if i > 0:
                    tiles[i * size + j][0] = tiles[(i - 1) * size + j][3]
                else:
                    tiles[i * size + j][0] = random.randint(0, 9)
                if j > 0:
                    tiles[i * size + j][1] = tiles[i * size + j - 1][2]
                else:
                    tiles[i * size + j][1] = random.randint(0, 9)
                tiles[i * size + j][3] = random.randint(0, 9)
                tiles[i * size + j][2] = random.randint(0, 9)
        
        # Shuffle the tiles
        cpy_tiles = tiles.copy()
        random.shuffle(tiles)
        
        # Generate the input file
        generated_input_folder = "generated_input/" + str(size) + "x" + str(size)
        if not os.path.exists(generated_input_folder):
            os.makedirs(generated_input_folder)
        # Input file path with the format size-timestamp.txt
        input_path = os.path.join(generated_input_folder,  "s" + str(size) + "-" + str(k) + ".txt")
        with open(input_path, "w") as input_file:
            for i, tile in enumerate(tiles):
                input_file.write(str(tile[0]) + str(tile[1]) + str(tile[2]) + str(tile[3]))
                if cpy_tiles[i] == tile:
                    input_file.write("@")
                input_file.write("\n")
        
        # Generate the expected output file
        generated_expected_output_folder = "generated_expected_output/" + str(size) + "x" + str(size)
        if not os.path.exists(generated_expected_output_folder):
            os.makedirs(generated_expected_output_folder)
        
        # Expected output file path with the format size-timestamp-ref.txt
        expected_output_path = os.path.join(generated_expected_output_folder,  "s" + str(size) + "-" + str(k) +"-ref.txt")
        with open(expected_output_path, "w") as expected_output_file:
            for tile in cpy_tiles:
                expected_output_file.write(str(tile[0]) + str(tile[1]) + str(tile[2]) + str(tile[3]) + "\n")
        
            
            
# Main function
if __name__ == "__main__":
    
    # Option handling: Usage: python script.py [--folder] [--file] input expected_output
    if len(sys.argv) == 4:
        if sys.argv[1] == "--folder":
            test_solver_on_folder(sys.argv[2], sys.argv[3])
        elif sys.argv[1] == "--file":
            test_solver_on_file(sys.argv[2], sys.argv[3])
        elif sys.argv[1] == "--generate":
            generate_tetravex(int(sys.argv[2]), int(sys.argv[3]))
        else:
            print("Usage: python script.py [--folder] [--file] input expected_output")
            print("Or: python script.py --generate size nb")
    else:
        print("Usage: python script.py [--folder] [--file] input expected_output")
        print("Or: python script.py --generate size nb")
    