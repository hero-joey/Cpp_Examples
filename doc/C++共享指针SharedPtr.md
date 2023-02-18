### C++共享指针SharedPtr

##### 共享指针SharedPtr 用于解决内存泄漏和空指针问题

##### 当使用原始指针构造SharedPtr时，引用计数增加1

##### 当SharedPtr离开作用域时, 析构函数中会将引用计数减1, 如果引用计数为0, 则释放原始指针



```c++
// SmartPtrSample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <memory>
#include <functional>
using namespace std;

struct Sample
{
    int m_number = 0;

    Sample(int number)
    {
        m_number = number;
        std::cout << "CONSTRUCTOR\n";
    }
    
    Sample()
    {
        std::cout << "CONSTRUCTOR EMPTY\n";
    }
    
    ~Sample()
    {
        std::cout << "DESTRUCTOR\n";
    }
    
    void dummyFunction()
    {
        cout << "dumy function: " << m_number << endl;
    }

};

// function that calls the delete [] on received pointer
void deleter(Sample* x)
{
    std::cout << "DELETER FUNCTION CALLED\n";
    delete[] x;
}

class Deleter
{
public:
    void operator() (Sample* x) {
        std::cout << "FUNTION OBJECT DELETER FUNCTION CALLED\n";
        delete[] x;
    }
};


template<typename T>
class MemoryPool
{
public:
    T* AquireMemory()
    {
        std::cout << "AQUIRING MEMORY\n";
        return (new T());
    }
    void ReleaseMemory(T* ptr)
    {
        std::cout << "RELEASE MEMORY\n";
        delete ptr;
    }
};

int main()
{
	//创建智能指针method1
    std::shared_ptr<int> p1(new int(10));

	//创建智能指针method2
	std::shared_ptr<int> p2 = std::make_shared<int>();
	
	//查看智能指针引用计数
	std::cout << p1.use_count() << endl;
	//获取裸指针
	std::cout << p1.get() << endl;
	
	//重置智能指针,引用计数变为0
	p1.reset();
	std::cout << p1.use_count() << endl;
	
	p1.reset(new int(50));
	std::cout << p1.use_count() << endl;
	cout << *p1 << endl;
	
	std::shared_ptr<int> p3(p1);
	std::cout << p1.use_count() << endl;
	std::cout << p3.use_count() << endl;
	cout << *p3 << endl;
	
	if (p1 == p3)
	{
	    cout << "Same pointer" << endl;
	}
	
	shared_ptr<Sample> samplePtr = make_shared<Sample>(50);
	samplePtr->dummyFunction();
	(*samplePtr).dummyFunction();
	
	std::shared_ptr<Sample> ptr3;
	if (!ptr3)
	    std::cout << "Yes, ptr3 is empty" << std::endl;
	if (ptr3 == NULL)
	    std::cout << "ptr3 is empty" << std::endl;
	if (ptr3 == nullptr)
	    std::cout << "ptr3 is empty" << std::endl;
	
	std::shared_ptr<Sample> ptr = std::make_shared<Sample>();
	
	//Ideally we should not use this because it can cause problem if we delete this pointer by mistake
	Sample* rawptr = ptr.get();
	
	// Creating a shared+ptr with custom deleter
	std::shared_ptr<Sample> p6(new Sample[2], deleter);
	
	// Function Object as deleter
	std::shared_ptr<Sample> p10(new Sample[2]{ {2}, {3} }, Deleter());
	p10->dummyFunction();
	
	// Lambda function as deleter
	std::shared_ptr<Sample> p11(new Sample[3], [](Sample* x) {
	    std::cout << "LAMBDA DELETER FUNCTION CALLED\n";
	    delete[] x;
	    });


    std::shared_ptr<MemoryPool<Sample> > memoryPoolPtr = std::make_shared<
        MemoryPool<Sample> >();
    std::shared_ptr<Sample> p12(memoryPoolPtr->AquireMemory(),
        std::bind(&MemoryPool<Sample>::ReleaseMemory, memoryPoolPtr,
            std::placeholders::_1));
    
    return 0;

}
```



##### 创建shared_ptr可以使用原始指针直接创建，例如 std::shared_ptr<int> p1(new int(10)), 也可以通过std::shared_ptr<int> p2 = std::make_shared<int>()。

##### shared_ptr可以通过use_count()查看引用计数; reset()函数用于重置或者重新关联原始指针; get()函数用于获取原始指针; ==已经被重载，用于判断两个原始指针是否相同。

##### share_ptr支持拷贝或者赋值, 相应的引用计数也会增加。

##### shared_ptr可以定制释放函数，例如shared_ptr指向数组则不能使用默认的释放函数, 如果我们使用的是对象池, 则同样不能真正释放该对象。

##### shared_ptr存在循环引用的问题, 见weak_ptr。

