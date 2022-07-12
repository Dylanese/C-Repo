/* 代理模式 */
class ISubject
{
public:
    virtual void Run() = 0;
};

class RealSubject : public ISubject
{
public:
    virtual void Run() {
        cout << "system run" << endl;
    }
};

class RealProxy :public ISubject
{
public:
    RealProxy(string userName, string password) : 
        userName(userName), 
        password(password),
        real(new RealSubject()) {
    }
    virtual void Run() {
        cout << "proxy run" << endl;
        if (checkUserNameAndPassword()) {
            m_real->Run();
        }
        else {
            cout << "system check error" << endl;
        }
    }
    ~RealProxy() {
        delete m_real;
    }
private:
    bool checkUserNameAndPassword() {
        // ......
    }
public:
    RealSubject* m_real;
    string m_userName;
    string m_password;
};

int main()
{
    RealProxy proxy("user", "pwd");
    proxy.Run();
    return 0;
}

/*********** 桥接模式1 ***********/
class Messager{
protected:
     MessagerImpl *msgerImp; //桥接，组合代替继承
public:
    Messager(MessagerImpl *imp) : msgerImp(imp) {}
    virtual void Login(string username, string password)=0;
    virtual void SendMessage(string message)=0;
    virtual void SendPicture(Image image)=0;
    virtual ~Messager(){}
};
// 不同的变化方向(业务和平台)，所以分为两个类
class MessagerImpl{
public:
    virtual void PlaySound()=0;
    virtual void DrawShape()=0;
    virtual void WriteText()=0;
    virtual void Connect()=0;
    MessagerImpl(){}
};
class PCMessagerImpl : public MessagerImpl{
public:
    virtual void PlaySound(){
    }
    virtual void DrawShape(){
    }
    virtual void WriteText(){
    }
    virtual void Connect(){
    }
};
class MobileMessagerImpl : public MessagerImpl{
public:
    virtual void PlaySound(){
    }
    virtual void DrawShape(){
    }
    virtual void WriteText(){
    }
    virtual void Connect(){
    }
};
class MessagerLite :public Messager {
public:
    virtual void Login(string username, string password){ 
        msgerImp->Connect();
    }
    virtual void SendMessage(string message){
        msgerImp->WriteText();
    }
    virtual void SendPicture(Image image){
        msgerImp->DrawShape();
    }
};
class MessagerPerfect  :public Messager {
public:
    virtual void Login(string username, string password){
        msgerImp->PlaySound();
        msgerImp->Connect();
    }
    virtual void SendMessage(string message){
        msgerImp->PlaySound();
        msgerImp->WriteText();
    }
    virtual void SendPicture(Image image){
        msgerImp->PlaySound();
        msgerImp->DrawShape();
    }
};
void Process(){
    //运行时装配
    MessagerImpl* mImp = new PCMessagerImp();
    Messager *msger = new MessagerPerfect(mImp);
}


/*********** 桥接模式2 ***********/
#ifndef ABSTRACTION_H_
#define ABSTRACTION_H_

#include <string>
#include "Implementation.h"

// 抽象类: Pen
class Pen {
 public:
    virtual void draw(std::string name) = 0;
    void set_color(Color* color) {
        color_ = color;
    }

 protected:
    Color* color_;
};

#endif  // ABSTRACTION_H_

#ifndef REFINED_ABSTRACTION_H_
#define REFINED_ABSTRACTION_H_

#include <string>
#include "Abstraction.h"

// 精确抽象类: BigPen
class BigPen : public Pen {
 public:
    void draw(std::string name) {
        std::string pen_type = "大号钢笔绘制";
        color_->bepaint(pen_type, name);
    }
};

// 精确抽象类: SmallPencil
class SmallPencil : public Pen {
 public:
    void draw(std::string name) {
        std::string pen_type = "小号铅笔绘制";
        color_->bepaint(pen_type, name);
    }
};

#endif  // REFINED_ABSTRACTION_H_

#ifndef IMPLEMENTATION_H_
#define IMPLEMENTATION_H_

#include <string>
#include <iostream>

