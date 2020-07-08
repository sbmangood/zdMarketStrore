#include "mySqlConnPool.h"

using namespace std;
using namespace sql;

std::mutex DBPool::m_mutex;

//Singleton: get the single object
DBPool& DBPool::GetInstance()
{
	
	static DBPool instance_;
	return instance_;
}

void DBPool::initPool(std::string url_, std::string user_, std::string password_, int maxSize_)
{
	this->user = user_;
	this->password = password_;
	this->url = url_;
	this->maxSize = maxSize_;
	this->curSize = 0;

	try {
		this->driver = sql::mysql::get_driver_instance();
	}
	catch (sql::SQLException& e)
	{
		perror("Get sql driver failed");
	}
	catch (std::runtime_error& e)
	{
		perror("Run error");
	}
	this->InitConnection(maxSize / 2);
}

//init conn pool
void DBPool::InitConnection(int initSize)
{
	Connection* conn;
	std::lock_guard<std::mutex> lk(m_mutex);
	
	for (int i = 0; i < initSize; i++)
	{
		conn = this->CreateConnection();

		if (conn)
		{
			connList.push_back(conn);
			++(this->curSize);
		}
		else
		{
			perror("create conn error");
		}
	}
	

}

Connection* DBPool::CreateConnection()
{
	Connection* conn;
	try {
		conn = driver->connect(this->url, this->user, this->password);  //create a conn 
		return conn;
	}
	catch (sql::SQLException& e)
	{
		perror("link error");
		return NULL;
	}
	catch (std::runtime_error& e)
	{
		perror("run error");
		return NULL;
	}
}

Connection* DBPool::GetConnection()
{
	Connection* conn;

	std::lock_guard<std::mutex> lk(m_mutex);

	if (connList.size() > 0)//the pool have a conn 
	{
		conn = connList.front();
		connList.pop_front();//move the first conn 
		if (conn->isClosed())//if the conn is closed, delete it and recreate it
		{
			delete conn;
			conn = this->CreateConnection();
		}

		if (conn == NULL)
		{
			--curSize;
		}

		

		return conn;
	}
	else
	{
		if (curSize < maxSize)//the pool no conn
		{
			conn = this->CreateConnection();
			if (conn)
			{
				++curSize;
				//lock.unlock();
				return conn;
			}
			else
			{
				//lock.unlock();
				return NULL;
			}
		}
		else //the conn count > maxSize
		{
			
			return NULL;
		}
	}
}

//put conn back to pool
void DBPool::ReleaseConnection(Connection *conn)
{
	if (conn)
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		connList.push_back(conn);
		
	}
}

void DBPool::DestoryConnPool()
{
	list<Connection*>::iterator iter;
	std::lock_guard<std::mutex> lk(m_mutex);
	for (iter = connList.begin(); iter != connList.end(); ++iter)
	{
		this->DestoryConnection(*iter);
	}
	curSize = 0;
	connList.clear();
	//lock.unlock();
}


void DBPool::DestoryConnection(Connection* conn)
{
	if (conn)
	{
		try {
			conn->close();
		}
		catch (sql::SQLException&e)
		{
			perror(e.what());
		}
		catch (std::exception& e)
		{
			perror(e.what());
		}
		delete conn;
	}
}

DBPool::~DBPool()
{
	this->DestoryConnPool();
}



void mysqlConnPooltest()
{
	DBPool dbPool = DBPool::GetInstance();

	dbPool.initPool("tcp://127.0.0.1:3306", "root", "123456", 100);

	Connection *con;
	Statement *state;
	ResultSet *result;

	std::vector< Connection *> vc;



	con = dbPool.GetConnection();	//get a db conn
	if (con == nullptr)
	{
		std::cout << "con=nullptr\n";
		while (1);
	}
	else
	{
		std::cout << "con=nullptr ok\n";
	}
	state = con->createStatement();

	try {

		state->execute("use ywh_test22");

		state->execute("create table user_age_table"
			" ("
			"name VARCHAR(100), "
			"age INT, "
			"likeS VARCHAR(100) "
			" ) ENGINE=InnoDB DEFAULT CHARSET=utf8 ");
	}
	catch (sql::SQLException &e) {
		std::cout << e.what() << std::endl;

	}

	std::string cmd = "INSERT INTO user_age_table ";
	std::string key = "(name,age,likeS) ";



	for (int i = 0; i < 11; i++)
	{
		std::string name = "(\"ywh";
		name = name + std::to_string(i);

		std::string value = "VALUES " + name + "\"" + "," + std::to_string(i) + "," + "\"suibian\")";
		cmd = cmd + key + value;
		//std::cout << cmd << std::endl;

		try {
			state->execute(cmd);
		}

		catch (sql::SQLException &e) {
			std::cout << e.what() << std::endl;

		}
		cmd = "insert into user_age_table ";
	}




	//²éÑ¯
	result = state->executeQuery("select name,age from user_age_table where age=5;");

	//Êä³ö²éÑ¯
	while (result->next())
	{
		try
		{
			std::string name = result->getString("name");
			int age = result->getInt("age");
			std::cout << "name: " << name << "   age: " << age << std::endl;

		}
		catch (sql::SQLException &e) {
			std::cout << e.what() << std::endl;

		}
	}

	delete result;
	delete state;
	dbPool.ReleaseConnection(con);
}