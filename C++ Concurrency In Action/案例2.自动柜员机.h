#pragma once
#include <string>
#include "案例1.消息队列.h"
#include <iostream>

// 第一部分：柜员机消息，无需继承消息基类，而是作为wrapped_message的模板参数
struct withdraw
{
	std::string account;
	unsigned amount;
	mutable messaging::sender atm_queue;
	withdraw(const std::string& account_, unsigned amount_, messaging::sender atm_queue_)
		: account{ account_ }
		, amount{ amount_ }
		, atm_queue{ atm_queue_ }
	{
	}
};

struct withdraw_ok {};
struct withdraw_denied {};

struct cancel_withdrawal
{
	std::string account;
	unsigned amount;
	cancel_withdrawal(const std::string& account_, unsigned amount_)
		: account{ account_ }
		, amount{ amount_ }
	{
	}
};

struct withdrawal_processed
{
	std::string account;
	unsigned amount;
	withdrawal_processed(const std::string& account_, unsigned amount_)
		: account{ account_ }
		, amount{ amount_ }
	{
	}
};

struct card_inserted
{
	std::string account;
	explicit card_inserted(const std::string& account_)
		: account{ account_ }
	{
	}
};

struct digit_pressed
{
	char digit;
	explicit digit_pressed(char digit_)
		: digit{ digit_ }
	{
	}
};

struct clear_last_pressed {};
struct eject_card {};

struct withdraw_pressed 
{
	unsigned amount;
	explicit withdraw_pressed(unsigned amount_)
		: amount{ amount_ }
	{
	}
};

struct cancel_pressed {};

struct issue_money
{
	unsigned amount;
	issue_money(unsigned amount_)
		: amount{ amount_ }
	{
	}
};

struct verify_pin
{
	std::string account;
	std::string pin;
	mutable messaging::sender atm_queue;
	verify_pin(const std::string& account_, const std::string& pin_, messaging::sender atm_queue_)
		: account{ account_ }
		, pin{ pin_ }
		, atm_queue{ atm_queue_ }
	{
	}
};

struct pin_verified {};
struct pin_incorrect {};
struct display_enter_pin {};
struct display_enter_card {};
struct display_insufficient_funds {};
struct display_withdrawal_cancelled {};
struct display_withdrawal_options {};
struct display_pin_incorret_message {};

struct get_balance 
{
	std::string account;
	mutable messaging::sender atm_queue;
	get_balance(const std::string& account_, messaging::sender atm_queue_)
		: account{ account_ }
		, atm_queue{ atm_queue_ }
	{
	}
};

struct balance
{
	unsigned amount;
	explicit balance(unsigned amount_)
		: amount{ amount_ }
	{
	}
};

struct display_balance
{
	unsigned amount;
	explicit display_balance(unsigned amount_)
		: amount{ amount_ }
	{
	}
};

struct balance_pressed {};

// 第二部分：柜员机状态机
class atm
{
	messaging::receiver incoming;
	messaging::sender bank;
	messaging::sender interface_hardware;

	using atm_men_func = void (atm::*)();
	atm_men_func state; // 直接将成员函数指针作为状态类型，处于什么状态就执行什么函数

	std::string account;
	unsigned withdrawal_amout;
	std::string pin;

