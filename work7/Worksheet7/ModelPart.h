#pragma once

#include <QList>
#include <QVariant>
#include <QVector>
#include <QString>

#include <vtkSmartPointer.h>

class vtkSTLReader;
class vtkPolyDataMapper;
class vtkActor;

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

    void loadSTL(const QString& fileName);
    vtkSmartPointer<vtkActor> getActor() const;
    bool isVisible() const;

private:
    QList<ModelPart*> m_childItems;
    QVector<QVariant> m_itemData;
    ModelPart* m_parentItem = nullptr;

    int m_r = 255;
    int m_g = 255;
    int m_b = 255;

    vtkSmartPointer<vtkSTLReader> m_stlReader;
    vtkSmartPointer<vtkPolyDataMapper> m_mapper;
    vtkSmartPointer<vtkActor> m_actor;
};
