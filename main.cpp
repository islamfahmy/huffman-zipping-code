#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std ;
int sz = 0;
struct TreeNode{
  int val ;
   char c;
  TreeNode *left,*right;
  TreeNode() : left(nullptr),right(nullptr){}
  TreeNode(int x) : val(x),left(nullptr),right(nullptr){}
  TreeNode(int x ,char ch) :val(x),c(ch),left(nullptr),right(nullptr){}
  TreeNode(char ch) :c(ch),left(nullptr),right(nullptr){}
};
string out1 ;
bool Compare(TreeNode * a, TreeNode * b)
{
    return a->val>b->val;
}
TreeNode * BuildTree(string name)
{   int sum = 0;
    map< char,int> mp;
    ifstream myfile(name);
    myfile >> std::noskipws;
    if (myfile.is_open())
    {
      while (! myfile.eof() )
          {
            sz ++ ;
             char c;
              myfile >> c;
              bitset<8> x(c);
                mp[c]++;sum++;
          }
        myfile.close();
    }
    else
        cout<<"file not found";

    priority_queue <TreeNode *, vector<TreeNode *>, function<bool(TreeNode *, TreeNode *)> > pq(Compare);
    for(auto itr : mp)
    pq.push(new TreeNode(itr.second,itr.first));

  while(pq.size()!= 1)
  {
    TreeNode * f = pq.top();
    pq.pop();
    TreeNode * s = pq.top();
    pq.pop();
    TreeNode *n = new TreeNode(f->val+s->val);
    n->left = f;
    n->right= s;
    pq.push(n);

  }

  return pq.top();

}
 void save(TreeNode *n, ofstream &destfile)
 {
  if(!n->left&&!n->right)
  {
    destfile<<"1"<<n->c;
    return ;
  }
  destfile<<"0";
  save(n->left,destfile);
  save(n->right,destfile);
 }