	/*
		以下的所有状态函数都是private，主要逻辑是生成消息分发链
		消息分发链的起点都是receiver.wait(). 它返回一个 dispatcher
		在它之后调用handle，模板参数是希望处理的消息的类型
		handle返回 TemplateDispatcher<...>对象，它也有声明式相同的handle函数，返回的也是TemplateDispatcher<...>
		于是handle可以链式调用，生成一系列匿名的TemplateDispatcher<...>
		每个TemplateDispatcher<...>都会将前一个的chained设为true
		最终只有最后一个TemplateDispatcher<...>的chained是false
		而它的析构函数只会在!chained时处理队列里的消息，所以，只有最后一个 TemplateDispatcher<...>会真正地
		访问队列处理消息。但是它只处理特定的消息，如果消息不匹配，则传递给上一个，直至传递到dispatch
		这就要求最后一个在析构时，其他对象还存在。
		恰好，C++规定对象以构造顺序的逆序去析构，因此是最后一个TemplateDispatcher<...>开始析构
		它的 chained是false，于是开始工作。。。

	*/
	void process_withdrawal()
	{
		incoming.wait()
			.handle<withdraw_ok>(
				[&]()
				{
					interface_hardware.send(
						issue_money{ withdrawal_amout });
					bank.send(
						withdrawal_processed{ account, withdrawal_amout });
					state = &atm::done_processing;
				})
			.handle<withdraw_denied>(
				[&](const withdraw_denied& msg)
				{
					interface_hardware.send(display_insufficient_funds{});
					state = &atm::done_processing;
				})
			.handle<cancel_pressed>(
				[&](const cancel_pressed& msg)
				{
					bank.send(
						cancel_withdrawal{ account,  withdrawal_amout });
					interface_hardware.send(display_withdrawal_cancelled{});
					state = &atm::done_processing;
				});
	}

	void process_balance()
	{
		incoming.wait()
			.handle<balance>(
				[&](const balance& msg)
				{
					interface_hardware.send(display_balance{ msg.amount });
					state = &atm::wait_for_action;
				})
			.handle<cancel_pressed>(
				[&](const cancel_pressed& msg)
				{
					state = &atm::done_processing;
				});
	}

	void wait_for_action()
	{
		interface_hardware.send(display_withdrawal_options{});
		incoming.wait()
			.handle<withdraw_pressed>(
				[&](const withdraw_pressed& msg)
				{
					withdrawal_amout = msg.amount;
					bank.send(withdraw{ account, msg.amount, incoming });
					state = &atm::process_withdrawal;
				})
			.handle<balance_pressed>(
				[&](const balance_pressed& msg)
				{
					bank.send(get_balance{ account, incoming });
					state = &atm::process_balance;
				})
			.handle<cancel_pressed>(
				[&](const cancel_pressed& msg)
				{
					state = &atm::done_processing;
				});
	}

	void verifying_pin()
	{
		incoming.wait()
			.handle<pin_verified>(
				[&](const pin_verified& msg)
				{
					state = &atm::wait_for_action;
				})
			.handle<pin_incorrect>(
				[&](const pin_incorrect& msg)
				{
					interface_hardware.send(display_pin_incorret_message{});
					state = &atm::done_processing;
				})
			.handle<cancel_pressed>(
				[&](const cancel_pressed& msg)
				{
					state = &atm::done_processing;
				});
	}

	void getting_pin()
	{
		incoming.wait()
			.handle<digit_pressed>(
				[&](const digit_pressed& msg)
				{
					const unsigned pin_length = 4;
					pin += msg.digit;
					if (pin.length() == pin_length)
					{
						bank.send(verify_pin{ account, pin, incoming });
						state = &atm::verifying_pin;
					}
				})
			.handle<clear_last_pressed>(
				[&](const clear_last_pressed& msg)
				{
					if (!pin.empty())
					{
						pin.pop_back();
					}
				})
			.handle<cancel_pressed>(
				[&](const cancel_pressed& msg)
				{
					state = &atm::done_processing;
				});
	}

	void waiting_for_card()
	{
		// 等待插卡
		// 发送显示插卡消息到 interface_hardware
		interface_hardware.send(display_enter_card{});
		// 建立 等待插卡 消息分发链
		// 因为 等待插卡状态只需要处理卡插入消息，所以只有一个dispatcher
		incoming.wait()
			.handle<card_inserted>(
				[&](const card_inserted& msg)
				{
					account = msg.account;// 获取账户
					pin = "";// 清空pin的旧值
					interface_hardware.send(display_enter_pin{});// 发送 显示输入pin 消息到 interface_hardware
					state = &atm::getting_pin;// 切换状态到getting_pin
				});
	}

