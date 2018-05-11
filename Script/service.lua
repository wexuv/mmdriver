require("Script.config");

ThreadCount=2;

ServiceType=
{
	INVALID = -1;
	LOGIN = 0;	--登录
	HTTP = 1;	--http
	MONITOR = 2;--监控
	CLIENT = 3;	--主动发起连接
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