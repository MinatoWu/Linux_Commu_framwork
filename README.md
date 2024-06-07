# HighConcurrencyComm-Framework-
基于Linux的高并发服务器通讯脚手架
1. 为解决数据粘包问题，采用包头+包体格式正确接收客户端发送的数据包。
2. 通过类有限状态机完成数据包和业务处理逻辑的一一对应，自定义封应用层协议。
3. 通过线程池技术减少资源损耗，增强项目稳定性，一对一完成业务结果的传输。
4. 通过将连接对象指针存储在 epoll 事件的字段中，实现直接访问连接信息，减少延迟并提高代码的可维护性
5. 通过延迟回收技术完成连接池的设计，进一步增强服务器的稳定性。


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
