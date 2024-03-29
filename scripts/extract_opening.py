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
        if '15. ' in line: # we only want to consider openings that are played out (what if a player quits after move 3?)
            if 'eval' not in line: # We want to ignore lines which have eval added to pgn
                curr_line = line
                openings[curr_opening].append(line)

    # Remove openings with fewer than 5 entries:
    rem_openings = []
    for open_name in openings.keys():
        if len(openings[open_name]) < 5:
            rem_openings.append(open_name)
    for open_name in rem_openings:
        del openings[open_name]

    # For the openings remaining, find the maximum line that all openings contain
    for name, lines in openings.items():
        lines.sort(key=len)
        max_str = lines[0]
        for line in lines:
            curr_str = ""
            for i in range(min(len(max_str), len(line))):
                if line[i] != max_str[i]:
                    max_str = curr_str
                    break
                else:
                    curr_str += line[i]
        final_openings[name] = max_str

    # Remove openings which have early multiple branches (branch before move 4)
    rem_openings = []
    for open_name, line in final_openings.items():
        if '4.' not in line:
            rem_openings.append(open_name)
    for open_name in rem_openings:
        del final_openings[open_name]

    # Modify the openings to only contain the moves:
    for name, line in final_openings.items():
        # Remove all move position items
        text = line.strip().split(' ')
        text = [word for word in text if '.' not in word]
        text = ' '.join(text)
        final_openings[name] = text

    # Print the final openings:
    for name, line in final_openings.items():
        # Handle last move error:
        if len(line.split(' ')[-1]) == 1:
            line = ' '.join(line.split(' ')[:-1])
        print(name + ': ' + line)
