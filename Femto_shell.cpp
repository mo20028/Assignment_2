#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

vector<string> split_command(const string &str)
{
    vector<string> words;
    istringstream stream(str);
    string word;

    while (stream >> word)
    {
        words.push_back(word);
    }

    return words;
}
int main()
{
    string cmd;
    vector<string> V_cmd;
    pid_t pid;
    int status;

    while (true)
    {
        cout << "yalla: ";
        // read cmd
        getline(cin, cmd);
        if (cmd.empty())
        {
            continue;
        }

        if (cmd == "exit")
        {
            break;
        }

        V_cmd = split_command(cmd);
        // Convert the vector of strings to a vector of char*
        std::vector<char *> Args;
        for (const auto &arg : V_cmd)
        {
            Args.push_back(const_cast<char *>(arg.c_str()));
        }
        Args.push_back(nullptr); // exec functions expect a null-terminated array

        pid = fork();

        if (pid < 0)
        {
            cerr << "fail to fork" << endl;
            return -1;
        }
        else if (pid == 0)
        {
            execvp(Args[0], Args.data());
        }
        else
        {
            while (wait(&status) != pid)
                ;
        }
    }

    return 0;
}