	void done_processing()
	{
		// 完成处理
		// 发送弹出卡消息到 interface_hardware
		interface_hardware.send(eject_card{});
		// 切换状态到waiting_for_card
		state = &atm::waiting_for_card;
	}

	atm(const atm&) = delete;
	atm& operator=(const atm&) = delete;
public:
	atm(messaging::sender bank_, messaging::sender interface_hardware_)
		: bank{ bank_ }
		, interface_hardware{ interface_hardware_ }
	{
	}

	messaging::sender get_sender()
	{
		return incoming;
	}

	void done()
	{
		get_sender().send(messaging::close_queue{});
	}

	void run()
	{
		/*
			atm启动函数，直接设置state为waiting_for_card，然后死循环执行state
			直至state抛出了异常
		*/
		state = &atm::waiting_for_card;
		try
		{
			for (;;)
			{
				(this->*state)();
			}
		}
		catch (const messaging::close_queue&)
		{

		}
	}
};

// 银行状态机
class bank_machine
{
	messaging::receiver incoming;
	unsigned balance;
public:
	bank_machine()
		: balance{ 199 }
	{
	}
	
	messaging::sender get_sender()
	{
		return incoming;
	}

	void done()
	{
		get_sender().send(messaging::close_queue{});
	}

	void run()
	{
		try 
		{
			for (;;)
			{
				incoming.wait()
					.handle<verify_pin>(
						[&](const verify_pin& msg)
						{
							if ("1937" == msg.pin)
							{
								msg.atm_queue.send(pin_verified{});
							}
							else 
							{
								msg.atm_queue.send(pin_incorrect{});
							}
						})
					.handle<withdraw>(
						[&](const withdraw& msg)
						{
							if (balance >= msg.amount)
							{
								msg.atm_queue.send(withdraw_ok{});
								balance -= msg.amount;
							}
							else
							{
								msg.atm_queue.send(withdraw_denied{});
							}
						})
					.handle<get_balance>(
						[&](const get_balance& msg)
						{
							msg.atm_queue.send(::balance{ balance });
						})
					.handle<withdrawal_processed>(
						[&](const withdrawal_processed& msg)
						{

						})
					.handle<cancel_withdrawal>(
						[&](const cancel_withdrawal& msg)
						{

						});
			}
		}
		catch(const messaging::close_queue&)
		{

		}
	}
};

// 用户界面状态机
class  interface_machine
{
	messaging::receiver incoming;
	std::mutex iom;
public:
	messaging::sender get_sender()
	{
		return incoming;
	}

	void done()
	{
		get_sender().send(messaging::close_queue{});
	}

	void run()
	{
		try
		{
			for (;;)
			{
				incoming.wait()
					.handle<issue_money>(
						[&](const issue_money& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "Issuing " << msg.amount << std::endl;
							}
						})
					.handle<display_insufficient_funds>(
						[&](const display_insufficient_funds& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "insufficient funds" << std::endl;
							}
						})
					.handle<display_enter_pin>(
						[&](const display_enter_pin& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "Please enter your PIN(0-9)" << std::endl;
							}
						})
					.handle<display_enter_card>(
						[&](const display_enter_card& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "Please enter your card(I)" << std::endl;
							}
						})
					.handle<display_balance>(
						[&](const display_balance& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "The balance of your account is " << msg.amount << std::endl;
							}
						})
					.handle<display_withdrawal_options>(
						[&](const display_withdrawal_options& msg)
						{ 
							{
								std::lock_guard lk{ iom };
								std::cout << "Withdraw 50 (w)" << std::endl
									<< "Display Balance? (b)" << std::endl
									<< "Cancel? (c)" << std::endl;
							}
						})
					.handle<display_withdrawal_cancelled>(
						[&](const display_withdrawal_cancelled& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "Withdrawal cancelled" << std::endl;
							}
						})
					.handle<display_pin_incorret_message>(
						[&](const display_pin_incorret_message& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "PIN incorrect" << std::endl;
							}
						})
					.handle<eject_card>(
						[&](const eject_card& msg)
						{
							{
								std::lock_guard lk{ iom };
								std::cout << "Ejecting card" << std::endl;
							}
						});
			}
		}
		catch (const messaging::close_queue&)
		{
			
		}
	}
};

