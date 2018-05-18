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

	Http =
	{
		type=ServiceType.HTTP;
		count=1;
		server=ServerSettings.Server1;
		channel={MessageChannel.LOGIN_HTTP};
	};
};