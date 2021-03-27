#include <iostream>
#include <queue>
#include <fstream>
#include <map>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <bits/stdc++.h>
#include<cmath>
#include <sstream>
#include <bitset>
using namespace std;


typedef struct       //Struct Value dataType for multi-Value Map
{
    int freq;
    string code;
} value;

map<char, value> charactersTable;
map<char, value>::iterator itr;

map<string, string> charactersTable_code;
map<string, string>::iterator itrCode;

bool writeToFile = false, readCompFile = false, read_Folder = false, decodeFolder = false, bin= false;
double srcSize = 0, desSize = 0;
string fileName, tmp = "files", tmp2 = "files", tmp3 = "files" ;
string strArry[100];
int countF = 0;
ofstream decodedFile;



struct MinHeapNode            // A Huffman tree node
{
    char data;                // One of the input characters
    unsigned freq;             // Frequency of the character
    MinHeapNode *left, *right; // Left and right child

    MinHeapNode(char data, unsigned freq, MinHeapNode *left, MinHeapNode *right)
    {
        //left = right = NULL;
        this->data = data;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }
};
typedef struct
{
    string code ;
    string data ;
} node;

struct compare
{
    bool operator()(MinHeapNode *l, MinHeapNode *r)
    {
        return (l->freq > r->freq);
    }
};

void printCodes(struct MinHeapNode *root, string str)   // print codes by traversing the built Huffman Tree
{
    if (!root)
        return;

    if (root->data != '\0')
    {
        int val = int(root->data);
        string bin = "";
        while (val > 0)
        {
            (val % 2)? bin.push_back('1') :
            bin.push_back('0');
            val /= 2;
        }
        reverse(bin.begin(), bin.end());

        cout <<"\t"<<"WORD"<<"\t"<< "BYTE"<<"\t"<< "CODE"<<"\t"<<"\t"<< "NEW CODE"<<endl;
        cout <<"\t" << root->data << "\t" << int(root->data) << ": " <<"\t"<<bin<<"\t"<<"\t"<< str << "\n";
        itr = charactersTable.find(root->data);
        itr->second.code = str;
        char tmp = root->data;
        desSize = desSize + str.length();
    }
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}


priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> minHeap;

void HuffmanCodes(char *data, int *freq, int size)   // The main function that builds a Huffman Tree and
{
    struct MinHeapNode *left, *right, *top;

    // Create a min heap & inserts all characters of data[]
    for (int i = 0; i < size; ++i)
    {
        //left=right=NULL;
        minHeap.push(new MinHeapNode(data[i], freq[i],NULL,NULL));
    }
    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1)
    {
        // Extract the two minimum freq items from min heap
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        top = new MinHeapNode('\0', left->freq + right->freq,left,right);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    // Print Huffman codes using the Huffman tree built above
    printCodes(minHeap.top(), "");
}

void mapping(char c)//to create the character Table
{
    map<char, value>::iterator iterate = charactersTable.find(c);
    value tmp;
    tmp.freq = 1;
    tmp.code = "0";

    if (iterate == charactersTable.end()) //character not Found
    {
        charactersTable.insert(pair<char, value>(c, tmp));

    }
    else
    {
        iterate->second.freq = iterate->second.freq + 1;
    }
}
//***********************FILE FUNCTIONS **************************************8
void writeFile(string st)
{
    map<char, value>::iterator itrr;
    ofstream destfile;
    string bit_buffer ;
    string str;
    int bit, coun = 0;

    if (bin)
    {
        destfile.open (fileName,ios::binary);
    }
    else
    {
        destfile.open (fileName,ios::binary);
    }

    string sign = "#";  // using in Folder compression only (as files flag)

    if (destfile.is_open())
    {
        if (readCompFile)
        {
            for (char &c : st)
            {
                destfile << c;
            }

            if (read_Folder)
            {
                destfile << sign;
            }
            destfile.close();
        }
        else
        {
          //  cout<<"the st length"<<st<<endl;
            char c;
            int i;
            for ( i=0 ; i< st.length(); i++)
            {
                c=st[i];
                itrr = charactersTable.find(c);
                str += itrr->second.code;
                desSize = desSize + (itrr->second.code).length();

            }

            /*  std::stringstream sstream(str);
              string output="";
              std::bitset<8> bits;
              while(sstream.good())
              {
                  sstream >> bits;
                  output += char(bits.to_ulong());
              }*/
            destfile<<i<<endl;

            destfile << str << "!" ;
            char f;
            string s;
            map<char, value>::iterator itrrr;


            for(auto pair :charactersTable)
            {
                f = pair.first;
                itrrr = charactersTable.find(f);
                s=itrrr->second.code ;
                destfile << f << "\t" << s << "\t";
                cout <<f<<" , "<<s<<"$  ";
            }
        }

        destfile.close();
    }

    else cout << "Unable to open file";

}

