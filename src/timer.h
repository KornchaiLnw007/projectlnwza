#include <raylib.h>
#include <string>

using namespace std;

class Timer {
public:
    float time;
    bool running;

    Timer() : time(0.0f), running(true) {}

    void Update() {
        if (running) {
            time += GetFrameTime();
        }
    }

    void Reset() {
        time = 0.0f;
        running = true;
    }

    void Stop() {
        running = false;
    }

    void Draw(int x, int y) {
        string timeText = TextFormat("Time: %.2f seconds", time);
        DrawText(timeText.c_str(), x, y, 20, WHITE);
    }
};
