connect(ui->init_5,SIGNAL(clicked()),pThread05,SLOT(startInit()));//初始化5
connect(ui->run_5,SIGNAL(clicked()),pThread05,SLOT(startRun()));//运行5
connect(ui->pause_5,SIGNAL(clicked()),pThread05,SLOT(startPause()));//暂停5
connect(ui->alarm_5,SIGNAL(clicked()),pThread05,SLOT(startAlarm()));//报警5
connect(ui->init_5,SIGNAL(clicked()),pThread05,SLOT(startRestart()));//重启5
