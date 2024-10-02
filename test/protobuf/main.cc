#include <iostream>
#include "test.pb.h"
#include <string>
using namespace fixbug;

int main()
{
    // LoginResponse res;
    // ResultCode *rc = res.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("error");

    GetFriendListResponse friendRes;

    ResultCode *rc =  friendRes.mutable_result();
    rc->set_errcode(0);
    
    User * friend1 = friendRes.add_friend_list();
    friend1->set_name("zhang san");
    friend1->set_age(16);
    friend1->set_sex(User::MAN);

    User * friend2 = friendRes.add_friend_list();
    friend2->set_name("li si");
    friend2->set_age(20);
    friend2->set_sex(User::WOMAN);

    std::cout << friendRes.friend_list_size() << std::endl;

    std::string str;

    if(friendRes.SerializeToString(&str))
    {
        std::cout << "Serialize OK" << std::endl;
    }

    GetFriendListResponse friendResB;
    if(friendResB.ParseFromString(str))
    {
        std::cout << "reSerialize OK" << std::endl;
        if(friendResB.result().errcode() == 1)
        {
            std::cout << friendResB.result().errmsg() << std::endl; 
        }
        else if(friendResB.result().errcode() == 0)
        {
            int size =  friendResB.friend_list_size();
            for(int i = 0;i<size;i++)
            {
                std::cout << "name:" << friendResB.friend_list(i).name() << std::endl;
                std::cout << "age:" << friendResB.friend_list(i).age() << std::endl;
                std::cout << "sex:" << friendResB.friend_list(i).sex() << std::endl;
                std::cout << "----------------------"<< std::endl;;
            }
            
        }
        
    }


    return 0;
}


int main1()
{
    //數據的封裝
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    //數據系列化
    std::string str;
    if(req.SerializeToString(&str))
    {
        std::cout << str.c_str() << std::endl;
    }

    //數據的反序列化
    LoginRequest reqA;
    if(reqA.ParseFromString(str))
    {
        std::cout << reqA.name() << std::endl;
        std::cout << reqA.pwd() << std::endl;
    }

    return 0;
}