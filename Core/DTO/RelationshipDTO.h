#pragma once
#include <QVector>
#include "AttributeDTO.h"

// An entity taking part in a relationship: "Student (0, N) [total, role: 'member']"
struct ParticipantDTO
{
   QString entity;
   QString min; // empty if only one value is written: (N)
   QString max;
   bool total = false; // double line
   QString role;
   int line = 0;

   // How the cardinality is written next to the line: "N" or "(0,N)"
   QString cardinality() const
   {
      return min.isEmpty() ? max : QStringLiteral("(%1,%2)").arg(min, max);
   }
};

struct RelationshipDTO
{
   QString name;
   bool identifying = false; // double diamond
   QString color;
   QString note;
   QVector<ParticipantDTO> participants;
   QVector<AttributeDTO> attributes;
   int line = 0;
   int endLine = 0;
};
