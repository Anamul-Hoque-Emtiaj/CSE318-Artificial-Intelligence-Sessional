#!/bin/bash

# Directory containing .rud files
input_dir="set1"

# Output CSV file for the report
output_file="report.csv"

# Create or clear the output file
> "$output_file"

# Add the header row
echo ''>> "$output_file"
echo ''>> "$output_file"
echo ',,,,,,,Reports'>> "$output_file"
echo ''>> "$output_file"
echo ',,,"Problem",,,"Constructive algorithm",,"Local search",,"GRASP",' >> "$output_file"
echo ',,"Name","|V| or n","|E| or m","Simple Randomized or Randomized-1","Simple Greedy or Greedy-1","Semi-greedy-1","Simple local or local-1",,"GRASP-1",,' >> "$output_file"
echo ',,,,,,,,"No. of iterations","Best value","No. of iterations","Best value",' >> "$output_file"

# Compile the C++ program
g++ test.cpp -o test

# Iterate over .rud files in the directory
for input_file in "$input_dir"/*.rud; do
    file_name=$(basename "$input_file" .rud)
    echo Running file "$file_name"
    # Run the C++ program with the input file
    output=$(./test "$input_file")

    # Append output to the CSV report
    echo ",,$file_name,$output" >> "$output_file"
    echo "$file_name,$output"
done
