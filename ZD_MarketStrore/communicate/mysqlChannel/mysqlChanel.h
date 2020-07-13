#include "globalInclude.h"

class MysqlChannel:public IChannel
{

	class InsertDB_Task
	{
	public:
		std::string sql;
		std::string dbName;
		std::shared_ptr<DBPool>				dbPool;	

	public:
		void work();

	};

public:
	explicit MysqlChannel(boost::shared_ptr<IProtocol> protocol, MysqlConConfig &dbC, std::string &dbName,int threadNum=5);
	~MysqlChannel();

	virtual bool start() override;
	virtual bool send_data(const unsigned char* buffer, unsigned int size) override;

private:

	std::shared_ptr<DBPool>				dbPool;
	MysqlConConfig						dbConfig;
	

	std::string							dbName;
	boost::basic_thread_pool			threadPool;
	
	
};