// 实现类接口: 颜色
class Color {
 public:
    virtual void bepaint(std::string pen_type, std::string name) = 0;
};

#endif  // IMPLEMENTATION_H_


#ifndef CONCRETE_IMPLEMENTATION_H_
#define CONCRETE_IMPLEMENTATION_H_

#include <string>
#include "Implementation.h"

// 具体实现类: Red
class Red : public Color {
 public:
    void bepaint(std::string pen_type, std::string name) override {
        std::cout << pen_type << "红色的" << name << "." << std::endl;
    }
};

// 具体实现类: Green
class Green : public Color {
 public:
    void bepaint(std::string pen_type, std::string name) override {
        std::cout << pen_type << "绿色的" << name << "." << std::endl;
    }
};


#endif  // CONCRETE_IMPLEMENTATION_H_

#include "ConcreteImplementation.h"
#include "RefinedAbstraction.h"

int main() {
    // 客户端根据运行时参数获取对应的Color和Pen
    Color* color = new Red();
    Pen* pen = new SmallPencil();

    pen->set_color(color);
    pen->draw("太阳");

    delete color;
    delete pen;
}

/*********** 装饰器模式 ***********/

// 组件: 是具体组件和装饰类的共同基类, 在C++中实现成抽象基类
class Stream{
public:
    virtual char Read(int number)=0;
    virtual void Seek(int position)=0;
    virtual void Write(char data)=0;
    virtual ~Stream(){}
};

// 具体组件提供操作的默认实现, 这些类在程序中可能会有几个变体
class FileStream: public Stream{
public:
    virtual char Read(int number){...}   //读文件流
    virtual void Seek(int position){...} //定位文件流
    virtual void Write(char data){...}   //写文件流
};
class NetworkStream :public Stream{
public:
    virtual char Read(int number){...}   //读网络流  
    virtual void Seek(int position){...} //定位网络流  
    virtual void Write(char data){...}   //写网络流
};

// 装饰基类和其他组件遵循相同的接口。
// 该类的主要任务是定义所有具体装饰的封装接口。
// 封装的默认实现代码中可能会包含一个保存被封装组件的成员变量，并且负责对其进行初始化。
class DecoratorStream: public Stream{
protected:
    DecoratorStream(Stream *stm) : stream(stm){}
    Stream *stream;
};

// 加密装饰器
class CryptoStream: public DecoratorStream {
public:
    CryptoStream(Stream *stm) : DecoratorStream(stm){}
    virtual char Read(int number){
        //额外的加密操作...
        stream->Read(number);
    }
    virtual void Seek(int position){
        //额外的加密操作...
        stream->Seek(position);
    }
    virtual void Write(char data){
        //额外的加密操作...
        stream->Write(data);
    }
};
// 缓存装饰器
class BufferedStream : public DecoratorStream{
public:
    BufferedStream(Stream *stm) : DecoratorStream(stm){}
};

void Process(){
    //运行时装配
    FileStream* s1=new FileStream();
    CryptoStream* s2=new CryptoStream(s1);
    BufferedStream* s3=new BufferedStream(s1);
}



/*********** 适配器模式 ***********/

//遗留接口（老接口）
class IAdaptee{
public:
    virtual void foo(int data)=0;
    virtual int bar()=0;
};
//遗留类型
class OldClass: public IAdaptee{
    //....
};


//目标接口（新接口）
class ITarget{
public:
    virtual void process()=0;
};
//对象适配器
class Adapter: public ITarget{  /* 继承 */
protected:
    IAdaptee* pAdaptee;         /* 组合 */
public:
    Adapter(IAdaptee* pAt) : pAdaptee(pAt) { }
    virtual void process(){
        int data=pAdaptee->bar();
        pAdaptee->foo(data);
    }
};

//类适配器
class Adapter: public ITarget, protected OldClass{ //多继承      
}

int main(){
    IAdaptee* pAdaptee=new OldClass();
    ITarget* pTarget=new Adapter(pAdaptee);
    pTarget->process();
    return 0;
}

template<typename T>
class stack{
    deque<T> container;
    
};
template<typename T>
class queue{
    deque<T> container;
};



