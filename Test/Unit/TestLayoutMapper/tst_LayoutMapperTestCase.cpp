#include <QtTest>
#include "Layout/LayoutMapper.h"

class LayoutMapperTestCase : public QObject
{
   Q_OBJECT

private slots:
   void testRoundTrip();
   void testWholePixels();
   void testNotALayout();
   void testSkipsBrokenPoints();
};

void LayoutMapperTestCase::testRoundTrip()
{
   LayoutDTO layout;
   layout.nodes.insert("Student", QPointF(10, -20));
   layout.nodes.insert("Student.name.first", QPointF(-300, 450));
   layout.nodes.insert("Order Item", QPointF(0, 0));
   bool ok = false;
   const LayoutDTO read = LayoutMapper::fromJson(LayoutMapper::toJson(layout), &ok);
   QVERIFY(ok);
   QCOMPARE(read.nodes, layout.nodes);
}

void LayoutMapperTestCase::testWholePixels()
{
   LayoutDTO layout;
   layout.nodes.insert("A", QPointF(10.4, 20.6));
   const LayoutDTO read = LayoutMapper::fromJson(LayoutMapper::toJson(layout));
   QCOMPARE(read.nodes.value("A"), QPointF(10, 21));
}

void LayoutMapperTestCase::testNotALayout()
{
   bool ok = true;
   QVERIFY(LayoutMapper::fromJson("not json", &ok).isEmpty());
   QVERIFY(!ok);
   QVERIFY(LayoutMapper::fromJson("{\"tables\": {}}", &ok).isEmpty());
   QVERIFY(!ok);
   QVERIFY(LayoutMapper::fromJson("[1, 2]", &ok).isEmpty());
   QVERIFY(!ok);
}

void LayoutMapperTestCase::testSkipsBrokenPoints()
{
   bool ok = false;
   const LayoutDTO read = LayoutMapper::fromJson(
      "{\"version\": 1, \"nodes\": {\"A\": {\"x\": 1, \"y\": 2}, \"B\": {\"x\": \"one\"}, \"C\": 5}}", &ok);
   QVERIFY(ok);
   QCOMPARE(read.nodes.size(), 1);
   QCOMPARE(read.nodes.value("A"), QPointF(1, 2));
}

QTEST_APPLESS_MAIN(LayoutMapperTestCase)

#include "tst_LayoutMapperTestCase.moc"
