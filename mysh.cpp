#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

class myShCommands
{
private:
    string prompt;

public:

    myShCommands();
    ~myShCommands();	
    
    void echo(vector<string> &n);
    void echoNoCR(vector<string> &n);     //if -n option is used with echo, this function runs
    void PS1(vector<string> &h);          //changes user prompt
    void cat(vector <string> &l);         //displays content of a file
    void cp(vector <string> & cpVec);     //copies from one file to the next, if more than two files copies into directory
    void rm(vector <string> & rmVec);
    bool exit();                          //exits mysh
    void userError(vector <string> &m);   //prints error if user entered something wrong
    void printPrompt();                   //prints the current prompt, initialized by constructor to default prompt "$ "
	
    //test functions
    void printVector(vector<string> &n);
 };

//Test functions
void myShCommands::printVector(vector<string> &n)
{
    cout << "The vector elements are: " << endl;
   
    for (int i =0; i < n.size(); i++) {
        cout << n.at(i) << endl;
    }
}

myShCommands::myShCommands()
{
    prompt = "$ ";
}

myShCommands::~myShCommands()
{}

void myShCommands::echo(vector<string> &n)
{
	for (int i = 1; i < n.size(); i++)
	{
		cout << n.at(i) << ' ';
	}
	cout << "\r"; //assumes and outputs a carriage return at end of output
}

void myShCommands::echoNoCR(vector<string> &n)
//no carriage return as string parsed in main does not read carriage return
{
    //make sure to delete carriage return from n here
	for (int i = 2; i < n.size(); i++) {
		cout << n.at(i) << ' ';
	}
}

void myShCommands::PS1(vector <string> &h)
{
	//check to see if command is space delimited
	if (h.at(0) != "PS1") {
        cout << "PS1 command needs to be space delimited. i.e PS1 = \"your prompt name\"" << endl;
        cout << "No spaces allowed in prompt name. Feature to be implemented soon.";
        return;
        }
        else {
        	string x;
      		x = h.at(2);
      		char nochars[]="\"";
      
      		for (int i = 0; i < strlen(nochars); i++) {
        		x.erase (remove(x.begin(), x.end(), nochars[i]), x.end());
     		}
     	//add a space
     	prompt = x.append(" ");
	}	
}

//cat only works with txt files at the moment
void myShCommands::cat(vector <string> &l) 
{
	ifstream out; //file we're opening to read
	string ins;   //variable to store lines read
	string he;    //variable to store name of file

	for (int i = 1; i < l.size(); i++) 
	{
		he = l.at(i);
		out.open(he.c_str(), ios::in); //c_str() is a 0 terminated string
		
		//check to see if file is open
		if (!out.is_open()) {
			cout << "Error: could not open file.\n";
			return;
		}

		while (!out.eof())
		{
			getline(out, ins);
			cout << ins << endl;
		}
		out.close();
	}
}

void myShCommands::cp(vector <string> & cpVec)
{
    fstream filein;
    ofstream filecp; //file to copy

    string fin;
    string fcp;     //copy arguments to string

    if ((cpVec.size() - 1) >= 3) 
    {  //-1 because we have the cp command in the vector there
       //deal with more than three arguments.
       //last argument must be a directory
       
       //get directory name at the end of the vector
       string dirname;
       dirname = cpVec.back();
       
       //create a directory
       mkdir(dirname.c_str(), 0777);

       //Iterate through the vector and copy each file found into the directory
       //cpVec.size() - 1 is there so we don't include name of directory at the end
       //i = 1 so we don't iterate cp command
       for (int i = 1; i < cpVec.size() - 1; i++) 
       {
           //get name of file at vector position in i
           fin = cpVec.at(i);
           
           //open the file to be copied and check to see if file opened
           filein.open(fin.c_str());

           if (!filein.is_open()) {
               cout << "Could not open file " << fin << endl;
               return;
           } 

           //create a fullpath variable
           string fullpath;
           fullpath = dirname+"/"+fin;
           
           //open the file at the path specified in fullpath
           filecp.open(fullpath.c_str());
           
           //check to see if file opened
           if (!filecp.is_open()) {
               cout << "Could not open directory file " << fullpath << endl;
               return;
           }

           //copy contents of file into the file in the path specified, this case our directory
           while(!filein.eof()) {
                string x;
                getline(filein, x);
                filecp << x << "\n";
           }
           //close files so we can open a new file
           filein.close();
           filecp.close();
        }
        
    //close files just in case
    filein.close();
    filecp.close();
   }
   else {
       //deal with two files
       fin = cpVec.at(1);
       fcp = cpVec.at(2);

       filein.open(fin.c_str());
       filecp.open(fcp.c_str());

       if (!filein.is_open()) {
           cout << "Error: could not open file " << fin << " " << "\n";
           return;
        }
    
       if (!filecp.is_open()) {
           cout << "Error: could not open file " << fcp << " " << "\n";
           return;
        }

       while (!filein.eof()) 
       {
           string x;
           getline(filein, x);         //get lines from first file
           filecp << x << "\n";        //copy into second file name
       }
    }
    //close files
    filein.close();
    filecp.close();
}

void myShCommands::rm(vector <string> & rmVec)
{
    string pathname;

    for (int i = 1; i < rmVec.size(); i++) {
        pathname = rmVec.at(i);
        remove(pathname.c_str());
    }
}

bool myShCommands::exit()
{
    return false;
}

void myShCommands::userError(vector <string> &m)
{
    cout << m.at(0) << " command not found.\n";
}

void myShCommands::printPrompt()
{
    cout << prompt;
}

int main(int argc, char *argv[])
{
    bool run = true;

    string userInput;
    string userCommand;
    string userArgs;
   
    string ps1var;

    vector <string> args;
    
    myShCommands cmd;
   	
    while (run) {
      cmd.printPrompt(); 
      getline(cin, userCommand); //gets the whole line including whitespace, carriage return and newline

      //remove whitespace and input each individual command and arguments into vector seperately
      istringstream a1(userCommand);        //sets the string as a stream for input
      string temp;                          //temp variable to store our keywords without whitespace or newline/carriage return into vector

      while (a1 >> temp) {
        args.push_back(temp);
      }

      //test vector input
      //cmd.printVector(args);

      if (args.at(0) == "echo") {
        
        if (args.at(1) == "-n") {
          cmd.echoNoCR(args);
        }
			else {
        cmd.echo(args);
      }
    }

    else if (args.at(0) == "PS1") {
      cmd.PS1(args);
    }

		else if (args.at(0) == "cat") {
			cmd.cat(args);
		}
        
		else if (args.at(0) == "cp") {
			cmd.cp(args);
		}

		else if (args.at(0) == "rm") {
      cmd.rm(args);
    }
        
		else if (args.at(0) == "exit") {
      run = cmd.exit();
    }
    else {
      cmd.userError(args);    //print user error message 
    }

    //clear the vector for next set of arguments and keep memory free
    args.clear();
    cout << endl; //set a newline for prompt
  } //end while

  return 0;
}
