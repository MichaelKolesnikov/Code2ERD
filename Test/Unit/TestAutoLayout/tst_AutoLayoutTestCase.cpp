#include <QtTest>
#include <QLineF>
#include <QRectF>
#include <cmath>
#include "Layout/AutoLayout.h"
#include "Erd/ErdParser.h"

namespace
{
   const char* University =
      "Entity Student {\n"
      "  id [key]\n"
      "  name { first, last }\n"
      "  phone [multivalued]\n"
      "  age [derived]\n"
      "}\n"
      "Entity Course { code [key], title, credits }\n"
      "Entity Professor { id [key], name, office }\n"
      "Entity Department { name [key], building }\n"
      "Entity Dependent [weak] { name [partial key], birthday }\n"
      "Relationship Enrolls { Student (0, N), Course (1, N), grade }\n"
      "Relationship Teaches { Professor (1, N), Course (1, 1) }\n"
      "Relationship WorksIn { Professor (1, 1), Department (1, N), since }\n"
      "Relationship Offers { Department (0, N), Course (1, 1) }\n"
      "Relationship Has [identifying] { Professor (1), Dependent (N) [total] }\n"
      "Entity Lonely { a, b }\n";

   void collect(const QString& owner, const QVector<AttributeDTO>& attributes, QMap<QString, QSizeF>& sizes, QMap<QString, QString>* parents)
   {
      for (const AttributeDTO& attribute : attributes)
      {
         const QString key = attributeKey(owner, attribute.name);
         sizes.insert(key, QSizeF(30 + attribute.name.size() * 7, 34));
         if (parents)
         {
            parents->insert(key, owner);
         }
         collect(key, attribute.children, sizes, parents);
      }
   }

   QMap<QString, QSizeF> sizesOf(const DiagramDTO& diagram, QMap<QString, QString>* parents = nullptr)
   {
      QMap<QString, QSizeF> sizes;
      for (const EntityDTO& entity : diagram.entities)
      {
         sizes.insert(entity.name, QSizeF(40 + entity.name.size() * 8, 44));
         collect(entity.name, entity.attributes, sizes, parents);
      }
      for (const RelationshipDTO& relationship : diagram.relationships)
      {
         sizes.insert(relationship.name, QSizeF(60 + relationship.name.size() * 12, 64));
         collect(relationship.name, relationship.attributes, sizes, parents);
      }
      return sizes;
   }

   QRectF rectOf(const QPointF& center, const QSizeF& size)
   {
      return QRectF(center - QPointF(size.width() / 2, size.height() / 2), size);
   }

   DiagramDTO parsed(const char* text)
   {
      const ErdParseResult result = ErdParser::parse(text);
      if (!result.ok())
      {
         qWarning() << result.errors.first().message;
      }
      return result.diagram;
   }
}

class AutoLayoutTestCase : public QObject
{
   Q_OBJECT

private slots:
   void testEveryNodeIsPlaced();
   void testNoOverlaps();
   void testSameInputSamePicture();
   void testAttributesStayNearTheirOwner();
   void testAttributesAvoidTheLines();
   void testConnectedNodesAreCloser();
   void testCompleteKeepsKnownPositions();
   void testCompletePlacesANewAttributeNextToItsOwner();
   void testCompletePlacesANewEntityNearItsRelationship();
   void testEmptyDiagram();
};

void AutoLayoutTestCase::testEveryNodeIsPlaced()
{
   const DiagramDTO diagram = parsed(University);
   const QMap<QString, QSizeF> sizes = sizesOf(diagram);
   const QMap<QString, QPointF> positions = AutoLayout::arrange(diagram, sizes);
   QCOMPARE(positions.keys(), sizes.keys());
   for (const QPointF& point : positions)
   {
      QVERIFY(std::isfinite(point.x()) && std::isfinite(point.y()));
   }
}

void AutoLayoutTestCase::testNoOverlaps()
{
   const DiagramDTO diagram = parsed(University);
   const QMap<QString, QSizeF> sizes = sizesOf(diagram);
   const QMap<QString, QPointF> positions = AutoLayout::arrange(diagram, sizes);
   const QStringList keys = sizes.keys();
   for (int a = 0; a < keys.size(); ++a)
   {
      for (int b = a + 1; b < keys.size(); ++b)
      {
         const QRectF p = rectOf(positions[keys[a]], sizes[keys[a]]);
         const QRectF q = rectOf(positions[keys[b]], sizes[keys[b]]);
         QVERIFY2(!p.intersects(q), qPrintable(keys[a] + " overlaps " + keys[b]));
      }
   }
}

void AutoLayoutTestCase::testSameInputSamePicture()
{
   const DiagramDTO diagram = parsed(University);
   const QMap<QString, QSizeF> sizes = sizesOf(diagram);
   QCOMPARE(AutoLayout::arrange(diagram, sizes), AutoLayout::arrange(diagram, sizes));
}

