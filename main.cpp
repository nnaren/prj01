#include <iostream>
#include <hiredis/hiredis.h>
using namespace std;

int main()
{
    cout << "Learn redisConnect" << endl;
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if(c == NULL || c->err)
    {
        if(c)
        {
            cout << "Error: " << c->errstr << endl;
        } else 
        {
            cout << "Can't allocate redis context" << endl;
        }
    }

    /* 学习 redisCommand函数
     * 函数原型
     * void *redisCommand(redisContext *c, const char *format, ...);
     * 
     */
    cout << "Learn redisCommand" << endl;
    redisReply* reply;
    // SET msg 'hello world'
    string key = "msg";
    string value = "hello world";
    reply =(redisReply*) redisCommand(c, "SET %s %s", key.c_str(), value.c_str());
    cout << reply->str << endl;

    // GET msg2
    string key2 = "msg2";
    reply = (redisReply*)  redisCommand(c, "GET  %s", key2.c_str());
    if(reply->type == REDIS_REPLY_NIL)
        cout << "not find" << endl;
    else
        cout << reply->str << endl;

    // GET msg1
    reply = (redisReply*)  redisCommand(c, "GET  %s", key.c_str());
    if(reply->type == REDIS_REPLY_NIL)
        cout << "not find" << endl;
    else
        cout << reply->str << endl;

    // HSET website google "www.g.cn"  
    string hash3 = "myhash";
    string key3 = "google";
    string value3 = "www.google.cn";
    reply = (redisReply*)  redisCommand(c, "HSET  %s %s %s ", hash3.c_str(), key3.c_str(), value3.c_str());
    
    // GET website google "www.g.cn"
    reply = (redisReply*)  redisCommand(c, "HGET  %s %s ", hash3.c_str(), key3.c_str());
    cout << reply->str << endl;

    /* 
     * 学习redisCommandArgv函数
     * 函数原型
     * void *redisCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);
     * 
     */
    //SET msg3 "yuan"
    const char set5[] = "SET";
    const char key5[] = "msg3";
    const char value5[] = "yuan";
    int argc = 3;
    // 因为"[]"的优先级要比"*"要高
    // *a[] 数组的指针
    // (*a)[] 指针的数组
    // **a  指针的指针
    const char *argv[] = {set5, key5, value5}; 
    reply = (redisReply*)redisCommandArgv(c, argc, argv, nullptr);

    //HSET myhash tenx "www.tx.com"
    char hset4[] = "hset";
    char key4[] = "myhash";
    char hkey4[] = "tenx";
    char hvalue4[] = "www.tx.com";
    int  argc2 = 4;
    const char *argv2[] = {hset4, key4, hkey4, hvalue4};
    size_t argvlen2[] = {4, 6, 4, 10};
    reply = (redisReply*) redisCommandArgv(c, argc2 , argv2, argvlen2);

    /* 
     * 基于管线的调用方式，即pipleline模式
     * void redisAppendCommand(redisContext *c, const char *format, ...);
     * 
     * redisGetReply 直到才发生给redis服务器
     */
    redisAppendCommand(c, "SET com ZTE");
    redisAppendCommand(c, "GET com ");
    redisGetReply(c, (void **)&reply);
    cout << reply->type << endl;
    cout << reply->str << endl;
    redisGetReply(c, (void **)&reply);
    if(reply->type == REDIS_REPLY_NIL)
        cout << "not find" << endl;
    else
        cout << reply->str << endl;
    /* 
     * 学习释放资源
     * 函数原型
     * void freeReplyObject(void *reply);
     * 释放redisCommand执行后返回的的redisReply所占用的内存。
     * void redisFree(redisContext *c);
     * 释放redisConnect()所产生的连接。
     */
    freeReplyObject(reply);
    redisFree(c);
    return 0;
}