string decode_file(map <string,string> c, string s)
{

    string ans ;
    countF = 0 ;
    std::stringstream sstream(s);
    string output="";
    std::bitset<1> bits;
    while(sstream.good())
    {
        sstream >> bits;
        output += bits.to_string();
        for (auto itr = c.begin(); itr != c.end(); itr++)
        {


            if (itr->second == output)

            {
                //itr =c.find(output);
                ans += itr->first;
                // cout<<"this is ans:  "<<itr->first<<endl;
                output="";
            }
        }
    }
    fileName="cmp.txt";
    // readCompFile=true;
    writeFile(ans);
    return ans;
    //return ans + '\0';
}

//Read source file
void readFile(string srcFile)
{

    string encodedText;
    std::string tmp;
    char ch;
    string bin = "";
    string code ="";

    fstream fin(srcFile, fstream::in);
    if (writeToFile)
    {
        while (fin  >> noskipws >>ch )
        {
            tmp += ch;

        }
        writeFile(tmp);

        writeToFile = false;
        fin.close();
        return;
    }
    else if (readCompFile)
    {
        int x;
        fin>>x;
        cout<<"this is the length of x: "<<x<<endl;
        std::getline( fin, tmp, '!' );
        cout<<"this is the length of tmp: "<<tmp.length()<<endl;
        //encodedText += tmp;
        // cout<<tmp<<"this is temp"<<endl;
        /* int val= int(ch);
         while (val > 0)
         {
             (val % 2)? bin.push_back('1') :bin.push_back('0');
             val /= 2;
         }
         while(bin.size() < 8 )
         {

             bin.push_back('0');
         }
         reverse(bin.begin(), bin.end());
         code += bin;
         bin.clear();*/


        string first;
        string second;
        string str;
        while(std::getline( fin, first, '\t' ) && std::getline( fin, second,'\t' ))
        {

            charactersTable_code.insert(pair<string, string>(first, second));
            cout <<"this is first: "<<first<<"this is second: "<<second<<endl;
        }

        string f,s;
        for(auto pair :charactersTable_code)
        {
            f = pair.first;
            //itrCode = charactersTable_code.find(f);
            s=pair.second ;
            cout <<"  this is first: "<<f<<"  this is second: "<<s<<endl;
        }
        fin.close();
        decode_file(charactersTable_code, tmp);
        return;

    }
    else
    {
        while (fin >> noskipws >> ch )
        {
            srcSize ++ ;

            mapping(ch); // update exciting character's frequency value OR add a new character
        }
        fin.close();

    }
}

//**********************FOLDER FUNCTIONS***************************


string decode_file_from_folder(struct MinHeapNode *root, string s)
{
    string ans = "";
    countF = 0 ;
    struct MinHeapNode *curr = root;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        // reached leaf node
        if (curr->left == NULL and curr->right == NULL)
        {
            if (curr->data == '#')
            {

                fileName = strArry[countF];

                writeFile(ans);
                countF++;
                ans = "";
            }
            else
            {
                ans += curr->data;
            }

            curr = root;
        }
    }
    cout << ans << endl;
    fileName = "decoded.txt";
    writeFile(ans);
    return ans + '\0';
}
void readFile_from_folder(string srcFile)
{

    string encodedText, tmp;
    char ch;

    fstream fin(srcFile, fstream::in);
    //ifstream fin;
    // fin.open (srcFile, ios::in | ios::app | ios::binary);

    if (writeToFile)
    {
        while (fin >> noskipws >> ch)
        {
            tmp += ch;
        }
        cout << tmp;
        writeFile(tmp);
        writeToFile = false;
        fin.close();
        return;
    }
    else if (readCompFile)
    {
        while (fin >> noskipws >> ch)
        {
            encodedText += ch;

        }
        fin.close();
        decode_file_from_folder(minHeap.top(), encodedText);
        return;
    }
    else
    {
        while (fin >> noskipws >> ch)
        {
            srcSize ++ ;
            mapping(ch); // update exciting character's frequency value OR add a new character
        }
        fin.close();

    }
}