void AutoLayoutTestCase::testAttributesStayNearTheirOwner()
{
   const DiagramDTO diagram = parsed(University);
   QMap<QString, QString> parents;
   const QMap<QString, QSizeF> sizes = sizesOf(diagram, &parents);
   const QMap<QString, QPointF> positions = AutoLayout::arrange(diagram, sizes);
   QStringList owners;
   for (const EntityDTO& entity : diagram.entities)
   {
      owners << entity.name;
   }
   for (const RelationshipDTO& relationship : diagram.relationships)
   {
      owners << relationship.name;
   }
   // an attribute of an entity or a relationship is closer to its shape than to the shape of any other of them
   auto distance = [&](const QString& attribute, const QString& owner) {
      const QPointF point = positions[attribute];
      const QRectF rect = rectOf(positions[owner], sizes[owner]);
      const qreal dx = std::max({rect.left() - point.x(), 0.0, point.x() - rect.right()});
      const qreal dy = std::max({rect.top() - point.y(), 0.0, point.y() - rect.bottom()});
      return std::hypot(dx, dy);
   };
   for (auto it = parents.cbegin(); it != parents.cend(); ++it)
   {
      if (!owners.contains(it.value()))
      {
         continue;
      }
      const qreal own = distance(it.key(), it.value());
      for (const QString& other : qAsConst(owners))
      {
         if (other != it.value())
         {
            QVERIFY2(own < distance(it.key(), other), qPrintable(it.key() + " is closer to " + other));
         }
      }
   }
   // the parts of a composite attribute are further out than the attribute
   const QPointF student = positions["Student"];
   const qreal name = QLineF(student, positions["Student.name"]).length();
   QVERIFY(QLineF(student, positions["Student.name.first"]).length() > name);
   QVERIFY(QLineF(student, positions["Student.name.last"]).length() > name);
}

void AutoLayoutTestCase::testAttributesAvoidTheLines()
{
   const DiagramDTO diagram = parsed(
      "Entity A { a1, a2, a3, a4 }\n"
      "Entity B { b1, b2 }\n"
      "Relationship R { A (1), B (N) }\n");
   const QMap<QString, QSizeF> sizes = sizesOf(diagram);
   const QMap<QString, QPointF> positions = AutoLayout::arrange(diagram, sizes);
   const QLineF line(positions["A"], positions["R"]);
   for (const char* key : {"A.a1", "A.a2", "A.a3", "A.a4"})
   {
      const QLineF toAttribute(positions["A"], positions[key]);
      const qreal angle = std::min(line.angleTo(toAttribute), toAttribute.angleTo(line));
      QVERIFY2(angle > 20, qPrintable(QString("%1 is %2 degrees from the line").arg(QLatin1String(key)).arg(angle)));
   }
}

void AutoLayoutTestCase::testConnectedNodesAreCloser()
{
   const DiagramDTO diagram = parsed(University);
   const QMap<QString, QPointF> positions = AutoLayout::arrange(diagram, sizesOf(diagram));
   const qreal connected = QLineF(positions["Enrolls"], positions["Student"]).length();
   const qreal unrelated = QLineF(positions["Enrolls"], positions["Dependent"]).length();
   QVERIFY(connected < unrelated);
}

void AutoLayoutTestCase::testCompleteKeepsKnownPositions()
{
   const DiagramDTO diagram = parsed(University);
   const QMap<QString, QSizeF> sizes = sizesOf(diagram);
   QMap<QString, QPointF> known = AutoLayout::arrange(diagram, sizes);
   known["Student"] += QPointF(500, 300);
   const QMap<QString, QPointF> completed = AutoLayout::complete(diagram, sizes, known);
   QCOMPARE(completed, known);
}

void AutoLayoutTestCase::testCompletePlacesANewAttributeNextToItsOwner()
{
   const DiagramDTO before = parsed("Entity A { x, y }\nEntity B { z }\nRelationship R { A (1), B (N) }\n");
   const QMap<QString, QPointF> known = AutoLayout::arrange(before, sizesOf(before));
   const DiagramDTO after = parsed("Entity A { x, y, w }\nEntity B { z }\nRelationship R { A (1), B (N) }\n");
   const QMap<QString, QSizeF> sizes = sizesOf(after);
   const QMap<QString, QPointF> completed = AutoLayout::complete(after, sizes, known);
   for (auto it = known.cbegin(); it != known.cend(); ++it)
   {
      QCOMPARE(completed.value(it.key()), it.value());
   }
   const QPointF w = completed.value("A.w");
   QVERIFY(QLineF(w, completed["A"]).length() < QLineF(w, completed["B"]).length());
   QVERIFY(QLineF(w, completed["A"]).length() < 250);
   for (const QString& key : known.keys())
   {
      QVERIFY2(!rectOf(w, sizes["A.w"]).intersects(rectOf(completed[key], sizes[key])), qPrintable("A.w overlaps " + key));
   }
}

void AutoLayoutTestCase::testCompletePlacesANewEntityNearItsRelationship()
{
   const DiagramDTO before = parsed("Entity A\nEntity B\nRelationship R { A (1), B (N) }\nEntity Far\n");
   QMap<QString, QPointF> known = AutoLayout::arrange(before, sizesOf(before));
   known["Far"] = known["R"] + QPointF(3000, 0);
   const DiagramDTO after = parsed("Entity A\nEntity B\nEntity C\nRelationship R { A (1), B (N), C (1) }\nEntity Far\n");
   const QMap<QString, QPointF> completed = AutoLayout::complete(after, sizesOf(after), known);
   QVERIFY(completed.contains("C"));
   QVERIFY(QLineF(completed["C"], completed["R"]).length() < QLineF(completed["C"], completed["Far"]).length());
}

void AutoLayoutTestCase::testEmptyDiagram()
{
   QVERIFY(AutoLayout::arrange(DiagramDTO(), {}).isEmpty());
}

QTEST_APPLESS_MAIN(AutoLayoutTestCase)

#include "tst_AutoLayoutTestCase.moc"
