#include<iostream>
#include<string>
#include "contacts.pb.h"
#include<fstream>


void Addusertocontacts(contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
    while(true)
    {
        phone.clear();
        std::cout<<"your phone: ";
        getline(std::cin,phone);
        if(phone.empty()) break;

        contact::PeopleInfo_phone * phone_number=contacts->add_phone_numbers();
        phone_number->set_phone_number(phone);
    }

}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    //宏: 验证没有意外链接到与编译的头⽂件不兼容的库版本。如果检测到版本不匹配，程序将中⽌。
    //注意，每个 .pb.cc ⽂件在启动时都会⾃动调⽤此宏。在使⽤ C++ Protocol Buffer 库之前执⾏此宏是⼀种很好的做法，但不是绝对必要的。
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
    //在程序结束时调⽤ ShutdownProtobufLibrary()，删除 Protocol Buffer 库分配的所有全局对象。
    //对于⼤多数程序来说这是不必要的，因为该过程⽆论如何都要退出，并且操作系统将负责回收其所有内存。
    //但是，如果你使⽤了内存泄漏检查程序，该程序需要释放每个最后对象，或者你正在编写可以由单个进程多次加载和卸载的库，
    //那么你可能希望强制使⽤ Protocol Buffers 来清理所有内容。
}

void Readcontacts()
{
    contact::Contacts contacts;

    std::fstream ifs("contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl; 

    for(int i=0;i<contacts.contacts_size();i++)
    {
        const contact::PeopleInfo& people=contacts.contacts(i);
        std::cout<<"name: "<<people.name()<<std::endl;
        for(int i=0;i<people.phone_numbers_size();i++)
        {
            std::cout<<i<<" : "<<people.phone_numbers(i).phone_number()<<std::endl;
        }
        
    }
}
int main()
{
    Writecontacts();
    Readcontacts();
    return 0;
}