void MakeHeader(TreeNode *n, unordered_map< char ,string>& m,string s)
{
  if(!n) return ;
  if(!n->left&&!n->right)
   {
    m[n->c]=s;
    return ;
   }
  MakeHeader(n->left,m,s+"0");
  MakeHeader(n->right,m,s+"1");
}
void compress(string in,string out,unordered_map< char ,string> &h,TreeNode * head)
{
    cout<<"map size = "<<h.size()<<endl;
    ifstream myfile(in);
    myfile >> std::noskipws;
    string s="";
    ofstream destfile;
    int sz = 0 ;
    while (! myfile.eof() )
    {       sz++;
             char c;
            myfile >> c;
            bitset<8> x(c);
            string temp =h[c];
            s+=temp;
    }
    int no = 0;
    out1=s;
    cout<<"binary string size = "<<s.size()<<endl;
    while(s.size()%8!=0)
    {
      no++;
      s.push_back('0');
    }
    stringstream sstream(s);
    string output;
    while(sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output += c;
    }

    destfile.open(out,ios::binary);
    destfile<<no<<"#";
    save(head,destfile);
    for(char &c :output)
      destfile << c;
    cout<<"compression ratio = "<<(sz*8.0)/s.size()<<endl;
    myfile.close();



}
TreeNode * rebuild_tree(ifstream &infile)
{
    char c ; //
    infile>>c;
    if(c=='1')
    {
      infile>>c;
      return new TreeNode(c);
    }
    TreeNode * curr =new TreeNode();
    curr->left = rebuild_tree(infile);
    curr->right = rebuild_tree(infile);
    return curr;
}
void p(TreeNode *n,string s="")
{
  if(!n->left&&!n->right)
  {
    cout<<n->c<<"  "<<s<<endl;
    return ;
  }
  p(n->left,s+"0");
  p(n->right,s+"1");
}
 char decode(TreeNode * h,int &index,string& s,int depth = 0)
{
 if(!h->left&&!h->right)
 {
   return h->c;
 }
  if(index==s.size())
 {
  cout<<"erooooooooooor "<<depth;
  return s[index-1] ;
 }

 if(s[index++]=='0')
   return  decode(h->left,index,s,depth+1);
 return decode(h->right,index,s,depth+1);

}
void writeToFile(string out,string &s,TreeNode *head)
{
    fstream   out_file;
    out_file.open(out, ios::out);
    int index = 0;
    while(index!=s.size())
      out_file.put(decode(head,index,s));

}
void decompress(string in,string out)
{
 map<string, char> h;
 ifstream infile(in,ios::binary);
 infile >> std::noskipws;
 char c ;
 int exes; //beta3et el padding
 infile>>exes;//metsayeva fel 2awel
 infile>>c;//2esmaha ana ba2ra characters
 TreeNode * head =rebuild_tree(infile);//
 string s ="";
 int x = 0 ;
 while(!infile.eof())
 {  x++;
  infile>>c;
  s+=bitset<8>(c).to_string();
 }
 cout<<"size = "<<x<<endl;
 s=s.substr(0,s.size()-16-exes);
 if(out1==s)
  cout<<"no lose"<<endl;
 else cout<<"error happened"<<endl;
 cout<<"binary string size after decompression = "<<s.size()<<endl;
 writeToFile(out,s,head);



}
void compressFile(name)
{
  TreeNode *root =  BuildTree(name);
  unordered_map< char ,string> header ;
  MakeHeader(root,header,"");
  compress(name,"output.bin",header,root);
  decompress("output.bin","output.txt");

}
void helper(string input,ofstream &destfile)
{
    TreeNode *root =  BuildTree(input);
    unordered_map< char ,string> h ;
    MakeHeader(root,h,"");
    destfile<<'#';//delimter
    string name = input.substr(6);
    int name_size = name.size();
    destfile<<name_size;
    destfile<<name;
    string data = "";
    ifstream myfile(input,ios::binary);
    myfile >> std::noskipws;
    int sz = 0 ;
    while (! myfile.eof() )
      {      
         sz++;
         char c;
         myfile >> c;
         string temp =h[c];
         data+=temp;
      }
    cout<<"no of bytes before compressing= "<<sz<<endl;
    int exess = 0 ;
    while(data.size()%8!=0)
      {
        exess++;
        data.push_back('0');
      }
    cout<<"exess = "<<exess<<endl;
    destfile<<data.size()/8;//no of bytes
    destfile<<'#';//delimter
    destfile<<exess;
    destfile<<'#';//delimter
    save(root,destfile);
    destfile<<"#";
    stringstream sstream(data);
    string output;
    int a = data.size() ;
  
    while(sstream.good())
    {    
          bitset<8> bits;
          sstream >> bits;
          char c = char(bits.to_ulong());
             output += c;
    }
    output = output.substr(0,output.size()-1);
    for(char &c :output)
      destfile << c;
     
    cout<<"no of bytes after compression = "<<data.size()/8<<endl;
    cout<<name<<" compressed";
    cout<<"compression ratio = "<<(sz*8.0)/data.size()<<endl<<endl;
  }

  void compressFolder(string input,ofstream &destfile)
  { 
    DIR *pdir = nullptr;
    pdir = opendir(input.c_str());
    struct dirent *pent = nullptr;
    if (pdir == nullptr) 
      cout << "\nERROR! no existing folder";
    while ((pent = readdir(pdir)))
    {   
        string temp = pent->d_name;
        if (pent == nullptr)
          cout << "\nsomething wrong happened";
        if(!strcmp(pent->d_name,".")||!strcmp(pent->d_name,".."))
          continue ;
        if(temp.find(".txt") != string::npos) // folder
        {  
           int type= 0;
           destfile<<'#';
           destfile<<type;
           cout << "compressing file" <<input+"/"+ pent->d_name << endl ;
           helper(input+"/"+ pent->d_name,destfile);   
        }
        else 
        {
          int type= 1;
          destfile<<'#';
          destfile<<type;
          cout << "compressing folder : " <<input+"/"+ pent->d_name << endl ;
          DIR *pdir1 = nullptr;
          pdir1 = opendir((input+"/"+ pent->d_name).c_str());
          struct dirent *pent1 = nullptr;
          int no =0;
          while ((pent1 = readdir(pdir1)))
          {   
            string temp1 = pent1->d_name;
            if (pent1 == nullptr)
             cout << "\nsomething wrong happened";
            if(!strcmp(pent1->d_name,".")||!strcmp(pent1->d_name,".."))
             continue ;
            no++;
          }
          cout<<"no = "<<no<<endl;
          destfile<<'#';
          destfile<<no;
          destfile<<'#';
          string temp =pent->d_name; 
          destfile<<temp.size();
          destfile<<temp;
          compressFolder(input+"/"+ pent->d_name,destfile);

        }
        
    }
  }
  void helper_dec(ifstream &infile,string base )
  {
    char c ;
    infile>>c; 
    cout<<"delimter "<<c<<endl;
    int name_size ;
    infile>>name_size;
    string file_name = "";
    while(name_size--)
    {
      infile>>c; 
      file_name+=c;
    }
    cout<<"decompressing to :"<<base+file_name<<endl;
  int data_size ;
  infile>>data_size; 
  cout<<"size of compressed file = "<<data_size<<endl;
  infile>>c; 
  cout<<"delimiter "<<c<<endl;
  int exess ;
  infile>>exess;
  cout<<"exess = "<<exess<<endl;
  infile>>c; 
  cout<<"delimiter "<<c<<endl;
  ofstream   out_file;
  string temp = base +file_name;
  out_file.open(temp, ios::binary); 
  TreeNode * head =rebuild_tree(infile);
  infile>>c; 
  cout<<"delimiter "<<c<<endl;
  string data ="";
  while(data_size--)
  {
    infile>>c;
    data+=bitset<8>(c).to_string();
  }
  data=data.substr(0,data.size()-exess); 
  int index = 0;
  while(index!=data.size())
  out_file.put(decode(head,index,data));
  cout<<"decompressed "<<file_name<<endl<<endl; 
    
}
void decompressSubdr(ifstream &infile,string base,int no)
{   
    while(no--)
    { 
      char c ;
      infile>>c;
      cout<<"delimter "<<c<<endl;
      int x;
      infile>>x;
      if(infile.eof())
        break;
      cout<<"type : "<<x<<endl;
      if(x==0)
        helper_dec(infile,base);
      else if (x==1)
      {
        
        infile>>c;
        int no ;
        cin>>no ;
        infile>>c;
        int siz = 0 ;
        infile>>sz;
        string name  = "";
        while(sz--)
        {
        infile>>c;
        name+=c;  
        } 
        cout<<"decomprising folder : "<<base+"/"+name<<endl;
        mkdir(name.c_str(),0777);
        decompressSubdr(infile,base+"/"+name,no);
      }
      else 
      {
        cout<<"eroor ";
      } 
    }
 
}
void decompressFolder(ifstream &infile,string base)
{
    while(!infile.eof())
    { 
      char c ;
      infile>>c;
      cout<<"delimter "<<c<<endl;
      int x;
      infile>>x;
      if(infile.eof())
        break;
      cout<<"type : "<<x<<endl;
      if(x==0)
        helper_dec(infile,base);
      else if (x==1)
      {
        infile>>c;
        int no ;
        infile>>no ;
        infile>>c;
        int siz = 0 ;
        infile>>sz;
        string name  = "";
        while(sz--)
        {
        infile>>c;
        name+=c;  
        } 
        cout<<"decomprising folder : "<<base+name<<endl;
        mkdir((base+name).c_str(),0777);
        decompressSubdr(infile,base,no);
      }
      else 
      {
        cout<<"eroor ";
      } 
    }

 
}
int main()
{
  cout<<" enter 1 to compress files 2 for folders\n";
  int choice ;
  cin>>choice ;
  if(choice ==1 )
  {  
    string name ;
    cout<<"enter file name\n";
    cin>>name
    compressFile(name);
    decompress("output.bin","out.txt");
  }
  else {
    ofstream destfile;
    destfile.open("files.bin",ios::binary);
    compressFolder("files",destfile);
    destfile.close();
    ifstream infile("files.bin",ios::binary);
    infile >> std::noskipws;
    decompressFolder(infile,"./outfile/");
    cout<<endl;
  }
}
