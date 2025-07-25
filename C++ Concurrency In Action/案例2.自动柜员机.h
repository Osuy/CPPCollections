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
			.handle<balance>(
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