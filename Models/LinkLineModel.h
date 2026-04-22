#pragma once
#include "LineModel.h"
#include "DTO/LinkLineDTO.h"

class LinkLineModel : public LineModel
{
   Q_OBJECT
   Q_PROPERTY(QString minCardinality READ minCardinality WRITE setMinCardinality)
   Q_PROPERTY(QString maxCardinality READ maxCardinality WRITE setMaxCardinality)

public:
   explicit LinkLineModel(const LinkLineDTO& dto = LinkLineDTO());
   ~LinkLineModel() = default;

   const LinkLineDTO& dto() const { return m_dto; }

   const QString& minCardinality() const { return m_dto.minCardinality; }
   const QString& maxCardinality() const { return m_dto.maxCardinality; }

   void setMinCardinality(const QString& value);
   void setMaxCardinality(const QString& value);

private:
   LinkLineDTO m_dto;
};
