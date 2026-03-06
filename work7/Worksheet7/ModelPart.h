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

    int red() const;
    int green() const;
    int blue() const;
    void setRGB(int r, int g, int b);

private:
    QList<ModelPart*> m_childItems;
    QVector<QVariant> m_itemData;
    ModelPart* m_parentItem;

    int m_r = 255;
    int m_g = 255;
    int m_b = 255;
};
