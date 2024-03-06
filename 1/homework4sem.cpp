#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

void centerAlign(const char* argv1, const char* argv2, ostream& outputFile) {
    ifstream file(argv1);
    int targetLength = stoi(argv2);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> words{istream_iterator<string>{iss},istream_iterator<string>{} };
        if (line.length() > targetLength) {
            auto it = words.begin();
            int cur = 0;
            ostringstream newLine;
            while (it != words.end()) {
                const string& word = *it;
                cur += word.length() + 1;
                if ((cur>= targetLength) || (it == --words.end())) {
                    if (word.length() > targetLength) {//если строка без пробелов 
                        outputFile << word.substr(0, targetLength - 1) << endl;;
                        *it = word.substr(targetLength - 1, word.length());
                        continue;
                    }
                    if (cur<targetLength && it == --words.end()) {
                        newLine << word << ' ';
                        ++it;
                    }
                    else cur -= word.length();
                    int leftSpaces = (targetLength - cur) / 2;
                    int rightSpaces = targetLength - cur - leftSpaces - 1;
                    if (rightSpaces < 0) rightSpaces = 0;
                    if (word.length() == targetLength) newLine << word << ' ';
                    outputFile << string(leftSpaces, ' ') << newLine.str() << string(leftSpaces, ' ') << endl;
                    newLine.str("");
                    cur = 0;
                    continue;
                }
                newLine << word << ' ';
                ++it;
            };
        }
    }
}
int main(int argc, char* argv[]) {
    streambuf* buf;
    ofstream of;
    if (argc > 3 != 0) {//если нет названия файла, то выводим в консоль
        of.open(argv[3]);
        buf = of.rdbuf();
    }
    else buf = std::cout.rdbuf();
    ostream outputFile(buf);
    centerAlign(argv[1], argv[2], outputFile);
}