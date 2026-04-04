#pragma once
#include <QObject>

class ERDItemModel : public QObject
{
   Q_OBJECT
public:
   explicit ERDItemModel(int id);

   int id() const;

private:
   const int m_id;
};

