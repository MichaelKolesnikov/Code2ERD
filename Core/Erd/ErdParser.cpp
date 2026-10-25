#include "ErdParser.h"
#include "ErdLexer.h"
#include <QHash>
#include <algorithm>

namespace
{
   struct Setting
   {
      QString key; // lowercase, words joined by one space: "partial key"
      ErdToken keyToken;
      ErdToken value; // End if there is no value
      bool hasValue = false;
   };

   struct PendingParticipant
   {
      int relationship = 0;
      ErdToken token;
   };

   enum class Owner
   {
      Entity,
      WeakEntity,
      Relationship
   };

   class Parser
   {
   public:
      Parser(const QString& source, ErdParseResult& result)
         : m_result(result), m_diagram(result.diagram)
      {
         m_tokens = ErdLexer::tokenize(source, &m_result.errors);
      }

      void run()
      {
         while (true)
         {
            skipNewlines();
            if (peek().kind == ErdToken::End)
            {
               break;
            }
            const int start = m_pos;
            if (!parseElement())
            {
               skipElement(start);
            }
         }

         resolveParticipants();

         std::stable_sort(m_result.errors.begin(), m_result.errors.end(), [](const ErdError& a, const ErdError& b) {
            return a.line != b.line ? a.line < b.line : a.column < b.column;
         });
      }

   private:
      // ---- token access ----

      const ErdToken& peek(int offset = 0) const
      {
         const int index = std::max(0, std::min(m_pos + offset, m_tokens.size() - 1));
         return m_tokens[index];
      }

      const ErdToken& next()
      {
         const ErdToken& token = m_tokens[m_pos];
         if (m_pos < m_tokens.size() - 1)
         {
            ++m_pos;
         }
         return token;
      }

      bool isSymbol(const char* symbol, int offset = 0) const
      {
         const ErdToken& token = peek(offset);
         return token.kind == ErdToken::Symbol && token.text == QLatin1String(symbol);
      }

      bool isKeyword(const char* keyword, int offset = 0) const
      {
         const ErdToken& token = peek(offset);
         return token.kind == ErdToken::Identifier && token.text.compare(QLatin1String(keyword), Qt::CaseInsensitive) == 0;
      }

      static bool isName(const ErdToken& token)
      {
         return token.kind == ErdToken::Identifier || token.kind == ErdToken::QuotedIdentifier;
      }

      bool atLineEnd() const
      {
         return peek().kind == ErdToken::Newline || peek().kind == ErdToken::End;
      }

      // "{" on this line or at the start of the next one
      bool braceAhead() const
      {
         return isSymbol("{") || (peek().kind == ErdToken::Newline && isSymbol("{", 1));
      }

      static QString describe(const ErdToken& token)
      {
         switch (token.kind)
         {
         case ErdToken::Newline:
            return QStringLiteral("the end of the line");
         case ErdToken::End:
            return QStringLiteral("the end of the text");
         case ErdToken::String:
            return QStringLiteral("a string");
         case ErdToken::QuotedIdentifier:
            return QStringLiteral("\"%1\"").arg(token.text);
         default:
            return QStringLiteral("'%1'").arg(token.text);
         }
      }

      void skipNewlines()
      {
         while (peek().kind == ErdToken::Newline)
         {
            next();
         }
      }

      // ---- errors ----

      void error(const ErdToken& token, const QString& message)
      {
         m_result.errors.append(ErdError{token.line, token.column, std::max(1, token.length), message});
      }

      bool expectSymbol(const char* symbol, const QString& context)
      {
         if (isSymbol(symbol))
         {
            next();
            return true;
         }
         error(peek(), QStringLiteral("Expected '%1' %2, found %3").arg(QLatin1String(symbol), context, describe(peek())));
         return false;
      }

      // To the end of the line, over the nested braces opened on it
      void skipLine()
      {
         int depth = 0;
         while (peek().kind != ErdToken::End)
         {
            if (peek().kind == ErdToken::Newline && depth <= 0)
            {
               return;
            }
            if (isSymbol("{"))
            {
               ++depth;
            }
            else if (isSymbol("}"))
            {
               if (depth <= 0)
               {
                  return; // the closing brace of the body
               }
               --depth;
            }
            next();
         }
      }

