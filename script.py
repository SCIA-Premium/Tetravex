import os
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
        #print_tetravex(expected_output)
        print()
        print("Output :")
        #print_tetravex(output)
    
def test_solver_on_folder(input_folder, expected_output_folder):
    # List of input files sorted by name
    input_files = sorted(os.listdir(input_folder))
    
    # List of expected output files
    expected_output_paths = os.listdir(expected_output_folder)
    
    # Execute the solver on each input file and compare the output with the expected output
    for input_file in input_files:
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

# Main function
if __name__ == "__main__":
    
    # Option handling: Usage: python script.py [--folder] [--file] input expected_output
    if len(sys.argv) == 4:
        if sys.argv[1] == "--folder":
            test_solver_on_folder(sys.argv[2], sys.argv[3])
        elif sys.argv[1] == "--file":
            test_solver_on_file(sys.argv[2], sys.argv[3])
        else:
            print("Usage: python script.py [--folder] [--file] input expected_output")
    else:
        print("Usage: python script.py [--folder] [--file] input expected_output")
    