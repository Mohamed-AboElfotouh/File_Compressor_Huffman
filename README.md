## DAAB File Compressor

This is our simple program that applies Huffman coding to compress `.txt` files and decompress our unique `.daab` files.

---

## How to Use

### Installation & Setup
1. Clone/pull the repository to your local machine.
2. Open the project using **Qt Creator** by loading the `GUI.pro` file in the `GUI` directory.
3. Build and run the project.

---

### Using the Application

#### Main Interface
- Upon launching, the main window will display all available features.
- Toggle between **Light Mode** and **Dark Mode** based on your preference.

---

#### File Compression & Decompression

**Adding Files:**
- Drag and drop any number of `.txt` or `.daab` (our custom compressed format) files into the program.
- Files are automatically sorted into their respective tables (text files vs. compressed files).
- If a file is not `.txt` or `.daab`, it will be rejected and a warning popup will appear.

---

**Selecting Files:**
- Click on individual files to select them, or use the checkbox at the top of the column to select all.
- Clicking on a file displays its **size** and **contents** in the preview panel.
- A warning popup will appear if you attempt to compress/decompress without selecting any files.

---

**Processing Files:**
- Click **Compress** to compress selected `.txt` files.
- Click **Decompress** to decompress selected `.daab` files.
- Processed files are transferred to the opposite table upon completion.

---

### Output File Naming

| Action      | Input       | Output                   |
|-------------|------------|---------------------------|
| Compress    | `test.txt` | `test.daab`               |
| Decompress  | `test.daab`| `test_decompressed.txt`    |

---

### Output Location
- All output files are saved in the **same directory** as the original source file.
- The original file remains untouched — the program only creates a copy.

> **Note:** There is no limit on the number of files that can be processed at once.

> **Fun Fact:** The `.daab` file extension is named after the initials of our team members’ surnames:  
> **D**awood, **A**boElfotouh, **A**laskndrani, **B**eshir.

---

## Contributions

### Mohamed AboElfotouh
- Assigned tasks throughout the 3 milestones with thorough task descriptions for each member.
- Implemented `hasLeftChild()`, `hasRightChild()`, `hasParent()`, `isEmpty()`, and `shiftUp()` in `priorityQ`.
- Implemented `>` and `cout` overload in `Node`.
- Implemented the preliminary GUI presented in Milestone 2, including the homepage and compression/decompression pages.
- Created the final presentation and slides.

---

### Mohammad Dawood
- Modified `>` and `cout` overload in `Node`.
- Implemented `<` and `==` operators in `Node`.
- Implemented `getCapacity()` in `priorityQ`.
- Implemented `generateFreqTable()` — collects character frequencies in `huffmanTree`.
- Implemented `makeCode()` — assigns binary codes to characters in `huffmanTree`.
- Implemented `populate()` — constructs Huffman trees for compression/decompression in `huffmanTree`.
- Implemented `compress()` — enables compression functionality in `huffmanTree`.
- Designed and implemented `freqTable` — stores gathered frequencies for decompression in `huffmanTree`.
- Responsible for making and hiding `freqTable` in `huffmanTree`.
- Upgraded the overall GUI design in Milestone 3.
- Added `clear` buttons.
- Repurposed `compress` and `decompress` buttons.
- Added Drag and Drop functionality.
- Enabled seamless transition between compressed, uncompressed, and decompressed files.
- Tuned various behaviors and interactions.
- Added Light/Dark mode functionality.
- Added Super Extreme Large Dataset for testing (10 large datasets combined).
- Updated the README.md.

---

### Omar Beshir

- Core Huffman Functionality: Developed the decompression logic (decompress()) and contributed to shared helper functions used by both compression and decompression.
- Compression Standardization: Contributed a major improvement to compress() to standardize file writing and reading, particularly concerning how whitespaces are handled.
- Huffman Tree Generation: Rewrote the populate() function to correctly generate the Huffman tree.
- Preorder Functionality: Implemented a function to produce the preorder output of the Huffman tree (now removed).
- Bit Packing Implementation:
-- Researched and implemented bit packing (writing to the file in binary) to significantly improve compression ratios.
-- Rewrote the main compression and decompression functions to utilize bitwise operations for binary file I/O, drastically reducing compressed file size.
- Codebase Refinement & Debugging:
-- Improved memory management by implementing checks to ensure dynamically allocated data is correctly deleted, eliminating memory leaks (e.g., in the priority queue and throughout the codebase).
-- Refined various functions to standardize code style and ensure proper file closing.
-- Added a .gitignore file to exclude system files and folders from the repository.
-- User Interface (UI) Features: Implemented TA-suggested features for the GUI, including showing a preview of the input/output files and their metadata (revisiting Qt concepts for UI elements).
-- Quality Assurance: Discovered and coordinated the resolution of an implicit bug within the GUI implementation.

---

### Mahmoud Alaskndrani
- Implemented `priorityQ()`, `~priorityQ()`, `push()`, `pop()`, `top()`, `getLeftChild()`, `getRightChild()`, `getParent()`, and `shiftDown()` in `priorityQ`.
- Implemented `operator>` in `Node`.
- Resolved merge conflicts and fixed resulting errors to ensure stability.
- Connected GUI to backend (created `driver.cpp`).
- Added large dataset for testing.
- Implemented file extension validation for compression (`.txt`) and decompression (`.daab`) with error handling in both GUI and backend.
