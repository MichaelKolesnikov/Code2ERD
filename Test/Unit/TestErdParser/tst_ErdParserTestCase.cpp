#include <QtTest>
#include "Erd/ErdParser.h"
#include "Erd/ErdLexer.h"

class ErdParserTestCase : public QObject
{
   Q_OBJECT

private slots:
   void testEntityWithAttributes();
   void testCompositeAttribute();
   void testWeakEntityAndIdentifyingRelationship();
   void testRelationshipParticipants();
   void testRecursiveRelationship();
   void testBracesOnTheirOwnLines();
   void testEntityWithoutBody();
   void testCommasAndOneLineBodies();
   void testCommentsAndCase();
   void testEntitiesInAnyOrder();
   void testUnknownEntity();
   void testParticipantIsRelationship();
   void testTooFewParticipants();
   void testDuplicateNames();
   void testDuplicateAttribute();
   void testPartialKeyOutsideWeakEntity();
   void testKeyOnRelationshipAttribute();
   void testBadCardinality();
   void testUnknownSetting();
   void testErrorRecovery();
   void testUnclosedEntity();
   void testErrorsAreSorted();
};

void ErdParserTestCase::testEntityWithAttributes()
{
   const auto result = ErdParser::parse(
      "Entity Student [color: #3498db, note: 'Studies here'] {\n"
      "  student_id [key]\n"
      "  \"full name\"\n"
      "  phone [multivalued, note: 'Any number']\n"
      "  age [derived]\n"
      "  email [pk, multi]\n"
      "}\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   QCOMPARE(result.diagram.entities.size(), 1);
   const EntityDTO& student = result.diagram.entities.first();
   QCOMPARE(student.name, QString("Student"));
   QCOMPARE(student.color, QString("#3498db"));
   QCOMPARE(student.note, QString("Studies here"));
   QVERIFY(!student.weak);
   QCOMPARE(student.line, 1);
   QCOMPARE(student.endLine, 7);
   QCOMPARE(student.attributes.size(), 5);

   QCOMPARE(student.attributes[0].name, QString("student_id"));
   QVERIFY(student.attributes[0].key);
   QCOMPARE(student.attributes[0].line, 2);
   QCOMPARE(student.attributes[1].name, QString("full name"));
   QVERIFY(!student.attributes[1].key);
   QVERIFY(student.attributes[2].multivalued);
   QCOMPARE(student.attributes[2].note, QString("Any number"));
   QVERIFY(student.attributes[3].derived);
   QVERIFY(student.attributes[4].key);
   QVERIFY(student.attributes[4].multivalued);
}

void ErdParserTestCase::testCompositeAttribute()
{
   const auto result = ErdParser::parse(
      "Entity Person {\n"
      "  id [key]\n"
      "  address {\n"
      "    street {\n"
      "      name\n"
      "      number\n"
      "    }\n"
      "    city\n"
      "  }\n"
      "}\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   const EntityDTO& person = result.diagram.entities.first();
   QCOMPARE(person.attributes.size(), 2);
   const AttributeDTO& address = person.attributes[1];
   QCOMPARE(address.name, QString("address"));
   QCOMPARE(address.line, 3);
   QCOMPARE(address.children.size(), 2);
   QCOMPARE(address.children[0].name, QString("street"));
   QCOMPARE(address.children[0].children.size(), 2);
   QCOMPARE(address.children[0].children[1].name, QString("number"));
   QCOMPARE(address.children[0].children[1].line, 6);
   QCOMPARE(address.children[1].name, QString("city"));
   QCOMPARE(person.endLine, 10);
   QCOMPARE(attributeKey(attributeKey("Person", "address"), "city"), QString("Person.address.city"));
}

void ErdParserTestCase::testWeakEntityAndIdentifyingRelationship()
{
   const auto result = ErdParser::parse(
      "Entity Employee { ssn [key] }\n"
      "Entity Dependent [weak] {\n"
      "  name [partial key]\n"
      "  birth_date\n"
      "}\n"
      "Relationship Has [identifying] {\n"
      "  Employee (1)\n"
      "  Dependent (N) [total]\n"
      "}\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   const EntityDTO& dependent = result.diagram.entities[1];
   QVERIFY(dependent.weak);
   QVERIFY(dependent.attributes[0].partialKey);
   QVERIFY(!dependent.attributes[0].key);
   const RelationshipDTO& has = result.diagram.relationships.first();
   QVERIFY(has.identifying);
   QCOMPARE(has.participants.size(), 2);
   QCOMPARE(has.participants[0].cardinality(), QString("1"));
   QVERIFY(!has.participants[0].total);
   QCOMPARE(has.participants[1].cardinality(), QString("N"));
   QVERIFY(has.participants[1].total);
}

void ErdParserTestCase::testRelationshipParticipants()
{
   const auto result = ErdParser::parse(
      "Entity Student {}\n"
      "Entity Course {}\n"
      "Relationship Enrolls [color: #e67e22] {\n"
      "  Student (0, N)\n"
      "  Course (1, *) [total]\n"
      "  grade\n"
      "  semester [note: 'Fall or spring']\n"
      "}\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   const RelationshipDTO& enrolls = result.diagram.relationships.first();
   QCOMPARE(enrolls.name, QString("Enrolls"));
   QCOMPARE(enrolls.color, QString("#e67e22"));
   QCOMPARE(enrolls.line, 3);
   QCOMPARE(enrolls.endLine, 8);
   QCOMPARE(enrolls.participants.size(), 2);
   QCOMPARE(enrolls.participants[0].entity, QString("Student"));
   QCOMPARE(enrolls.participants[0].min, QString("0"));
   QCOMPARE(enrolls.participants[0].max, QString("N"));
   QCOMPARE(enrolls.participants[0].cardinality(), QString("(0,N)"));
   QCOMPARE(enrolls.participants[0].line, 4);
   QCOMPARE(enrolls.participants[1].cardinality(), QString("(1,*)"));
   QVERIFY(enrolls.participants[1].total);
   QCOMPARE(enrolls.attributes.size(), 2);
   QCOMPARE(enrolls.attributes[0].name, QString("grade"));
   QCOMPARE(enrolls.attributes[1].note, QString("Fall or spring"));
}

void ErdParserTestCase::testRecursiveRelationship()
{
   const auto result = ErdParser::parse(
      "Entity Employee {}\n"
      "Relationship Supervises {\n"
      "  Employee (0, 1) [role: 'subordinate']\n"
      "  Employee (0, N) [role: 'supervisor']\n"
      "}\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   const RelationshipDTO& supervises = result.diagram.relationships.first();
   QCOMPARE(supervises.participants.size(), 2);
   QCOMPARE(supervises.participants[0].role, QString("subordinate"));
   QCOMPARE(supervises.participants[1].role, QString("supervisor"));
}

void ErdParserTestCase::testBracesOnTheirOwnLines()
{
   const auto result = ErdParser::parse(
      "Entity Person\n"
      "{\n"
      "   id [key]\n"
      "   address\n"
      "   {\n"
      "      city\n"
      "   }\n"
      "}\n"
      "Entity Car\n"
      "{\n"
      "}\n"
      "Relationship Owns\n"
      "{\n"
      "   Person (1)\n"
      "   Car (N)\n"
      "}\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   QCOMPARE(result.diagram.entities.size(), 2);
   QCOMPARE(result.diagram.entities[0].attributes[1].children.size(), 1);
   QCOMPARE(result.diagram.entities[0].endLine, 8);
   QCOMPARE(result.diagram.relationships.first().participants.size(), 2);
}

void ErdParserTestCase::testEntityWithoutBody()
{
   const auto result = ErdParser::parse(
      "Entity A\n"
      "Entity B [weak]\n"
      "Relationship R { A (1)\n B (N) }\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   QCOMPARE(result.diagram.entities.size(), 2);
   QCOMPARE(result.diagram.entities[0].endLine, 1);
   QVERIFY(result.diagram.entities[1].weak);
   QCOMPARE(result.diagram.relationships.first().participants.size(), 2);
}

void ErdParserTestCase::testCommasAndOneLineBodies()
{
   const auto result = ErdParser::parse(
      "Entity Person { id [key], name { first, last }, age [derived] }\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   const EntityDTO& person = result.diagram.entities.first();
   QCOMPARE(person.attributes.size(), 3);
   QCOMPARE(person.attributes[1].children.size(), 2);
   QVERIFY(person.attributes[2].derived);
}

void ErdParserTestCase::testCommentsAndCase()
{
   const auto result = ErdParser::parse(
      "// a comment\n"
      "ENTITY A { /* inline */ id [KEY] }\n"
      "entity B {\n"
      "  /* a block\n"
      "     comment */\n"
      "  x // after\n"
      "}\n"
      "relationship R { A (1), B (N) }\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
   QCOMPARE(result.diagram.entities.size(), 2);
   QVERIFY(result.diagram.entities[0].attributes[0].key);
   QCOMPARE(result.diagram.entities[1].attributes[0].name, QString("x"));
   QCOMPARE(result.diagram.entities[1].attributes[0].line, 6);
}

void ErdParserTestCase::testEntitiesInAnyOrder()
{
   const auto result = ErdParser::parse(
      "Relationship Writes { Author (1, N), Book (1, N) }\n"
      "Entity Author\n"
      "Entity Book\n");
   QVERIFY2(result.ok(), qPrintable(result.errors.value(0).message));
}

void ErdParserTestCase::testUnknownEntity()
{
   const auto result = ErdParser::parse(
      "Entity A\n"
      "Relationship R {\n"
      "  A (1)\n"
      "  Missing (N)\n"
      "}\n");
   QCOMPARE(result.errors.size(), 1);
   QCOMPARE(result.errors[0].line, 4);
   QCOMPARE(result.errors[0].column, 3);
   QCOMPARE(result.errors[0].length, 7);
   QVERIFY(result.errors[0].message.contains("Unknown entity 'Missing'"));
   // the relationship is still there
   QCOMPARE(result.diagram.relationships.size(), 1);
}

void ErdParserTestCase::testParticipantIsRelationship()
{
   const auto result = ErdParser::parse(
      "Entity A\n"
      "Entity B\n"
      "Relationship R { A (1), B (1) }\n"
      "Relationship S { A (1), R (1) }\n");
   QCOMPARE(result.errors.size(), 1);
   QCOMPARE(result.errors[0].line, 4);
   QVERIFY(result.errors[0].message.contains("is a relationship"));
}

void ErdParserTestCase::testTooFewParticipants()
{
   const auto result = ErdParser::parse(
      "Entity A\n"
      "Relationship Lonely {\n"
      "  A (1)\n"
      "}\n");
   QCOMPARE(result.errors.size(), 1);
   QCOMPARE(result.errors[0].line, 2);
   QCOMPARE(result.errors[0].column, 14);
   QVERIFY(result.errors[0].message.contains("at least two"));
}

void ErdParserTestCase::testDuplicateNames()
{
   const auto result = ErdParser::parse(
      "Entity A\n"
      "Entity B\n"
      "Entity A\n"
      "Relationship B { A (1), A (N) }\n");
   QCOMPARE(result.errors.size(), 2);
   QCOMPARE(result.errors[0].line, 3);
   QVERIFY(result.errors[0].message.contains("already defined at line 1"));
   QCOMPARE(result.errors[1].line, 4);
   QVERIFY(result.errors[1].message.contains("already defined at line 2"));
}

void ErdParserTestCase::testDuplicateAttribute()
{
   const auto result = ErdParser::parse(
      "Entity A {\n"
      "  x\n"
      "  y { x, z }\n"
      "  x\n"
      "}\n");
   QCOMPARE(result.errors.size(), 1);
   QCOMPARE(result.errors[0].line, 4);
   QVERIFY(result.errors[0].message.contains("already has the attribute 'x' at line 2"));
}

void ErdParserTestCase::testPartialKeyOutsideWeakEntity()
{
   const auto result = ErdParser::parse(
      "Entity A {\n"
      "  x [partial key]\n"
      "}\n"
      "Entity W [weak] {\n"
      "  part { y [partial] }\n"
      "}\n");
   QCOMPARE(result.errors.size(), 1);
   QCOMPARE(result.errors[0].line, 2);
   QVERIFY(result.errors[0].message.contains("weak entity"));
}

void ErdParserTestCase::testKeyOnRelationshipAttribute()
{
   const auto result = ErdParser::parse(
      "Entity A\n"
      "Relationship R {\n"
      "  A (1)\n"
      "  A (N)\n"
      "  since [key]\n"
      "}\n");
   QCOMPARE(result.errors.size(), 1);
   QCOMPARE(result.errors[0].line, 5);
   QVERIFY(result.errors[0].message.contains("cannot be a key"));
}

void ErdParserTestCase::testBadCardinality()
{
   const auto result = ErdParser::parse(
      "Entity A\n"
      "Entity B\n"
      "Relationship R {\n"
      "  A (5, 2)\n"
      "  B (N, M)\n"
      "  A (1, 2, 3)\n"
      "  B ('x')\n"
      "}\n");
   QCOMPARE(result.errors.size(), 4);
   QVERIFY(result.errors[0].message.contains("greater than the maximum"));
   QCOMPARE(result.errors[0].line, 4);
   QVERIFY(result.errors[1].message.contains("minimum is a number"));
   QCOMPARE(result.errors[1].line, 5);
   QVERIFY(result.errors[2].message.contains("one value"));
   QCOMPARE(result.errors[2].line, 6);
   QVERIFY(result.errors[3].message.contains("Expected a cardinality"));
   QCOMPARE(result.errors[3].line, 7);
}

void ErdParserTestCase::testUnknownSetting()
{
   const auto result = ErdParser::parse(
      "Entity A [strong] {\n"
      "  x [unique]\n"
      "}\n");
   QCOMPARE(result.errors.size(), 2);
   QVERIFY(result.errors[0].message.contains("Unknown setting 'strong'"));
   QCOMPARE(result.errors[0].column, 11);
   QCOMPARE(result.errors[0].length, 6);
   QVERIFY(result.errors[1].message.contains("Unknown setting 'unique'"));
   QCOMPARE(result.diagram.entities.first().attributes.size(), 1);
}

void ErdParserTestCase::testErrorRecovery()
{
   const auto result = ErdParser::parse(
      "Entity A {\n"
      "  good\n"
      "  bad ) thing\n"
      "  [oops]\n"
      "  also_good\n"
      "}\n"
      "Entiti Typo {\n"
      "  x\n"
      "}\n"
      "Entity B\n"
      "Relationship R { A (1), B (N) }\n");
   QCOMPARE(result.errors.size(), 3);
   QCOMPARE(result.errors[0].line, 3);
   QCOMPARE(result.errors[1].line, 4);
   QCOMPARE(result.errors[2].line, 7);
   QVERIFY(result.errors[2].message.contains("Expected 'Entity' or 'Relationship'"));
   QCOMPARE(result.diagram.entities.size(), 2);
   const EntityDTO& a = result.diagram.entities.first();
   QCOMPARE(a.attributes.size(), 2);
   QCOMPARE(a.attributes[1].name, QString("also_good"));
   QCOMPARE(result.diagram.relationships.size(), 1);
}

void ErdParserTestCase::testUnclosedEntity()
{
   const auto result = ErdParser::parse(
      "Entity A {\n"
      "  x\n"
      "  y\n");
   QCOMPARE(result.errors.size(), 1);
   QCOMPARE(result.errors[0].line, 1);
   QVERIFY(result.errors[0].message.contains("not closed"));
   QCOMPARE(result.diagram.entities.first().attributes.size(), 2);
}

void ErdParserTestCase::testErrorsAreSorted()
{
   const auto result = ErdParser::parse(
      "Relationship R { A (1), Missing (N) }\n"
      "Entity A { x [what] }\n"
      "Entity A\n");
   QVERIFY(result.errors.size() >= 3);
   for (int i = 1; i < result.errors.size(); ++i)
   {
      const ErdError& previous = result.errors[i - 1];
      const ErdError& current = result.errors[i];
      QVERIFY(previous.line < current.line || (previous.line == current.line && previous.column <= current.column));
   }
}

QTEST_APPLESS_MAIN(ErdParserTestCase)

#include "tst_ErdParserTestCase.moc"
