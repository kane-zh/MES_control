TEMPLATE = subdirs
#跨平台插件
win32|unix{
SUBDIRS += \
        keyence \   #基恩士上位链路
        modbus\     #modbus串口通讯
        modbustcp\  #modbus网口通讯
        mysql\      #mysql数据库
        sqlserver\  #sqlserver数据库
        postgresql\ #postgresql数据库
        sqllite\    #sqllite数据库
        influxdb\   #influxdb数据库
        mes\        #慧智通MES
#        mqtt_pub
        control2\   #3D模型演示(测试)
        control3\   #发那科生产监控(丰光精密)
}


##window平台专用插件
win32{
SUBDIRS += \
        fanuc\
        control1\   #mes对接(潍坊精卫)
        control3\   #发那科自动报工(丰光精密)
        control4\   #发那科生产监控(丰光精密)
}



##linux平台专用插件
unix{
SUBDIRS += \
         opcua\
         control5
}
