#include "ItemDelegate.h"
#include <QComboBox>
#include <QAbstractItemView>
#include <QtDebug>
#include "../../mainApp/pluginManage/driveinfor.h"
ItemDelegate::ItemDelegate(QObject *parent):
    QItemDelegate(parent)
{

}

/*创建编辑器*/
QWidget *ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &id) const
{
    if(id.column()==1||id.column()==2)
     {
        QComboBox *combox = new QComboBox(parent);
        return combox;
     }
}
/*设置编辑器数值*/
void ItemDelegate::setEditorData(QWidget *editor,const QModelIndex &id) const
{
    QString value=id.sibling(id.row(),1).data(Qt::DisplayRole).toString();
    if(id.column()==1)
    {
        QComboBox *combox = static_cast<QComboBox *>(editor);
        combox->clear();
        for(int i = 0; i< driveInfor.size();++i)
        {
            combox->addItem(driveInfor.at(i));
        }
    }
    if(id.column()==2)
    {  
        QComboBox *combox = static_cast<QComboBox *>(editor);
        combox->clear();
        QString cmd=id.sibling(id.row(),1).data(Qt::DisplayRole).toString();
        QJsonArray array=dataSetInfor.value(cmd).toArray();
        for (int i = 0; i < array.count(); i++)
         {
            QJsonObject value = array.at(i).toObject();
            combox->addItem(value["name"].toString(),value["id"].toDouble());
          }
    }

}

/*将编辑器的值设置成模型中的值*/
void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &id) const
{
    if(id.column()==1)
    {
       QComboBox *combox = static_cast<QComboBox *>(editor);
       model->setData(id, combox->currentText());
    }
    if(id.column()==2)
    {
       QComboBox *combox = static_cast<QComboBox *>(editor);
       QModelIndex nextIndex=id.sibling(id.row(),3);
       int  data=combox->currentData().toInt();
       QString  text=combox->currentText();
       model->setData(id, text);
       model->setData(nextIndex, data);
    }

}
/*设置编辑器显示的位置*/
void ItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &id) const
{
    editor->setGeometry(option.rect);
}

/*设置驱动信息*/
void ItemDelegate::setDriveInfor(QStringList value)
{
    driveInfor=value;
}

void ItemDelegate::setDataSetInfor(QJsonObject value)
{
    dataSetInfor=value;
}
