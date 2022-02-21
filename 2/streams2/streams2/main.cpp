#include <windows.h>
#include <iostream>

#define ITERS 10

void reader(HANDLE mutex, HANDLE file) {
    for (int i = 0; i < ITERS; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << "reader" << std::endl;
        ReadFile(file, NULL, 0, NULL, NULL);
        ReleaseMutex(mutex);
    }
}

void writer(HANDLE mutex, HANDLE file) {
    for (int i = 0; i < ITERS; i++) {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << "writer" << std::endl;
        WriteFile(file, NULL, 0, NULL, NULL);
        ReleaseMutex(mutex);
    }
}

int main(int argc, char* argv[]) {
    /*
     * argv[1] - filename
     * argv[2] - process type: reader or writer
     * argv[3] - mutexname
     *
     * если аргументы 2 и 3 не указаны, то тип процесса считается основным
     */
    if (argc == 2) {// main
        HANDLE mutex = CreateMutex(NULL, false, "mutex");

        std::string filename = argv[1];
        std::string command_line_reader = std::string(argv[0]) + " " + argv[1] + " reader mutex";
        std::string command_line_writer = std::string(argv[0]) + " " + argv[1] + " writer mutex";

        auto si = STARTUPINFO();
        PROCESS_INFORMATION pi;

        CreateProcess(NULL, _strdup(command_line_reader.c_str()), NULL, NULL, false, NULL, NULL, NULL, &si, &pi);
        CreateProcess(NULL, _strdup(command_line_writer.c_str()), NULL, NULL, false, NULL, NULL, NULL, &si, &pi);
    }
    else if (argc == 4) {// читатель или писатель
        auto filename = argv[1];
        auto process_type = argv[2];
        auto mutexname = argv[3];

        HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, false, mutexname);
        HANDLE file = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (std::string(process_type) == "reader")
            reader(mutex, file);
        else if (std::string(process_type) == "writer")
            writer(mutex, file);
        else
            std::cout << "wrong process type" << "\'" << process_type << "\'" << std::endl;
    }
    else
        std::cout << "wrong amount of arguments" << std::endl;
}
