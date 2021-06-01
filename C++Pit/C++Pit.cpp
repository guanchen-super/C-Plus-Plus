// C++Pit.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <list>
#include <vector>
#include <chrono>
#include <future>
#include <thread>
#include <iostream>
#include <algorithm>

void erase(std::vector<int>& vec, int a) {
	// 这个正确
	for (auto iter = vec.begin(); iter != vec.end();)
	{
		if (*iter == a)
		{
			iter = vec.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	//for (auto iter = vec.begin(); iter != vec.end(); ++iter) {  // error
	//	if (*iter == a) {
	//		vec.erase(iter); // error
	//	}
	//}
}

void func1()
{
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::cout << __FUNCDNAME__ << std::endl;
}

void func2()
{
	std::cout << __FUNCDNAME__ << std::endl;
}

bool isOdd(int i) 
{ 
	return i & 1; 
}

void print(const std::vector<int>& vec)
{
	for (const auto& i : vec)
	{
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}

int main()
{
	// 参考链接 url: https://mp.weixin.qq.com/s/w9yIbJdMvlc1rYzCS06nsg
    // PIT 1 
	// 无符号整数的错误使用
	/*for (unsigned int i = 10; i >= 0; --i)
	{
		std::cout << "i = " << i << std::endl;
	}*/

	// PIT 2
	// 容器的size()返回类型是无符号整数
	/*std::vector<int> vec;
	vec.push_back(1);
	for (auto idx = vec.size(); idx >= 0; idx--)
	{
		std::cout << "===== \n";
	}*/

	// PIT 3 
	// memcpy、memset只适用于POD结构

	// PIT 4
	// STL遍历删除时注意迭代器失效问题
	std::vector<int> vec{ 1, 2, 3, 4, 5 };
	int a = 2;
	erase(vec, a);

	// PIT 5
	// std::list排序使用自己的成员方法
	// 一般的容器排序都使用std::sort()，但是list特殊。
	std::list<int> list{ 1, 2, 3, 2 };
	list.sort();
	//std::sort(list.begin(), list.end());  // error
	for (auto i : list) {
		std::cout << i << " ";
	}
	std::cout << "==========================================\n";

	// PIT 6
	// new/delete、new[]/delete[]、malloc/free严格配对
	
	// PIT 7
	// 基类析构函数要是虚函数
	// 如果不是虚函数的话，可能会有内存泄漏的问题

	// PIT 8
	// 注释用/**/，而不是//
	// 注释用/**/，可能会出问题。原因:utf-8和ANSC(GB2312)编码混乱后，
	// 中文注释就乱码了，乱码中藏着 */，匹配错了，导致IDE实际注释的部分并非肉眼所见，
	// 定位极其困难，常见于Windows中。

	// PIT 9
	// 成员变量初始化
	// 成员变量没有默认初始化行为，需要手动初始化。

	// PIT 10
	// 不要返回局部变量的指针或引用

	// PIT 11
	// 浮点数判断是否相等问题
	float f = 0.2f;
	if (f == 0.2) {} // 错误用法
	if (abs(f - 0.2) < 0.00001) {} // 正确用法

	// PIT 12
	// vector clear和swap问题
	// 清空某个vector，可以使用swap而不是其clear方法，这样可以更早的释放vector内部内存。
	std::vector<int> vec2;
	std::vector<int>().swap(vec2);
	vec2.clear();

	// PIT 13
	// vector问题
	// 尽量不要在vector中存放bool类型，vector为了做优化，它的内部存放的其实不是bool。

	// PIT 14
	// 条件变量
	// 条件变量的使用有两大问题：信号丢失和虚假唤醒，相当重要

	// PIT 15
	// 类型转换
	// 在C++中尽量使用C++风格的四种类型转换，而不要使用C语言风格的强制类型转换。

	// PIT 16
	// 异步操作中async的使用
	// std::async 这货返回的 future 和通过 promise 获取的 future 行为不同，
	// async 返回的 future 对象在析构时会阻塞等待 async 中的线程执行完毕，
	// 这就导致在大部分场景中 async达不到你直觉的认为它能达到的目的。
	std::async(std::launch::async, [] { func1(); }); // 临时量的析构函数等待 func1()
	std::async(std::launch::async, [] { func2(); }); // func1() 完成前不开始

	// PIT 17 
	// 智能指针
	// 一个裸指针不要使用多个智能指针包裹，尽可能使用make_unique，make_shared。
	// 当需要在类得内部接口中，需要将this作为智能指针使用，需要用该类派生自
	// enable_shared_from_this

	// PIT 18 
	// 栈内存使用
	// 合理使用栈内存，特别是数组，数组越界问题容易导致栈空间损坏，
	// 可以考虑使用std::array替代普通的数组。

	// PIT 19
	// std::thread的使用
	// 一定要记得join或这detach，否则会crash。
	std::thread t(func1);
	if (t.joinable())
	{
		t.join(); // 或者t.detach(); 
	}

	// PIT 20
	// enum使用
	// 尽量使用enum class替代enum，enum class 是带有作用域的枚举类型。

	// PIT 21
	// enum使用
	// 尽量使用enum class替代enum，enum class 是带有作用域的枚举类型。

	// PIT 22
	// 空指针使用nullptr而不是NULL

	// PIT 23
	// std::remove的使用
	// 这个remove其实并没有真正的删除元素，需要和erase配合使用，跑一下这段代码就知道啦。


	// PIT 24
	// 全局变量初始化问题
	// 不同文件中的全局变量初始化顺序不固定，全局变量尽量不要互相依赖，
	// 否则由于初始化顺序不固定的问题，可能会导致bug产生。

	std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	print(v);

	std::remove(v.begin(), v.end(), 5);  // error
	print(v);

	v.erase(std::remove(v.begin(), v.end(), 5), v.end());
	print(v);

	v.erase(std::remove_if(v.begin(), v.end(), isOdd), v.end());
	print(v);

	return std::getchar();
}