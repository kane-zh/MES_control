#ifndef COMBOXDELEGATE1_H
#define COMBOXDELEGATE1_H
#include <QItemDelegate>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
class ItemDelegate : public QItemDelegate
{
   Q_OBJECT
public:
   explicit  ItemDelegate(QObject *parent=0);
   QWidget   *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
   void setEditorData(QWidget *editor,const QModelIndex &index) const;
   void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const;
   void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index)const;
public slots:
   void setDriveInfor(QStringList value);
   void setDataSetInfor(QJsonObject value);
private:
   QStringList driveInfor;
   QJsonObject dataSetInfor;
};

#endif // COMBOXDELEGATE_H