      // A broken top-level element: to its closing brace, or to the end of the line without one
      void skipElement(int start)
      {
         m_pos = start;
         int depth = 0;
         bool seenBrace = false;
         while (peek().kind != ErdToken::End)
         {
            const ErdToken& token = next();
            if (token.kind == ErdToken::Symbol && token.text == '{')
            {
               ++depth;
               seenBrace = true;
            }
            else if (token.kind == ErdToken::Symbol && token.text == '}')
            {
               --depth;
               if (depth <= 0 && seenBrace)
               {
                  return;
               }
            }
            else if (token.kind == ErdToken::Newline && depth <= 0 && !seenBrace)
            {
               skipNewlines();
               if (!isSymbol("{"))
               {
                  return;
               }
            }
         }
      }

      // ---- settings ----

      QVector<Setting> parseSettings()
      {
         QVector<Setting> settings;
         next(); // [
         while (true)
         {
            if (isSymbol("]"))
            {
               next();
               return settings;
            }
            // settings stay on one line, so a missing ']' does not swallow the next line
            if (atLineEnd() || isSymbol("}") || isSymbol("{"))
            {
               error(peek(), QStringLiteral("Expected ']' to close the settings"));
               return settings;
            }

            Setting setting;
            setting.keyToken = peek();
            QStringList words;
            while (peek().kind == ErdToken::Identifier)
            {
               words << next().text.toLower();
            }
            if (words.isEmpty())
            {
               error(peek(), QStringLiteral("Expected a setting, found %1").arg(describe(peek())));
               skipSetting();
               continue;
            }
            setting.key = words.join(' ');
            setting.keyToken.length = peek(-1).position + peek(-1).length - setting.keyToken.position;

            if (isSymbol(":"))
            {
               next();
               if (atLineEnd() || isSymbol(",") || isSymbol("]"))
               {
                  error(setting.keyToken, QStringLiteral("Setting '%1' needs a value").arg(setting.key));
               }
               else
               {
                  setting.value = next();
                  setting.hasValue = true;
               }
            }
            settings << setting;

            if (isSymbol(","))
            {
               next();
            }
            else if (!isSymbol("]") && !atLineEnd())
            {
               error(peek(), QStringLiteral("Expected ',' or ']' in the settings, found %1").arg(describe(peek())));
               skipSetting();
            }
         }
      }

      void skipSetting()
      {
         while (!isSymbol(",") && !isSymbol("]") && !atLineEnd() && !isSymbol("}"))
         {
            next();
         }
         if (isSymbol(","))
         {
            next();
         }
      }

      bool flag(const Setting& setting)
      {
         if (setting.hasValue)
         {
            error(setting.keyToken, QStringLiteral("Setting '%1' takes no value").arg(setting.key));
         }
         return true;
      }

      QString stringValue(const Setting& setting)
      {
         if (!setting.hasValue)
         {
            error(setting.keyToken, QStringLiteral("Setting '%1' needs a value: %1: 'text'").arg(setting.key));
            return QString();
         }
         if (setting.value.kind != ErdToken::String)
         {
            error(setting.value, QStringLiteral("Expected a string in quotes: '%1'").arg(setting.value.text));
            return QString();
         }
         return setting.value.text;
      }

      QString colorValue(const Setting& setting)
      {
         if (!setting.hasValue)
         {
            error(setting.keyToken, QStringLiteral("Setting 'color' needs a value: color: #3498db"));
            return QString();
         }
         if (setting.value.kind != ErdToken::Color)
         {
            error(setting.value, QStringLiteral("Expected a color like #3498db, found %1").arg(describe(setting.value)));
            return QString();
         }
         return setting.value.text;
      }

      void unknownSetting(const Setting& setting, const QString& allowed)
      {
         error(setting.keyToken, QStringLiteral("Unknown setting '%1', expected %2").arg(setting.key, allowed));
      }

