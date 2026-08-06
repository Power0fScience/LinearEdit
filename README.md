# LinearEdit
A lightweight terminal-based text editor written in C++, featuring raw keyboard input, cursor navigation, file I/O, and a 1D-to-2D text rendering model using POSIX terminal APIs.


A lightweight terminal-based text editor written in C++ from scratch.

LinearEdit is a minimalist text editor that demonstrates low-level terminal programming, real-time keyboard input handling, cursor navigation, text manipulation, and file operations without relying on external GUI frameworks or editor libraries.

## Features

- Load text from a file
- Insert and delete characters
- Arrow key cursor navigation
- Save changes to disk
- Raw terminal input handling
- Lightweight and dependency-free

## Technologies

- C++17
- Standard Template Library (STL)
- POSIX terminal functions (`termios`, `unistd`, `select`)

## How it Works

The editor stores the document as a one-dimensional string while mapping the cursor position to a two-dimensional editing interface.

Internally:

```text
absolute_position = cursorX + cursorY × lineWidth
```

This approach keeps the text buffer simple while allowing intuitive cursor movement inside the terminal.

## Build

```bash
g++ -std=c++17 main.cpp -o LinearEdit
```

## Run

```bash
./LinearEdit
```

The editor loads `file.txt` from the current directory.

## Controls

| Key | Action |
|------|--------|
| ← ↑ ↓ → | Move cursor |
| Enter | New line |
| Backspace | Delete character |
| Ctrl + O | Save file |
| Ctrl + A | Exit |

## Project Goals

This project was built to explore:

- Low-level terminal programming
- Keyboard event handling
- File I/O
- Text buffer manipulation
- Cursor management
- C++ systems programming

## Future Improvements

- [ ] Dynamic line wrapping
- [ ] Horizontal and vertical scrolling
- [ ] Undo / Redo
- [ ] Search
- [ ] Syntax highlighting
- [ ] Multiple file support
- [ ] Status bar
- [ ] Line numbers

## License

MIT License
