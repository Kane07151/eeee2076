#pragma once

#include <QList>
#include <QVariant>
#include <QVector>

class ModelPart
{
public:
    explicit ModelPart(const QVector<QVariant>& data, ModelPart* parentItem = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* child);

    ModelPart* child(int row) const;
    int childCount() const;
    int columnCount() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant& value);

    int row() const;
    ModelPart* parentItem() const;

private:
    QList<ModelPart*> m_childItems;
    QVector<QVariant> m_itemData;
    ModelPart* m_parentItem;
};