      // ---- elements ----

      bool parseElement()
      {
         if (isKeyword("entity"))
         {
            return parseEntity();
         }
         if (isKeyword("relationship"))
         {
            return parseRelationship();
         }
         error(peek(), QStringLiteral("Expected 'Entity' or 'Relationship', found %1").arg(describe(peek())));
         return false;
      }

      bool parseElementName(const QString& what, QString& name, ErdToken& token)
      {
         if (!isName(peek()))
         {
            error(peek(), QStringLiteral("Expected the name of the %1, found %2").arg(what, describe(peek())));
            return false;
         }
         token = next();
         name = token.text;
         if (m_elementLines.contains(name))
         {
            error(token, QStringLiteral("'%1' is already defined at line %2").arg(name).arg(m_elementLines.value(name)));
         }
         else
         {
            m_elementLines.insert(name, token.line);
         }
         return true;
      }

      // Opens a body: "{" on this line or on the next one
      bool openBody(const QString& context)
      {
         if (peek().kind == ErdToken::Newline && isSymbol("{", 1))
         {
            next();
         }
         return expectSymbol("{", context);
      }

      bool parseEntity()
      {
         const ErdToken keyword = next();
         EntityDTO entity;
         entity.line = keyword.line;
         ErdToken nameToken;
         if (!parseElementName(QStringLiteral("entity"), entity.name, nameToken))
         {
            return false;
         }
         if (isSymbol("["))
         {
            for (const Setting& setting : parseSettings())
            {
               if (setting.key == QLatin1String("weak"))
               {
                  entity.weak = flag(setting);
               }
               else if (setting.key == QLatin1String("color"))
               {
                  entity.color = colorValue(setting);
               }
               else if (setting.key == QLatin1String("note"))
               {
                  entity.note = stringValue(setting);
               }
               else
               {
                  unknownSetting(setting, QStringLiteral("weak, color or note"));
               }
            }
         }
         entity.endLine = entity.line;
         if (braceAhead())
         {
            openBody(QStringLiteral("to start the attributes"));
            entity.attributes = parseAttributeList(entity.weak ? Owner::WeakEntity : Owner::Entity, entity.name, entity.endLine);
         }
         else if (!atLineEnd())
         {
            error(peek(), QStringLiteral("Expected '{' or the end of the line after the entity, found %1").arg(describe(peek())));
            skipLine();
         }
         m_diagram.entities << entity;
         return true;
      }

      bool parseRelationship()
      {
         const ErdToken keyword = next();
         RelationshipDTO relationship;
         relationship.line = keyword.line;
         ErdToken nameToken;
         if (!parseElementName(QStringLiteral("relationship"), relationship.name, nameToken))
         {
            return false;
         }
         if (isSymbol("["))
         {
            for (const Setting& setting : parseSettings())
            {
               if (setting.key == QLatin1String("identifying"))
               {
                  relationship.identifying = flag(setting);
               }
               else if (setting.key == QLatin1String("color"))
               {
                  relationship.color = colorValue(setting);
               }
               else if (setting.key == QLatin1String("note"))
               {
                  relationship.note = stringValue(setting);
               }
               else
               {
                  unknownSetting(setting, QStringLiteral("identifying, color or note"));
               }
            }
         }
         if (!openBody(QStringLiteral("to list the entities of the relationship")))
         {
            return false;
         }

         const int index = m_diagram.relationships.size();
         QHash<QString, int> attributeLines;
         while (true)
         {
            skipSeparators();
            if (isSymbol("}"))
            {
               relationship.endLine = next().line;
               break;
            }
            if (peek().kind == ErdToken::End)
            {
               error(keyword, QStringLiteral("The relationship '%1' is not closed with '}'").arg(relationship.name));
               relationship.endLine = peek().line;
               break;
            }
            if (isName(peek()) && isSymbol("(", 1))
            {
               ParticipantDTO participant;
               if (parseParticipant(participant, index))
               {
                  relationship.participants << participant;
               }
               else
               {
                  skipLine();
               }
               continue;
            }
            AttributeDTO attribute;
            if (parseAttribute(attribute, Owner::Relationship, relationship.name, attributeLines))
            {
               relationship.attributes << attribute;
            }
            else
            {
               skipLine();
            }
         }

         if (relationship.participants.size() < 2)
         {
            error(nameToken, QStringLiteral("The relationship '%1' needs at least two entities: Entity (min, max) on a line each, "
                                            "the same entity twice for a recursive one").arg(relationship.name));
         }
         m_diagram.relationships << relationship;
         return true;
      }

