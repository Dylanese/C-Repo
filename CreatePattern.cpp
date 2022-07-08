#include <iostream>
#include <pthread.h>
#include <string>

/* 单例模式 */
class single
{
public:
    static single *getSinle();

private:
    single() {}
    ~single() {}
    static single *p;
    static pthread_mutex_t lock;
};
single *single::p = NULL;
pthread_mutex_t single::lock = PTHREAD_MUTEX_INITIALIZER;
single *single::getSinle()
{
    if (p = NULL)
    {
        pthread_mutex_lock(&lock);
        if (p == NULL)
        {
            p = new single();
        }
        pthread_mutex_unlock(&lock);
    }
}

/* 简单工厂 */
class RuleConfigSource
{
public:
    RuleConfig load(string ruleConfigFilePath)
    {
        string ruleConfigFileExtension = getFileExtension(ruleConfigFilePath);
        IRuleConfigParser parser = RuleConfigParserFactory::createParser(ruleConfigFileExtension);
        string configText = "";
        RuleConfig ruleConfig = parser.parse(configText); //从ruleConfigFilePath文件中读取配置文本到configText中
        return ruleConfig;
    }

private:
    string getFileExtension(string filePath)
    {
        return "json"; //...解析文件名获取扩展名，比如rule.json，返回json
    }
}

class RuleConfigParserFactory // 第一种简单工厂
{
public:
    static IRuleConfigParser createParser(string configFormat)
    {
        IRuleConfigParser parser = null;
        if (configFormat == "json")
        {
            parser = new JsonRuleConfigParser();
        }
        else if (configFormat == "xml")
        {
            parser = new XmlRuleConfigParser();
        }
        else if (configFormat == "yaml")
        {
            parser = new YamlRuleConfigParser();
        }
        else if (configFormat == "properties")
        {
            parser = new PropertiesRuleConfigParser();
        }
        return parser;
    }
}

class RuleConfigParserFactory // 第二种简单工厂
{
public:
    RuleConfigParserFactory()
    {
        cachedParsers["json"] = new JsonRuleConfigParser();
        cachedParsers["xml"] = new XmlRuleConfigParser();
        cachedParsers["yaml"] = new YamlRuleConfigParser();
        cachedParsers["properties"] = new PropertiesRuleConfigParser();
    }
    IRuleConfigParser createParser(string configFormat)
    {
        IRuleConfigParser parser = cachedParsers.find(configFormat);
        return parser;
    }

private:
    unordered_map<string, IRuleConfigParser> cachedParsers;
}

/* 工厂方法 */
class RuleConfigSource
{
public:
    RuleConfig load(string filePath)
    {
        string fileExtension = getFileExtension(filePath);
        RuleConfigParserFactoryMap parserFactoryMap;
        RuleConfigParserFactory parserFactory = parserFactoryMap.getParserFactory(fileExtension);

        IRuleConfigParser parser = parserFactory.createParser();
        string configText = "";
        RuleConfig ruleConfig = parser.parse(configText); //从filePath文件中读取配置文本到configText中
        return ruleConfig;
    }

private:
    string getFileExtension(string filePath)
    {
        return "json"; //...解析文件名获取扩展名，比如rule.json，返回json
    }
}

class RuleConfigParserFactoryMap
{ //工厂的工厂
public:
    RuleConfigParserFactoryMap()
    {
        cachedFactories["json"] = new JsonRuleConfigParserFactory();
        cachedFactories["xml"] = new XmlRuleConfigParserFactory();
        cachedFactories["yaml"] = new YamlRuleConfigParserFactory();
        cachedFactories["properties"] = new PropertiesRuleConfigParserFactory();
    }
    RuleConfigParserFactory getParserFactory(string type)
    {
        RuleConfigParserFactory parserFactory = cachedFactories.find(type);
        return parserFactory;
    }

private:
    unordered_map<string, RuleConfigParserFactory> cachedFactories;
}

// 利用多态实现工厂扩展， 如果createParser中步骤比较繁琐，多态的优势才能体现出来
class RuleConfigParserFactory
{
public:
    IRuleConfigParser createParser() = 0;
}

class JsonRuleConfigParserFactory : public RuleConfigParserFactory
{
public:
    IRuleConfigParser createParser() override
    {
        return new JsonRuleConfigParser();
    }
}

