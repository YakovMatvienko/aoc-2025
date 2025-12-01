#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

/*
--- Part Two ---
You're sure that's the right password, but the door won't open. You knock, but nobody answers. You build a snowman while you think.

As you're rolling the snowballs for your snowman, you find another security document that must have fallen into the snow:

"Due to newer security protocols, please use password method 0x434C49434B until further notice."

You remember from the training seminar that "method 0x434C49434B" means you're actually supposed to count the number of times any click causes the dial to point at 0, regardless of whether it happens during a rotation or at the end of one.

Following the same rotations as in the above example, the dial points at zero a few extra times during its rotations:

The dial starts by pointing at 50.
The dial is rotated L68 to point at 82; during this rotation, it points at 0 once.
The dial is rotated L30 to point at 52.
The dial is rotated R48 to point at 0.
The dial is rotated L5 to point at 95.
The dial is rotated R60 to point at 55; during this rotation, it points at 0 once.
The dial is rotated L55 to point at 0.
The dial is rotated L1 to point at 99.
The dial is rotated L99 to point at 0.
The dial is rotated R14 to point at 14.
The dial is rotated L82 to point at 32; during this rotation, it points at 0 once.
In this example, the dial points at 0 three times at the end of a rotation, plus three more times during a rotation. So, in this example, the new password would be 6.

Be careful: if the dial were pointing at 50, a single rotation like R1000 would cause the dial to point at 0 ten times before returning back to 50!

Using password method 0x434C49434B, what is the password to open the door?
*/

using namespace std;

struct Rotation {
    int clicks;
    bool left;
};

int ParseInput(const string& path, vector<Rotation>& rotations) {
    ifstream file(path);
    if (!file.good()) {
        return -1;
    }
    string line;
    while (getline(file, line)) {
        bool left;
        if (line[0] == 'R') {
            left = false;
        }
        else if (line[0] == 'L') {
            left = true;
        }
        else {
            return -1;
        }
        const int value = stoi(line.substr(1));
        Rotation r{ value, left };
        rotations.push_back(r);
    }
    return 0;
}

int Dial(const vector<Rotation>& rotations) {
    int dialPoint = 50;
    int count = 0;
    for (const auto & r : rotations) {
        int newDial = (r.left) ? dialPoint - r.clicks : dialPoint + r.clicks;
        if (dialPoint && newDial <= 0) {
            ++count;
        }
        count += (newDial < 0) ? -newDial / 100 : newDial / 100;
        dialPoint = (newDial < 0) ? 100 - (-newDial % 100) : newDial % 100;
        if (dialPoint == 100) {
            dialPoint = 0;
        }
    }

    return count;
}

int main() {
    vector<Rotation> rotations;
    if (ParseInput("input.txt", rotations)) {
        return -1;
    }
    cout << Dial(rotations) << "\n";
    return 0;
}