#pragma once
#include "Core_global.h"
#include "EntityDTO.h"
#include "RelationshipDTO.h"

// Everything written in the text, in the order of the text
struct CORE_EXPORT DiagramDTO
{
   QVector<EntityDTO> entities;
   QVector<RelationshipDTO> relationships;

   const EntityDTO* entity(const QString& name) const;
   const RelationshipDTO* relationship(const QString& name) const;
   bool isEmpty() const;
};