class XmlRuleConfigParserFactory : public RuleConfigParserFactory
{
public:
    IRuleConfigParser createParser() override
    {
        return new XmlRuleConfigParser();
    }
}

/* 抽象工厂 */
class ConfigParserFactory
{
public:
    IRuleConfigParser createRuleParser() = 0;
    ISystemConfigParser createSystemParser() = 0;
}

class JsonConfigParserFactory : public ConfigParserFactory
{
public:
    IRuleConfigParser createRuleParser() override
    {
        return new JsonRuleConfigParser();
    }
    ISystemConfigParser createSystemParser() override
    {
        return new JsonSystemConfigParser();
    }
}


/* 构建者 */
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class Product
{
private:
    std::vector<string> parts;

public:
    void Add(string part) {
        parts.push_back(part);
    }
};

class Builder
{
public:
    virtual void BuildPartA() = 0;
    virtual void BuildPartB() = 0;
    virtual Product *GetResult() = 0;
};

class ConcreteBulider1 : public Builder
{
private:
    Product *product;
public:
    ConcreteBulider1() {
        product = new Product();
    }
    ~ConcreteBulider1() {
        delete product;
    }
    void BuildPartA() {
        product->Add("part A");
    }
    void BuildPartB() {
        product->Add("part B");
    }
    Product *GetResult() {
        return product;
    }
};

class ConcreteBulider2 : public Builder
{
private:
    Product *product;
public:
    ConcreteBulider2() {
        product = new Product();
    }
    ~ConcreteBulider2() {
        delete product;
    }
    void BuildPartA() {
        product->Add("part X");
    }
    void BuildPartB() {
        product->Add("part Y");
    }
    Product *GetResult() {
        return product;
    }
};

class Director
{
public:
    void Construct(Builder *builder) {
        builder->BuildPartA();
        builder->BuildPartB();
    }
};

// Client 客户不知道具体的建造过程
void main()
{
    Director *director = new Director();
    Builder *builder1 = new ConcreteBulider1();
    Builder *builder2 = new ConcreteBulider2();

    cout << "指挥者用ConcreteBuilder1的方法建造产品：" << endl;
    director->Construct(builder1);
    Product *p1 = builder1->GetResult();
    p1->Show();
    cout << endl;

    cout << "指挥者用ConcreteBuilder2的方法建造产品：" << endl;
    director->Construct(builder2);
    Product *p2 = builder2->GetResult();
    p2->Show();
    cout << endl;

    delete director;
    delete builder1;
    delete builder2;
}

/************* 构建者2 *************/
class House
{
public:
	House(){}
	void setFloor(string iFloor) {
		floor = iFloor;
	}
    void setWall(string iWall) {
        wall = iWall;
    }
    void setRoof(string iRoof) {
		roof = iRoof;
	}
private:
	string floor;
	string wall;
	string roof;
};

class AbstractBuilder
{
public:
	virtual void buildFloor() = 0;
	virtual void buildWall() = 0;
	virtual void buildRoof() = 0;
    House *getHouse() {
		return house;
	}
	House *house;
};

class ConcreteBuilderA :public AbstractBuilder
{
public:
	ConcreteBuilderA() {
		house = new House();
	}
	void buildFloor() {
		house->setFloor("Floor_A");
	}
	void buildWall() {
		house->setWall("Wall_A");
	}
	void buildRoof() {
		house->setRoof("Roof_A");
	}
};

class ConcreteBuilderB :public AbstractBuilder
{
public:
	ConcreteBuilderB() {
		house = new House();
	}
	void buildFloor() {
		house->setFloor("Floor_B");
	}
	void buildWall() {
		house->setWall("Wall_B");
	}
	void buildRoof() {
		house->setRoof("Roof_B");
	}
};

class Director
{
public:
	Director(){}
	void setBuilder(AbstractBuilder *iBuilder) {
		builder = iBuilder;
	}
	House *construct() {
		builder->buildFloor();
		builder->buildWall();
		builder->buildRoof();
		return builder->getHouse();
	}
private:
	AbstractBuilder *builder;
};
 
int main()
{
	AbstractBuilder *builder;
	Director *director = new Director();
	House *house;
 
	builder = new ConcreteBuilderA();
	director->setBuilder(builder);
	house = director->construct();
 
	builder = new ConcreteBuilderB();
	director->setBuilder(builder);
	house = director->construct();
	return 0;
}

