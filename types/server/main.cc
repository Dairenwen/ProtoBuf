#include<iostream>
#include<string>
#include"contact.pb.h"
#include<fstream>


void Addusertocontacts(s_contact::PeopleInfo * contacts)
{
    std::string name;
    std::cout<<"your name: ";
    getline(std::cin,name);
    contacts->set_name(name);

    std::string phone;
    
    std::cout<<"your phone: ";
    getline(std::cin,phone);
    s_contact::PeopleInfo_phone* phone_number=contacts->mutable_phone_number();
    phone_number->set_number(phone);

    //添加enum字段
    std::cout<<"your type: ";
    int input; std::cin>>input;
    std::cin.ignore(256,'\n');
    switch(input)
    {
        case 1:phone_number->set_type(s_contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_MP); break;
        case 2:phone_number->set_type(s_contact::PeopleInfo_phone_Type::PeopleInfo_phone_Type_TEL); break;
    }


    //添加oneof字段
    std::string other;
    std::cout<<"qq: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_qq(other);
    }
    other.clear();
    std::cout<<"wechat: ";
    getline(std::cin,other);
    if(!other.empty())
    {
        contacts->set_wechat(other);
    }


    //添加map字段
    std::string remark_label;
    std::string remark_contain;
    while(true)
    {
        remark_label.clear();
        remark_contain.clear();
        std::cout<<"remark label: ";
        getline(std::cin,remark_label);
        if(remark_label.empty()) break;

        std::cout<<"remark contain: ";
        getline(std::cin,remark_contain);
        contacts->mutable_remark()->insert({remark_label,remark_contain});
    }

}

void Writecontacts()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    s_contact::Contacts contacts;

    std::fstream ifs("../contacts.bin",std::ios::binary|std::ios::in);
    if(ifs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.ParseFromIstream(&ifs)==false) std::cerr<<"parse failed"<<std::endl;

    Addusertocontacts(contacts.add_contacts());

    std::fstream ofs("../contacts.bin",std::ios::out|std::ios::binary);
    if(ofs.is_open()==false) std::cerr<<"file open failed"<<std::endl;
    if(contacts.SerializeToOstream(&ofs)==false) std::cerr<<"serialize failed"<<std::endl;

    google::protobuf::ShutdownProtobufLibrary();
}


int main()
{
    Writecontacts();
    return 0;
}