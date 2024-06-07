# HighConcurrencyComm-Framework-

精华技术：
epoll高并发
TCP数据粘包解决
延迟回收设计连接池
线程池
多线程访问消息队列
根据数据包查找相应业务逻辑
心跳线程检测客户端连接



  以epoll去触发事件流向，
    读【采用了两阶段{包头+包体}】，然后通过触发线程处理我们的业务逻辑
    发【单独的线程去处理我们要发送的数据】 我们采用先write，如果没有发完，再加到我们的epoll事件


	
```

ngx_read_request_handler													//读操作
	void ngx_wait_request_handler_proc_p1(lpngx_connection_t pConn,bool &isflood); 		//包头收完整后的处理，我们称为包处理阶段1：写成函数，方便复用
	void ngx_wait_request_handler_proc_plast(lpngx_connection_t pConn,bool &isflood);      //收到一个完整包后的处理，放到一个函数中，方便调用	
		 g_threadpool.inMsgRecvQueueAndSignal(pConn->precvMemPointer); 				//入消息队列并触发线程处理消息
		 	Call();                    										//可以激发一个线程来干活了，刺激ThreadFunc线程函数
				g_socket.threadRecvProcFunc(jobbuf);    						//处理消息队列中来的消息
    					(this->*statusHandler[imsgCode])(p_Conn,pMsgHeader,(char *)pPkgBody,pkglen-m	_iLenPkgHeader); 	//(4)调用消息码对应的成员函数来处理
					// 收包, 检测crc, 填充包
					msgSend(p_sendbuf);  
						m_MsgSendQueue.push_back(psendbuf);     // void* CSocekt::ServerSendQueueThread(void* threadData)  //专门用来发送数据的线程, 交给发送线程处理
						sem_post(&m_semEventSendQueue）



ngx_write_request_handler													//写操作
	sendproc(pConn,pConn->psendbuf,pConn->isendlen);
	sem_post(&m_semEventSendQueue);											//触发线程处理  
		void* CSocekt::ServerSendQueueThread(void* threadData)						//专门用来发送数据的线程
		sem_wait(&pSocketObj->m_semEventSendQueue)								//等待触发
          	sendsize = pSocketObj->sendproc(p_Conn,p_Conn->psendbuf,p_Conn->isendlen); //注意参数
```

![image](https://github.com/18953014746/HighConcurrencyComm-Framework-/assets/125641755/4a08b89d-5f52-4f54-bf39-35f8bbe2447d)