      // Student (0, N) [total, role: 'member']
      bool parseParticipant(ParticipantDTO& participant, int relationship)
      {
         const ErdToken nameToken = next();
         participant.entity = nameToken.text;
         participant.line = nameToken.line;
         m_pendingParticipants << PendingParticipant{relationship, nameToken};
         next(); // (

         QStringList values;
         QVector<ErdToken> valueTokens;
         while (!isSymbol(")"))
         {
            const ErdToken& token = peek();
            const bool isValue = token.kind == ErdToken::Number || token.kind == ErdToken::Identifier || isSymbol("*");
            if (!isValue)
            {
               error(token, QStringLiteral("Expected a cardinality like 1, N or (0, N), found %1").arg(describe(token)));
               return false;
            }
            valueTokens << next();
            values << valueTokens.last().text;
            if (isSymbol(","))
            {
               next();
            }
            else if (!isSymbol(")"))
            {
               error(peek(), QStringLiteral("Expected ',' or ')' in the cardinality, found %1").arg(describe(peek())));
               return false;
            }
         }
         const ErdToken& close = next();
         if (values.isEmpty() || values.size() > 2)
         {
            ErdToken range = valueTokens.isEmpty() ? close : valueTokens.first();
            error(range, QStringLiteral("A cardinality has one value, (N), or two, (min, max)"));
            return false;
         }
         participant.max = values.last();
         if (values.size() == 2)
         {
            participant.min = values.first();
            bool minIsNumber = false;
            bool maxIsNumber = false;
            const int min = participant.min.toInt(&minIsNumber);
            const int max = participant.max.toInt(&maxIsNumber);
            if (minIsNumber && maxIsNumber && min > max)
            {
               error(valueTokens.first(), QStringLiteral("The minimum %1 is greater than the maximum %2").arg(min).arg(max));
            }
            if (!minIsNumber)
            {
               error(valueTokens.first(), QStringLiteral("The minimum is a number: 0 or 1, found '%1'").arg(participant.min));
            }
         }

         if (isSymbol("["))
         {
            for (const Setting& setting : parseSettings())
            {
               if (setting.key == QLatin1String("total"))
               {
                  participant.total = flag(setting);
               }
               else if (setting.key == QLatin1String("role"))
               {
                  participant.role = stringValue(setting);
               }
               else
               {
                  unknownSetting(setting, QStringLiteral("total or role"));
               }
            }
         }
         return expectLineEnd(QStringLiteral("after the entity of the relationship"));
      }

      // An entry ends with the line, a comma or the closing brace
      bool expectLineEnd(const QString& context)
      {
         if (atLineEnd() || isSymbol(",") || isSymbol("}"))
         {
            return true;
         }
         error(peek(), QStringLiteral("Expected the end of the line %1, found %2").arg(context, describe(peek())));
         return false;
      }

      void skipSeparators()
      {
         while (peek().kind == ErdToken::Newline || isSymbol(","))
         {
            next();
         }
      }

      // The attributes up to the closing brace, which is consumed; its line goes to endLine
      QVector<AttributeDTO> parseAttributeList(Owner owner, const QString& ownerName, int& endLine)
      {
         QVector<AttributeDTO> attributes;
         QHash<QString, int> lines;
         const ErdToken open = peek(-1);
         while (true)
         {
            skipSeparators();
            if (isSymbol("}"))
            {
               endLine = next().line;
               return attributes;
            }
            if (peek().kind == ErdToken::End)
            {
               error(open, QStringLiteral("'{' of '%1' is not closed with '}'").arg(ownerName));
               endLine = peek().line;
               return attributes;
            }
            AttributeDTO attribute;
            if (parseAttribute(attribute, owner, ownerName, lines))
            {
               attributes << attribute;
            }
            else
            {
               skipLine();
            }
         }
      }

