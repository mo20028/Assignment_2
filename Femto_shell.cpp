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
bool equal_sign(const string &str , string &key ,string &val)
{
    size_t pos=str.find('=');
    if( pos != string::npos)
    {
        key = str.substr(0,pos);
        val = str.substr(pos+1);
    }
    else
    {
        return 0;
    }
    return 1;
}
int main()
{
    string cmd;
    vector<string> V_cmd;
    pid_t pid;
    int status;
    string key ,val;
    vector<pair<string,string>> local_var;

    while (true)
    {
        cout << "yalla: ";
        // read cmd
        getline(cin, cmd);
        // enter command
        if (cmd.empty())
        {
            continue;
        }
        // exit the program
        if (cmd == "exit")
        {
            break;
        }
        // sign a new var to local vars
        if(equal_sign(cmd,key,val))
        {
            local_var.push_back({key,val});
        }

        V_cmd = split_command(cmd);
        
        if(V_cmd[0]=="set")
        {
            for(int i=0;i<local_var.size();i++)
            {
                cout<<"local_var["<<i<<"] : "<<local_var[i].first<<" = "<<local_var[i].second<<endl;
            }
        }
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