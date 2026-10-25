#include "LayoutMapper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <cmath>

namespace
{
   const int FormatVersion = 1;
}

QByteArray LayoutMapper::toJson(const LayoutDTO &layout)
{
   QJsonObject nodes;
   for (auto it = layout.nodes.cbegin(); it != layout.nodes.cend(); ++it)
   {
      // whole pixels are enough and keep the file readable
      nodes.insert(it.key(), QJsonObject{{"x", std::round(it.value().x())}, {"y", std::round(it.value().y())}});
   }
   QJsonObject root;
   root.insert("version", FormatVersion);
   root.insert("nodes", nodes);
   return QJsonDocument(root).toJson(QJsonDocument::Indented);
}

LayoutDTO LayoutMapper::fromJson(const QByteArray &data, bool *ok)
{
   QJsonParseError error;
   const QJsonDocument document = QJsonDocument::fromJson(data, &error);
   const bool valid = error.error == QJsonParseError::NoError && document.isObject() && document.object().value("nodes").isObject();
   if (ok)
   {
      *ok = valid;
   }
   LayoutDTO layout;
   if (!valid)
   {
      return layout;
   }
   const QJsonObject nodes = document.object().value("nodes").toObject();
   for (auto it = nodes.constBegin(); it != nodes.constEnd(); ++it)
   {
      const QJsonObject point = it.value().toObject();
      if (point.value("x").isDouble() && point.value("y").isDouble())
      {
         layout.nodes.insert(it.key(), QPointF(point.value("x").toDouble(), point.value("y").toDouble()));
      }
   }
   return layout;
}
