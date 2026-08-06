#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

// Key codes for Windows/Linux terminal input
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_CTRL_O 15


int cursorX, cursorY, absolute_position;
size_t lines;
size_t start_index, num_chars;
size_t textLength;
bool running;

std::string text,subtext,displayText, textWithcursor;

static struct termios oldt;
static bool rawModeEnabled = false;

void clearScreen(){
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
    std::cout.flush();
#endif
}

void enableRawMode(){
    if (rawModeEnabled) {
        return;
    }

    tcgetattr(STDIN_FILENO, &oldt);
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    rawModeEnabled = true;
}

void disableRawMode(){
    if (!rawModeEnabled) {
        return;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    rawModeEnabled = false;
}

int _kbhit(){
    struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv) > 0;
}

int _getch(){
    unsigned char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) <= 0) {
        return 0;
    }
    return ch;
}

void readFile(){
    std::fstream inFile("file.txt");

    if (!inFile) {
        std::cerr << "Error, can't find the file" << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    text = buffer.str();
    inFile.close();

}

void display(){
    clearScreen();

    textLength = text.length();
    lines = 0;
    start_index = 0;
    num_chars = 100;
    subtext = text.substr(start_index, num_chars);
    textWithcursor = text;
    absolute_position = cursorX + cursorY*num_chars;

    while (start_index < textWithcursor.length())
    {
        std::string subtext = textWithcursor.substr(start_index, num_chars);
        start_index +=num_chars;
        lines++;
    }
    start_index = 0;
    num_chars = 100;

    if (absolute_position < textWithcursor.length()){
        cursorX = absolute_position % num_chars;
        cursorY = absolute_position / num_chars;
        textWithcursor.insert(absolute_position, ":");
    }
    if (absolute_position >= textWithcursor.length()) {
    // Optional fallback: If the coordinates somehow broke, 
    // put the cursor safely at the absolute end of the file.
        absolute_position = textWithcursor.length(); 
        textWithcursor.insert(absolute_position, ":");
        cursorX = absolute_position % num_chars;
        cursorY = absolute_position / num_chars;
}

    if (cursorX < 0){cursorX++;}
    else if (cursorX > num_chars){cursorX--;}
    else if (cursorY > lines + 1){cursorY--;}
    else if (cursorY < 0){cursorY++;}

    while (start_index < textWithcursor.length()) {
        // substr automatically stops at the end of the string if fewer than n chars remain
        std::string subtext = textWithcursor.substr(start_index, num_chars);
        std::cout << subtext << std::endl;
        // Move the index forward by 30 to grab the next chunk on the next turn
        start_index += num_chars; 
    }
}


void saveFile(){
    std::ofstream outputfile("file.txt");
    if (outputfile.is_open()){
        outputfile << text<<"\n";
        outputfile.close();
    }
}

void cursor(){
    cursorX = cursorY = 0;

    enableRawMode();
    running = true;
    while(running){

        if (_kbhit()){
            int ch = _getch();

            if (ch == 27) {
                if (_kbhit()) {
                    int seq1 = _getch();
                    if (seq1 == '[' && _kbhit()) {
                        int seq2 = _getch();
                        switch (seq2)
                        {
                        case 'A':
                            cursorY--;
                            break;
                        case 'B':
                            cursorY++;
                            break;
                        case 'C':
                            cursorX++;
                            break;
                        case 'D':
                            cursorX--;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            
            else if (ch == 8)
            {
                if (absolute_position > 0){
                    text.erase(absolute_position - 1, 1);
                    cursorX--;
                }
            }
            else if (ch == 13)
            {
                if (absolute_position <= text.length()){
                    text.insert(absolute_position, "\n");
                    cursorX++;
                }
            }
            
            else if (ch>=32 && ch<=126)
            {
                if (absolute_position <= text.length()){
                    text.insert(absolute_position, 1, ch);
                    cursorX++;
                }
            }
            else if (ch == 1){
                running = false; // Exit loop with Ctrl+A
            }

            else if (ch == KEY_CTRL_O)
            {
                saveFile();
            }
            display();
        }
        display();
    }

    disableRawMode();
}

int main(){
    running = true;
    readFile();        
    cursor();
    std::cout<<"\n File Saved Successfully";
}