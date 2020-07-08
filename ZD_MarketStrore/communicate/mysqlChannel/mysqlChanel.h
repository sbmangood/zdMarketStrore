#include "globalInclude.h"

class MysqlChannel:public IChannel
{
public:
	explicit MysqlChannel( boost::shared_ptr<IProtocol> protocol, MysqlConConfig &dbC);
	~MysqlChannel();

	virtual bool start() override;
	virtual bool send_data(const unsigned char* buffer, unsigned int size) override;

private:

	std::shared_ptr<DBPool>				dbPool;
	MysqlConConfig						dbConfig;
	Connection							*con;
	
	
};