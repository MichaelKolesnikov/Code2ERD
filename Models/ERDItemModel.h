#pragma once
#include <QObject>

class ERDItemModel : public QObject
{
   Q_OBJECT
public:
   explicit ERDItemModel(QString id);

   QString id() const;

private:
   const QString m_id;
};

