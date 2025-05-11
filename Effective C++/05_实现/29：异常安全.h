#pragma once
#include <iosfwd>
#include <mutex>

/*
	
*/

namespace case29
{
/*
	�۲����´���
	һ��֧���첽���������Ĵ�����
*/
class Image
{
public:
	Image(std::istream& imgSrc) {}
};
class PrettyMenu
{
public:
	// ...
	void changeBackground(std::istream& imgSrc);
	// ... 

private:
	std::mutex mutex;
	class Image* bgImage = nullptr;
	int imageChanges = 0;
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	mutex.lock();

	if (bgImage)delete bgImage;
	
	++imageChanges;

	bgImage = new Image(imgSrc);

	mutex.unlock();
}
/*
	����û�п���new�쳣�������������⣺
		1.mutexû�н���
		2.bgImage���е���Դ�Ѿ��ͷŵ���δ�ÿ�
		3.bgImage��imageChanges�޷��ָ�

	����mutex�Ľ������⣬�ο�case13��Ӧʹ����Դ���������Զ�����������������ʹ���쳣Ҳ�ܱ�������
	����bgImage���ؼ�����deleteҪ������new�ĺ��棬�����ƶ���䣬Ҳ���Կ��Ǹ�Ϊ����ָ��
	bgImage.reset(new Image{imgSrc});
	���ڵ���reset֮ǰ����Բ�����ֵ��������������new�ĺ��棬һ��newʧ�ܣ�reset���ᱻ���ã�Ҳ�Ͳ���delete��ָ��

	�쳣��ȫ�����ֱ�֤��
		1.������֤����֤�����쳣�󣬳�����Ա��ֺϷ�״̬������й¶�����������
		2.ǿ�ұ�֤����֤Ҫôִ�гɹ���Ҫô���ص����ú���ǰ��״̬
		3.���쳣��֤����֤�����׳��쳣

	ǿ�ұ�֤�ķ�����copy-and-swap
		��ĳһ�����������ϣ���ṩǿ�ұ�֤�����Կ���һ�������������ϲ���
		������в���ͨ��������swap��ԭ������
		�ﵽ�ˡ�Ҫôȫ�ԣ�Ҫôȫ����Ч��

	�쳣��ȫ��ľͰЧӦ
	�������´���
*/

void f1();
void f2();
void doSomeThings()
{
	// ...
	f1();
	// ... 
	f2();
	// ...
}
/*
	doSomeThings���쳣��ȫ������f1��f2��Ӱ��
	���f1�ṩ�˻�����֤����ôdoSomeThingsҲֻ�л�����֤��fͬ��
	����f1��f2���ṩ��ǿ�ұ�֤����doSomeThings�Ƿ�ʹﵽǿ�ұ�֤�������أ�
	��һ����
	��Ϊf2��ǿ�ұ�֤��ֻ��ʹ��̬�ص�f1����֮�󣬶����ܻص�doSomeThing����֮��
	f1�ɹ�����ɵ�һ���޸ģ�����f2ʧ��Ҳ������أ�����������Ȼֻ�л�����֤

	����һ��ϵͳ���ԣ����к�����ǿ�ұ�֤��Ȼ����ʹ������ϵ�ﵽǿ�ұ�֤
*/
}