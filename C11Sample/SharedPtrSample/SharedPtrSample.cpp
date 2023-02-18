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

