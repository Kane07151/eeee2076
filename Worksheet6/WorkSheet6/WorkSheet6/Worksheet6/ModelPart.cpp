#include "ModelPart.h"

ModelPart::ModelPart(const QVector<QVariant>& data, ModelPart* parentItem)
    : m_itemData(data), m_parentItem(parentItem)
{
}

ModelPart::~ModelPart()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

void ModelPart::appendChild(ModelPart* child)
{
    if (!child) return;
    m_childItems.append(child);
}

ModelPart* ModelPart::child(int row) const
{
    if (row < 0 || row >= m_childItems.size()) return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const
{
    return m_childItems.size();
}

int ModelPart::columnCount() const
{
    return m_itemData.size();
}

QVariant ModelPart::data(int column) const
{
    if (column < 0 || column >= m_itemData.size()) return {};
    return m_itemData.at(column);
}

bool ModelPart::setData(int column, const QVariant& value)
{
    if (column < 0 || column >= m_itemData.size()) return false;
    m_itemData[column] = value;
    return true;
}

int ModelPart::row() const
{
    if (!m_parentItem) return 0;
    return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
}

ModelPart* ModelPart::parentItem() const
{
    return m_parentItem;
}
