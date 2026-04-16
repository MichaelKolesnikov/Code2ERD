#pragma once
#include "LineModel.h"
#include "DTO/LinkLineDTO.h"

class LinkLineModel : public LineModel
{
   Q_OBJECT

   Q_PROPERTY(QString minCardinality READ minCardinality WRITE setMinCardinality NOTIFY minCardinalityChanged)

   Q_PROPERTY(QString maxCardinality READ maxCardinality WRITE setMaxCardinality NOTIFY maxCardinalityChanged)


public:
   explicit LinkLineModel(const LinkLineDTO& dto = LinkLineDTO());
   ~LinkLineModel() = default;

   const LinkLineDTO& dto() const { return m_dto; }

   // get

   const QString& minCardinality() const { return m_dto.minCardinality; }

   const QString& maxCardinality() const { return m_dto.maxCardinality; }


   // set

   void setMinCardinality(const QString& value);

   void setMaxCardinality(const QString& value);


signals:

   void minCardinalityChanged();

   void maxCardinalityChanged();

private:
   LinkLineDTO m_dto;
};
