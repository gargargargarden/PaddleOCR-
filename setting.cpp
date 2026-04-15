#include"setting.h"
#include<QMutex>
Setting* Setting::self;
void Setting::InitInstance()
{
	if (self == nullptr) {
		QMutex mutex;
		QMutexLocker locker(&mutex);
		if (self == nullptr) {
			// ÀÁººµ¥ÀýÄ£Ê½
			self = new Setting();
		}
	}
}

Setting* Setting::Instance()
{
	return self;
}
