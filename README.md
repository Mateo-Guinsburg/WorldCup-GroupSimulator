# World Cup 2026 Simulator

This project is a personal backend simulator for the upcoming 48-team FIFA World Cup. I built this to practice modern C++ architecture, handling object-oriented design and memory management without being forced to use the console terminal.

## Project contents
* **C++ Backend Engine:** All the logic (sorting teams, calculating goal difference, applying FIFA tie-breaking rules) is written in pure C++.
* **No Raw Pointers:** I focused on memory safety by using `std::shared_ptr` exclusively.
* **Local Web Server:** Instead of `std::cout`, the app runs a local HTTP server using `cpp-httplib`. It serves dynamic HTML tables directly to your browser.
* **Data Persistence:** It automatically saves and loads match results to a CSV file, so you don't lose your progress when you close the app.

## Technical Milestones
* **DRY Architecture:** I refactored the code to reuse search helpers and logic, making it cleaner and easier to maintain.
* **Modern Standards:** Compiled using C++17 to ensure compatibility with multithreading and modern server features.
* **Error Handling:** I made `try-catch` blocks to prevent the system from crashing if the CSV files are messy.

## A note on the Front-End
I want to be transparent: My current university knowledge is on C++ architecture, algorithms, and system design. Since I haven't taken any web development courses yet, the HTML/CSS/JS frontend for this simulator was generated with the help of AI. I designed the backend API and the C++ routing, but the interface is a visual layer made to make the backend data easier to view.

## How to run it
If on Windows:

1. **Clone the repo.**
2. **Double-click `build.bat`** (This script compiles everything with the correct settings).
3. **Run the generated `worldcup.exe`**.
4. **Open your browser** and head to `http://localhost:8080`.

*Note: If you are on Linux or macOS, you can compile it using `g++` and the flags specified in the `build.bat` file.*

---
*Developed by Mateo Guinsburg.*