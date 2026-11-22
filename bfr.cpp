#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <string>
#include "json.hpp"
const int buffer_size=8192;
class memory{
    struct buffer{
        buffer * left=nullptr;
        buffer * right=nullptr;
        char data[buffer_size];
    };
    struct pointer{
        int index=0;
        buffer * current_buffer=nullptr;
    };
    pointer memory_pointer;
    buffer * left_head=nullptr;    
    public:
        memory(){
            memory_pointer.current_buffer=new buffer;
            left_head=memory_pointer.current_buffer;
            memset(memory_pointer.current_buffer->data,0,sizeof(memory_pointer.current_buffer->data));
        }
        ~memory(){
            while (left_head->right!=nullptr){
                left_head=left_head->right;
                delete left_head->left;
            }
            delete left_head;
        }
        void left(){
            if (memory_pointer.index==0){
                if (memory_pointer.current_buffer->left==nullptr){
                    memory_pointer.current_buffer->left=new buffer;
                    memory_pointer.current_buffer->left->right=memory_pointer.current_buffer;
                    memset(memory_pointer.current_buffer->left->data,0,sizeof(memory_pointer.current_buffer->left->data));
                    left_head=memory_pointer.current_buffer->left;
                }
                memory_pointer.current_buffer=memory_pointer.current_buffer->left;
                memory_pointer.index=buffer_size-1;
            }else{
                memory_pointer.index--;
            }
        }
        void right(){
            if (memory_pointer.index==buffer_size-1){
                if (memory_pointer.current_buffer->right==nullptr){
                    memory_pointer.current_buffer->right=new buffer;
                    memory_pointer.current_buffer->right->left=memory_pointer.current_buffer;
                    memset(memory_pointer.current_buffer->right->data,0,sizeof(memory_pointer.current_buffer->right->data));
                }
                memory_pointer.current_buffer=memory_pointer.current_buffer->right;
                memory_pointer.index=0;
            }else{
                memory_pointer.index++;
            }
        }
        char read(){
            return memory_pointer.current_buffer->data[memory_pointer.index];
        }
        void write(char write_char){
            memory_pointer.current_buffer->data[memory_pointer.index]=write_char;
        }
};
std::string read_bf(const std::string & file_name){
    std::ifstream file(file_name);
    std::string command;
    std::string line;
    while (std::getline(file,line)){
        command+=line;
    }
    return command;
}
class interpreter{
    memory ptr;
    public:
        interpreter(memory memory_ptr){
            ptr=memory_ptr;
        }
        void run(const std::string & command){
            int i=0;
            while(i<command.size()){
                if (command[i]=='<'){
                    ptr.left();
                }else if(command[i]=='>'){
                    ptr.right();
                }else if(command[i]=='+'){
                    ptr.write(ptr.read()+1);
                }else if(command[i]=='-'){
                    ptr.write(ptr.read()-1);
                }else if(command[i]=='.'){
                    std::cout<<ptr.read();
                }else if(command[i]==','){
                    char write;
                    std::cin>>write;
                    ptr.write(write);
                }else if(command[i]=='['){
                    if (ptr.read()=='0'){
                        int depth=1;
                        while (depth>0){
                            
                            i++;
                            if (command[i]=='['){
                                depth++;
                            }else if(command[i]==']'){
                                depth--;
                            }
                        }
                    } 
                }else if(command[i]==']'){
                    if (ptr.read()!='0'){
                        int depth=1;
                        while (depth>0){
                            i--;
                            if (command[i]==']'){
                                depth++;
                            }else if(command[i]=='['){
                                depth--;
                            }

                        }
                    }
                }
            }
        }           
};
int main(int argc,char * argv[]){
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);      
    SetConsoleCP(CP_UTF8);
#endif
    memory ptr;
    interpreter mode(ptr);//todo
    if (argc==2){//执行模式
        mode.run(read_bf(argv[1]));
    }else{
        std::cout<<"please follow the usage,for example:'bfr'or'bfr xxx.bf'\n";
    }
    return 0;
}