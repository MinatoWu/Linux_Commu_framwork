
#ifndef __NGX_LOCKMUTEX_H__
#define __NGX_LOCKMUTEX_H__

#include <pthread.h> 

class CLock
{
public:
	CLock(pthread_mutex_t *pMutex)
	{
		m_pMutex = pMutex;
		pthread_mutex_lock(m_pMutex); //加锁互斥量
	}
	~CLock()
	{
		pthread_mutex_unlock(m_pMutex); //解锁互斥量
	}
private:
	pthread_mutex_t *m_pMutex;
    
};

#endif
