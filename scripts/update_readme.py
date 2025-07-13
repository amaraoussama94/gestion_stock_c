#this script updates the README.md file with the latest summary from a specified file.
import sys

SUMMARY_PATH = sys.argv[1]
README_PATH = "README.md"
START_TAG = "<!-- weekly-report-start -->"
END_TAG = "<!-- weekly-report-end -->"

with open(SUMMARY_PATH, "r", encoding="utf-8") as f:
    summary = f.read().strip()

with open(README_PATH, "r", encoding="utf-8") as f:
    lines = f.readlines()

new_lines = []
inside = False
for line in lines:
    if START_TAG in line:
        new_lines.append(line)
        new_lines.append(summary + "\n")
        inside = True
        continue
    if END_TAG in line:
        new_lines.append(line)
        inside = False
        continue
    if not inside:
        new_lines.append(line)

with open(README_PATH, "w", encoding="utf-8") as f:
    f.writelines(new_lines)

print(f" README updated with summary from {SUMMARY_PATH}")