      // name [key] or a composite one: name { first last }
      bool parseAttribute(AttributeDTO& attribute, Owner owner, const QString& ownerName, QHash<QString, int>& siblings)
      {
         if (!isName(peek()))
         {
            error(peek(), QStringLiteral("Expected the name of an attribute, found %1").arg(describe(peek())));
            return false;
         }
         const ErdToken nameToken = next();
         attribute.name = nameToken.text;
         attribute.line = nameToken.line;
         if (siblings.contains(attribute.name))
         {
            error(nameToken, QStringLiteral("'%1' already has the attribute '%2' at line %3")
                  .arg(ownerName, attribute.name).arg(siblings.value(attribute.name)));
         }
         else
         {
            siblings.insert(attribute.name, attribute.line);
         }

         if (isSymbol("["))
         {
            for (const Setting& setting : parseSettings())
            {
               if (setting.key == QLatin1String("key") || setting.key == QLatin1String("pk"))
               {
                  attribute.key = flag(setting);
                  if (owner == Owner::Relationship)
                  {
                     error(setting.keyToken, QStringLiteral("An attribute of a relationship cannot be a key"));
                  }
               }
               else if (setting.key == QLatin1String("partial key") || setting.key == QLatin1String("partial"))
               {
                  attribute.partialKey = flag(setting);
                  if (owner != Owner::WeakEntity)
                  {
                     error(setting.keyToken, QStringLiteral("A partial key belongs to a weak entity: Entity %1 [weak]").arg(ownerName));
                  }
               }
               else if (setting.key == QLatin1String("multivalued") || setting.key == QLatin1String("multi"))
               {
                  attribute.multivalued = flag(setting);
               }
               else if (setting.key == QLatin1String("derived"))
               {
                  attribute.derived = flag(setting);
               }
               else if (setting.key == QLatin1String("note"))
               {
                  attribute.note = stringValue(setting);
               }
               else
               {
                  unknownSetting(setting, QStringLiteral("key, partial key, multivalued, derived or note"));
               }
            }
         }
         if (attribute.key && attribute.partialKey)
         {
            error(nameToken, QStringLiteral("An attribute is either a key or a partial key"));
         }

         if (braceAhead())
         {
            openBody(QString());
            int endLine = 0;
            const QString key = attributeKey(ownerName, attribute.name);
            attribute.children = parseAttributeList(owner, key, endLine);
            if (attribute.children.isEmpty())
            {
               error(nameToken, QStringLiteral("The composite attribute '%1' has no parts").arg(attribute.name));
            }
            return true;
         }
         return expectLineEnd(QStringLiteral("after the attribute"));
      }

      // ---- after parsing ----

      void resolveParticipants()
      {
         for (const PendingParticipant& pending : qAsConst(m_pendingParticipants))
         {
            const QString& name = pending.token.text;
            if (m_diagram.entity(name))
            {
               continue;
            }
            if (m_diagram.relationship(name))
            {
               error(pending.token, QStringLiteral("'%1' is a relationship: a relationship connects entities").arg(name));
            }
            else
            {
               error(pending.token, QStringLiteral("Unknown entity '%1'").arg(name));
            }
         }
      }

      ErdParseResult& m_result;
      DiagramDTO& m_diagram;
      QVector<ErdToken> m_tokens;
      int m_pos = 0;
      QHash<QString, int> m_elementLines; // entity and relationship names share one namespace
      QVector<PendingParticipant> m_pendingParticipants;
   };
}

ErdParseResult ErdParser::parse(const QString &source)
{
   ErdParseResult result;
   Parser(source, result).run();
   return result;
}
