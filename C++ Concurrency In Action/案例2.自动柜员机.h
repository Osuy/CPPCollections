#pragma once
#include <string>
#include "����1.��Ϣ����.h"
#include <iostream>

// ��һ���֣���Ա����Ϣ������̳���Ϣ���࣬������Ϊwrapped_message��ģ�����
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

// �ڶ����֣���Ա��״̬��
class atm
{
	messaging::receiver incoming;
	messaging::sender bank;
	messaging::sender interface_hardware;

	using atm_men_func = void (atm::*)();
	atm_men_func state; // ֱ�ӽ���Ա����ָ����Ϊ״̬���ͣ�����ʲô״̬��ִ��ʲô����

	std::string account;
	unsigned withdrawal_amout;
	std::string pin;

	/*
		���µ�����״̬��������private����Ҫ�߼���������Ϣ�ַ���
		��Ϣ�ַ�������㶼��receiver.wait(). ������һ�� dispatcher
		����֮�����handle��ģ�������ϣ���������Ϣ������
		handle���� TemplateDispatcher<...>������Ҳ������ʽ��ͬ��handle���������ص�Ҳ��TemplateDispatcher<...>
		����handle������ʽ���ã�����һϵ��������TemplateDispatcher<...>
		ÿ��TemplateDispatcher<...>���Ὣǰһ����chained��Ϊtrue
		����ֻ�����һ��TemplateDispatcher<...>��chained��false
		��������������ֻ����!chainedʱ������������Ϣ�����ԣ�ֻ�����һ�� TemplateDispatcher<...>��������
		���ʶ��д�����Ϣ��������ֻ�����ض�����Ϣ�������Ϣ��ƥ�䣬�򴫵ݸ���һ����ֱ�����ݵ�dispatch
		���Ҫ�����һ��������ʱ���������󻹴��ڡ�
		ǡ�ã�C++�涨�����Թ���˳�������ȥ��������������һ��TemplateDispatcher<...>��ʼ����
		���� chained��false�����ǿ�ʼ����������

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
		// �ȴ��忨
		// ������ʾ�忨��Ϣ�� interface_hardware
		interface_hardware.send(display_enter_card{});
		// ���� �ȴ��忨 ��Ϣ�ַ���
		// ��Ϊ �ȴ��忨״ֻ̬��Ҫ����������Ϣ������ֻ��һ��dispatcher
		incoming.wait()
			.handle<card_inserted>(
				[&](const card_inserted& msg)
				{
					account = msg.account;// ��ȡ�˻�
					pin = "";// ���pin�ľ�ֵ
					interface_hardware.send(display_enter_pin{});// ���� ��ʾ����pin ��Ϣ�� interface_hardware
					state = &atm::getting_pin;// �л�״̬��getting_pin
				});
	}

	void done_processing()
	{
		// ��ɴ���
		// ���͵�������Ϣ�� interface_hardware
		interface_hardware.send(eject_card{});
		// �л�״̬��waiting_for_card
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
			atm����������ֱ������stateΪwaiting_for_card��Ȼ����ѭ��ִ��state
			ֱ��state�׳����쳣
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

// ����״̬��
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

// �û�����״̬��
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

// ��������
void luanch()
{
	// ��������״̬�����ڵ����߳�������
	bank_machine bank;
	std::thread bank_thread{ &bank_machine::run, &bank };

	// �����û���Ŀ״̬�����ڵ����߳�������
	interface_machine interface_hardware;
	std::thread if_thread{ &interface_machine::run, &interface_hardware };

	// ����atm״̬�������������к��û��ڵ��senderΪ��Ա��˵��atm�᲻�Ϸ�����Ϣ�����к��û���Ŀ
	// �û����û����潻�����û������������atm����������atm�ǽ����ĺ���
	atm machine{ bank.get_sender(), interface_hardware.get_sender() };
	std::thread atm_thread{ &atm::run, &machine };

	// ����ȡ��atm��sender��������Ϣ
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
			atmqueue.send(digit_pressed{ c }); break; // ��������
		case 'b':
			atmqueue.send(balance_pressed{}); break;// ��ѯ���
		case 'w':
			atmqueue.send(withdraw_pressed{ 50 }); break;// ȡ��
		case 'c':
			atmqueue.send(cancel_pressed{}); break; // ȡ��
		case 'q':
			quit_pressed = true; break; // �˳���ѭ��
		case 'i':
			atmqueue.send(card_inserted{ "acc1234" }); break; // ���������i������Ϊ�������˻�acc1234�����п�
		}
	}

	// ��ѭ������������״̬����ֹ������close_queue��Ϣ��
	bank.done();
	machine.done();
	interface_hardware.done();

	// �����̻߳��
	atm_thread.join();
	if_thread.join();
	bank_thread.join();
}

/*
	���н���
	����״̬�������С��û������atm�����������߳�������run����
	ǰ���ߵ�runֻ�ǽ�����Ϣ�ַ�����������������Ϊ��ʱ״̬���Ķ���û���κ���Ϣ
	atm��run��Ĭ����wait_for_card��ִ�У�
		1.��ui����display_enter_card��Ϣ����ui״̬���Ķ�����push��������ui�̣߳�
		2.������Ϣ�ַ��������� card_inserted ��Ϣ

	ui�̱߳����Ѻ�����ȡ��display_enter_card��Ϣ��������ӡ����뿨��

	���������߳��������
	main�̸߳�����ѭ������ͣ����io�����벢��atm����Ϣ
	���ڴ�ʱatm���� �ȴ��忨 ״̬�� ֻ����card_inserted��Ϣ��������Ϣ������
	������ѭ����������ʲô��û��Ӧ����������i��Ҳ���ǲ忨
	atm����忨����ȡ���˻���������display_enter_pin��Ϣ��ui���л���getting_pin״̬
	ui�̴߳�ӡ������pin

	getting_pin״̬����������Ϣ��
		1.ȡ�����л���done_processing״̬
		2.�˸�ɾ���ϸ�����
		3.�������֣��������ۼӵ���Ա�ַ���pin�У�������ȴﵽ��4λ������ verify_pin{�˻�����pin��atm��receiver } ��Ϣ��bank�����л�״̬��verifying_pin
	getting_pin״̬����һ�����л�״̬����Ϊδ�����ꡢɾ���������֣��������û��������飬���Ա����� getting_pin״̬
	��ȡ�������볤�ȴ��Ż��л�״̬

	done_processing״̬�ᷢ��eject_card��Ϣ��ui��Ȼ���л���wait_for_card״̬
	verifying_pin״̬���Ǵ���pin��֤�ɹ���ʧ�ܵ���Ϣ�����ʧ�ܣ�����bank��Ӧǰ�û�ȡ����֤������done_processing״̬
	���bank��֤�ɹ�������Ϣ�������wait_for_action״̬���ȴ��û����˻�����������


*/