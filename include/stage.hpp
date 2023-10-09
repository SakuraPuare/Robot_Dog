
#include "dog.h"
#include "util/color.hpp"

#include <chrono>
#include <string>

#include <QThread>

using namespace std;


class TimerThread : public QThread {
public:
    int task = TASK_STOP;
    int next_color = blue;
    int stage = 0;
    int laps = 1;

    chrono::time_point<chrono::steady_clock> start_time;

    void start_thread() {
        start_time = chrono::steady_clock::now();
        QThread::start();
    }

    void stop_thread() {
        requestInterruption();
        QThread::wait();
    }

    void run() override {
        // while thread is running
        while (!isInterruptionRequested()) {
            switch (task) {
                case TASK_LIMIT: {
                    QThread::sleep(1);
                    stage = 2;
                    QThread::sleep(6);
                    stage = 3;
                    QThread::sleep(1);
                    task = TASK_TRACK;
                    stage = 0;
                    break;
                }
                case TASK_RESIDENT: {
                    QThread::sleep(2);
                    stage = 2;
                    QThread::sleep(4);
                    stage = 3;
                    QThread::sleep(4);
                    stage = 4;
                    QThread::sleep(4);
                    stage = 5;
                    QThread::sleep(8);
                    stage = 6;
                    QThread::sleep(12);
                    stage = 7;
                    QThread::sleep(7);
                    task = TASK_TRACK;
                    stage = 0;
                    break;
                }
                case TASK_CROSS: {
                    QThread::sleep(1);
                    stage = 2;
                    QThread::sleep(1);
                    task = TASK_TRACK;
                    stage = 0;
                    break;
                }
                case TASK_UPSTAIR: {
                    QThread::sleep(7);
                    stage = 2;
                    QThread::sleep(1);
                    task = TASK_TRACK;
                    stage = 0;
                    break;
                }
            }
            QThread::msleep(50);
        }

        cout << "Running Time: "
             << chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count()
             << "s" << endl;
    };

    string DebugString() const {
        string str = "\nTimer:\n";
        str += "\ttask: " + std::to_string(task) + "\n";
        str += "\tnext_color: " + std::to_string(next_color) + "\n";
        str += "\tstage: " + std::to_string(stage) + "\n";
        str += "\tlaps: " + std::to_string(laps);
        return str;
    }
};