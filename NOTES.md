# Cells
- There is a hard limit of 30,000 cells. (0 to 29999)
- Going below cell 0 or above cell 29999 will cause an error and program termination.
- Each cell is 1 byte large (8 bits). They are unsigned (0 to 255).
- When a cell is incremented at 255, it rolls back to 0.
- When a cell is decremented at 0, it rolls up to 255.