// 驱动代码
void luanch()
{
	// 创建银行状态机，在单独线程上运行
	bank_machine bank;
	std::thread bank_thread{ &bank_machine::run, &bank };

	// 创建用户节目状态机，在单独线程上运行
	interface_machine interface_hardware;
	std::thread if_thread{ &interface_machine::run, &interface_hardware };

	// 创建atm状态机，并且以银行和用户节点的sender为成员，说明atm会不断发生消息给银行和用户节目
	// 用户和用户界面交互，用户界面和银行与atm交互，所以atm是交互的核心
	atm machine{ bank.get_sender(), interface_hardware.get_sender() };
	std::thread atm_thread{ &atm::run, &machine };

	// 单独取出atm的sender来发送消息
	messaging::sender atmqueue{ machine.get_sender() };
	bool quit_pressed = false;
	while (!quit_pressed)
	{
		char c = getchar();
		switch (c)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			atmqueue.send(digit_pressed{ c }); break; // 输入数字
		case 'b':
			atmqueue.send(balance_pressed{}); break;// 查询余额
		case 'w':
			atmqueue.send(withdraw_pressed{ 50 }); break;// 取款
		case 'c':
			atmqueue.send(cancel_pressed{}); break; // 取消
		case 'q':
			quit_pressed = true; break; // 退出主循环
		case 'i':
			atmqueue.send(card_inserted{ "acc1234" }); break; // 如果输入了i，则视为插入了账户acc1234的银行卡
		}
	}

	// 主循环结束，所有状态机终止（发送close_queue消息）
	bank.done();
	machine.done();
	interface_hardware.done();

	// 所有线程汇合
	atm_thread.join();
	if_thread.join();
	bank_thread.join();
}

/*
	运行解释
	三个状态机：银行、用户界面和atm独立在三个线程上运行run函数
	前二者的run只是建立消息分发链并立刻阻塞，因为此时状态机的队列没有任何消息
	atm的run会默认在wait_for_card上执行：
		1.向ui发送display_enter_card消息（向ui状态机的队列里push，并唤醒ui线程）
		2.建立消息分发链，处理 card_inserted 消息

	ui线程被唤醒后，立刻取出display_enter_card消息并处理（打印请插入卡）

	这样三个线程启动完成
	main线程负责主循环，不停接收io的输入并向atm发消息
	由于此时atm处于 等待插卡 状态， 只接收card_inserted消息，其他消息不做事
	所以主循环无论输入什么都没反应，除了输入i，也就是插卡
	atm处理插卡，获取到账户名，发送display_enter_pin消息给ui，切换到getting_pin状态
	ui线程打印请输入pin

	getting_pin状态处理三种消息：
		1.取消，切换到done_processing状态
		2.退格，删除上个数字
		3.输入数字，将数字累加到成员字符串pin中，如果长度达到了4位，则发送 verify_pin{账户名，pin，atm的receiver } 消息到bank，并切换状态到verifying_pin
	getting_pin状态并不一定会切换状态，因为未输入完、删除三个数字，都允许用户继续数组，所以保持在 getting_pin状态
	而取消和输入长度达标才会切换状态

	done_processing状态会发送eject_card消息到ui，然后切换到wait_for_card状态
	verifying_pin状态则是处理pin验证成功或失败的消息，如果失败，或者bank回应前用户取消验证，进入done_processing状态
	如果bank验证成功返回消息，则进入wait_for_action状态，等待用户对账户的其他操作


*/