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
    std::string m_userName;
    std::string m_password;
};

int main()
{
    RealProxy proxy("user", "pwd");
    proxy.Run();
    return 0;
}

/* 桥接模式 */
