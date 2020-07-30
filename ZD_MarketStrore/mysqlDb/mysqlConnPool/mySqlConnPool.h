#ifndef _DB_POOL_H_
#define _BD_POOL_H_

#include <iostream>
#include <mysql_connection.h>  
#include <mysql_driver.h>  
#include <cppconn/exception.h>  
#include <cppconn/driver.h>  
#include <cppconn/connection.h>  
#include <cppconn/resultset.h>  
#include <cppconn/prepared_statement.h>  
#include <cppconn/statement.h>  
#include <mutex>  
#include <list>  

#include <vector>
#include <iostream>
#include <stdexcept>  
#include <exception>  
#include <stdio.h> 

using namespace std;
using namespace sql;

class DBPool
{
public:
	// Singleton 
	static DBPool& GetInstance();

	//init pool
	void initPool(std::string url_, std::string user_, std::string password_, int maxSize_);

	//get a conn from pool
	Connection* GetConnection();

	//put the conn back to pool
	void ReleaseConnection(Connection *conn);

	//create a db and table for heartBeat
	void heartBeatThread();

	~DBPool();

private:
	DBPool() {}

	//init DB pool
	void InitConnection(int initSize);

	// create a connection
	Connection* CreateConnection();

	//destory connection
	void DestoryConnection(Connection *conn);

	//destory db pool
	void DestoryConnPool();

	//create  db and table for hertbeat
	void createDbTalbe();
private:
	string user;
	string password;
	string url;
	int maxSize;
	int curSize;

	Driver*             driver;     //sql driver (the sql will free it)
	list<Connection*>   connList;   //create conn list

	//thread lock mutex
	static std::mutex			m_mutex;

	//heartBeat
	const std::string heartBeatDb = "heartBeatDb";
	const std::string heartBeatTalble = "heartBeatTalble";
	const int heartBeatTime = (60*30);	//second
	bool heartBeatWork;
};

void mysqlConnPooltest();

#endif

