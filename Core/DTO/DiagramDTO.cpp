#include "DiagramDTO.h"

const EntityDTO* DiagramDTO::entity(const QString &name) const
{
   for (const EntityDTO& entity : entities)
   {
      if (entity.name == name)
      {
         return &entity;
      }
   }
   return nullptr;
}

const RelationshipDTO* DiagramDTO::relationship(const QString &name) const
{
   for (const RelationshipDTO& relationship : relationships)
   {
      if (relationship.name == name)
      {
         return &relationship;
      }
   }
   return nullptr;
}

bool DiagramDTO::isEmpty() const
{
   return entities.isEmpty() && relationships.isEmpty();
}
