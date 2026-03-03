#include "ModelPart.h"

static int clamp255(int v)
{
    if (v < 0) return 0;
    if (v > 255) return 255;
    return v;
}

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

int ModelPart::red() const { return m_r; }
int ModelPart::green() const { return m_g; }
int ModelPart::blue() const { return m_b; }

void ModelPart::setRGB(int r, int g, int b)
{
    m_r = clamp255(r);
    m_g = clamp255(g);
    m_b = clamp255(b);
}
