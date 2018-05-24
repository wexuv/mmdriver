require("Script.config");

ThreadCount=2;

ServiceType=
{
	INVALID = -1;
	LOGIN = 0;			--登录
	HTTP = 1;			--http
	HTTPMANAGER = 2,	--http调度服务
	MONITOR = 3;		--监控
	CLIENT = 4;			--主动发起连接
	DBSERVICE = 5;		--DBService
	MAX = 20,
};

Service = {
	Login =
	{
		type=ServiceType.LOGIN;
		count=1;
		server=ServerSettings.Server1;
		channel={MessageChannel.LOGIN_HTTP};
	};

	HttpManager =
	{
		type=ServiceType.HTTPMANAGER;
		count=1;
		server=ServerSettings.Server1;
		msgchannel =
		{
			channel1=
			{
				key=200;
				input=1048576;
				output=1048576;
			}
		}
	};
	Http =
	{
		type=ServiceType.HTTP;
		count=1;
		server=ServerSettings.Server1;
		msgchannel =
		{
			channel1=
			{
				key=100;
				input=65536;
				output=65536;
			}
		}
	};
	DBService =
	{
		type=ServiceType.DBSERVICE;
		count=1;
		server=ServerSettings.Server1;
		msgchannel =
		{
			channel1=
			{
				key=500;
				input=1048576;
				output=1048576;
			}
		}
	};
};