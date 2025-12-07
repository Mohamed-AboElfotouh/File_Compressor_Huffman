## How to Use

### Installation & Setup
1. Clone/pull the repository to your local machine
2. Open the project using **Qt Creator** by loading the `GUI.pro` file
3. Build and run the project

### Using the Application

#### Main Interface
- Upon launching, the main window will display all available features
- Toggle between **Light/Dark mode** based on your preference

#### File Compression & Decompression

**Adding Files:**
- Drag and drop any number of `.txt` or `.daab` (our custom compressed format) files into the program
- Files are automatically sorted into their respective tables (text `.txt` files vs. compressed `.daab` files)
- If a file is not `.txt` or `.daab`, it will be rejected and a warning popup will appear

**Selecting Files:**
- Check individual files checkbox to select them, or use the checkbox at the top of the column to select all
- Clicking on a file displays its **size** and **contents** in the preview panel
- A warning popup will appear if you attempt to compress/decompress without selecting any files

**Processing Files:**
- Click **Compress** to compress selected `.txt` files or **Decompress** to decompress selected `.daab` files
- Processed files are transferred to the opposite table upon completion

**Output File Naming:**
| Action | Input | Output |
|--------|-------|--------|
| Compress | `test.txt` | `test.daab` |
| Decompress | `test.daab` | `test_decompressed.txt` |

**Output Location:**
- All output files are saved in the **same directory** as the original source file
- The original file remains untouched — the program only creates a copy

> **Note:** There is no limit on the number of files that can be processed at once.

> **Fun Fact:** The `.daab` file extension is named after the initials of our team members' surnames: **D**awood, **A**boElfotouh, **A**laskndrani, **B**eshir!

---

## Contributions

### Mohamed AboElfotouh
- Assigned tasks throughout the 3 milestones with thorough task descriptions for each member
- Implemented `hasLeftChild()`, `hasRightChild()`, `hasParent()`, `isEmpty()`, and `shiftUp()` in priorityQ
- Implemented `>` and `cout` overload in Node
- Implemented the preliminary GUI presented in Milestone 2, including the homepage and compression/decompression pages
- Created the final presentation and slides

### Mohamed Dawood
- Modified `>` and `cout` overload in Node
- Implemented `<` and `==` operators in Node
- Implemented `getCapacity()` in priorityQ
- Implemented `generateFreqTable()`, `makeCode()`, `populate()`, and `compress()`

### Omar Beshir
- Implemented the decompression functionality
- Improved memory management and allocation throughout the project
- Standardized compressed file output to improve decompression
- Debugged and fixed multiple minor and major errors in the codebase
- Implemented the destructor in the heap's implementation to eliminate memory leaks
- Rewrote compression/decompression functionalities to include bit packing
- Improved the GUI by displaying file contents and size

### Mahmoud Alaskndrani
- Implemented `priorityQ()`, `~priorityQ()`, `push()`, `pop()`, `top()`, `getLeftChild()`, `getRightChild()`, `getParent()`, and `shiftDown()` in priorityQ
- Implemented `operator>` in Node
- Resolved merge conflicts and fixed resulting errors to ensure stability
- Connected GUI to backend (created `driver.cpp`)
- Added large dataset for testing
- Implemented file extension validation for compression (`.txt`) and decompression (`.daab`) with error handling in both GUI and backend