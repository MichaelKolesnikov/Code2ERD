#pragma once

class ERDItemModel
{
public:
   explicit ERDItemModel(int id);

   int id() const;

private:
   int m_id;
};

