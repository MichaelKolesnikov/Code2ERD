#pragma once
#include <QString>
#include <QVector>

// An attribute of an entity or a relationship; a composite one has children
struct AttributeDTO
{
   QString name;
   bool key = false;         // underlined
   bool partialKey = false;  // underlined with a dashed line, the discriminator of a weak entity
   bool multivalued = false; // double ellipse
   bool derived = false;     // dashed ellipse
   QString note;
   QVector<AttributeDTO> children;
   int line = 0;
};

// The name an attribute is kept under in the layout: Student.name.first
inline QString attributeKey(const QString& ownerKey, const QString& name)
{
   return ownerKey + '.' + name;
}