void decompressFolder(string *files)
{
    readFile_from_folder("src.txt");

}

string *readFolder()
{

    DIR *pdir = nullptr;
    pdir = opendir(tmp.c_str());   // "." will refer to the current directory
    struct dirent *pent = nullptr;
    string preTmp = tmp ;
    cout << "huffman tree of all files :" <<endl;

    if (pdir == nullptr) // if pdir wasn't initialised correctly
    {
        cout << "\nERROR! pdir could not be initialised correctly";
        exit(3);
    }

    while ((pent = readdir(pdir))) // while there is still something in the directory to list
    {
        if (pent == nullptr) // if pent has not been initialised correctly
        {
            // print an error message, and exit the program
            cout << "\nERROR! pent could not be initialised correctly";
            exit(3);
        }


        tmp = pent->d_name;
        cout << "TMMMMP" << tmp << endl ;
        if (strcmp(pent->d_name, ".") != 0)
        {
            if (strcmp(pent->d_name, "..") != 0)
            {
                if (tmp.find(".txt") != string::npos)
                {
                    //pos : The initial position from where the string search is to begin.
                    strArry[countF] = preTmp +"/"+ tmp ;
                    cout << "StrArry = " << strArry[countF] << endl;
                    cout << "count of files found : " << countF << endl;

                    tmp2 = tmp3+ "/"+ tmp;
                    readCompFile = true;
                    read_Folder = true;
                    writeToFile = true;
                    fileName = "src.txt";
                    readFile_from_folder(preTmp +"/"+ tmp);

                    countF++;
                }

                else if (tmp.find("DS") == string::npos)
                {

                    tmp =  preTmp+"/"+tmp   ;
                    cout << "tmp = " + tmp ;

                    readFolder() ;


                }
                cout << endl << "huffman tree of all files :" <<endl;
            }
        }

    }

    //close the directory
    closedir(pdir);

    return EXIT_SUCCESS;
}




void compressionRatio()
{
    cout << "Source file size = " << srcSize*8 << " bits" << endl
         << "Destination file size = " << desSize << " bits" << endl
         << "Compression ratio = % " << ((desSize) / (srcSize*8)) * 100 << endl;
}


int main()
{

    cout << "Press >>" << endl
         << " '0' : File Compression" << endl
         <<" '1' : File Decompression"    << endl
         << " '2' : Folder Compression and decompression" << endl
         << "> " ;
    int option ;
    while(cin >> option && option != -1)
    {
        while (option != 0 && option!= 1 && option != 2 )
        {
            cout << "Enter Valid value"<< endl
                 << "> " ;
            cin >> option ;
        }
        /**********/


        string *files = nullptr;
        string fName ;
        if (option == 2)
        {

            files = readFolder();
            readCompFile = false;
            read_Folder = false;
            writeToFile = false;
            readFile_from_folder("src.txt");
        }
        else if(option == 0 || option == 1)
        {
            cout << "Enter File name to be (de)compressed)" << endl ;
            cin >> fName ;
            readFile(fName);
        }


        char *chArr = (char *) malloc(charactersTable.size() * sizeof(char));   //Allocate for characters
        int *freArr = (int *) malloc(charactersTable.size() * sizeof(int));    //Allocate for characters's frequency

        int size = charactersTable.size() / sizeof(char);
        int count = 0;

        /***** Copy Characters Map ******/
        for (itr = charactersTable.begin(); itr != charactersTable.end(); ++itr)
        {
            chArr[count] = itr->first;
            freArr[count] = itr->second.freq;


            // srcSize = srcSize + (8 * freArr[count]);
            cout << '\t' << chArr[count]
                 << '\t' << freArr[count] << '\n';

            // if(chArr[count] == '\n')
            //      break;
            count++;
        }
        /************/

        HuffmanCodes(chArr, freArr, size);   /*Build Huffman Code*/


        if(option ==0)
        {

            /*Compress File(s) */
            writeToFile = true;
            bin = true ;
            fileName = "cmp.txt";
            readFile(fName);
            cout <<  "" << endl ;
            compressionRatio();
            bin = false ;
        }


        else if(option ==1)
        {
            /* Decompress File(s) */
            readCompFile = true;
            fileName = "cmp.txt";
            readFile("cmp.txt");
        }

        if (option == 2)
        {
            decompressFolder(files);  //Decompress the Folder
        }


        free(chArr);
        free(freArr);


        return 0;
    }
}
