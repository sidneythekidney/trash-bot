# Python file for extracting openings from the lichess database:
import sys
import re

if __name__ == "__main__":
    count = 0
    openings = {}
    final_openings = {}

    # Read in openings and lines
    for line in sys.stdin.readlines():
        count += 1
        if 'Opening' in line:
            # Record the opening
            curr_opening = re.findall(r'"([^"]*)"', line)[0]
            if curr_opening not in openings.keys():
                openings[curr_opening] = []
        if '1. ' in line:
            curr_line = line
            openings[curr_opening].append(line)

    # Remove openings with fewer than 5 entries:
    rem_openings = []
    for open_name in openings.keys():
        if len(openings[open_name]) < 5:
            rem_openings.append(open_name)
    for open_name in rem_openings:
        del openings[open_name]

    # For the openings remaining, find the maximum string that all openings contain
    for name, lines in openings.items():
        lines.sort(key=len)
        max_str = lines[0]
        for line in lines:
            curr_str = ""
            for i in range(min(len(max_str), len(line))):
                if line[i] != max_str[i]:
                    max_str = curr_str
                else:
                    curr_str += line[i]
        final_openings[name] = max_str

    # Print the final openings:
    for name, line in final_openings.items():
        print(name + ': ' + line)
