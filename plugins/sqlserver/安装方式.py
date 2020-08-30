[wingdow]
   无需配置
[linux]
  1.安装unixodbc　　　　apt-get install unixodbc　　　　apt-get install unixodbc-dev
  2.安装freetds
  3.配置odbc    gedit /etc/odbcinst.ini
	[ODBC]
	Trace           = Yes
	TraceFile       = /tmp/sql.log
	ForceTrace      = Yes
	Pooling         = No

	[SQL SERVER]
	Description     = SQLServer
	Driver          = /usr/lib/x86_64-linux-gnu/odbc/libtdsodbc.so
	Setup           = /usr/lib/x86_64-linux-gnu/odbc/libtdsS.so
	UsageCount      = 1
	CPTimeout       = 5
	CPReuse         = 5
	FileUsage       = 1


