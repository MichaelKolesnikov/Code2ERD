#include "Models/PropertyModel.h"

PropertyModel::PropertyModel(const PropertyDTO& dto)
   : LinkModel({dto.id, dto.name, dto.position, dto.size}), m_dto(dto)
{
}
