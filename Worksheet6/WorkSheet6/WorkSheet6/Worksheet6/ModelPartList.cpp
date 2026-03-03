#include "ModelPartList.h"
#include <QColor>

ModelPartList::ModelPartList(const QString& name, QObject* parent)
    : QAbstractItemModel(parent), m_name(name)
{
    // Root item stores headers
    m_rootItem = new ModelPart({QString("Part"), QString("Visible")});
}

ModelPartList::~ModelPartList()
{
    delete m_rootItem;
    m_rootItem = nullptr;
}

int ModelPartList::columnCount(const QModelIndex& /*parent*/) const
{
    return m_rootItem ? m_rootItem->columnCount() : 0;
}

QVariant ModelPartList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};

    auto* item = static_cast<ModelPart*>(index.internalPointer());
    if (!item) return {};

    // 显示文字（Part / Visible）
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return item->data(index.column());
    }

    // ✅ 用 RGB 让颜色“看得见”：给 Part 列加背景色
    if (role == Qt::BackgroundRole && index.column() == 0) {
        return QColor(item->red(), item->green(), item->blue());
    }

    return {};
}

Qt::ItemFlags ModelPartList::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ModelPartList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    if (!m_rootItem) return {};
    return m_rootItem->data(section);
}

QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) return {};

    ModelPart* parentItem = nullptr;
    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    if (!parentItem) return {};

    ModelPart* childItem = parentItem->child(row);
    if (!childItem) return {};

    return createIndex(row, column, childItem);
}

QModelIndex ModelPartList::parent(const QModelIndex& index) const
{
    if (!index.isValid()) return {};

    auto* childItem = static_cast<ModelPart*>(index.internalPointer());
    if (!childItem) return {};

    ModelPart* parentItem = childItem->parentItem();
    if (!parentItem || parentItem == m_rootItem) return {};

    return createIndex(parentItem->row(), 0, parentItem);
}

int ModelPartList::rowCount(const QModelIndex& parent) const
{
    ModelPart* parentItem = nullptr;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    if (!parentItem) return 0;
    return parentItem->childCount();
}

bool ModelPartList::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) return false;
    if (role != Qt::EditRole) return false;

    auto* item = static_cast<ModelPart*>(index.internalPointer());
    if (!item) return false;

    bool ok = item->setData(index.column(), value);
    if (ok) {
        // ✅ 让 view 同时刷新 Display + Background（颜色）
        emit dataChanged(index, index,
                         {Qt::DisplayRole, Qt::EditRole, Qt::BackgroundRole});
    }
    return ok;
}

ModelPart* ModelPartList::getRootItem() const
{
    return m_rootItem;
}
