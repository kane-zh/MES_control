#include "pluginmanage.h"

/*处理插件发出的信号*/
void PluginManage::dealSignaleOfPlugin(RequestMetaData request)
{
    auto obj=this->sender();
    ContainerInterface *plugin = qobject_cast<ContainerInterface *>(obj);
    QtConcurrent::run(this,&PluginManage::dd,request,plugin);
}
void PluginManage::dd(RequestMetaData request,ContainerInterface *plugin){
    DriveInfor *infor= new DriveInfor();
    ResponseMetaData responseData;
    responseData.from=request.target;
    responseData.target=request.from;
    responseData.type=request.type;
    switch(request.type){
    case getDrivesInfor:
        responseData.value=infor->getDrivesInfor();
        plugin->receiveMsgFromPluginManager(responseData);
        delete infor;
        break;
    case getDataSetInfor:
        responseData.value=infor->getDataSetInfor(request.drive);
        plugin->receiveMsgFromPluginManager(responseData);
        delete infor;
        break;
    case getValue:
        responseData.value=infor->getValue(request.drive,request.id);
        plugin->receiveMsgFromPluginManager(responseData);
        delete infor;
        break;
    case setValue:
        responseData.value=infor->setValue(request.drive,request.id,request.value);
        plugin->receiveMsgFromPluginManager(responseData);
        delete infor;
        break;
     }
}
// 加载所有插件
void PluginManage::loadAllPlugins()
{
    // 进入插件目录
    QDir path = QDir(qApp->applicationDirPath());
    QString configDir=path.path()+("/plugins/config");
    if (!path.exists(configDir))
    {
     path.mkdir(configDir);
    }
    path.cd("plugins/libs");
    // 初始化插件中的元数据
    foreach (QFileInfo info, path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
    {
            scan(info.absoluteFilePath());
    }
    // 加载插件
    foreach (QFileInfo info, m_names.keys())
    {
            loadPlugin(info.absoluteFilePath());
    }
}
// 加载插件
void PluginManage::loadPlugin(const QString& path)
{
    // 判断是否是库
    if (!QLibrary::isLibrary(path)){
        return;
    }
    // 检测插件依赖
    if (!check(path)){
        return;
     }
      // 加载插件
      QPluginLoader *loader = new QPluginLoader(path);
      if (loader->load()) {
          if(m_types.value(path)=="dataSource"){
              // 如果继承自 Plugin，则认为是自己的插件（防止外部插件注入）。
                DriveInterface *plugin = qobject_cast<DriveInterface *>(loader->instance());
                if (plugin)
                {
                    m_loaders.insert(path, loader); // 保存插件loader实例
                }
                else
                {
                    delete loader;
                    loader = Q_NULLPTR;
                }
            }
          if(m_types.value(path)=="container"){
              // 如果继承自 Plugin，则认为是自己的插件（防止外部插件注入）。
                ContainerInterface *plugin = qobject_cast<ContainerInterface *>(loader->instance());
                if (plugin)
                {
                    connect(loader->instance(),SIGNAL(sendMsgToPluginManager(RequestMetaData)),this,SLOT(dealSignaleOfPlugin(RequestMetaData)));
                    m_loaders.insert(path, loader); // 保存插件loader实例
                }
                else
                {
                    delete loader;
                    loader = Q_NULLPTR;
                }
            }
          if(m_types.value(path)=="debugControl"){
              // 如果继承自 Plugin，则认为是自己的插件（防止外部插件注入）。
                DebugControlInterface *plugin = qobject_cast<DebugControlInterface *>(loader->instance());
                if (plugin)
                {
                    connect(loader->instance(),SIGNAL(sendMsgToPluginManager(RequestMetaData)),this,SLOT(dealSignaleOfPlugin(RequestMetaData)));
                    m_loaders.insert(path, loader); // 保存插件loader实例
                }
                else
                {
                    delete loader;
                    loader = Q_NULLPTR;
                }
            }
          if(m_types.value(path)=="autoControl"){
              // 如果继承自 Plugin，则认为是自己的插件（防止外部插件注入）。
                AutoControlInterface *plugin = qobject_cast<AutoControlInterface *>(loader->instance());
                if (plugin)
                {
                    connect(loader->instance(),SIGNAL(sendMsgToPluginManager(RequestMetaData)),this,SLOT(dealSignaleOfPlugin(RequestMetaData)));
                    m_loaders.insert(path, loader); // 保存插件loader实例
                }
                else
                {
                    delete loader;
                    loader = Q_NULLPTR;
                }
            }
        }
      else{
          qDebug()<<loader->errorString();
      }
}
/*扫描插件元数据*/
void PluginManage::scan(const QString& path)
{
    /*****
     *  判断是否是库（后缀有效性）
     * Windows：.dll、.DLL
     * Unix/Linux：.so
   *****/
    if (!QLibrary::isLibrary(path)){
        return;
       }
    // 获取元数据（名称、版本、依赖）
    QPluginLoader *loader = new QPluginLoader(path);
    QJsonObject json = loader->metaData().value("MetaData").toObject();
    QJsonDocument document;
    document.setObject(json);
    QString json_str(document.toJson(QJsonDocument::Compact));
    m_names.insert(path, json.value("name").toVariant());
    m_versions.insert(path, json.value("version").toVariant());
    m_title.insert(path, json.value("title").toVariant());
    m_dependencies.insert(path, json.value("dependencies").toArray().toVariantList());
    m_types.insert(path, json.value("type").toString());
    delete loader;
    loader = Q_NULLPTR;
}

// 检测插件依赖
bool PluginManage::check(const QString& path)
{
    bool status = true;

    foreach (QVariant item, m_dependencies.value(path)) {
        QVariantMap map = item.toMap();
        // 依赖的插件名称、版本、路径
        QVariant name = map.value("name");
        QVariant version = map.value("version");
        QString path = m_names.key(name);

        /********** 检测插件是否依赖于其他插件 **********/
        // 先检测插件名称
        if (!m_names.values().contains(name)) {
            qDebug() << Q_FUNC_INFO << "  Missing dependency:" << name.toString() << "for plugin" << path;
            status = false;
            continue;
        }
        // 再检测插件版本
        if (m_versions.value(path) != version) {
            qDebug() << Q_FUNC_INFO << "    Version mismatch:" << name.toString() << "version"
                     << m_versions.value(m_names.key(name)).toString() << "but" << version.toString() << "required for plugin" << path;
            status = false;
            continue;
        }
        // 然后，检测被依赖的插件是否还依赖于另外的插件
        if (!check(path)) {
            qDebug() << Q_FUNC_INFO << "Corrupted dependency:" << name.toString() << "for plugin" << path;
            status = false;
            continue;
        }
    }
    return status;
}

// 卸载所有插件
void PluginManage::unloadAllPlugins()
{
    foreach (const QString &path, m_loaders.keys())
        unloadPlugin(path);
}

// 卸载插件
void PluginManage::unloadPlugin(const QString& path)
{
    QPluginLoader *loader = m_loaders.value(path);

    // 卸载插件，并从内部数据结构中移除
    if (loader->unload())
    {
        m_loaders.remove(path);
        delete loader;
        loader = Q_NULLPTR;
    }
}

QList<QPluginLoader *> PluginManage::allPlugins()
{
    return m_loaders.values();
}

QPluginLoader* PluginManage::getPlugin(const QString &name)
{
    QPluginLoader *loader = m_loaders.value(m_names.key(name));
    return  loader;
}

